//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/boolean.h"
#include "dukv8/isolate.h"
#include "dukv8/handle.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Boolean, v8::Primitive);

Boolean *Boolean::Init(DukContextRef duk_ctx, bool value) {
    Primitive::Init(duk_ctx);
    value_ = value;
    return this;
}

bool Boolean::Value() const {
    return value_;
}

void Boolean::Push() const {
    duk_push_boolean(duk_ctx_, value_);
}

Handle<Boolean> Boolean::New(bool value) {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Boolean>((new Boolean)->Init(ctx, value));
}

}

