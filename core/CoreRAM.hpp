/*
** EPITECH PROJECT, 2025
** B-PDG-300-LIL-3-1-PDGRUSH3-thibault.pouch
** File description:
** CoreRAM.hpp
*/

#ifndef CORERAM_HPP
#define CORERAM_HPP

#include "CoreModule.hpp"
#include <string>
#include <sstream>

class CoreRAM : public CoreModule {
    public:
        CoreRAM();
        void updateData() override;

    private:
        struct MemInfo {
            unsigned long total;    // Total physical memory
            unsigned long free;     // Free memory
            unsigned long active;   // Active memory
            unsigned long inactive; // Inactive memory
            unsigned long wired;    // Wired memory (cannot be paged out)
            unsigned long compressed; // Compressed memory
        };

        MemInfo getMemInfo() const;
        std::string formatSize(unsigned long size) const;
};

#endif
