/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreSysModule.cpp
*/

#ifndef CORENETMODULE_HPP
#define CORENETMODULE_HPP

#include "CoreModule.hpp"
#include <string>
#include <vector>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_dl.h>

class CoreNetModule : public CoreModule {
    public:
        CoreNetModule();
        void updateData() override;

    private:
        struct InterfaceInfo {
            std::string name;
            std::string mac;
            std::string status;
            uint64_t rx_bytes;
            uint64_t tx_bytes;
        };
        std::vector<InterfaceInfo> getInterfaces() const;
        std::string getMacAddress(const struct sockaddr_dl* sdl) const;
        std::string getInterfaceStatus(const std::string& interface) const;
        bool getInterfaceStatistics(const std::string& interface, uint64_t& rx_bytes, uint64_t& tx_bytes) const;
};

#endif