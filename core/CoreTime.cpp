/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreTime.cpp
*/

#include "CoreTime.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <sys/sysctl.h>
#include <mach/mach_time.h>

CoreTime::CoreTime() : CoreModule("Time Info")
{
    CoreTime::updateData();
}

void CoreTime::updateData()
{
    data = "Current Date and Time: " + getCurrentDateTime() + "\n";
    data += "Uptime: " + getUptime();
}

std::string CoreTime::getCurrentDateTime() const
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&now_time);
    
    // Remove trailing newline that ctime adds
    if (!time_str.empty() && time_str[time_str.length()-1] == '\n') {
        time_str.erase(time_str.length()-1);
    }
    
    return time_str;
}

std::string CoreTime::getUptime() const
{
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    
    if (sysctl(mib, 2, &boottime, &len, NULL, 0) < 0) {
        return "Unknown";
    }
    
    time_t now = time(NULL);
    time_t uptime = now - boottime.tv_sec;
    
    int days = uptime / 86400;
    int hours = (uptime % 86400) / 3600;
    int minutes = (uptime % 3600) / 60;
    int seconds = uptime % 60;
    
    std::ostringstream oss;
    if (days > 0) {
        oss << days << "d ";
    }
    oss << hours << "h " << minutes << "m " << seconds << "s";
    
    return oss.str();
}
