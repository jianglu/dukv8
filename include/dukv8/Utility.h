//
// Created by Jiang Lu on 6/2/15.
//

#ifndef DUKV8_UTILITY_H
#define DUKV8_UTILITY_H

#include "Base.h"
#include "Handle.h"
#include "Value.h"
#include "Message.h"
#include "StackTrace.h"
#include "String.h"

namespace v8 {

namespace internal {
class Object;
}

/**
 * Multiple threads in V8 are allowed, but only one thread at a time
 * is allowed to use any given V8 isolate. See Isolate class
 * comments. The definition of 'using V8 isolate' includes
 * accessing handles or holding onto object pointers obtained
 * from V8 handles while in the particular V8 isolate.  It is up
 * to the user of V8 to ensure (perhaps with locking) that this
 * constraint is not violated.  In addition to any other synchronization
 * mechanism that may be used, the v8::Locker and v8::Unlocker classes
 * must be used to signal thead switches to V8.
 *
 * v8::Locker is a scoped lock object. While it's
 * active (i.e. between its construction and destruction) the current thread is
 * allowed to use the locked isolate. V8 guarantees that an isolate can be
 * locked by at most one thread at any time. In other words, the scope of a
 * v8::Locker is a critical section.
 *
 * Sample usage:
* \code
 * ...
 * {
 *   v8::Locker locker(isolate);
 *   v8::Isolate::Scope isolate_scope(isolate);
 *   ...
 *   // Code using V8 and isolate goes here.
 *   ...
 * } // Destructor called here
 * \endcode
 *
 * If you wish to stop using V8 in a thread A you can do this either
 * by destroying the v8::Locker object as above or by constructing a
 * v8::Unlocker object:
 *
 * \code
 * {
 *   isolate->Exit();
 *   v8::Unlocker unlocker(isolate);
 *   ...
 *   // Code not using V8 goes here while V8 can run in another thread.
 *   ...
 * } // Destructor called here.
 * isolate->Enter();
 * \endcode
 *
 * The Unlocker object is intended for use in a long-running callback
 * from V8, where you want to release the V8 lock for other threads to
 * use.
 *
 * The v8::Locker is a recursive lock.  That is, you can lock more than
 * once in a given thread.  This can be useful if you have code that can
 * be called either from code that holds the lock or from code that does
 * not.  The Unlocker is not recursive so you can not have several
 * Unlockers on the stack at once, and you can not use an Unlocker in a
 * thread that is not inside a Locker's scope.
 *
 * An unlocker will unlock several lockers if it has to and reinstate
 * the correct depth of locking on its destruction. eg.:
 *
 * \code
 * // V8 not locked.
 * {
 *   v8::Locker locker(isolate);
 *   Isolate::Scope isolate_scope(isolate);
 *   // V8 locked.
 *   {
 *     v8::Locker another_locker(isolate);
 *     // V8 still locked (2 levels).
 *     {
 *       isolate->Exit();
 *       v8::Unlocker unlocker(isolate);
 *       // V8 not locked.
 *     }
 *     isolate->Enter();
 *     // V8 locked again (2 levels).
 *   }
 *   // V8 still locked (1 level).
 * }
 * // V8 Now no longer locked.
 * \endcode
 *
 *
 */
class V8EXPORT Unlocker {
public:
    /**
     * Initialize Unlocker for a given Isolate. NULL means default isolate.
     */
    explicit Unlocker(Isolate *isolate = NULL) { }
    ~Unlocker() { }
};


class V8EXPORT Locker {
public:
    /**
     * Initialize Locker for a given Isolate. NULL means default isolate.
     */
    explicit Locker(Isolate *isolate = NULL) { }
    ~Locker() { }

    /**
     * Start preemption.
     *
     * When preemption is started, a timer is fired every n milliseconds
     * that will switch between multiple threads that are in contention
     * for the V8 lock.
     */
    static void StartPreemption(int every_n_ms);

