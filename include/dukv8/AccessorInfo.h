//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_ACCESSORINFO_H
#define DUKV8_ACCESSORINFO_H

#include "Base.h"
#include "Handle.h"

namespace v8 {

/**
 * The information passed to an accessor callback about the context
 * of the property access.
 */
class V8EXPORT AccessorInfo {
public:
    AccessorInfo(Isolate *isolate,
                 Handle<Object> that,
                 Handle<Object> holder,
                 Handle<Value> data);

    ~AccessorInfo();

    Isolate *GetIsolate() const;
    Local<Value> Data() const;
    Local<Object> This() const;
    Local<Object> Holder() const;

private:
    Isolate *isolate_;
    Handle<Object> that_;
    Handle<Object> holder_;
    Handle<Value> data_;
};

}

#endif //DUKV8_ACCESSORINFO_H
