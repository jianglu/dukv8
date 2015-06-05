//
// Created by Jiang Lu on 6/5/15.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <stdio.h>
#include <dukv8.h>

// A LocalContext holds a reference to a v8::Context.
class LocalContext {
public:
    LocalContext(v8::ExtensionConfiguration *extensions = 0,
                 v8::Handle<v8::ObjectTemplate> global_template = v8::Handle<v8::ObjectTemplate>(),
                 v8::Handle<v8::Value> global_object = v8::Handle<v8::Value>())
            : context_(v8::Context::New(extensions, global_template, global_object)) {
        context_->Enter();
    }

    virtual ~LocalContext() {
        context_->Exit();
        context_.Dispose();
    }

    v8::Context *operator->() { return *context_; }
    v8::Context *operator*() { return *context_; }

    bool IsReady() { return !context_.IsEmpty(); }

    v8::Local<v8::Context> local() {
        return v8::Local<v8::Context>::New(context_);
    }

private:
    v8::Persistent<v8::Context> context_;
};

class Fixture {
public:
    Fixture() {
        v8::V8::Initialize();
        isolate_ = v8::Isolate::New();
        isolate_->Enter();
    }

    ~Fixture() {
        isolate_->Exit();
        isolate_->Dispose();
    }

    v8::Isolate *isolate() {
        return isolate_;
    }

private:
    v8::Isolate *isolate_;
};

static inline v8::Local<v8::Value> v8_num(double x) {
    return v8::Number::New(x);
}

static inline v8::Local<v8::String> v8_str(const char *x) {
    return v8::String::New(x);
}

static inline v8::Local<v8::Script> v8_compile(const char *x) {
    return v8::Script::Compile(v8_str(x));
}

static inline int StrLength(const char *string) {
    size_t length = strlen(string);
    ASSERT(length == static_cast<size_t>(static_cast<int>(length)));
    return static_cast<int>(length);
}

template<typename T>
T *NewArray(size_t size) {
    T *result = new T[size];
    ASSERT(result);
    return result;
}

static inline uint16_t *AsciiToTwoByteString(const char *source) {
    int array_length = StrLength(source) + 1;
    uint16_t *converted = NewArray<uint16_t>(array_length);
    for (int i = 0; i < array_length; i++) converted[i] = source[i];
    return converted;
}

TEST_CASE_METHOD(Fixture, "Handles") {
    v8::HandleScope scope;
    v8::Local<v8::Context> local_env;

    {
        LocalContext env;
        local_env = env.local();
    }

    // Local context should still be live.
    CHECK(!local_env.IsEmpty());
    local_env->Enter();

    v8::Handle<v8::Primitive> undef = v8::Undefined();
    CHECK(!undef.IsEmpty());
    CHECK(undef->IsUndefined());

    const char *source = "1 + 2 + 3";
    v8::Local<v8::Script> script = v8_compile(source);
    REQUIRE(6 == script->Run()->Int32Value());

    local_env->Exit();
}

TEST_CASE_METHOD(Fixture, "Script") {
    v8::HandleScope scope;

    v8::Persistent<v8::Context> context = v8::Context::New();
    v8::Context::Scope context_scope(context);

    const char *source = "1 + 2 + 3";

    v8::Local<v8::Script> script;
    {
        v8::HandleScope sub_scope;
        script = sub_scope.Close(v8_compile(source));
    }

    REQUIRE(6 == script->Run()->Int32Value());

    context.Dispose();
}

#include "string_external_ascii_string_resource.cpp"