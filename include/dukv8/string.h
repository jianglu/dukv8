//
// Created by Jiang Lu on 6/2/15.
//

#ifndef DUKV8_STRING_H
#define DUKV8_STRING_H

#include "primitive.h"

namespace v8 {

class String : public Primitive, public HeapObject {
public:
    RTTI_DECLARE();

    enum Encoding {
        UNKNOWN_ENCODING = 0x1,
        TWO_BYTE_ENCODING = 0x0,
        ASCII_ENCODING = 0x4
    };

    /**
     * Returns the number of characters in this string.
     */
    V8EXPORT int Length() const;

    /**
     * Returns the number of bytes in the UTF-8 encoded
     * representation of this string.
     */
    V8EXPORT int Utf8Length() const;

    /**
     * A fast conservative check for non-ASCII characters.  May
     * return true even for ASCII strings, but if it returns
     * false you can be sure that all characters are in the range
     * 0-127.
     */
    V8EXPORT bool MayContainNonAscii() const;

    static inline String *Cast(v8::Value *obj);

    /**
     * Allocates a new string from either UTF-8 encoded or ASCII data.
     * The second parameter 'length' gives the buffer length.
     * If the data is UTF-8 encoded, the caller must
     * be careful to supply the length parameter.
     * If it is not given, the function calls
     * 'strlen' to determine the buffer length, it might be
     * wrong if 'data' contains a null character.
     */
    V8EXPORT static Local<String> New(const char *data, int length = -1);

    /** Allocates a new string from 16-bit character codes.*/
    V8EXPORT static Local<String> New(const uint16_t *data, int length = -1);

    /** Creates a symbol. Returns one if it exists already.*/
    V8EXPORT static Local<String> NewSymbol(const char *data, int length = -1);

    /**
     * Creates a new string by concatenating the left and the right strings
     * passed in as parameters.
     */
    V8EXPORT static Local<String> Concat(Handle<String> left,
                                         Handle<String> right);

    /**
     * Write the contents of the string to an external buffer.
     * If no arguments are given, expects the buffer to be large
     * enough to hold the entire string and NULL terminator. Copies
     * the contents of the string and the NULL terminator into the
     * buffer.
     *
     * WriteUtf8 will not write partial UTF-8 sequences, preferring to stop
     * before the end of the buffer.
     *
     * Copies up to length characters into the output buffer.
     * Only null-terminates if there is enough space in the buffer.
     *
     * \param buffer The buffer into which the string will be copied.
     * \param start The starting position within the string at which
     * copying begins.
     * \param length The number of characters to copy from the string.  For
     *    WriteUtf8 the number of bytes in the buffer.
     * \param nchars_ref The number of characters written, can be NULL.
     * \param options Various options that might affect performance of this or
     *    subsequent operations.
     * \return The number of characters copied to the buffer excluding the null
     *    terminator.  For WriteUtf8: The number of bytes copied to the buffer
     *    including the null terminator (if written).
     */
    enum WriteOptions {
        NO_OPTIONS = 0,
        HINT_MANY_WRITES_EXPECTED = 1,
        NO_NULL_TERMINATION = 2,
        PRESERVE_ASCII_NULL = 4,
        REPLACE_INVALID_UTF8 = 8
    };

    // 16-bit character codes.
    V8EXPORT int Write(uint16_t *buffer,
                       int start = 0,
                       int length = -1,
                       int options = NO_OPTIONS) const;

    // ASCII characters.
    V8EXPORT int WriteAscii(char *buffer,
                            int start = 0,
                            int length = -1,
                            int options = NO_OPTIONS) const;

    // UTF-8 encoded characters.
    V8EXPORT int WriteUtf8(char *buffer,
                           int length = -1,
                           int *nchars_ref = NULL,
                           int options = NO_OPTIONS) const;

    V8EXPORT bool IsExternal() const;
    V8EXPORT bool IsExternalAscii() const;

    /**
     * A zero length string.
     */
    V8EXPORT static v8::Local<v8::String> Empty();

    /**
     * Converts an object to a UTF-8-encoded character array.  Useful if
     * you want to print the object.  If conversion to a string fails
     * (e.g. due to an exception in the toString() method of the object)
     * then the length() method returns 0 and the * operator returns
     * NULL.
     */
    class V8EXPORT Utf8Value {
    public:
        explicit Utf8Value(Handle<v8::Value> obj);
        ~Utf8Value();
        char *operator*() { return str_; }
        const char *operator*() const { return str_; }
        int length() const { return length_; }
    private:
        char *str_;
        int length_;

