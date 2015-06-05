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

HandleScope::HandleScope() :
        pool_(new i::GCObjectPool()) {
//    printf("%s\n", __PRETTY_FUNCTION__);
    OpenScope();
}

HandleScope::~HandleScope() {
    CloseScope();
    // 合适的时机清除自动释放池
    Isolate *isolate = Isolate::GetCurrent();
    isolate->GetAutoReleasePool()->Release();

    delete pool_;
//    printf("%s\n", __PRETTY_FUNCTION__);
}

void HandleScope::OpenScope() {
    previous_scope_ = s_current_scope_;
    s_current_scope_ = this;
}

void HandleScope::CloseScope() {
    assert(s_current_scope_ == this);
    s_current_scope_ = previous_scope_;
}

/**
 * Creates a new handle with the given value.
 */
internal::Object *HandleScope::CreateHandle(internal::Object *value) {

    i::GCObject *gcobject = RTTI_DynamicCast(
            i::GCObject, reinterpret_cast<i::GCObject *>(value));
    if (gcobject) {
        gcobject->AddToObjectPool(s_current_scope_->pool_);
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