    /**
     * Stop preemption.
     */
    static void StopPreemption();

    /**
     * Returns whether or not the locker for a given isolate, or default isolate
     * if NULL is given, is locked by the current thread.
     */
    static bool IsLocked(Isolate *isolate = NULL);

    /**
     * Returns whether v8::Locker is being used by this V8 instance.
     */
    static bool IsActive();
};

/**
 * A set of constraints that specifies the limits of the runtime's memory use.
 * You must set the heap size before initializing the VM - the size cannot be
 * adjusted after the VM is initialized.
 *
 * If you are using threads then you should hold the V8::Locker lock while
 * setting the stack limit and you must set a non-default stack limit separately
 * for each thread.
 */
class V8EXPORT ResourceConstraints {
public:
    ResourceConstraints();
    int max_young_space_size() const { return max_young_space_size_; }
    void set_max_young_space_size(int value) { max_young_space_size_ = value; }
    int max_old_space_size() const { return max_old_space_size_; }
    void set_max_old_space_size(int value) { max_old_space_size_ = value; }
    int max_executable_size() { return max_executable_size_; }
    void set_max_executable_size(int value) { max_executable_size_ = value; }
    uint32_t *stack_limit() const { return stack_limit_; }
    // Sets an address beyond which the VM's stack may not grow.
    void set_stack_limit(uint32_t *value) { stack_limit_ = value; }
private:
    int max_young_space_size_;
    int max_old_space_size_;
    int max_executable_size_;
    uint32_t *stack_limit_;
};

bool V8EXPORT SetResourceConstraints(ResourceConstraints *constraints);

// --- Extensions ---

class V8EXPORT ExternalAsciiStringResourceImpl
        : public String::ExternalAsciiStringResource {
public:
    ExternalAsciiStringResourceImpl() : data_(0), length_(0) { }
    ExternalAsciiStringResourceImpl(const char *data, size_t length)
            : data_(data), length_(length) { }
    const char *data() const { return data_; }
    size_t length() const { return length_; }

private:
    const char *data_;
    size_t length_;
};

typedef void (*FatalErrorCallback)(const char *location, const char *message);

typedef void (*MessageCallback)(Handle<Message> message, Handle<Value> error);

// --- Leave Script Callback ---
typedef void (*CallCompletedCallback)();

// --- Failed Access Check Callback ---
//typedef void (*FailedAccessCheckCallback)(Local < Object > target,
//                                          AccessType type,
//                                            Local <Value> data);

// --- AllowCodeGenerationFromStrings callbacks ---

/**
 * Callback to check if code generation from strings is allowed. See
 * Context::AllowCodeGenerationFromStrings.
 */
//typedef bool (*AllowCodeGenerationFromStringsCallback)(Local < Context > context);

class StartupData {
public:
    enum CompressionAlgorithm {
        kUncompressed,
        kBZip2
    };

    const char *data;
    int compressed_size;
    int raw_size;
};

typedef int *(*CounterLookupCallback)(const char *name);

typedef void *(*CreateHistogramCallback)(const char *name,
                                         int min,
                                         int max,
                                         size_t buckets);

typedef void (*AddHistogramSampleCallback)(void *histogram, int sample);

// --- Garbage Collection Callbacks ---

/**
 * Applications can register callback functions which will be called
 * before and after a garbage collection.  Allocations are not
 * allowed in the callback functions, you therefore cannot manipulate
 * objects (set or delete properties for example) since it is possible
 * such operations will result in the allocation of objects.
 */
enum GCType {
    kGCTypeScavenge = 1 << 0,
    kGCTypeMarkSweepCompact = 1 << 1,
    kGCTypeAll = kGCTypeScavenge | kGCTypeMarkSweepCompact
};

enum GCCallbackFlags {
    kNoGCCallbackFlags = 0,
    kGCCallbackFlagCompacted = 1 << 0
};

typedef void (*GCPrologueCallback)(GCType type, GCCallbackFlags flags);
typedef void (*GCEpilogueCallback)(GCType type, GCCallbackFlags flags);

typedef void (*GCCallback)();


// --- Memory Allocation Callback ---
enum ObjectSpace {
    kObjectSpaceNewSpace = 1 << 0,
    kObjectSpaceOldPointerSpace = 1 << 1,
    kObjectSpaceOldDataSpace = 1 << 2,
    kObjectSpaceCodeSpace = 1 << 3,
    kObjectSpaceMapSpace = 1 << 4,
    kObjectSpaceLoSpace = 1 << 5,

