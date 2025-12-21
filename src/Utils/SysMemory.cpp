
#include "SysMemory.h"
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "User32.Lib")
#endif

namespace MyEngine {
    SysMemory::SysMemStatus SysMemory::getSystemMemoryStatus(bool* ok) {
        SysMemory::SysMemStatus m_status;
#ifdef _WIN32
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
            while (fgets(line, 256, file)) {
                if (sscanf(line, "MemTotal: %zu kB", &m_status.total_mem) == 1) continue;
                if (sscanf(line, "MemAvailable: %zu kB", &m_status.available_mem) == 1) break;
            }
            m_status.used_mem = m_status.total_mem - m_status.available_mem;
            fclose(file);
#endif
        if (ok) *ok = true;
        return m_status;
    }

    /// Get current process used memory size
    size_t SysMemory::getCurProcUsedMemSize(bool* ok) {
        size_t used_mem = 0;
#ifdef _WIN32
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
    
}

