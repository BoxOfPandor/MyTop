/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreSysModule.cpp
*/

#include "CoreSys.hpp"
#include <sys/utsname.h>
#include <pwd.h>
#include <unistd.h>
#include <sstream>

CoreSysModule::CoreSysModule() : CoreModule("System Info")
{
    CoreSysModule::updateData();
}

void CoreSysModule::updateData()
{
    std::ostringstream oss;
    oss << "Hostname: " << getHostname() << "\n";
    oss << "Username: " << getUser() << "\n";
    oss << "Kernel Version: " << getKernel() << "\n";
    oss << "Shell: " << getShell();

    data = oss.str();
}

std::string CoreSysModule::getHostname() const
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    return "Unknown";
}

std::string CoreSysModule::getUser() const
{
    struct passwd *pw = getpwuid(getuid());
    return pw ? pw->pw_name : "Unknown";
}

std::string CoreSysModule::getKernel() const
{
    struct utsname unameData;
    if (uname(&unameData) == 0) {
        return std::string(unameData.release);
    }
    return "Unknown";
}

std::string CoreSysModule::getShell() const
{
    struct passwd *pw = getpwuid(getuid());
    return pw ? pw->pw_shell : "Unknown";
}