//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/heap_statistics.h"
#include "dukv8/utility.h"

namespace v8 {

HeapStatistics::HeapStatistics() :
        total_heap_size_(0),
        total_heap_size_executable_(0),
        used_heap_size_(0),
        heap_size_limit_() {
}

/*static*/ void HeapProfiler::DefineWrapperClass(uint16_t class_id, WrapperInfoCallback callback) { /*TODO();*/ }

}