    kObjectSpaceAll = kObjectSpaceNewSpace | kObjectSpaceOldPointerSpace |
                      kObjectSpaceOldDataSpace | kObjectSpaceCodeSpace | kObjectSpaceMapSpace |
                      kObjectSpaceLoSpace
};

enum AllocationAction {
    kAllocationActionAllocate = 1 << 0,
    kAllocationActionFree = 1 << 1,
    kAllocationActionAll = kAllocationActionAllocate | kAllocationActionFree
};

typedef void (*MemoryAllocationCallback)(ObjectSpace space,
                                         AllocationAction action,
                                         int size);


class RetainedObjectInfo;

/**
 * Interface for controlling heap profiling.
 */
class V8EXPORT HeapProfiler {
public:
    /**
     * Callback function invoked for obtaining RetainedObjectInfo for
     * the given JavaScript wrapper object. It is prohibited to enter V8
     * while the callback is running: only getters on the handle and
     * GetPointerFromInternalField on the objects are allowed.
     */
    typedef RetainedObjectInfo *(*WrapperInfoCallback)
            (uint16_t class_id, Handle<Value> wrapper);

    /** Returns the number of snapshots taken. */
//    static int GetSnapshotsCount();

    /** Returns a snapshot by index. */
//    static const HeapSnapshot *GetSnapshot(int index);

    /** Returns a profile by uid. */
//    static const HeapSnapshot *FindSnapshot(unsigned uid);

    /**
     * Returns SnapshotObjectId for a heap object referenced by |value| if
     * it has been seen by the heap profiler, kUnknownObjectId otherwise.
     */
//    static SnapshotObjectId GetSnapshotObjectId(Handle<Value> value);

    /**
     * A constant for invalid SnapshotObjectId. GetSnapshotObjectId will return
     * it in case heap profiler cannot find id  for the object passed as
     * parameter. HeapSnapshot::GetNodeById will always return NULL for such id.
     */
//    static const SnapshotObjectId kUnknownObjectId = 0;

    /**
     * Takes a heap snapshot and returns it. Title may be an empty string.
     * See HeapSnapshot::Type for types description.
     */
//    static const HeapSnapshot *TakeSnapshot(
//            Handle<String> title,
//            HeapSnapshot::Type type = HeapSnapshot::kFull,
//            ActivityControl *control = NULL);

    /**
     * Starts tracking of heap objects population statistics. After calling
     * this method, all heap objects relocations done by the garbage collector
     * are being registered.
     */
//    static void StartHeapObjectsTracking();

    /**
     * Adds a new time interval entry to the aggregated statistics array. The
     * time interval entry contains information on the current heap objects
     * population size. The method also updates aggregated statistics and
     * reports updates for all previous time intervals via the OutputStream
     * object. Updates on each time interval are provided as a stream of the
     * HeapStatsUpdate structure instances.
     * The return value of the function is the last seen heap object Id.
     *
     * StartHeapObjectsTracking must be called before the first call to this
     * method.
     */
//    static SnapshotObjectId PushHeapObjectsStats(OutputStream *stream);

    /**
     * Stops tracking of heap objects population statistics, cleans up all
     * collected data. StartHeapObjectsTracking must be called again prior to
     * calling PushHeapObjectsStats next time.
     */
//    static void StopHeapObjectsTracking();

