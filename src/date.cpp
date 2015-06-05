//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/date.h"
#include "dukv8/isolate.h"
#include "dukv8/local.h"
#include "dukv8/value.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Date, v8::Object);

Date *Date::Init(DukContextRef ctx, double time) {
    Object::Init(ctx);
    return this;
}

/*static*/ Local<Value> Date::New(double time) {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<Value>::New(Handle<Date>((new Date)->Init(ctx, time)));
}

}
