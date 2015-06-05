//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/gcobject.h"
#include "dukv8/global_stash.h"
#include "dukv8/isolate.h"
#include "dukv8/duk_stack_scope.h"

namespace v8 {
namespace internal {

#define THIS_INDEX 0

RTTI_IMPLEMENT_ROOT(v8::internal::GCObject);

static duk_int_t GCObjectFinalizer(duk_context *ctx) {
    DUK_STACK_SCOPE(ctx);
    duk_get_prop_index(ctx, -1, THIS_INDEX);
    GCObject *gcobject = static_cast<GCObject *>(duk_get_pointer(ctx, -1));
    gcobject->Deinit();
    delete gcobject;
    return DUK_ERR_NONE;
}

GCObject::GCObject() :
        gcobject_heap_ptr_(NULL),
        gcobject_ref_count_(0) {
    Isolate *isolate = Isolate::GetCurrent();
    DukContextRef ctx = isolate->GetDukContext();

    duk_push_object(ctx);

    duk_push_pointer(ctx, this);
    duk_put_prop_index(ctx, -2, THIS_INDEX);

    duk_push_c_lightfunc(ctx, &GCObjectFinalizer, 1, 1, 1);
    duk_set_finalizer(ctx, -2);

    gcobject_heap_ptr_ = duk_get_heapptr(ctx, -1);

    // 加入自动释放池, 避免没有任何句柄引用的状态下导致的 duk_pop 时析构
    isolate->GetAutoReleasePool()->AutoRelease(this);

    duk_pop(ctx);
}

GCObject::~GCObject() {
//    printf("%s[%p] 析构\n", GetRTTI()->GetName(), this);
}

GCObject *GCObject::Init() {
    // printf("%s[%p] 构造\n", GetTypeName(), this);
    return this;
}

void GCObject::Deinit() {
    // printf("%s[%p] 析构\n", GetTypeName(), this);
}

//void GCObject::AddToObjectPool(GCObjectPool *pool) const {
//    pool->AddObject(gcobject_heap_ptr_);
//}

//void GCObject::AddToGlobalStash(v8::internal::GlobalStash *stash) const {
//    const_cast<GCObject *>(this)->gcobject_stash_index_
//            = stash->AddObject(gcobject_heap_ptr_);
//}
//
//void GCObject::RemoveFromGlobalStash(v8::internal::GlobalStash *stash) const {
//    stash->RemoveObject(gcobject_stash_index_);
//    const_cast<GCObject *>(this)->gcobject_stash_index_ = 0;
//}

void GCObject::Retain() {
    if (gcobject_ref_count_ == 0) {
        i::GlobalStash stash("__gcobjects");
        gcobject_stash_index_ = stash.AddObject(gcobject_heap_ptr_);
    }
    gcobject_ref_count_++;
}

void GCObject::Release() {
    gcobject_ref_count_--;
    if (gcobject_ref_count_ == 0) {
        i::GlobalStash stash("__gcobjects");
        ASSERT(gcobject_stash_index_ != 0);
        stash.RemoveObject(gcobject_stash_index_);
    }
}

}
}

