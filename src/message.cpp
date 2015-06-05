//
// Created by Jiang Lu on 6/2/15.
//

#include "dukv8/message.h"
#include "dukv8/local.h"
#include "dukv8/value.h"
#include "dukv8/string.h"
#include "dukv8/stack_trace.h"

namespace v8 {


Local<String> Message::Get() const {
    TODO();
    return Local<String>();
}

Local<String> Message::GetSourceLine() const {
    TODO();
    return Local<String>();
}

Handle<Value> Message::GetScriptResourceName() const {
    TODO();
    return Handle<Value>();
}

Handle<Value> Message::GetScriptData() const {
    TODO();
    return Handle<Value>();
}

Handle<StackTrace> Message::GetStackTrace() const {
    TODO();
    return Handle<StackTrace>();
}

int Message::GetLineNumber() const {
    TODO();
    return 0;
}

int Message::GetStartPosition() const {
    TODO();
    return 0;
}

int Message::GetEndPosition() const {
    TODO();
    return 0;
}

int Message::GetStartColumn() const {
    TODO();
    return 0;
}

int Message::GetEndColumn() const {
    TODO();
    return 0;
}

/*static*/ void Message::PrintCurrentStackTrace(FILE *out) {
    TODO();
}

}