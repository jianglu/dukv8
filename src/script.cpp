//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/script.h"
#include "dukv8/local.h"
#include "dukv8/string.h"
#include "dukv8/duk_stack_scope.h"
#include "dukv8/isolate.h"
#include "dukv8/context.h"
#include "dukv8/function.h"
#include "dukv8/boolean.h"
#include "dukv8/integer.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Script, v8::internal::GCObject);

Script::Script(Handle<String> source,
               Handle<Value> file_name,
               Handle<Integer> line_number,
               Handle<String> script_data) :
        source_(source),
        file_name_(file_name),
        line_number_(line_number),
        script_data_(script_data) {
}

Script::~Script() {
}

Script *Script::Init() {
    i::GCObject::Init();
    return this;
}

void Script::Deinit() {
    i::GCObject::Deinit();
}

Local<Script> Script::New(Handle<String> source,
                          ScriptOrigin *origin,
                          ScriptData *pre_data,
                          Handle<String> script_data) {
    Handle<Value> file_name;
    Handle<Integer> line_number;
    if (origin) {
        file_name = origin->resource_name_;
        line_number = origin->resource_line_offset_;
    }
    return Local<Script>::New(Handle<Script>(
            (new Script(source, file_name, line_number, script_data))->Init()));
}

Local<Script> Script::New(Handle<String> source,
                          Handle<Value> file_name) {
    return Local<Script>();
}

Local<Script> Script::Compile(Handle<String> source,
                              ScriptOrigin *origin,
                              ScriptData *pre_data,
                              Handle<String> script_data) {

    Local<Script> script = Script::New(source, origin, pre_data, script_data);

    // 绑定到当前上下文中
    script->context_ = Context::GetCurrent();
    script->TryCompile();
    return Local<Script>::New(script);
}


Local<Script> Script::Compile(Handle<String> source,
                              Handle<Value> file_name,
                              Handle<String> script_data) {
    ScriptOrigin origin(file_name);
    return Script::Compile(source, &origin, NULL, script_data);
}

Local<Value> Script::Run() {
    Isolate *isolate = Isolate::GetCurrent();
    DukContextRef ctx = isolate->GetDukContext();

    Handle<Context> context = context_;
    if (context.IsEmpty()) {
        context = Context::GetCurrent();
    }

    Context::Scope context_scope(context);

    Handle<Object> global_object = context->Global();

    DUK_STACK_SCOPE(ctx);

    function_->Push();
    global_object->Push();

    if (duk_pcall_method(ctx, 0) == DUK_ERR_NONE) {
        return Local<Value>::New(Value::FromStack(ctx, -1));
    } else {
        // 发生错误, 转换成 Message 放入最近的 TryCatch
        return Local<Value>::New(Value::FromStack(ctx, -1));
    }
}

void Script::TryCompile() {
    Isolate *isolate = Isolate::GetCurrent();
    DukContextRef ctx = isolate->GetDukContext();

    DUK_STACK_SCOPE(ctx);

    source_->Push();
    if (file_name_.IsEmpty()) {
        duk_push_string(ctx, "");
    } else {
        file_name_->Push();
    }

    if (duk_pcompile(ctx, DUK_COMPILE_EVAL) == DUK_ERR_NONE) {
        // success
        // duk_call(ctx, 0);      /* [ func ] -> [ result ] */
        // printf("Compile Type: %d\n", duk_get_type(ctx, -1));
        function_ = Handle<Function>(
                (new Function)->Init(ctx, duk_get_heapptr(ctx, -1)));
        ASSERT(2 == function_->GetRefCount());
    } else {
        // failure
        printf("compile failed: %s\n", duk_safe_to_string(ctx, -1));

        // 获得最顶端的 TryCatch 对象
        // TryCatch *try_catch = Context::GetCurrent()->GetTopTryCatch();
        // try_catch->setErrorObject();
    }
}

}
