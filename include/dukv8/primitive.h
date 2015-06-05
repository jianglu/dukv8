//
// Created by Jiang Lu on 6/2/15.
//

#ifndef DUKV8_PRIMITIVE_H
#define DUKV8_PRIMITIVE_H

#include "value.h"

namespace v8 {

/**
 * The superclass of primitive values.  See ECMA-262 4.3.2.
 */
class Primitive : public Value {
public:
    RTTI_DECLARE();

protected:
    Primitive(duk_context *duk_ctx) :
            Value(duk_ctx) { }
};

Handle<Primitive> V8EXPORT Undefined();
Handle<Primitive> V8EXPORT Null();
Handle<Boolean> V8EXPORT True();
Handle<Boolean> V8EXPORT False();

}


#endif //DUKV8_PRIMITIVE_H
