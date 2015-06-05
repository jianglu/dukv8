//
// Created by Jiang Lu on 6/5/15.
//

#include "catch.hpp"
#include <dukv8.h>

#define IMMUTABLE_STRING(string_literal)                                \
  ::ImmutableAsciiSource::CreateFromLiteral(                      \
      string_literal "", sizeof(string_literal) - 1)
#define BUILTIN_ASCII_ARRAY(array, len)                                 \
  ::ImmutableAsciiSource::CreateFromLiteral(array, len)

class ImmutableAsciiSource : public v8::String::ExternalAsciiStringResource {
public:
    static v8::Handle<v8::String> CreateFromLiteral(const char *string_literal,
                                                    size_t length) {
        v8::HandleScope scope;
        v8::Local<v8::String> ret = v8::String::NewExternal(new ImmutableAsciiSource(
                string_literal,
                length));
        return scope.Close(ret);
    }

    ImmutableAsciiSource(const char *src, size_t src_len)
            : buffer_(src),
              buf_len_(src_len) {
    }

    ~ImmutableAsciiSource() {
    }

    const char *data() const {
        return buffer_;
    }

    size_t length() const {
        return buf_len_;
    }

private:
    const char *buffer_;
    size_t buf_len_;
};

TEST_CASE_METHOD(Fixture, "String::ExternalAsciiStringResource") {
    v8::HandleScope scope;
    v8::Local<v8::Context> local_env;

    {
        LocalContext env;
        local_env = env.local();
    }

    // Local context should still be live.
    CHECK(!local_env.IsEmpty());
    local_env->Enter();

    v8::Handle<v8::String> source = IMMUTABLE_STRING("1 + 2 + 3");

    CHECK(source->IsString());
    CHECK(!source->IsExternal());
    CHECK(source->IsExternalAscii());

    v8::Local<v8::Script> script = v8::Script::Compile(source);
    v8::Local<v8::Value> value = script->Run();
    CHECK(value->IsNumber());
    REQUIRE(6 == value->Int32Value());

    local_env->Exit();
}