    /**
     * Deletes all snapshots taken. All previously returned pointers to
     * snapshots and their contents become invalid after this call.
     */
//    static void DeleteAllSnapshots();

    /** Binds a callback to embedder's class ID. */
    static void DefineWrapperClass(
            uint16_t class_id,
            WrapperInfoCallback callback);

    /**
     * Default value of persistent handle class ID. Must not be used to
     * define a class. Can be used to reset a class of a persistent
     * handle.
     */
//    static const uint16_t kPersistentHandleNoClassId = 0;

    /** Returns the number of currently existing persistent handles. */
//    static int GetPersistentHandleCount();

    /** Returns memory used for profiler internal data and snapshots. */
//    static size_t GetMemorySizeUsedByProfiler();
};


/**
 * Interface for providing information about embedder's objects
 * held by global handles. This information is reported in two ways:
 *
 *  1. When calling AddObjectGroup, an embedder may pass
 *     RetainedObjectInfo instance describing the group.  To collect
 *     this information while taking a heap snapshot, V8 calls GC
 *     prologue and epilogue callbacks.
 *
 *  2. When a heap snapshot is collected, V8 additionally
 *     requests RetainedObjectInfos for persistent handles that
 *     were not previously reported via AddObjectGroup.
 *
 * Thus, if an embedder wants to provide information about native
 * objects for heap snapshots, he can do it in a GC prologue
 * handler, and / or by assigning wrapper class ids in the following way:
 *
 *  1. Bind a callback to class id by calling SetWrapperClassInfoProvider.
 *  2. Call SetWrapperClassId on certain persistent handles.
 *
 * V8 takes ownership of RetainedObjectInfo instances passed to it and
 * keeps them alive only during snapshot collection. Afterwards, they
 * are freed by calling the Dispose class function.
 */
class V8_EXPORT RetainedObjectInfo {  // NOLINT
public:
    /** Called by V8 when it no longer needs an instance. */
    virtual void Dispose() = 0;

    /** Returns whether two instances are equivalent. */
    virtual bool IsEquivalent(RetainedObjectInfo *other) = 0;

    /**
     * Returns hash value for the instance. Equivalent instances
     * must have the same hash value.
     */
    virtual intptr_t GetHash() = 0;

    /**
     * Returns human-readable label. It must be a null-terminated UTF-8
     * encoded string. V8 copies its contents during a call to GetLabel.
     */
    virtual const char *GetLabel() = 0;

    /**
     * Returns human-readable group label. It must be a null-terminated UTF-8
     * encoded string. V8 copies its contents during a call to GetGroupLabel.
     * Heap snapshot generator will collect all the group names, create
     * top level entries with these names and attach the objects to the
     * corresponding top level group objects. There is a default
     * implementation which is required because embedders don't have their
     * own implementation yet.
     */
    virtual const char *GetGroupLabel() { return GetLabel(); }

    /**
     * Returns element count in case if a global handle retains
     * a subgraph by holding one of its nodes.
     */
    virtual intptr_t GetElementCount() { return -1; }

    /** Returns embedder's object size in bytes. */
    virtual intptr_t GetSizeInBytes() { return -1; }

protected:
    RetainedObjectInfo() { }
    virtual ~RetainedObjectInfo() { }

private:
    RetainedObjectInfo(const RetainedObjectInfo &);
    RetainedObjectInfo &operator=(const RetainedObjectInfo &);
};

/**
 * EntropySource is used as a callback function when v8 needs a source
 * of entropy.
 */
typedef bool (*EntropySource)(unsigned char *buffer, size_t length);

/**
 * Container class for static utility functions.
 */
class V8EXPORT V8 {
public:
    /** Set the callback to invoke in case of fatal errors. */
    static void SetFatalErrorHandler(FatalErrorCallback that);

