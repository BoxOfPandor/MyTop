/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** ICoreModule.hpp
*/

#ifndef ICOREMODULE_HPP
#define ICOREMODULE_HPP

#include <string>
#include <vector>
#include <utility>

class ICoreModule {
public:
    virtual ~ICoreModule() = default;

    // Retourne le nom du module
    virtual std::string getName() const = 0;
    
    // Met à jour les données internes du module
    virtual void updateData() = 0;
    
    // Retourne les données sous forme de paires clé-valeur
    virtual std::vector<std::pair<std::string, std::string>> getData() const = 0;
};

#endif // ICOREMODULE_HPP
