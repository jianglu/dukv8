//
// Created by Jiang Lu on 6/2/15.
//

#include <cassert>
#include "dukv8/handle_scope.h"
#include "dukv8/object_list.h"
#include "dukv8/value.h"
#include "dukv8/string.h"
#include "dukv8/object.h"
#include "dukv8/isolate.h"
#include "dukv8/gcobject.h"

namespace v8 {

HandleScope *HandleScope::s_current_scope_ = NULL;

HandleScope::HandleScope() {
//    printf("[%p] %s\n", this, __PRETTY_FUNCTION__);
    OpenScope();
}

HandleScope::~HandleScope() {
    CloseScope();

    // 合适的时机清除自动释放池
    Isolate *isolate = Isolate::GetCurrent();
    isolate->GetAutoReleasePool()->Release();

    // delete pool_;
    for (std::set<i::GCObject *>::iterator it = gcobject_set_.begin();
         it != gcobject_set_.end(); ++it) {
        (*it)->Release();
    }
//    printf("[%p] %s\n", this, __PRETTY_FUNCTION__);
}

int HandleScope::NumberOfHandles() {
    return (int) s_current_scope_->gcobject_set_.size();
}

void HandleScope::OpenScope() {
    previous_scope_ = s_current_scope_;
    s_current_scope_ = this;
}

void HandleScope::CloseScope() {
    assert(s_current_scope_ == this);
    s_current_scope_ = previous_scope_;
}

void HandleScope::AddObjectToScope(v8::internal::GCObject *gcobject) {
    std::set<i::GCObject *> &gcobject_set = s_current_scope_->gcobject_set_;
    std::set<i::GCObject *>::iterator it = gcobject_set.find(gcobject);
    if (it == gcobject_set.end()) {
        gcobject->Retain();
        gcobject_set.insert(gcobject);
    }
}

/**
 * Creates a new handle with the given value.
 */
internal::Object *HandleScope::CreateHandle(internal::Object *value) {

    i::GCObject *gcobject = RTTI_DynamicCast(
            i::GCObject, reinterpret_cast<i::GCObject *>(value));
    if (gcobject) {
        s_current_scope_->AddObjectToScope(gcobject);
    }

    // String *string = RTTI_DynamicCast(String, reinterpret_cast<Data *>(value));
    // if (string) {
    //     string->AddToObjectPool(s_current_scope_->pool_);
    //     // s_current_scope_->pool_->AddObject(string->GetHeapPtr());
    //     return value;
    // }
    //
    // Object *object = RTTI_DynamicCast(Object, reinterpret_cast<Object *>(value));
    // if (object) {
    //     object->AddToObjectPool(s_current_scope_->pool_);
    //     // s_current_scope_->pool_->AddObject(object->GetHeapPtr());
    //     return value;
    // }
    //
    // printf("UNKNOW Object !!\n");

    return value;
}

}