//
// Created by Jiang Lu on 6/2/15.
//

#include <dukv8/Primitive.h>
#include <dukv8/Handle.h>
#include <dukv8/Isolate.h>
#include <dukv8/Boolean.h>

namespace v8 {

RTTI_IMPLEMENT(v8::Primitive, v8::Value);

namespace internal {

class Undefined : public Primitive {
public:
    RTTI_DECLARE();

    Undefined(DukContextRef duk_ctx) : Primitive(duk_ctx) { }

    virtual void Push() const {
        duk_push_undefined(duk_ctx_);
    }
};

RTTI_IMPLEMENT(v8::internal::Undefined, v8::Primitive);

class Null : public Primitive {
public:
    RTTI_DECLARE();

    Null(DukContextRef duk_ctx) : Primitive(duk_ctx) { }

    virtual void Push() const {
        duk_push_null(duk_ctx_);
    }
};

RTTI_IMPLEMENT(v8::internal::Null, v8::Primitive);

}

Handle<Primitive> Undefined() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Primitive>(new i::Undefined(ctx));
}

Handle<Primitive> Null() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Primitive>(new i::Null(ctx));
}

Handle<Boolean> True() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Boolean>(new Boolean(ctx, true));
}

Handle<Boolean> False() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Handle<Boolean>(new Boolean(ctx, false));
}

}