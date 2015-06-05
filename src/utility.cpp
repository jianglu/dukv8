//
// Created by Jiang Lu on 6/2/15.
//

#include "dukv8/utility.h"
#include "dukv8/isolate.h"
#include "dukv8/persistent.h"
#include "dukv8/global_stash.h"

namespace v8 {

/**
 * Schedules an exception to be thrown when returning to JavaScript.  When an
 * exception has been scheduled it is illegal to invoke any JavaScript
 * operation; the caller must return immediately and only after the exception
 * has been handled does it become legal to invoke JavaScript operations.
 */
Handle<Value> V8EXPORT ThrowException(Handle<Value> exception) {
//#if 1 // V8LIKE_DEBUG
//    if (strstr(*String::Utf8Value(exception->ToString()), "ENOENT") == NULL) {
//        printf("exception: %s\n", *String::Utf8Value(exception->ToString()));
//        if (exception->IsObject()) {
//            Handle<Value> stack = exception.As<Object>()->Get(String::NewSymbol("stack"));
//            printf("%s\n", *String::Utf8Value(stack->ToString()));
//        }
//    }
//#endif
//    sg_last_exception = exception;
//    return exception;
    return exception;
}

ResourceConstraints::ResourceConstraints() :
        max_young_space_size_(0), max_old_space_size_(0), max_executable_size_(0), stack_limit_(NULL) { }

bool SetResourceConstraints(ResourceConstraints *constraints) {
    TODO();
    return false;
}

/*static*/ void V8::SetFatalErrorHandler(FatalErrorCallback that) { /*TODO();*/ }

void V8::SetFlagsFromCommandLine(int *argc, char **argv, bool remove_flags) {
}

const char *V8::GetVersion() {
    return "3.14.5.9";
}

static void V8Exit() {
    Isolate::GetCurrent()->Dispose();
}

bool V8::Initialize() {
    Isolate *isolate = Isolate::New();
    isolate->Enter();
    atexit(&V8Exit);
    return true;
}

bool V8::Dispose() {
    return true;
}

/*static*/
bool V8::IsDead() {
    TODO();
    return false;
}

internal::Object *V8::GlobalizeReference(internal::Object *handle) {

    i::GCObject *gcobject = RTTI_DynamicCast(
            i::GCObject, reinterpret_cast<i::GCObject *>(handle));
    if (gcobject) {
        i::GlobalStash stash("__persistent");
        gcobject->AddToGlobalStash(&stash);
    }

    return handle;
}

void V8::DisposeGlobal(internal::Object *global_handle) {
    i::GCObject *gcobject = RTTI_DynamicCast(
            i::GCObject, reinterpret_cast<i::GCObject *>(global_handle));
    if (gcobject) {
        i::GlobalStash stash("__persistent");
        gcobject->RemoveFromGlobalStash(&stash);
    }
}

void V8::MakeWeak(internal::Object *global_handle,
                  void *data,
                  WeakReferenceCallback callback) {
}

/*static*/ intptr_t V8::AdjustAmountOfExternalAllocatedMemory(intptr_t change_in_bytes) {
    static intptr_t _bytes = 0;
    //printf("%s: %ld + %ld -> %ld\n", __PRETTY_FUNCTION__, _bytes, change_in_bytes, _bytes + change_in_bytes);
    return _bytes;
}

/*static*/ void V8::AddGCPrologueCallback(GCPrologueCallback callback, GCType gc_type_filter) { TODO(); }

/*static*/ void V8::AddGCEpilogueCallback(GCEpilogueCallback callback, GCType gc_type_filter) { TODO(); }

/*static*/ void V8::SetEntropySource(EntropySource source) { TODO(); }

/*static*/ void V8::GetHeapStatistics(HeapStatistics *heap_statistics) { TODO(); }

}