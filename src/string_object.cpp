//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/string_object.h"
#include "dukv8/isolate.h"
#include "dukv8/local.h"
#include "dukv8/string.h"

namespace v8 {

RTTI_IMPLEMENT(v8::StringObject, v8::Object);

Local<Value> StringObject::New(Handle<String> value) {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<Value>::New(Handle<Value>(
            (new StringObject)->Init(ctx, value)));
}

StringObject *StringObject::Init(DukContextRef duk_ctx, Handle<String> value) {
    Object::Init(duk_ctx);
    value->Push(); // [string]
    duk_to_object(duk_ctx, -1); // [object]

    // // !!TEST!!
    // duk_push_pointer(duk_ctx, this);
    // duk_put_prop_string(duk_ctx, -2, "\xff\xff" "cobj");
    //
    // auto finalizer = [](duk_context *ctx) {
    //     duk_get_prop_string(ctx, -1, "\xff\xff" "cobj");
    //     StringObject *thiz = (StringObject *) duk_to_pointer(ctx, -1);
    //     printf("StringObject~DuktapeObject(%p) Finalizer\n", thiz);
    //     duk_pop(ctx);
    //     return DUK_ERR_NONE;
    // };
    //
    // duk_push_c_lightfunc(duk_ctx, finalizer, 1, 2, 3);
    // duk_set_finalizer(duk_ctx, -2);

    AttachDuktapeObject(duk_get_heapptr(duk_ctx, -1));
    duk_pop(duk_ctx);
    return this;
}

}