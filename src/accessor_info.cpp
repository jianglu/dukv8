//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/accessor_info.h"
#include "dukv8/local.h"
#include "dukv8/integer.h"
#include "dukv8/object.h"

namespace v8 {

AccessorInfo::AccessorInfo(Isolate *isolate,
                           Handle<Object> that,
                           Handle<Object> holder,
                           Handle<Value> data) :
        isolate_(isolate),
        that_(that),
        holder_(holder),
        data_(data) {
}

AccessorInfo::~AccessorInfo() {
}

Isolate *AccessorInfo::GetIsolate() const {
    return isolate_;
}

Local<Value> AccessorInfo::Data() const {
    return Local<Value>::New(data_);
}

Local<Object> AccessorInfo::This() const {
    return Local<Object>::New(that_);
}

Local<Object> AccessorInfo::Holder() const {
    return Local<Object>::New(holder_);
}

}