    /**
     * Set the callback to invoke to check if code generation from
     * strings should be allowed.
     */
//    static void SetAllowCodeGenerationFromStringsCallback(
//            AllowCodeGenerationFromStringsCallback that);

    /**
     * Ignore out-of-memory exceptions.
     *
     * V8 running out of memory is treated as a fatal error by default.
     * This means that the fatal error handler is called and that V8 is
     * terminated.
     *
     * IgnoreOutOfMemoryException can be used to not treat an
     * out-of-memory situation as a fatal error.  This way, the contexts
     * that did not cause the out of memory problem might be able to
     * continue execution.
     */
//    static void IgnoreOutOfMemoryException();

    /**
     * Check if V8 is dead and therefore unusable.  This is the case after
     * fatal errors such as out-of-memory situations.
     */
    static bool IsDead();

    /**
     * The following 4 functions are to be used when V8 is built with
     * the 'compress_startup_data' flag enabled. In this case, the
     * embedder must decompress startup data prior to initializing V8.
     *
     * This is how interaction with V8 should look like:
     *   int compressed_data_count = v8::V8::GetCompressedStartupDataCount();
     *   v8::StartupData* compressed_data =
     *     new v8::StartupData[compressed_data_count];
     *   v8::V8::GetCompressedStartupData(compressed_data);
     *   ... decompress data (compressed_data can be updated in-place) ...
     *   v8::V8::SetDecompressedStartupData(compressed_data);
     *   ... now V8 can be initialized
     *   ... make sure the decompressed data stays valid until V8 shutdown
     *
     * A helper class StartupDataDecompressor is provided. It implements
     * the protocol of the interaction described above, and can be used in
     * most cases instead of calling these API functions directly.
     */
//    static StartupData::CompressionAlgorithm GetCompressedStartupDataAlgorithm();
//    static int GetCompressedStartupDataCount();
//    static void GetCompressedStartupData(StartupData *compressed_data);
//    static void SetDecompressedStartupData(StartupData *decompressed_data);

    /**
     * Adds a message listener.
     *
     * The same message listener can be added more than once and in that
     * case it will be called more than once for each message.
     *
     * If data is specified, it will be passed to the callback when it is called.
     * Otherwise, the exception object will be passed to the callback instead.
     */
//    static bool AddMessageListener(MessageCallback that,
//                                   Handle<Value> data = Handle<Value>());

    /**
     * Remove all message listeners from the specified callback function.
     */
//    static void RemoveMessageListeners(MessageCallback that);

    /**
     * Tells V8 to capture current stack trace when uncaught exception occurs
     * and report it to the message listeners. The option is off by default.
     */
//    static void SetCaptureStackTraceForUncaughtExceptions(
//            bool capture,
//            int frame_limit = 10,
//            StackTrace::StackTraceOptions options = StackTrace::kOverview);

    /**
     * Sets V8 flags from a string.
     */
//    static void SetFlagsFromString(const char *str, int length);

    /**
     * Sets V8 flags from the command line.
     */
    static void SetFlagsFromCommandLine(int *argc,
                                        char **argv,
                                        bool remove_flags);

    /** Get the version string. */
    static const char *GetVersion();

    /**
     * Enables the host application to provide a mechanism for recording
     * statistics counters.
     */
//    static void SetCounterFunction(CounterLookupCallback);

    /**
     * Enables the host application to provide a mechanism for recording
     * histograms. The CreateHistogram function returns a
     * histogram which will later be passed to the AddHistogramSample
     * function.
     */
//    static void SetCreateHistogramFunction(CreateHistogramCallback);
//    static void SetAddHistogramSampleFunction(AddHistogramSampleCallback);

    /**
     * Enables the computation of a sliding window of states. The sliding
     * window information is recorded in statistics counters.
     */
//    static void EnableSlidingStateWindow();

    /** Callback function for reporting failed access checks.*/
//    static void SetFailedAccessCheckCallbackFunction(FailedAccessCheckCallback);

