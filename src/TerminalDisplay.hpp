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
#include <map>
#include "core/ICoreModule.hpp"
#include "IDisplay.hpp"

class TerminalDisplay : public IDisplay {
public:
    TerminalDisplay();
    ~TerminalDisplay();

    void init() override;
    void addModule(std::shared_ptr<ICoreModule> module) override;
    void refresh() override;
    void handleInput() override;
    bool shouldExit() const override;
    void cleanup() override;

private:
    void drawModule(const std::shared_ptr<ICoreModule>& module, int startY, int startX, int width, int height);
    void calculateLayout();
    void showModuleSelector();
    void toggleModuleVisibility(int index);

    std::vector<std::shared_ptr<ICoreModule>> modules;
    std::map<std::string, bool> moduleVisibility; // Pour suivre quels modules sont visibles
    bool exitFlag;
    bool selectorMode; // Mode sélecteur activé ou non
    int refreshRate; // en millisecondes
    int selectedModuleIndex; // Pour la navigation dans le sélecteur
    
    struct Layout {
        int rows;
        int cols;
        int moduleWidth;
        int moduleHeight;
        int modulesPerRow;
    } layout;
};

#endif // TERMINALDISPLAY_HPP
