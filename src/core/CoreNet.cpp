/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreSysModule.cpp
*/

#include "CoreNet.hpp"
#include <iomanip>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>

CoreNetModule::CoreNetModule() : CoreModule("Network Info")
{
    CoreNetModule::updateData();
}

void CoreNetModule::updateData()
{
    data.clear();
    std::vector<InterfaceInfo> interfaces = getInterfaces();
    
    for (const auto& iface : interfaces) {
        if (iface.name != "lo0") {
            data.push_back({iface.name + " Status", iface.status});
            data.push_back({iface.name + " MAC", iface.mac});
            data.push_back({iface.name + " RX bytes", std::to_string(iface.rx_bytes)});
            data.push_back({iface.name + " TX bytes", std::to_string(iface.tx_bytes)});
        }
    }
    
    if (data.empty()) {
        data.push_back({"Network", "No interfaces found"});
    }
}

std::vector<CoreNetModule::InterfaceInfo> CoreNetModule::getInterfaces() const
{
    std::vector<InterfaceInfo> interfaces;
    struct ifaddrs *ifaddr;

    if (getifaddrs(&ifaddr) == -1) {
        return interfaces;
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr)
            continue;

        if (ifa->ifa_addr->sa_family == AF_LINK) {
            InterfaceInfo info;
            info.name = ifa->ifa_name;
            info.mac = getMacAddress((struct sockaddr_dl*)ifa->ifa_addr);
            info.status = (ifa->ifa_flags & IFF_UP) ? "up" : "down";

            getInterfaceStatistics(info.name, info.rx_bytes, info.tx_bytes);

            interfaces.push_back(info);
        }
    }

    freeifaddrs(ifaddr);
    return interfaces;
}

std::string CoreNetModule::getMacAddress(const struct sockaddr_dl* sdl) const
{
    if (!sdl)
        return "00:00:00:00:00:00";

    const unsigned char* ptr = (unsigned char*)LLADDR(sdl);
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (int i = 0; i < 6; i++) {
        if (i > 0) oss << ":";
        oss << std::setw(2) << static_cast<int>(ptr[i]);
    }

    return oss.str();
}

bool CoreNetModule::getInterfaceStatistics(const std::string& interface, uint64_t& rx_bytes, uint64_t& tx_bytes) const
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return false;

    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);

    if (ioctl(sock, SIOCGIFMEDIA, &ifr) == 0) {
        struct if_data* data = (struct if_data*)ifr.ifr_data;
        rx_bytes = data->ifi_ibytes;
        tx_bytes = data->ifi_obytes;
    } else {
        rx_bytes = 0;
        tx_bytes = 0;
    }

    close(sock);
    return true;
}