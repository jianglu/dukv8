//
// Created by Jiang Lu on 6/5/15.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <v8.h>

TEST_CASE("InitializeAndDisposeOnce") {
    CHECK(v8::V8::Initialize());
    CHECK(v8::V8::Dispose());
}

// Tests that call v8::V8::Dispose() cannot be threaded.
TEST_CASE("InitializeAndDisposeMultiple") {
    for (int i = 0; i < 3; ++i) CHECK(v8::V8::Dispose());
    for (int i = 0; i < 3; ++i) CHECK(v8::V8::Initialize());
    for (int i = 0; i < 3; ++i) CHECK(v8::V8::Dispose());
    for (int i = 0; i < 3; ++i) CHECK(v8::V8::Initialize());
    for (int i = 0; i < 3; ++i) CHECK(v8::V8::Dispose());
}
