//
// Created by Jiang Lu on 6/2/15.
//

#ifndef DUKV8_FUNCTIONTEMPLATE_H
#define DUKV8_FUNCTIONTEMPLATE_H

#include "template.h"
#include "handle.h"
#include "function.h"

namespace v8 {

class FunctionTemplate : public Template {
public:
    RTTI_DECLARE();

    /** Creates a function template.*/
    static Local<FunctionTemplate> New(
            InvocationCallback callback = 0,
            Handle<Value> data = Handle<Value>(),
            Handle<Signature> signature = Handle<Signature>());

    /** Returns the unique function instance in the current execution context.*/
    Local<Function> GetFunction();

    /** Get the InstanceTemplate. */
    Local<ObjectTemplate> InstanceTemplate();

    /** Causes the function template to inherit from a parent function template.*/
    void Inherit(Handle<FunctionTemplate> parent);

    /**
     * A PrototypeTemplate is the template used to create the prototype object
     * of the function created by this template.
     */
    Local<ObjectTemplate> PrototypeTemplate();

    /**
     * Set the class name of the FunctionTemplate.  This is used for
     * printing objects created with the function created from the
     * FunctionTemplate as its constructor.
     */
    void SetClassName(Handle<String> name);

    /**
     * Returns true if the given object is an instance of this function
     * template.
     */
    bool HasInstance(Handle<Value> object);

protected:
    FunctionTemplate(InvocationCallback callback,
                     Handle<Value> data,
                     Handle<Signature> signature);

    virtual ~FunctionTemplate();

private:
    InvocationCallback callback_;

    Handle<Value> data_;
    Handle<Signature> signature_;

    Handle<String> class_name_;
    Handle<Function> function_;

    friend class Function;
};

}

#endif //DUKV8_FUNCTIONTEMPLATE_H
