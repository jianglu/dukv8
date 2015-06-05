//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_GCOBJECT_H
#define DUKV8_GCOBJECT_H

#include "base.h"
#include "global_stash.h"

namespace v8 {
namespace internal {

class GCObject {
public:
    RTTI_DECLARE_ROOT(v8::internal::GCObject);

    GCObject();
    virtual ~GCObject();

    GCObject *Init();
    virtual void Deinit();

    void Retain();

    void Release();

    int GetRefCount() {
        return gcobject_ref_count_;
    }

private:
    void *gcobject_heap_ptr_;
    int gcobject_stash_index_;
    int gcobject_ref_count_;
};

}
}


#endif //DUKV8_GCOBJECT_H
