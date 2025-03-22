/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** CoreSysModule.hpp
*/

#ifndef CORESYSMODULE_HPP
#define CORESYSMODULE_HPP
#include "CoreModule.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

class CoreSysModule : public CoreModule {
public:
    CoreSysModule();
    void updateData() override;

private:
    std::string getHostname() const;
    std::string getUser() const;
    std::string getDistro() const;
    std::string getKernel() const;
    std::string getShell() const;
    std::string readFirstLine(const std::string& filepath) const;
};

#endif //CORESYSMODULE_HPP
