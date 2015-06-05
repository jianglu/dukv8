//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/array.h"
#include "dukv8/isolate.h"
#include "dukv8/local.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Array, v8::Object);

Array *Array::Init(DukContextRef duk_ctx, int length) {
    Object::Init(duk_ctx);
    return this;
}

/*static*/
Local<Array> Array::New(int length) {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<Array>::New(Handle<Array>((new Array)->Init(ctx, length)));
}

uint32_t Array::Length() const {
    return 0;
}

/*static*/
Array *Array::Cast(Value *value) {

    if (RTTI_IsKindOf(Array, value)) {
        return RTTI_StaticCast(Array, value);
    }

    if (value) {
        TODO();
    }
    return NULL;
}
}