        // Disallow copying and assigning.
        Utf8Value(const Utf8Value &);
        void operator=(const Utf8Value &);
    };

    /**
     * Converts an object to an ASCII string.
     * Useful if you want to print the object.
     * If conversion to a string fails (eg. due to an exception in the toString()
     * method of the object) then the length() method returns 0 and the * operator
     * returns NULL.
     */
    class V8EXPORT AsciiValue {
    public:
        explicit AsciiValue(Handle<v8::Value> obj);
        ~AsciiValue();
        char *operator*() { return str_; }
        const char *operator*() const { return str_; }
        int length() const { return length_; }
    private:
        char *str_;
        int length_;

        // Disallow copying and assigning.
        AsciiValue(const AsciiValue &);
        void operator=(const AsciiValue &);
    };

    class V8EXPORT ExternalStringResourceBase {  // NOLINT
    public:
        virtual ~ExternalStringResourceBase() { }

    protected:
        ExternalStringResourceBase() { }

        /**
         * Internally V8 will call this Dispose method when the external string
         * resource is no longer needed. The default implementation will use the
         * delete operator. This method can be overridden in subclasses to
         * control how allocated external string resources are disposed.
         */
        virtual void Dispose() { delete this; }

    private:
        // Disallow copying and assigning.
        ExternalStringResourceBase(const ExternalStringResourceBase &);
        void operator=(const ExternalStringResourceBase &);

        friend class v8::internal::Heap;
    };

    /**
     * An ExternalStringResource is a wrapper around a two-byte string
     * buffer that resides outside V8's heap. Implement an
     * ExternalStringResource to manage the life cycle of the underlying
     * buffer.  Note that the string data must be immutable.
     */
    class V8EXPORT ExternalStringResource
            : public ExternalStringResourceBase {
    public:
        /**
         * Override the destructor to manage the life cycle of the underlying
         * buffer.
         */
        virtual ~ExternalStringResource() { }

        /**
         * The string data from the underlying buffer.
         */
        virtual const uint16_t *data() const = 0;

        /**
         * The length of the string. That is, the number of two-byte characters.
         */
        virtual size_t length() const = 0;

    protected:
        ExternalStringResource() { }
    };

    /**
     * An ExternalAsciiStringResource is a wrapper around an ASCII
     * string buffer that resides outside V8's heap. Implement an
     * ExternalAsciiStringResource to manage the life cycle of the
     * underlying buffer.  Note that the string data must be immutable
     * and that the data must be strict (7-bit) ASCII, not Latin-1 or
     * UTF-8, which would require special treatment internally in the
     * engine and, in the case of UTF-8, do not allow efficient indexing.
     * Use String::New or convert to 16 bit data for non-ASCII.
     */

    class V8EXPORT ExternalAsciiStringResource
            : public ExternalStringResourceBase {
    public:
        /**
         * Override the destructor to manage the life cycle of the underlying
         * buffer.
         */
        virtual ~ExternalAsciiStringResource() { }
        /** The string data from the underlying buffer.*/
        virtual const char *data() const = 0;
        /** The number of ASCII characters in the string.*/
        virtual size_t length() const = 0;
    protected:
        ExternalAsciiStringResource() { }
    };

    /**
     * Creates a new external string using the data defined in the given
     * resource. When the external string is no longer live on V8's heap the
     * resource will be disposed by calling its Dispose method. The caller of
     * this function should not otherwise delete or modify the resource. Neither
     * should the underlying buffer be deallocated or modified except through the
     * destructor of the external string resource.
     */
    /**
     * src/node_string.cc
     * Local<String> ret = String::NewExternal(new ImmutableAsciiSource(
     */
    V8EXPORT static Local<String> NewExternal(ExternalAsciiStringResource *resource);

    INTERNAL virtual void Push() const;

protected:
    String *Init(DukContextRef duk_ctx, const char *data, int length);
    String *Init(DukContextRef duk_ctx, void *heap_ptr);
    String *Init(DukContextRef duk_ctx, ExternalAsciiStringResource *external_resource);
    virtual void Deinit();

private:
    V8EXPORT static void CheckCast(v8::Value *obj);

    friend class Value;

private:
    ExternalAsciiStringResource *external_ascii_string_resource_;
};

String *String::Cast(v8::Value *value) {
    return static_cast<String *>(value);
}


}


#endif //DUKV8_STRING_H
