//
// Created by Jiang Lu on 6/2/15.
//

#ifndef DUKV8_OBJECT_H
#define DUKV8_OBJECT_H

#include "base.h"
#include "handle.h"
#include "value.h"

namespace v8 {

enum ExternalArrayType {
    kExternalByteArray = 1,
    kExternalUnsignedByteArray,
    kExternalShortArray,
    kExternalUnsignedShortArray,
    kExternalIntArray,
    kExternalUnsignedIntArray,
    kExternalFloatArray,
    kExternalDoubleArray,
    kExternalPixelArray
};

/**
 * Accessor[Getter|Setter] are used as callback functions when
 * setting|getting a particular property. See Object and ObjectTemplate's
 * method SetAccessor.
 */
typedef Handle<Value> (*AccessorGetter)(Local<String> property,
                                        const AccessorInfo &info);


typedef void (*AccessorSetter)(Local<String> property,
                               Local<Value> value,
                               const AccessorInfo &info);

/**
 * Access control specifications.
 *
 * Some accessors should be accessible across contexts.  These
 * accessors have an explicit access control parameter which specifies
 * the kind of cross-context access that should be allowed.
 *
 * Additionally, for security, accessors can prohibit overwriting by
 * accessors defined in JavaScript.  For objects that have such
 * accessors either locally or in their prototype chain it is not
 * possible to overwrite the accessor by using __defineGetter__ or
 * __defineSetter__ from JavaScript code.
 */
enum AccessControl {
    DEFAULT = 0,
    ALL_CAN_READ = 1,
    ALL_CAN_WRITE = 1 << 1,
    PROHIBITS_OVERWRITING = 1 << 2
};

/**
 * A JavaScript object (ECMA-262, 4.3.3)
 */
class Object : public Value, public HeapObject {
public:
    RTTI_DECLARE();

    Object(duk_context *duk_ctx);
    Object(duk_context *duk_ctx, void *heap_ptr);

    virtual ~Object();

    V8EXPORT bool Set(Handle<Value> key,
                      Handle<Value> value,
                      PropertyAttribute attrib = None);

    V8EXPORT bool Set(uint32_t index,
                      Handle<Value> value);

    V8EXPORT Local<Value> Get(Handle<Value> key);

    V8EXPORT Local<Value> Get(uint32_t index);

    // functions with generic ones that accept a Handle<Value> key.
    V8EXPORT bool Has(Handle<String> key);

    V8EXPORT bool Delete(Handle<String> key);

    V8EXPORT bool SetAccessor(Handle<String> name,
                              AccessorGetter getter,
                              AccessorSetter setter = 0,
                              Handle<Value> data = Handle<Value>(),
                              AccessControl settings = DEFAULT,
                              PropertyAttribute attribute = None);

    /**
     * Get the prototype object.  This does not skip objects marked to
     * be skipped by __proto__ and it does not consult the security
     * handler.
     */
    V8EXPORT Local<Value> GetPrototype();

    /** Gets the number of internal fields for this Object. */
    V8EXPORT int InternalFieldCount();

    /** Gets the value in an internal field. */
    Local<Value> GetInternalField(int index);

    /** Gets a native pointer from an internal field. */
    void *GetPointerFromInternalField(int index);

    /** Sets a native pointer in an internal field. */
    V8EXPORT void SetPointerInInternalField(int index, void *value);

    /**
     * Access hidden properties on JavaScript objects. These properties are
     * hidden from the executing JavaScript and only accessible through the V8
     * C++ API. Hidden properties introduced by V8 internally (for example the
     * identity hash) are prefixed with "v8::".
     */
    V8EXPORT bool SetHiddenValue(Handle<String> key, Handle<Value> value);

    V8EXPORT Local<Value> GetHiddenValue(Handle<String> key);

    /**
     * Set the backing store of the indexed properties to be managed by the
     * embedding layer. Access to the indexed properties will follow the rules
     * spelled out for the CanvasArray subtypes in the WebGL specification.
     * Note: The embedding program still owns the data and needs to ensure that
     *       the backing store is preserved while V8 has a reference.
     */
    V8EXPORT void SetIndexedPropertiesToExternalArrayData(
            void *data,
            ExternalArrayType array_type,
            int number_of_elements);

    V8EXPORT bool HasIndexedPropertiesInExternalArrayData();

    V8EXPORT void *GetIndexedPropertiesExternalArrayData();

    V8EXPORT ExternalArrayType GetIndexedPropertiesExternalArrayDataType();

    V8EXPORT int GetIndexedPropertiesExternalArrayDataLength();

    V8EXPORT static Local<Object> New();

    /**
     * Returns the function invoked as a constructor for this object.
     * May be the null value.
     */
    V8EXPORT Local<Value> GetConstructor();

    static Object *Cast(Value *obj);

    virtual void Push() const;

    void AttachDuktapeObject(void *heap_ptr);
};

}

#endif //DUKV8_OBJECT_H
