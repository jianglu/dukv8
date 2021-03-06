//
// Created by Jiang Lu on 6/3/15.
//

#include <cassert>
#include "dukv8/function.h"
#include "dukv8/isolate.h"
#include "dukv8/string.h"
#include "dukv8/integer.h"
#include "dukv8/function_template.h"
#include "dukv8/duk_stack_scope.h"
#include "dukv8/local.h"
#include "dukv8/arguments.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Function, v8::Object);

Function *Function::Init(DukContextRef duk_ctx, void *heap_ptr) {
    Object::Init(duk_ctx);
    DUK_STACK_SCOPE(duk_ctx);
    if (heap_ptr) {
        duk_obj_heapptr_ = heap_ptr;
        duk_obj_index_ = DukObjectRetain(duk_obj_heapptr_);
    }
    return this;
}

Function *Function::Cast(Value *value) {
    if (RTTI_IsKindOf(Function, value)) {
        return RTTI_StaticCast(Function, value);
    }

    if (value) {
        if (value->IsFunction()) {
            DukContextRef ctx = value->GetDukContext();
            DUK_STACK_SCOPE(ctx);
            value->Push();
            return (new Function)->Init(ctx, duk_get_heapptr(ctx, -1));
        }
    }
    return NULL;
}

Local<Object> Function::NewInstance() const {
    TODO();
    return Local<Object>();
}

Local<Object> Function::NewInstance(int argc, Handle<Value> argv[]) const {
    TODO();
    return Local<Object>();
}

Local<Value> Function::Call(Handle<Object> that, int argc, Handle<Value> argv[]) {

    if (!function_template_.IsEmpty() && function_template_->callback_) {
        // C++ 函数

        Arguments args(Isolate::GetCurrent(),
                       Handle<Function>(this),
                       that,
                       that,
                       function_template_->data_,
                       argc,
                       argv,
                       false); // TODO: holder?

        Handle<Value> result = function_template_->callback_(args);

        return Local<Value>::New(result);

    } else {
        // JavaScript 函数
        Push();
        assert(duk_is_function(duk_ctx_, -1));

        for (int i = 0; i < argc; i++) {
            argv[i]->Push();
        }

        duk_int_t rc = duk_pcall(duk_ctx_, argc);
        if (rc == DUK_EXEC_SUCCESS) {
            return Local<Value>::New(Value::FromStack(duk_ctx_, -1));
        } else {
            printf("error: %s\n", duk_to_string(duk_ctx_, -1));
            return Local<Value>();
        }
    }
}

void Function::SetName(Handle<String> name) {
    name_ = name;
}

Handle<Value> Function::GetName() const {
    return name_;
}

}