/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreModule.cpp
*/

#include "CoreModule.hpp"

/**
 * @brief CoreModule constructor.
 * @param moduleName The name assigned to this module.
 */
CoreModule::CoreModule(const std::string& moduleName) : name(moduleName), data("N/A") {}

/**
 * @brief Get the module's name.
 * @return The name of the module.
 */
std::string CoreModule::getName() const
{
    return name;
}

/**
 * @brief Get the current data stored in the module.
 * @return A string representing the module's data.
 */
std::string CoreModule::getData() const
{
    return data;
}

/**
 * @brief Print the module's name and its current data in a structured format.
 */
void CoreModule::printModule() const
{
    std::cout << "------------ " << name << " ------------" << std::endl;
    std::cout << data << std::endl;
    std::cout << "------------------------------------" << std::endl;
}
