/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** TerminalDisplay.hpp
*/

#ifndef TERMINALDISPLAY_HPP
#define TERMINALDISPLAY_HPP

#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <ncurses.h>
#include "core/ICoreModule.hpp"

class TerminalDisplay {
public:
    TerminalDisplay();
    ~TerminalDisplay();

    void init();
    void addModule(std::shared_ptr<ICoreModule> module);
    void refresh();
    void handleInput();
    bool shouldExit() const;
    void cleanup();

private:
    void drawModule(const std::shared_ptr<ICoreModule>& module, int startY, int startX, int width, int height);
    void calculateLayout();

    std::vector<std::shared_ptr<ICoreModule>> modules;
    bool exitFlag;
    int refreshRate; // en millisecondes
    
    struct Layout {
        int rows;
        int cols;
        int moduleWidth;
        int moduleHeight;
    } layout;
};

#endif // TERMINALDISPLAY_HPP
