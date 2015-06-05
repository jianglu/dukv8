//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/auto_release_pool.h"
#include "dukv8/gcobject.h"

namespace v8 {
namespace internal {

void AutoReleasePool::AutoRelease(GCObject *gcobject) {
    gcobject->Retain();
    objects_.push_back(gcobject);
}

void AutoReleasePool::Release() {
    for (std::vector<i::GCObject *>::iterator it = objects_.begin();
         it != objects_.end(); ++it) {
        (*it)->Release();
    }
    objects_.clear();
}

}
}
