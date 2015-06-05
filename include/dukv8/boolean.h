//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_BOOLEAN_H
#define DUKV8_BOOLEAN_H

#include "base.h"
#include "primitive.h"

namespace v8 {

class V8EXPORT Boolean : public Primitive {
public:
    RTTI_DECLARE();

    V8EXPORT bool Value() const;

    static Handle<Boolean> New(bool value);

    virtual void Push() const;

    static Boolean *Create(DukContextRef duk_ctx, bool value) {
        return (new Boolean)->Init(duk_ctx, value);
    }

private:
    bool value_;

protected:
    Boolean *Init(DukContextRef duk_ctx, bool value);

    friend class Primitive;
};

}

#endif //DUKV8_BOOLEAN_H
