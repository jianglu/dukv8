//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_INTEGER_H
#define DUKV8_INTEGER_H

#include "Base.h"
#include "Number.h"

namespace v8 {

/**
 * A JavaScript value representing a signed integer.
 */
class Integer : public Number {
public:
    RTTI_DECLARE();

    V8EXPORT static Local<Integer> New(int32_t value);
    V8EXPORT static Local<Integer> NewFromUnsigned(uint32_t value);
    V8EXPORT static Local<Integer> New(int32_t value, Isolate *);
    V8EXPORT static Local<Integer> NewFromUnsigned(uint32_t value, Isolate *);
    V8EXPORT int64_t Value() const;
    static inline Integer *Cast(v8::Value *obj);

private:
    V8EXPORT Integer();
    V8EXPORT static void CheckCast(v8::Value *obj);

private:
    int64_t value_;
};

/**
 * A JavaScript value representing a 32-bit signed integer.
 */
class Int32 : public Integer {
public:
    RTTI_DECLARE();
    V8EXPORT int32_t Value() const;
private:
    V8EXPORT Int32();
};

/**
 * A JavaScript value representing a 32-bit unsigned integer.
 */
class Uint32 : public Integer {
public:
    RTTI_DECLARE();
    V8EXPORT uint32_t Value() const;
private:
    V8EXPORT Uint32();
};

}


#endif //DUKV8_INTEGER_H
