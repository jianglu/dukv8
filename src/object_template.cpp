//
// Created by Jiang Lu on 6/2/15.
//

#include "dukv8/object_template.h"
#include "dukv8/local.h"
#include "dukv8/object.h"
#include "dukv8/isolate.h"
#include "dukv8/duk_stack_scope.h"

namespace v8 {

RTTI_IMPLEMENT(v8::ObjectTemplate, v8::Template);

ObjectTemplate::ObjectTemplate() {
}

ObjectTemplate::~ObjectTemplate() {
}

/** Creates an ObjectTemplate. */
Local<ObjectTemplate> ObjectTemplate::New() {
    return Local<ObjectTemplate>::New(
            Handle<ObjectTemplate>(new ObjectTemplate()));
}

/** Creates a new instance of this template.*/
Local<Object> ObjectTemplate::NewInstance() {
    DukContextRef ctx = Isolate::GetCurrent()->GetDukContext();
    DUK_STACK_SCOPE(ctx);
    duk_push_object(ctx);
    Handle<Object> object(new Object(ctx, duk_get_heapptr(ctx, -1)));
    ObjectTemplate::ApplyToObject(object);
    // internal::Helper *helper = object->GetHelper();
    // helper->m_object_template = Handle<ObjectTemplate>(this);
    // helper->SetInternalFieldCount(m_internal_field_count);
    return Local<Object>::New(object);
}

void ObjectTemplate::ApplyToObject(Handle<Object> object) {
    Template::ApplyToObject(object);
}

//
// 设置属性拦截器
//
void ObjectTemplate::SetNamedPropertyHandler(NamedPropertyGetter getter,
                                             NamedPropertySetter setter,
                                             NamedPropertyQuery query,
                                             NamedPropertyDeleter deleter,
                                             NamedPropertyEnumerator enumerator,
                                             Handle<Value> data) {
}

/**
 * Gets the number of internal fields for objects generated from
 * this template.
 */
int ObjectTemplate::InternalFieldCount() {
    return 0;
}

/**
 * Sets the number of internal fields for objects generated from
 * this template.
 */
void ObjectTemplate::SetInternalFieldCount(int value) {
}


void ObjectTemplate::SetAccessor(Handle<String> name,
                                 AccessorGetter getter,
                                 AccessorSetter setter,
                                 Handle<Value> data,
                                 AccessControl settings,
                                 PropertyAttribute attrib,
                                 Handle<AccessorSignature> signature) {
    // m_template_accessor_map[name] = internal::TemplateAccessor(getter, setter, data, settings, attrib, signature);
}


}