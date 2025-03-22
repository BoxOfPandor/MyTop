#include "CoreCPU.hpp"
#include <sys/sysctl.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>
#include <mach/vm_map.h>
#include <iomanip>
#include <sstream>

CoreCPU::CoreCPU() : CoreModule("CPU Info") {
    CoreCPU::updateData();
}

void CoreCPU::updateData() {
    data.clear();
    data.push_back({"Model", getModel()});
    data.push_back({"Frequency", getFrequency()});
    data.push_back({"Cores", std::to_string(getNumberOfCores())});
    data.push_back({"Global Activity", getActivity()});

    // Add per-core activity
    std::vector<std::string> coreActivities = getPerCoreActivity();
    for (size_t i = 0; i < coreActivities.size(); i++) {
        data.push_back({"Core " + std::to_string(i) + " Activity", coreActivities[i]});
    }
}

std::string CoreCPU::getModel() const {
    // Implémentation inchangée
    char buffer[1024];
    size_t size = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &size, nullptr, 0) == 0) {
        return std::string(buffer);
    }
    return "Unknown Model";
}

std::string CoreCPU::getFrequency() const {
    // Implémentation inchangée
    uint64_t freq = 0;
    size_t size = sizeof(freq);
    if (sysctlbyname("hw.cpufrequency", &freq, &size, nullptr, 0) == 0) {
        return std::to_string(freq / 1000000) + " MHz";
    }
    return "Unknown Frequency";
}

int CoreCPU::getNumberOfCores() const {
    // Implémentation inchangée
    int ncpu;
    size_t size = sizeof(ncpu);
    if (sysctlbyname("hw.ncpu", &ncpu, &size, nullptr, 0) == 0) {
        return ncpu;
    }
    return 1;
}

std::vector<std::string> CoreCPU::getPerCoreActivity() const {
    std::vector<std::string> coreActivities;
    processor_cpu_load_info_t cpuLoad;
    mach_msg_type_number_t processorMsgCount;
    natural_t processorCount;

    kern_return_t err = host_processor_info(mach_host_self(),
                                          PROCESSOR_CPU_LOAD_INFO,
                                          &processorCount,
                                          (processor_info_array_t*)&cpuLoad,
                                          &processorMsgCount);

    if (err == KERN_SUCCESS) {
        // Calculate activity for each core
        for (natural_t i = 0; i < processorCount; i++) {
            uint64_t totalCoreTicks = 0;

            for (int state = 0; state < CPU_STATE_MAX; state++) {
                totalCoreTicks += cpuLoad[i].cpu_ticks[state];
            }

            uint64_t idleTicks = cpuLoad[i].cpu_ticks[CPU_STATE_IDLE];
            double coreActivity = 100.0 * (1.0 - ((double)idleTicks / totalCoreTicks));

            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << coreActivity << "%";
            coreActivities.push_back(oss.str());
        }

        // Don't forget to deallocate the memory
        vm_deallocate(mach_task_self(), (vm_address_t)cpuLoad,
                      processorMsgCount * sizeof(processor_cpu_load_info_data_t));
    } else {
        // Add a placeholder if we couldn't get the data
        coreActivities.push_back("Unknown");
    }

    return coreActivities;
}

std::string CoreCPU::getActivity() const {
    // Implémentation inchangée
    processor_cpu_load_info_t cpuLoad;
    mach_msg_type_number_t processorMsgCount;
    natural_t processorCount;

    kern_return_t err = host_processor_info(mach_host_self(),
                                          PROCESSOR_CPU_LOAD_INFO,
                                          &processorCount,
                                          (processor_info_array_t*)&cpuLoad,
                                          &processorMsgCount);

    if (err == KERN_SUCCESS) {
        uint64_t totalTicks = 0;
        uint64_t idleTicks = 0;

        for (natural_t i = 0; i < processorCount; i++) {
            for (int state = 0; state < CPU_STATE_MAX; state++) {
                totalTicks += cpuLoad[i].cpu_ticks[state];
                if (state == CPU_STATE_IDLE) {
                    idleTicks += cpuLoad[i].cpu_ticks[state];
                }
            }
        }

        double activity = 100.0 * (1.0 - ((double)idleTicks / totalTicks));
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << activity << "%";

        // Free the allocated memory
        vm_deallocate(mach_task_self(), (vm_address_t)cpuLoad,
                      processorMsgCount * sizeof(processor_cpu_load_info_data_t));

        return oss.str();
    }

    return "Unknown Activity";
}