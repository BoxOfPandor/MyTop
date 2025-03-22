/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreModule.hpp
*/

#ifndef COREMODULE_HPP
#define COREMODULE_HPP
#include "ICoreModule.hpp"
#include <iostream>

/**
 * @class CoreModule
 * @brief Base class for all system monitoring modules.
 *
 * CoreModule provides a standard structure for modules, including a name,
 * a data storage system, and a formatted display method.
 */
class CoreModule : public ICoreModule {
protected:
    std::string name;
    std::string data;

public:
    CoreModule(const std::string& moduleName);

    std::string getName() const override;
    std::string getData() const override;
    virtual void updateData() override = 0;
    void printModule() const;
};

#endif //COREMODULE_HPP
