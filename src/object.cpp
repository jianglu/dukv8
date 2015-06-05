//
// Created by Jiang Lu on 6/2/15.
//

#include <cassert>
#include "dukv8/object.h"
#include "dukv8/local.h"
#include "dukv8/boolean.h"
#include "dukv8/string.h"
#include "dukv8/duk_stack_scope.h"
#include "dukv8/isolate.h"

namespace v8 {

RTTI_IMPLEMENT(v8::Object, v8::Value);

Object *Object::Init(DukContextRef duk_ctx) {
    Value::Init(duk_ctx);
    return this;
}

Object *Object::Init(DukContextRef duk_ctx, void *heap_ptr) {
    Value::Init(duk_ctx);
    AttachDuktapeObject(heap_ptr);
    return this;
}

void Object::Deinit() {
    if (duk_obj_heapptr_) {
        DukObjectRelease(duk_obj_index_);
    }
    Value::Deinit();
}

void Object::AttachDuktapeObject(void *heap_ptr) {
    assert(duk_obj_heapptr_ == NULL);
    duk_obj_heapptr_ = heap_ptr;
    duk_obj_index_ = DukObjectRetain(duk_obj_heapptr_);
}

bool Object::Set(Handle<Value> key,
                 Handle<Value> value,
                 PropertyAttribute attrib) {
    Push();
    key->Push();
    value->Push();

    if (attrib == None) {
        duk_put_prop(duk_ctx_, -3);

    } else {
        duk_uint_t flags = DUK_DEFPROP_HAVE_VALUE;

        flags |= DUK_DEFPROP_HAVE_WRITABLE;
        if ((attrib & ReadOnly) == 0) {
            flags |= DUK_DEFPROP_WRITABLE;
        }

        flags |= DUK_DEFPROP_HAVE_ENUMERABLE;
        if ((attrib & DontEnum) == 0) {
            flags |= DUK_DEFPROP_ENUMERABLE;
        }

        flags |= DUK_DEFPROP_HAVE_CONFIGURABLE;
        if ((attrib & DontDelete) == 0) {
            flags |= DUK_DEFPROP_CONFIGURABLE;
        }

        duk_def_prop(duk_ctx_, -3, flags);
    }

    return true;
}

Local<Value> Object::Get(Handle<Value> key) {
    DUK_STACK_SCOPE(duk_ctx_);
    Push();
    key->Push();
    duk_get_prop(duk_ctx_, -2);
    return Local<Value>::New(Value::FromStack(duk_ctx_, -1));
}

Local<Value> Object::Get(uint32_t index) {
    DUK_STACK_SCOPE(duk_ctx_);
    Push();
    duk_get_prop_index(duk_ctx_, -2, index);
    return Local<Value>::New(Value::FromStack(duk_ctx_, -1));
}

bool Object::Set(uint32_t index, Handle<Value> value) {
    return true;
}

bool Object::Has(Handle<String> key) {
    DUK_STACK_SCOPE(duk_ctx_);
    Push();
    key->Push();
    duk_has_prop(duk_ctx_, -2);
    return (bool) duk_get_boolean(duk_ctx_, -1);
}

bool Object::Delete(Handle<String> key) {
    DUK_STACK_SCOPE(duk_ctx_);
    Push();
    key->Push();
    return (bool) duk_del_prop(duk_ctx_, -2);
}

bool Object::SetAccessor(Handle<String> name,
                         AccessorGetter getter,
                         AccessorSetter setter,
                         Handle<Value> data,
                         AccessControl settings,
                         PropertyAttribute attribute) {
    return false;
}

Local<Value> Object::GetPrototype() {
    TODO();
    return Local<Value>();
}

Local<Value> Object::GetConstructor() {
    TODO();
    return Local<Value>();
}

Local<Object> Object::New() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<Object>::New(Handle<Object>((new Object)->Init(ctx)));
}

Object *Object::Cast(Value *obj) {
    if (RTTI_IsKindOf(Object, obj)) {
        return RTTI_StaticCast(Object, obj);
    }

    if (obj) {
        TODO();
//        JSContextRef js_ctx = Context::GetCurrentJSContext();
//        JSValueRef js_value = obj->m_js_value;
//        if (JSValueIsObjectOfClass(js_ctx, js_value, Object::GetJSClass())) {
//            JSObjectRef js_object = JSValueToObject(js_ctx, js_value, NULL);
//            assert(js_object == js_value);
//            assert(JSObjectGetPrivate(js_object) != NULL);
//            return (Object *) JSObjectGetPrivate(js_object);
//        }
//        if (JSValueIsObject(js_ctx, js_value)) {
//            JSObjectRef js_object = JSValueToObject(js_ctx, js_value, NULL);
//            assert(js_object == js_value);
//            return Local<Object>(internal::ImportObject(js_ctx, js_object));
//        }
    }
    return NULL;
}

bool Object::SetHiddenValue(Handle<String> key, Handle<Value> value) {
    return false;
}

Local<Value> Object::GetHiddenValue(Handle<String> key) {
    return Local<Value>();
}

/** Gets the number of internal fields for this Object. */
int Object::InternalFieldCount() {
    return 0;
}

/** Gets the value in an internal field. */
Local<Value> Object::GetInternalField(int index) {
    return Local<Value>();
}

/** Gets a native pointer from an internal field. */
void *Object::GetPointerFromInternalField(int index) {
    return NULL;
}

/** Sets a native pointer in an internal field. */
void Object::SetPointerInInternalField(int index, void *value) {
}

void Object::SetIndexedPropertiesToExternalArrayData(
        void *data,
        ExternalArrayType array_type,
        int number_of_elements) {
}

bool Object::HasIndexedPropertiesInExternalArrayData() {
    return true;
}

void *Object::GetIndexedPropertiesExternalArrayData() {
    return NULL;
}

ExternalArrayType Object::GetIndexedPropertiesExternalArrayDataType() {
    TODO();
    return kExternalByteArray;
}

int Object::GetIndexedPropertiesExternalArrayDataLength() {
    TODO();
    return 0;
}

void Object::Push() const {
    duk_push_heapptr(duk_ctx_, duk_obj_heapptr_);
}

}