//
// Created by Jiang Lu on 6/3/15.
//

#include <dukv8/Date.h>
#include <dukv8/Isolate.h>
#include <dukv8/Local.h>
#include <dukv8/Value.h>

namespace v8 {

RTTI_IMPLEMENT(v8::Date, v8::Object);

Date::Date(DukContextRef ctx, double time) :
        Object(ctx) {
}

/*static*/ Local<Value> Date::New(double time) {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<Value>::New(Handle<Date>(new Date(ctx, time)));
}

}
