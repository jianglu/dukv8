//
// Created by Jiang Lu on 6/3/15.
//

#include <dukv8/Array.h>
#include <dukv8/Isolate.h>
#include <dukv8/Local.h>

namespace v8 {

RTTI_IMPLEMENT(v8::Array, v8::Object);

Array::Array(DukContextRef ctx, int length) :
        Object(ctx) {
}

/*static*/
Local<Array> Array::New(int length) {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<Array>::New(Handle<Array>(new Array(ctx, length)));
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
