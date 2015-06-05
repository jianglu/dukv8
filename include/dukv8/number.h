//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_NUMBER_H
#define DUKV8_NUMBER_H

#include "base.h"
#include "primitive.h"

namespace v8 {

/**
 * A JavaScript number value (ECMA-262, 4.3.20)
 */
class Number : public Primitive {
public:
    RTTI_DECLARE();

    V8EXPORT double Value() const;

    V8EXPORT static Local<Number> New(double value);

    static Number *Cast(v8::Value *obj);

    virtual void Push() const;

    V8EXPORT static void CheckCast(v8::Value *value);

    static Number *Create(DukContextRef ctx, double value) {
        return (new Number)->Init(ctx, value);
    }
private:
    double value_;

protected:
    Number *Init(DukContextRef ctx, double value);
};

}

#endif //DUKV8_NUMBER_H
