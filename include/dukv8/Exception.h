//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_EXCEPTION_H
#define DUKV8_EXCEPTION_H

#include "Base.h"
#include "Handle.h"

namespace v8 {

/**
 * Schedules an exception to be thrown when returning to JavaScript.  When an
 * exception has been scheduled it is illegal to invoke any JavaScript
 * operation; the caller must return immediately and only after the exception
 * has been handled does it become legal to invoke JavaScript operations.
 */
Handle<Value> V8EXPORT ThrowException(Handle<Value> exception);

/**
 * Create new error objects by calling the corresponding error object
 * constructor with the message.
 */
class V8EXPORT Exception {
public:
    static Local<Value> RangeError(Handle<String> message);
    static Local<Value> ReferenceError(Handle<String> message);
    static Local<Value> SyntaxError(Handle<String> message);
    static Local<Value> TypeError(Handle<String> message);
    static Local<Value> Error(Handle<String> message);
};

}

#endif //DUKV8_EXCEPTION_H
