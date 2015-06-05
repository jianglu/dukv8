//
// Created by Jiang Lu on 6/5/15.
//

#ifndef DUKV8_PLATFORM_H
#define DUKV8_PLATFORM_H

#include <climits>
#include "base.h"

namespace v8 {
namespace internal {

class Thread {
public:
    // Opaque data type for thread-local storage keys.
    // LOCAL_STORAGE_KEY_MIN_VALUE and LOCAL_STORAGE_KEY_MAX_VALUE are specified
    // to ensure that enumeration type has correct value range (see Issue 830 for
    // more details).
    enum LocalStorageKey {
        LOCAL_STORAGE_KEY_MIN_VALUE = INT_MIN,
        LOCAL_STORAGE_KEY_MAX_VALUE = INT_MAX
    };

    static LocalStorageKey CreateThreadLocalKey();

    static inline void *GetExistingThreadLocal(LocalStorageKey key) {
        return GetThreadLocal(key);
    }

    static void *GetThreadLocal(LocalStorageKey key);

    static void SetThreadLocal(LocalStorageKey key, void *value);
};

}
}
#endif //DUKV8_PLATFORM_H
