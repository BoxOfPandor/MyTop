/*
** EPITECH PROJECT, 2025
** B-PDG-300-LIL-3-1-PDGRUSH3-thibault.pouch
** File description:
** CoreRAM.cpp
*/

#include "CoreRAM.hpp"
#include <iomanip>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <sys/sysctl.h>

CoreRAM::CoreRAM() : CoreModule("RAM Info")
{
    CoreRAM::updateData();
}

void CoreRAM::updateData()
{
    data.clear();
    MemInfo info = getMemInfo();
    unsigned long used = info.total - info.free;

    data.push_back({"Total RAM", formatSize(info.total)});
    data.push_back({"Used RAM", formatSize(used)});
    data.push_back({"Free RAM", formatSize(info.free)});
    data.push_back({"Active", formatSize(info.active)});
    data.push_back({"Inactive", formatSize(info.inactive)});
    data.push_back({"Wired", formatSize(info.wired)});
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << (float)(used * 100) / info.total << "%";
    data.push_back({"Usage", oss.str()});
}

CoreRAM::MemInfo CoreRAM::getMemInfo() const
{
    MemInfo info = {0, 0, 0, 0, 0, 0};
    
    // Get total physical memory
    int mib[2] = {CTL_HW, HW_MEMSIZE};
    size_t length = sizeof(info.total);
    if (sysctl(mib, 2, &info.total, &length, NULL, 0) == 0) {
        // Convert bytes to kilobytes
        info.total /= 1024;
    }
    
    // Get VM statistics
    vm_size_t page_size;
    mach_port_t mach_port = mach_host_self();
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    
    host_page_size(mach_port, &page_size);
    
    if (host_statistics64(mach_port, HOST_VM_INFO64, 
                        (host_info64_t)&vm_stats, &count) == KERN_SUCCESS) {
        // Calculate memory stats in KB
        info.free = (vm_stats.free_count * page_size) / 1024;
        info.active = (vm_stats.active_count * page_size) / 1024;
        info.inactive = (vm_stats.inactive_count * page_size) / 1024;
        info.wired = (vm_stats.wire_count * page_size) / 1024;
    }
    
    return info;
}

std::string CoreRAM::formatSize(unsigned long size) const
{
    std::ostringstream oss;
    if (size > 1024 * 1024) {
        oss << std::fixed << std::setprecision(2)
            << (float)size / (1024 * 1024) << " GB";
    } else if (size > 1024) {
        oss << std::fixed << std::setprecision(2)
            << (float)size / 1024 << " MB";
    } else {
        oss << size << " KB";
    }
    return oss.str();
}