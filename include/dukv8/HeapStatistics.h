//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_HEAPSTATISTICS_H
#define DUKV8_HEAPSTATISTICS_H

#include "Base.h"

namespace v8 {

/**
 * Collection of V8 heap information.
 *
 * Instances of this class can be passed to v8::V8::HeapStatistics to
 * get heap statistics from V8.
 */
class V8EXPORT HeapStatistics {
public:
    HeapStatistics();
    size_t total_heap_size() { return total_heap_size_; }
    size_t total_heap_size_executable() { return total_heap_size_executable_; }
    size_t used_heap_size() { return used_heap_size_; }
    size_t heap_size_limit() { return heap_size_limit_; }

private:
    void set_total_heap_size(size_t size) { total_heap_size_ = size; }
    void set_total_heap_size_executable(size_t size) {
        total_heap_size_executable_ = size;
    }
    void set_used_heap_size(size_t size) { used_heap_size_ = size; }
    void set_heap_size_limit(size_t size) { heap_size_limit_ = size; }

    size_t total_heap_size_;
    size_t total_heap_size_executable_;
    size_t used_heap_size_;
    size_t heap_size_limit_;

    friend class V8;
};

}

#endif //DUKV8_HEAPSTATISTICS_H
