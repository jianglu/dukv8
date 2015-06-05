//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_AUTORELEASEPOOL_H
#define DUKV8_AUTORELEASEPOOL_H

#include <vector>
#include "base.h"

namespace v8 {
namespace internal {

class AutoReleasePool {
public:
    ~AutoReleasePool() {
        Release();
    }

    void AutoRelease(GCObject *gcobject);

    void Release();

private:
    std::vector<GCObject *> objects_;
};

}
}

#endif //DUKV8_AUTORELEASEPOOL_H
