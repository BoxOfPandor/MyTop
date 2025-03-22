/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreCPU.hpp
*/

#ifndef CORECPU_HPP
#define CORECPU_HPP

#include "CoreModule.hpp"
#include <string>
#include <vector>

class CoreCPU : public CoreModule {
public:
    CoreCPU();
    void updateData() override;

private:
    std::string getModel() const;
    std::string getFrequency() const;
    int getNumberOfCores() const;
    std::string getActivity() const;
    std::vector<std::string> getPerCoreActivity() const;
};

#endif // CORECPU_HPP