    /**
     * Enables the host application to receive a notification before a
     * garbage collection.  Allocations are not allowed in the
     * callback function, you therefore cannot manipulate objects (set
     * or delete properties for example) since it is possible such
     * operations will result in the allocation of objects. It is possible
     * to specify the GCType filter for your callback. But it is not possible to
     * register the same callback function two times with different
     * GCType filters.
     */
    static void AddGCPrologueCallback(
            GCPrologueCallback callback, GCType gc_type_filter = kGCTypeAll);

    /**
     * This function removes callback which was installed by
     * AddGCPrologueCallback function.
     */
//    static void RemoveGCPrologueCallback(GCPrologueCallback callback);

    /**
     * The function is deprecated. Please use AddGCPrologueCallback instead.
     * Enables the host application to receive a notification before a
     * garbage collection.  Allocations are not allowed in the
     * callback function, you therefore cannot manipulate objects (set
     * or delete properties for example) since it is possible such
     * operations will result in the allocation of objects.
     */
//    static void SetGlobalGCPrologueCallback(GCCallback);

    /**
     * Enables the host application to receive a notification after a
     * garbage collection.  Allocations are not allowed in the
     * callback function, you therefore cannot manipulate objects (set
     * or delete properties for example) since it is possible such
     * operations will result in the allocation of objects. It is possible
     * to specify the GCType filter for your callback. But it is not possible to
     * register the same callback function two times with different
     * GCType filters.
     */
    static void AddGCEpilogueCallback(
            GCEpilogueCallback callback, GCType gc_type_filter = kGCTypeAll);

    /**
     * This function removes callback which was installed by
     * AddGCEpilogueCallback function.
     */
//    static void RemoveGCEpilogueCallback(GCEpilogueCallback callback);

    /**
     * The function is deprecated. Please use AddGCEpilogueCallback instead.
     * Enables the host application to receive a notification after a
     * major garbage collection.  Allocations are not allowed in the
     * callback function, you therefore cannot manipulate objects (set
     * or delete properties for example) since it is possible such
     * operations will result in the allocation of objects.
     */
//    static void SetGlobalGCEpilogueCallback(GCCallback);

    /**
     * Enables the host application to provide a mechanism to be notified
     * and perform custom logging when V8 Allocates Executable Memory.
     */
//    static void AddMemoryAllocationCallback(MemoryAllocationCallback callback,
//                                            ObjectSpace space,
//                                            AllocationAction action);

    /**
     * Removes callback that was installed by AddMemoryAllocationCallback.
     */
//    static void RemoveMemoryAllocationCallback(MemoryAllocationCallback callback);

    /**
     * Adds a callback to notify the host application when a script finished
     * running.  If a script re-enters the runtime during executing, the
     * CallCompletedCallback is only invoked when the outer-most script
     * execution ends.  Executing scripts inside the callback do not trigger
     * further callbacks.
     */
//    static void AddCallCompletedCallback(CallCompletedCallback callback);

    /**
     * Removes callback that was installed by AddCallCompletedCallback.
     */
//    static void RemoveCallCompletedCallback(CallCompletedCallback callback);

    /**
     * Allows the host application to group objects together. If one
     * object in the group is alive, all objects in the group are alive.
     * After each garbage collection, object groups are removed. It is
     * intended to be used in the before-garbage-collection callback
     * function, for instance to simulate DOM tree connections among JS
     * wrapper objects.
     * See v8-profiler.h for RetainedObjectInfo interface description.
     */
//    static void AddObjectGroup(Persistent<Value> *objects,
//                               size_t length,
//                               RetainedObjectInfo *info = NULL);

    /**
     * Allows the host application to declare implicit references between
     * the objects: if |parent| is alive, all |children| are alive too.
     * After each garbage collection, all implicit references
     * are removed.  It is intended to be used in the before-garbage-collection
     * callback function.
     */
//    static void AddImplicitReferences(Persistent<Object> parent,
//                                      Persistent<Value> *children,
//                                      size_t length);

