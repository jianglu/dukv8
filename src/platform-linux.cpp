//
// Created by Jiang Lu on 6/5/15.
//

#include <pthread.h>
#include "dukv8/platform.h"

namespace v8 {
namespace internal {

Thread::LocalStorageKey Thread::CreateThreadLocalKey() {
    pthread_key_t key;
    int result = pthread_key_create(&key, NULL);
    // USE(result);
    ASSERT(result == 0);
    return static_cast<LocalStorageKey>(key);
}

void *Thread::GetThreadLocal(LocalStorageKey key) {
    pthread_key_t pthread_key = static_cast<pthread_key_t>(key);
    return pthread_getspecific(pthread_key);
}

void Thread::SetThreadLocal(LocalStorageKey key, void *value) {
    pthread_key_t pthread_key = static_cast<pthread_key_t>(key);
    pthread_setspecific(pthread_key, value);
}

}
}
