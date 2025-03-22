/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreSysModule.cpp
*/

#ifndef COREBATMODULE_HPP
#define COREBATMODULE_HPP

#include "CoreModule.hpp"
#include <string>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>
#include <CoreFoundation/CoreFoundation.h>

class CoreBatModule : public CoreModule {
    public:
        CoreBatModule();
        void updateData() override;

    private:
        bool hasBattery() const;
        std::string getBatteryStatus() const;
        std::string getBatteryPercentage() const;
};

#endif