    /**
     * Initializes from snapshot if possible. Otherwise, attempts to
     * initialize from scratch.  This function is called implicitly if
     * you use the API without calling it first.
     */
    static bool Initialize();

    /**
     * Allows the host application to provide a callback which can be used
     * as a source of entropy for random number generators.
     */
    static void SetEntropySource(EntropySource source);

    /**
     * Allows the host application to provide a callback that allows v8 to
     * cooperate with a profiler that rewrites return addresses on stack.
     */
//    static void SetReturnAddressLocationResolver(
//            ReturnAddressLocationResolver return_address_resolver);

    /**
     * Allows the host application to provide the address of a function that's
     * invoked on entry to every V8-generated function.
     * Note that \p entry_hook is invoked at the very start of each
     * generated function.
     *
     * \param entry_hook a function that will be invoked on entry to every
     *   V8-generated function.
     * \returns true on success on supported platforms, false on failure.
     * \note Setting a new entry hook function when one is already active will
     *   fail.
     */
//    static bool SetFunctionEntryHook(FunctionEntryHook entry_hook);

    /**
     * Allows the host application to provide the address of a function that is
     * notified each time code is added, moved or removed.
     *
     * \param options options for the JIT code event handler.
     * \param event_handler the JIT code event handler, which will be invoked
     *     each time code is added, moved or removed.
     * \note \p event_handler won't get notified of existent code.
     * \note since code removal notifications are not currently issued, the
     *     \p event_handler may get notifications of code that overlaps earlier
     *     code notifications. This happens when code areas are reused, and the
     *     earlier overlapping code areas should therefore be discarded.
     * \note the events passed to \p event_handler and the strings they point to
     *     are not guaranteed to live past each call. The \p event_handler must
     *     copy strings and other parameters it needs to keep around.
     * \note the set of events declared in JitCodeEvent::EventType is expected to
     *     grow over time, and the JitCodeEvent structure is expected to accrue
     *     new members. The \p event_handler function must ignore event codes
     *     it does not recognize to maintain future compatibility.
     */
//    static void SetJitCodeEventHandler(JitCodeEventOptions options,
//                                       JitCodeEventHandler event_handler);

    /**
     * Adjusts the amount of registered external memory.  Used to give
     * V8 an indication of the amount of externally allocated memory
     * that is kept alive by JavaScript objects.  V8 uses this to decide
     * when to perform global garbage collections.  Registering
     * externally allocated memory will trigger global garbage
     * collections more often than otherwise in an attempt to garbage
     * collect the JavaScript objects keeping the externally allocated
     * memory alive.
     *
     * \param change_in_bytes the change in externally allocated memory
     *   that is kept alive by JavaScript objects.
     * \returns the adjusted value.
     */
    static intptr_t AdjustAmountOfExternalAllocatedMemory(
            intptr_t change_in_bytes);

    /**
     * Suspends recording of tick samples in the profiler.
     * When the V8 profiling mode is enabled (usually via command line
     * switches) this function suspends recording of tick samples.
     * Profiling ticks are discarded until ResumeProfiler() is called.
     *
     * See also the --prof and --prof_auto command line switches to
     * enable V8 profiling.
     */
//    static void PauseProfiler();

    /**
     * Resumes recording of tick samples in the profiler.
     * See also PauseProfiler().
     */
//    static void ResumeProfiler();

    /**
     * Return whether profiler is currently paused.
     */
//    static bool IsProfilerPaused();

    /**
     * Retrieve the V8 thread id of the calling thread.
     *
     * The thread id for a thread should only be retrieved after the V8
     * lock has been acquired with a Locker object with that thread.
     */
//    static int GetCurrentThreadId();

