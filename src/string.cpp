//
// Created by Jiang Lu on 6/2/15.
//

#include <cassert>
#include "dukv8/string.h"
#include "dukv8/local.h"
#include "dukv8/isolate.h"
#include "dukv8/duk_stack_scope.h"

namespace v8 {

RTTI_IMPLEMENT(v8::String, v8::Primitive);

String::String(duk_context *duk_ctx, const char *data, int length) :
        Primitive(duk_ctx) {
    DUK_STACK_SCOPE(duk_ctx);
    if (length >= 0) {
        duk_push_lstring(duk_ctx, data, length);
    } else {
        duk_push_string(duk_ctx, data);
    }
    duk_obj_heapptr_ = duk_get_heapptr(duk_ctx, -1);
    duk_obj_index_ = DukObjectRetain(duk_obj_heapptr_);
//    printf("%s\n", __PRETTY_FUNCTION__);
}

String::String(duk_context *duk_ctx, void *heap_ptr) :
        Primitive(duk_ctx) {
    DUK_STACK_SCOPE(duk_ctx);
    duk_push_heapptr(duk_ctx, heap_ptr);
    assert(duk_is_string(duk_ctx, -1));
    duk_obj_heapptr_ = duk_get_heapptr(duk_ctx, -1);
    duk_obj_index_ = DukObjectRetain(duk_obj_heapptr_);
//    printf("%s\n", __PRETTY_FUNCTION__);
}

String::~String() {
    DukObjectRelease(duk_obj_index_);
//    printf("%s\n", __PRETTY_FUNCTION__);
}

void String::Push() const {
    duk_push_heapptr(duk_ctx_, duk_obj_heapptr_);
}

/**
 * Returns the number of characters in this string.
 */
int String::Length() const {
    return 0;
}

/**
 * Returns the number of bytes in the UTF-8 encoded
 * representation of this string.
 */
int String::Utf8Length() const {
    return 0;
}

bool String::MayContainNonAscii() const {
    return true;
}

/**
 * Allocates a new string from either UTF-8 encoded or ASCII data.
 * The second parameter 'length' gives the buffer length.
 * If the data is UTF-8 encoded, the caller must
 * be careful to supply the length parameter.
 * If it is not given, the function calls
 * 'strlen' to determine the buffer length, it might be
 * wrong if 'data' contains a null character.
 */
Local<String> String::New(const char *data, int length) {
    duk_context *ctx = Isolate::GetCurrent()->GetDukContext();
    return Local<String>::New(Handle<String>(new String(ctx, data, length)));
}

/** Allocates a new string from 16-bit character codes.*/
Local<String> String::New(const uint16_t *data, int length) {
    TODO();
    return Local<String>();
}

/** Creates a symbol. Returns one if it exists already.*/
Local<String> String::NewSymbol(const char *data, int length) {
    return String::New(data, length);
}

Local<String> String::Concat(Handle<String> left,
                             Handle<String> right) {
    DukContextRef ctx = left->duk_ctx_;
    DUK_STACK_SCOPE(ctx);
    left->Push();
    right->Push();
    duk_concat(ctx, 2);
    void *heap_ptr = duk_get_heapptr(ctx, -1);
    return Local<String>::New(Handle<String>(new String(ctx, heap_ptr)));
}

// 16-bit character codes.
int String::Write(uint16_t *buffer,
                  int start,
                  int length,
                  int options) const {
    return 0;
}

// ASCII characters.
int String::WriteAscii(char *buffer,
                       int start,
                       int length,
                       int options) const {
    return 0;
}

// UTF-8 encoded characters.
int String::WriteUtf8(char *buffer,
                      int length,
                      int *nchars_ref,
                      int options) const {
    return 0;
}

Local<String> String::NewExternal(String::ExternalStringResource *resource) {
    TODO();
    return Local<String>();
}

Local<String> String::NewExternal(String::ExternalAsciiStringResource *resource) {
    TODO();
    return Local<String>();
}

/**
 * A zero length string.
 */
v8::Local<v8::String> String::Empty() {
    TODO();
    return String::New("", 0);
}

String::Utf8Value::Utf8Value(Handle<v8::Value> obj) {
    DukContextRef ctx = obj->GetDukContext();
    DUK_STACK_SCOPE(ctx);
    obj->Push();
    str_ = strdup(duk_safe_to_string(ctx, -1));
}

String::Utf8Value::~Utf8Value() {
    if (str_) {
        delete[] str_;
        str_ = NULL;
    }
}

String::AsciiValue::AsciiValue(Handle<v8::Value> obj) {
    TODO();
}

String::AsciiValue::~AsciiValue() {
}

}