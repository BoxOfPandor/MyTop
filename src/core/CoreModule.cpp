/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreModule.cpp
*/

#include "CoreModule.hpp"

CoreModule::CoreModule(const std::string& moduleName) : name(moduleName) {}

std::string CoreModule::getName() const {
    return name;
}

std::vector<std::pair<std::string, std::string>> CoreModule::getData() const {
    return data;
}
