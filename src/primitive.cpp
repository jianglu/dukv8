//
// Created by Jiang Lu on 6/2/15.
//

#include "dukv8/primitive.h"
#include "dukv8/handle.h"
#include "dukv8/isolate.h"
#include "dukv8/boolean.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Primitive, v8::Value);

Primitive *Primitive::Init(DukContextRef duk_ctx) {
    Value::Init(duk_ctx);
    return this;
}

namespace internal {

class Undefined : public Primitive {
public:
    RTTI_DECLARE();

    Undefined *Init(DukContextRef duk_ctx) {
        Primitive::Init(duk_ctx);
        return this;
    }

    virtual void Push() const {
        duk_push_undefined(duk_ctx_);
    }
};

RTTI_IMPLEMENT(v8::internal::Undefined, v8::Primitive);

class Null : public Primitive {
public:
    RTTI_DECLARE();

    Null *Init(DukContextRef duk_ctx) {
        Primitive::Init(duk_ctx);
        return this;
    }

    virtual void Push() const {
        duk_push_null(duk_ctx_);
    }
};

RTTI_IMPLEMENT(v8::internal::Null, v8::Primitive);

}

Handle<Primitive> Undefined() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Primitive>((new i::Undefined)->Init(ctx));
}

Handle<Primitive> Null() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Primitive>((new i::Null)->Init(ctx));
}

Handle<Boolean> True() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Boolean>(Boolean::Create(ctx, true));
}

Handle<Boolean> False() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Boolean>(Boolean::Create(ctx, false));
}

}