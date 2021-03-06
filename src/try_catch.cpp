//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/try_catch.h"
#include "dukv8/local.h"
#include "dukv8/value.h"
#include "dukv8/message.h"

namespace v8 {

TryCatch::TryCatch() { }
TryCatch::~TryCatch() { }

bool TryCatch::HasCaught() const {
    return false;
}

bool TryCatch::CanContinue() const {
    TODO();
    return true;
}

Handle<Value> TryCatch::ReThrow() {
    TODO();
    return Handle<Value>();
}

Local<Value> TryCatch::Exception() const {
    TODO();
    return Local<Value>();
}

Local<Value> TryCatch::StackTrace() const {
    TODO();
    return Local<Value>();
}

Local<Message> TryCatch::Message() const {
    TODO();
    return Local<v8::Message>();
}

void TryCatch::Reset() {
    TODO();
}

void TryCatch::SetVerbose(bool value) {
    TODO();
}

void TryCatch::SetCaptureMessage(bool value) {
    TODO();
}

}
