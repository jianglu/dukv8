//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/number.h"
#include "dukv8/handle.h"
#include "dukv8/local.h"
#include "dukv8/isolate.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Number, v8::Primitive);

double Number::Value() const {
    return value_;
}

Local<Number> Number::New(double value) {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<Number>::New(Handle<Number>((new Number)->Init(ctx, value)));
}

Number *Number::Cast(v8::Value *obj) {
    if (RTTI_IsKindOf(Number, obj)) {
        return RTTI_StaticCast(Number, obj);
    }

    if (obj) {
        obj->Push();
        DukContextRef ctx = obj->GetDukContext();
        if (duk_is_number(ctx, -1)) {
            double value = duk_get_number(ctx, -1);
            return (new Number)->Init(ctx, value);
        }
    }
    return NULL;
}

void Number::Push() const {
    duk_push_number(duk_ctx_, value_);
}

Number *Number::Init(DukContextRef ctx, double value) {
    Primitive::Init(ctx);
    value_ = value;
    return this;
}

void Number::CheckCast(v8::Value *obj) {
}

}