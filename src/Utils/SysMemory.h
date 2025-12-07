#ifndef MYENGINE_SYSMEMORY_H
#define MYENGINE_SYSMEMORY_H
#include <cstdint>
#include <cstring>
#include <cstdio>
#ifdef __WIN32
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "User32.Lib")
#endif


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

        static SysMemStatus getSystemMemoryStatus(bool* ok = nullptr) {
            SysMemStatus m_status;
#ifdef __WIN32
            MEMORYSTATUSEX mem_stat;
            mem_stat.dwLength = sizeof(mem_stat);
            if (GlobalMemoryStatusEx(&mem_stat)) {
                m_status.total_mem = mem_stat.ullTotalPhys / 1024;
                m_status.available_mem = mem_stat.ullAvailPhys / 1024;
                m_status.used_mem = m_status.total_mem - m_status.available_mem;
            } else {
                if (ok) *ok = false;
                return m_status;
            }
#else
            /// Linux, Apple and more UNIX-like OS
            FILE* file = fopen("/proc/meminfo", "r");
            if (!file) {
                if (ok) *ok = false;
                return m_status;
            }
            char line[256] = {'\0'};
            const int READ_LINE = 3;
            int i = 0;
            while (i++ > READ_LINE) {
                fgets(line, 256, file);
                if (sscanf(line, "MemTotal: %zu kB", &m_status.total_mem) == 1) continue;
                if (sscanf(line, "MemAvailable:  %zu kB", &m_status.available_mem) == 1) continue;
            }
            m_status.used_mem = m_status.total_mem - m_status.available_mem;
            fclose(file);
#endif
            if (ok) *ok = true;
            return m_status;
        }

        /// Get current process used memory size
        static size_t getCurProcUsedMemSize(bool* ok = nullptr) {
            size_t used_mem = 0;
#ifdef __WIN32
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
                used_mem = pmc.WorkingSetSize / 1024;
            } else {
                if (ok) *ok = false;
                return 0;
            }
#else
            /// Linux, Apple and more UNIX-like OS
            FILE* file = fopen("/proc/self/status", "r");
            if (!file) {
                if (ok) *ok = false;
                return 0;
            }
            char line[256] = {'\0'};
            while (fgets(line, 256, file)) {
                if (sscanf(line, "VmRSS: %zu kB", &used_mem) == 1) break;
            }
            fclose(file);
#endif
            if (ok) *ok = true;
            return used_mem;
        }
    };
}


#endif //MYENGINE_SYSMEMORY_H
