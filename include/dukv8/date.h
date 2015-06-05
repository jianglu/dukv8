//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_DATE_H
#define DUKV8_DATE_H

#include "base.h"
#include "object.h"

namespace v8 {

class Date : public Object {
public:
    RTTI_DECLARE();

    V8EXPORT static Local<Value> New(double time);

    Date(DukContextRef ctx, double time);
};

}

#endif //DUKV8_DATE_H