    /**
     * Forcefully terminate execution of a JavaScript thread.  This can
     * be used to terminate long-running scripts.
     *
     * TerminateExecution should only be called when then V8 lock has
     * been acquired with a Locker object.  Therefore, in order to be
     * able to terminate long-running threads, preemption must be
     * enabled to allow the user of TerminateExecution to acquire the
     * lock.
     *
     * The termination is achieved by throwing an exception that is
     * uncatchable by JavaScript exception handlers.  Termination
     * exceptions act as if they were caught by a C++ TryCatch exception
     * handler.  If forceful termination is used, any C++ TryCatch
     * exception handler that catches an exception should check if that
     * exception is a termination exception and immediately return if
     * that is the case.  Returning immediately in that case will
     * continue the propagation of the termination exception if needed.
     *
     * The thread id passed to TerminateExecution must have been
     * obtained by calling GetCurrentThreadId on the thread in question.
     *
     * \param thread_id The thread id of the thread to terminate.
     */
//    static void TerminateExecution(int thread_id);

    /**
     * Forcefully terminate the current thread of JavaScript execution
     * in the given isolate. If no isolate is provided, the default
     * isolate is used.
     *
     * This method can be used by any thread even if that thread has not
     * acquired the V8 lock with a Locker object.
     *
     * \param isolate The isolate in which to terminate the current JS execution.
     */
//    static void TerminateExecution(Isolate *isolate = NULL);

    /**
     * Is V8 terminating JavaScript execution.
     *
     * Returns true if JavaScript execution is currently terminating
     * because of a call to TerminateExecution.  In that case there are
     * still JavaScript frames on the stack and the termination
     * exception is still active.
     *
     * \param isolate The isolate in which to check.
     */
//    static bool IsExecutionTerminating(Isolate *isolate = NULL);

    /**
     * Releases any resources used by v8 and stops any utility threads
     * that may be running.  Note that disposing v8 is permanent, it
     * cannot be reinitialized.
     *
     * It should generally not be necessary to dispose v8 before exiting
     * a process, this should happen automatically.  It is only necessary
     * to use if the process needs the resources taken up by v8.
     */
    static bool Dispose();

    /**
     * Get statistics about the heap memory usage.
     */
    static void GetHeapStatistics(HeapStatistics *heap_statistics);

    /**
     * Iterates through all external resources referenced from current isolate
     * heap. This method is not expected to be used except for debugging purposes
     * and may be quite slow.
     */
//    static void VisitExternalResources(ExternalResourceVisitor *visitor);

    /**
     * Iterates through all the persistent handles in the current isolate's heap
     * that have class_ids.
     */
//    static void VisitHandlesWithClassIds(PersistentHandleVisitor *visitor);

    /**
     * Optional notification that the embedder is idle.
     * V8 uses the notification to reduce memory footprint.
     * This call can be used repeatedly if the embedder remains idle.
     * Returns true if the embedder should stop calling IdleNotification
     * until real work has been done.  This indicates that V8 has done
     * as much cleanup as it will be able to do.
     *
     * The hint argument specifies the amount of work to be done in the function
     * on scale from 1 to 1000. There is no guarantee that the actual work will
     * match the hint.
     */
//    static bool IdleNotification(int hint = 1000);

    /**
     * Optional notification that the system is running low on memory.
     * V8 uses these notifications to attempt to free memory.
     */
//    static void LowMemoryNotification();

    /**
     * Optional notification that a context has been disposed. V8 uses
     * these notifications to guide the GC heuristic. Returns the number
     * of context disposals - including this one - since the last time
     * V8 had a chance to clean up.
     */
//    static int ContextDisposedNotification();

private:
    static internal::Object *GlobalizeReference(internal::Object *handle);

    static void DisposeGlobal(internal::Object *global_handle);

    static void MakeWeak(internal::Object *global_handle,
                         void *data,
                         WeakReferenceCallback callback);

    template<class T> friend class Persistent;
};

}

#endif //DUKV8_UTILITY_H
