//
// Created by Jiang Lu on 6/2/15.
//

#ifndef DUKV8_DATA_H
#define DUKV8_DATA_H

#include "base.h"
#include "gcobject.h"

namespace v8 {

class Data : public i::GCObject {
public:
    RTTI_DECLARE();

    Data *Init() {
        i::GCObject::Init();
        return this;
    }

    virtual void Deinit() {
        i::GCObject::Deinit();
    }

protected:
    virtual ~Data() { }
};

}


#endif //DUKV8_DATA_H
