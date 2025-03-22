/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreSysModule.cpp
*/

#include "CoreBat.hpp"
#include <sstream>

CoreBatModule::CoreBatModule() : CoreModule("Battery Info")
{
    CoreBatModule::updateData();
}

void CoreBatModule::updateData()
{
    if (!hasBattery()) {
        data = "No battery found";
        return;
    }
    data = "Status: " + getBatteryStatus() + "\n";
    data += "Battery: " + getBatteryPercentage() + "%";
}

bool CoreBatModule::hasBattery() const
{
    CFTypeRef powerSourceInfo = IOPSCopyPowerSourcesInfo();
    CFArrayRef powerSources = IOPSCopyPowerSourcesList(powerSourceInfo);

    bool hasBattery = powerSources && CFArrayGetCount(powerSources) > 0;

    if (powerSources) CFRelease(powerSources);
    if (powerSourceInfo) CFRelease(powerSourceInfo);

    return hasBattery;
}

std::string CoreBatModule::getBatteryStatus() const
{
    CFTypeRef powerSourceInfo = IOPSCopyPowerSourcesInfo();
    CFArrayRef powerSources = IOPSCopyPowerSourcesList(powerSourceInfo);

    if (!powerSources) {
        if (powerSourceInfo) CFRelease(powerSourceInfo);
        return "Unknown";
    }

    CFDictionaryRef powerSource = NULL;
    if (CFArrayGetCount(powerSources) > 0) {
        powerSource = IOPSGetPowerSourceDescription(powerSourceInfo, CFArrayGetValueAtIndex(powerSources, 0));

        CFStringRef powerState = (CFStringRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSPowerSourceStateKey));
        if (powerState) {
            if (CFEqual(powerState, CFSTR(kIOPSACPowerValue))) {
                return "Charging";
            } else if (CFEqual(powerState, CFSTR(kIOPSBatteryPowerValue))) {
                return "Discharging";
            }
        }
    }

    if (powerSources) CFRelease(powerSources);
    if (powerSourceInfo) CFRelease(powerSourceInfo);

    return "Unknown";
}

std::string CoreBatModule::getBatteryPercentage() const
{
    CFTypeRef powerSourceInfo = IOPSCopyPowerSourcesInfo();
    CFArrayRef powerSources = IOPSCopyPowerSourcesList(powerSourceInfo);

    if (!powerSources) {
        if (powerSourceInfo) CFRelease(powerSourceInfo);
        return "0";
    }

    std::string percentage = "0";
    if (CFArrayGetCount(powerSources) > 0) {
        CFDictionaryRef powerSource = IOPSGetPowerSourceDescription(powerSourceInfo, CFArrayGetValueAtIndex(powerSources, 0));

        CFNumberRef capacityRef = (CFNumberRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSCurrentCapacityKey));
        if (capacityRef) {
            int capacity;
            CFNumberGetValue(capacityRef, kCFNumberIntType, &capacity);
            percentage = std::to_string(capacity);
        }
    }

    if (powerSources) CFRelease(powerSources);
    if (powerSourceInfo) CFRelease(powerSourceInfo);

    return percentage;
}