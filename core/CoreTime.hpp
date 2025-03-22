/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreTime.hpp
*/

#ifndef CORETIME_HPP
#define CORETIME_HPP
#include "CoreModule.hpp"
#include <string>

class CoreTime : public CoreModule {
public:
    CoreTime();
    void updateData() override;

private:
    std::string getCurrentDateTime() const;
    std::string getUptime() const;
};

#endif //CORETIME_HPP