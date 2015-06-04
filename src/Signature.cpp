//
// Created by Jiang Lu on 6/3/15.
//

#include <dukv8/Signature.h>
#include <dukv8/Local.h>

namespace v8 {

Local<Signature> Signature::New(Handle<FunctionTemplate> receiver,
                                int argc,
                                Handle<FunctionTemplate> argv[]) {
    TODO();
    return Local<Signature>();
}

}