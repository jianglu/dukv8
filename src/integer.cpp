//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/integer.h"
#include "dukv8/local.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Integer, v8::Number);

Local<Integer> Integer::New(int32_t value) {
    return Local<Integer>::New(Handle<Integer>());
}

Local<Integer> Integer::NewFromUnsigned(uint32_t value) {
    return Local<Integer>::New(Handle<Integer>());
}

Local<Integer> Integer::New(int32_t value, Isolate *) {
    return Integer::New(value);
}

Local<Integer> Integer::NewFromUnsigned(uint32_t value, Isolate *) {
    return Integer::NewFromUnsigned(value);
}

int64_t Integer::Value() const {
    return value_;
}

RTTI_IMPLEMENT(v8::Int32, v8::Integer);

}
