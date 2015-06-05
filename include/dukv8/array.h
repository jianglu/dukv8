//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_ARRAY_H
#define DUKV8_ARRAY_H

#include "base.h"
#include "object.h"

namespace v8 {

/**
 * An instance of the built-in array constructor (ECMA-262, 15.4.2).
 */
class Array : public Object {
public:
    RTTI_DECLARE();

    V8EXPORT uint32_t Length() const;

    /**
     * Clones an element at index |index|.  Returns an empty
     * handle if cloning fails (for any reason).
     */
    V8EXPORT Local<Object> CloneElementAt(uint32_t index);

    /**
     * Creates a JavaScript array with the given length. If the length
     * is negative the returned array will have length 0.
     */
    V8EXPORT static Local<Array> New(int length = 0);

    static Array *Cast(Value *obj);

private:
    V8EXPORT static void CheckCast(Value *obj);

protected:
    Array *Init(DukContextRef duk_ctx, int length);
};

}

#endif //DUKV8_ARRAY_H
