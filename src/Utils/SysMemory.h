#ifndef MYENGINE_UTILS_SYSMEMORY_H
#define MYENGINE_UTILS_SYSMEMORY_H
#include <cstdint>
#include <cstring>
#include <cstdio>

namespace MyEngine {
    class SysMemory {
    public:
        explicit SysMemory() = delete;
        SysMemory(const SysMemory&) = delete;
        SysMemory(SysMemory&&) = delete;
        SysMemory& operator=(const SysMemory&) = delete;
        SysMemory& operator=(SysMemory&&) = delete;
        ~SysMemory() = delete;

        struct SysMemStatus {
            size_t total_mem{0};
            size_t used_mem{0};
            size_t available_mem{0};
        };

        static SysMemStatus getSystemMemoryStatus(bool* ok = nullptr);

        /// Get current process used memory size
        static size_t getCurProcUsedMemSize(bool* ok = nullptr);
    };
}


#endif //MYENGINE_UTILS_SYSMEMORY_H
