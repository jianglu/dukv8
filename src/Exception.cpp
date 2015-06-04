//
// Created by Jiang Lu on 6/3/15.
//

#include <dukv8/Exception.h>
#include <dukv8/Handle.h>
#include <dukv8/Local.h>
#include <dukv8/Value.h>
#include <dukv8/String.h>

namespace v8 {


/*static*/ Local<Value> Exception::RangeError(Handle<String> message) {
    return Local<Value>::New(message);
}

/*static*/ Local<Value> Exception::ReferenceError(Handle<String> message) {
    return Local<Value>::New(message);
}

/*static*/ Local<Value> Exception::SyntaxError(Handle<String> message) {
    return Local<Value>::New(message);
}

/*static*/ Local<Value> Exception::TypeError(Handle<String> message) {
    return Local<Value>::New(message);
}

/*static*/ Local<Value> Exception::Error(Handle<String> message) {
    return Local<Value>::New(message);
}

}