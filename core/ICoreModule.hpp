/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** ICoreModule.hpp
*/

#ifndef ICOREMODULE_HPP
#define ICOREMODULE_HPP
#include <string>

class ICoreModule {
public:
    virtual ~ICoreModule() = default;

    virtual std::string getName() const = 0;
    virtual void updateData() = 0;
    virtual std::string getData() const = 0;
};


#endif //ICOREMODULE_HPP
