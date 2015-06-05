//
// Created by Jiang Lu on 6/2/15.
//

#include <cassert>
#include "dukv8/template.h"
#include "dukv8/handle.h"
#include "dukv8/local.h"
#include "dukv8/data.h"
#include "dukv8/string.h"
#include "dukv8/integer.h"
#include "dukv8/object.h"
#include "dukv8/object_template.h"
#include "dukv8/function_template.h"

namespace v8 {

bool internal::StringCompare::operator()(const Handle<String> &lhs, const Handle<String> &rhs) const {
    DukContextRef ctx = lhs->GetDukContext();
    lhs->Push();
    rhs->Push();
    duk_bool_t retval = duk_equals(ctx, -1, -2);
    duk_pop_2(ctx);
    return (bool) retval; // key1->Compare(key2) < 0;
}

RTTI_IMPLEMENT(v8::Template, v8::Data);

Template::Template() {
}

void Template::Set(Handle<String> name, Handle<Data> value, PropertyAttribute attributes) {
    template_property_map_[name] = internal::TemplateProperty(value, attributes);
}

void Template::Set(const char *name, v8::Handle<Data> value) {
    Set(v8::String::New(name), value);
}

void Template::ApplyToObject(Handle<Object> object) {
    // copy each template property to object
    internal::TemplatePropertyMap::iterator template_property_it;
    for (template_property_it = template_property_map_.begin();
         template_property_it != template_property_map_.end();
         ++template_property_it) {

        const Handle<String> &name = template_property_it->first;
        const Handle<Data> &data = template_property_it->second.value_;
        const PropertyAttribute &attrib = template_property_it->second.attrib_;

        ObjectTemplate *object_template = RTTI_DynamicCast(ObjectTemplate, *data);
        if (object_template) {
            object->Set(name, object_template->NewInstance(), attrib);
            continue;
        }

        FunctionTemplate *function_template = RTTI_DynamicCast(FunctionTemplate, *data);
        if (function_template) {
            object->Set(name, function_template->GetFunction(), attrib);
            continue;
        }

        Value *value = RTTI_DynamicCast(Value, *data);
        if (value) {
            object->Set(name, Handle<Value>(value), attrib);
            continue;
        }

        assert(true);
    }
}

}