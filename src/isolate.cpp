//
// Created by Jiang Lu on 6/2/15.
//

#include <cassert>
#include "dukv8/isolate.h"

namespace v8 {

Isolate *Isolate::default_isolate_ = NULL;

i::Thread::LocalStorageKey Isolate::isolate_key_;

// Returns the isolate inside which the current thread is running.
Isolate *Isolate::Current() {
    Isolate *isolate = reinterpret_cast<Isolate *>(i::Thread::GetExistingThreadLocal(isolate_key_));
    ASSERT(isolate != NULL);
    return isolate;
}

Isolate *Isolate::UncheckedCurrent() {
    return reinterpret_cast<Isolate *>(i::Thread::GetThreadLocal(isolate_key_));
}

bool Isolate::Init() {
    ASSERT(state_ != INITIALIZED);
    ASSERT(Isolate::Current() == this);

    // GlobalHandles
    // HeapProfiler::SetUp();

    state_ = INITIALIZED;
    return true;
}

void Isolate::Deinit() {
    if (state_ == INITIALIZED) {
        //HeapProfiler::TearDown();

        // The default isolate is re-initializable due to legacy API.
        state_ = UNINITIALIZED;
    }
}

void Isolate::TearDown() {
    Deinit();
    Isolate *saved_isolate = previous_isolate_;
    if (!IsDefaultIsolate()) {
        delete this;
    }
    i::Thread::SetThreadLocal(isolate_key_, saved_isolate);
}

void Isolate::EnsureDefaultIsolate() {
    // !! No Thread Safe !!
    // ScopedLock lock(process_wide_mutex_);

    if (default_isolate_ == NULL) {
        isolate_key_ = i::Thread::CreateThreadLocalKey();
        default_isolate_ = new Isolate();
    }

    // Can't use SetIsolateThreadLocals(default_isolate_, NULL) here
    // because a non-null thread data may be already set.
    if (i::Thread::GetThreadLocal(isolate_key_) == NULL) {
        i::Thread::SetThreadLocal(isolate_key_, default_isolate_);
    }
}

void Isolate::EnterDefaultIsolate() {
    EnsureDefaultIsolate();
    ASSERT(default_isolate_ != NULL);
    default_isolate_->Enter();
}

Isolate::Isolate() :
        state_(UNINITIALIZED),
        previous_isolate_(NULL),
        data_(NULL),
        auto_release_pool_(new i::AutoReleasePool) {
    duk_ctx_ = duk_create_heap_default();
}

Isolate::~Isolate() {
    delete auto_release_pool_;
    duk_destroy_heap(duk_ctx_);
}

/**
 * Creates a new isolate.  Does not change the currently entered
 * isolate.
 *
 * When an isolate is no longer used its resources should be freed
 * by calling Dispose().  Using the delete operator is not allowed.
 */
//Isolate *Isolate::New() {
//    return new Isolate();
//}

/**
 * Returns the entered isolate for the current thread or NULL in
 * case there is no current isolate.
 */
Isolate *Isolate::GetCurrent() {
    return Isolate::UncheckedCurrent();
}

/**
 * Custom callback used by embedders to help V8 determine if it should abort
 * when it throws and no internal handler can catch the exception.
 * If FLAG_abort_on_uncaught_exception is true, then V8 will abort if either:
 * - no custom callback is set.
 * - the custom callback set returns true.
 * Otherwise it won't abort.
 */
void Isolate::SetAbortOnUncaughtException(abort_on_uncaught_exception_t callback) {
}

/**
 * Methods below this point require holding a lock (using Locker) in
 * a multi-threaded environment.
 */

/**
 * Sets this isolate as the entered one for the current thread.
 * Saves the previously entered one (if any), so that it can be
 * restored when exiting.  Re-entering an isolate is allowed.
 */
void Isolate::Enter() {
    previous_isolate_ = Isolate::UncheckedCurrent();
    i::Thread::SetThreadLocal(isolate_key_, this);
}

/**
 * Exits this isolate by restoring the previously entered one in the
 * current thread.  The isolate may still stay the same, if it was
 * entered more than once.
 *
 * Requires: this == Isolate::GetCurrent().
 */
void Isolate::Exit() {
    i::Thread::SetThreadLocal(isolate_key_, previous_isolate_);
}

/**
 * Disposes the isolate.  The isolate must not be entered by any
 * thread to be disposable.
 */
void Isolate::Dispose() {
    Isolate::TearDown();
}

}
