//
// Created by Jiang Lu on 6/2/15.
//

#ifndef DUKV8_ISOLATE_H
#define DUKV8_ISOLATE_H

#include <duktape.h>
#include "base.h"
#include "auto_release_pool.h"
#include "platform.h"

namespace v8 {

/**
 * Isolate represents an isolated instance of the V8 engine.  V8
 * isolates have completely separate states.  Objects from one isolate
 * must not be used in other isolates.  When V8 is initialized a
 * default isolate is implicitly created and entered.  The embedder
 * can create additional isolates and use them in parallel in multiple
 * threads.  An isolate can be entered by at most one thread at any
 * given time.  The Locker/Unlocker API must be used to synchronize.
 */
class V8EXPORT Isolate {
public:
    /**
     * Stack-allocated class which sets the isolate for all operations
     * executed within a local scope.
     */
    class V8EXPORT Scope {
    public:
        explicit Scope(Isolate *isolate) : isolate_(isolate) {
            isolate->Enter();
        }

        ~Scope() { isolate_->Exit(); }

    private:
        Isolate *const isolate_;

        // Prevent copying of Scope objects.
        Scope(const Scope &);
        Scope &operator=(const Scope &);
    };

    /**
     * Creates a new isolate.  Does not change the currently entered
     * isolate.
     *
     * When an isolate is no longer used its resources should be freed
     * by calling Dispose().  Using the delete operator is not allowed.
     */
    static Isolate *New();

    /**
     * Returns the entered isolate for the current thread or NULL in
     * case there is no current isolate.
     */
    static Isolate *GetCurrent();

    /**
     * Custom callback used by embedders to help V8 determine if it should abort
     * when it throws and no internal handler can catch the exception.
     * If FLAG_abort_on_uncaught_exception is true, then V8 will abort if either:
     * - no custom callback is set.
     * - the custom callback set returns true.
     * Otherwise it won't abort.
     */
    typedef bool (*abort_on_uncaught_exception_t)();
    void SetAbortOnUncaughtException(abort_on_uncaught_exception_t callback);

    /**
     * Methods below this point require holding a lock (using Locker) in
     * a multi-threaded environment.
     */

    /**
     * Sets this isolate as the entered one for the current thread.
     * Saves the previously entered one (if any), so that it can be
     * restored when exiting.  Re-entering an isolate is allowed.
     */
    void Enter();

    /**
     * Exits this isolate by restoring the previously entered one in the
     * current thread.  The isolate may still stay the same, if it was
     * entered more than once.
     *
     * Requires: this == Isolate::GetCurrent().
     */
    void Exit();

    /**
     * Disposes the isolate.  The isolate must not be entered by any
     * thread to be disposable.
     */
    void Dispose();

    /**
     * Associate embedder-specific data with the isolate
     */
    inline void SetData(void *data);

    /**
     * Retrieve embedder-specific data from the isolate.
     * Returns NULL if SetData has never been called.
     */
    inline void *GetData();

    /**
     * 直接通过 TLS 获得当前线程的 Isolate 实例
     */
    static Isolate *Current();
    static Isolate *UncheckedCurrent();

    static void EnsureDefaultIsolate();
    static void EnterDefaultIsolate();

    inline DukContextRef GetDukContext();

    inline i::AutoReleasePool *GetAutoReleasePool();

    bool Init();
    void Deinit();

    bool IsInitialized() { return state_ == INITIALIZED; }

    // Destroys the non-default isolates.
    // Sets default isolate into "has_been_disposed" state rather then destroying,
    // for legacy API reasons.
    void TearDown();

    bool IsDefaultIsolate() const { return this == default_isolate_; }

    Isolate();
    ~Isolate();

private:
    Isolate(const Isolate &);
    Isolate &operator=(const Isolate &);

private:
    enum State {
        UNINITIALIZED,    // Some components may not have been allocated.
        INITIALIZED       // All components are fully initialized.
    };

    State state_;

    static Isolate *default_isolate_;
    Isolate *previous_isolate_;
    void *data_;
    i::AutoReleasePool *auto_release_pool_;
    duk_context *duk_ctx_;

    static i::Thread::LocalStorageKey isolate_key_;
};

void Isolate::SetData(void *data) {
    data_ = data;
}

void *Isolate::GetData() {
    return data_;
}

DukContextRef Isolate::GetDukContext() {
    return duk_ctx_;
}

i::AutoReleasePool *Isolate::GetAutoReleasePool() {
    return auto_release_pool_;
}

}


#endif //DUKV8_ISOLATE_H
