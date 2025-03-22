/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreModule.hpp
*/

#ifndef COREMODULE_HPP
#define COREMODULE_HPP

#include "ICoreModule.hpp"
#include <string>
#include <vector>
#include <utility>

class CoreModule : public ICoreModule {
protected:
    std::string name;
    std::vector<std::pair<std::string, std::string>> data;

public:
    CoreModule(const std::string& moduleName);

    std::string getName() const override;
    std::vector<std::pair<std::string, std::string>> getData() const override;
    virtual void updateData() override = 0;
};

#endif // COREMODULE_HPP
