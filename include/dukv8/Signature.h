//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_SIGNATURE_H
#define DUKV8_SIGNATURE_H

#include "Base.h"
#include "Data.h"
#include "Handle.h"

namespace v8 {

/**
 * A Signature specifies which receivers and arguments are valid
 * parameters to a function.
 */
class V8EXPORT Signature : public Data {
public:
    static Local<Signature> New(
            Handle<FunctionTemplate> receiver = Handle<FunctionTemplate>(),
            int argc = 0,
            Handle<FunctionTemplate> argv[] = 0);
private:
    Signature();
};

}

#endif //DUKV8_SIGNATURE_H
