/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** TerminalDisplay.cpp
*/
#include "TerminalDisplay.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

TerminalDisplay::TerminalDisplay() 
    : exitFlag(false), 
      refreshRate(1000), 
      selectorMode(false), 
      selectedModuleIndex(0) {}

TerminalDisplay::~TerminalDisplay() {
    cleanup();
}

void TerminalDisplay::init() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(refreshRate); // Non-blocking mode
    curs_set(0); // Hide cursor

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);  // Low percentage (<50%)
        init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Medium percentage (50-80%)
        init_pair(6, COLOR_RED, COLOR_BLACK);    // High percentage (>80%)
        init_pair(7, COLOR_BLACK, COLOR_WHITE);  // Selected item in menu
    }

    calculateLayout();
}

void TerminalDisplay::addModule(std::shared_ptr<ICoreModule> module) {
    modules.push_back(module);
    // Par défaut, tous les modules sont visibles
    moduleVisibility[module->getName()] = true;
}

void TerminalDisplay::refresh() {
    clear();

    // Main title
    attron(A_BOLD);
    std::string title = "*** MyTop System Monitor ***";
    mvprintw(0, (COLS - title.length()) / 2, "%s", title.c_str());
    attroff(A_BOLD);

    if (selectorMode) {
        showModuleSelector();
    } else {
        // Update and display modules
        int visibleCount = 0;
        for (auto& module : modules) {
            if (moduleVisibility[module->getName()]) {
                visibleCount++;
            }
        }

        // Calculer une disposition optimale
        int modulesPerRow = std::min(3, (int)layout.modulesPerRow);
        int rowCount = (visibleCount + modulesPerRow - 1) / modulesPerRow; // Arrondi au supérieur
        
        int moduleIndex = 0;
        int visibleIndex = 0;
        for (auto& module : modules) {
            if (moduleVisibility[module->getName()]) {
                module->updateData();
                
                int row = 2 + (visibleIndex / modulesPerRow) * (layout.moduleHeight + 1);
                int col = (visibleIndex % modulesPerRow) * (layout.moduleWidth + 2);
                
                drawModule(module, row, col, layout.moduleWidth, layout.moduleHeight);
                visibleIndex++;
            }
            moduleIndex++;
        }
    }

    // Instructions
    mvprintw(LINES - 2, 2, "Press 'm' to toggle module selector");
    
    std::stringstream ss;
    ss << "Press 'q' to quit, '+'/'-' to adjust refresh rate (" << refreshRate << " ms)";
    mvprintw(LINES - 1, 2, "%s", ss.str().c_str());

    // Refresh screen
    ::refresh();
}

void TerminalDisplay::handleInput() {
    int ch = getch();
    
    if (selectorMode) {
        switch (ch) {
            case 'q':
            case 'Q':
            case 'm':
            case 'M':
                selectorMode = false;
                break;
            case KEY_UP:
                selectedModuleIndex = (selectedModuleIndex - 1 + modules.size()) % modules.size();
                break;
            case KEY_DOWN:
                selectedModuleIndex = (selectedModuleIndex + 1) % modules.size();
                break;
            case ' ':
            case '\n':
                toggleModuleVisibility(selectedModuleIndex);
                break;
        }
    } else {
        switch (ch) {
            case 'q':
            case 'Q':
                exitFlag = true;
                break;
            case 'm':
            case 'M':
                selectorMode = true;
                break;
            case '+':
                refreshRate = std::max(100, refreshRate - 100);
                timeout(refreshRate);
                break;
            case '-':
                refreshRate = std::min(5000, refreshRate + 100);
                timeout(refreshRate);
                break;
        }
    }
}

bool TerminalDisplay::shouldExit() const {
    return exitFlag;
}

void TerminalDisplay::cleanup() {
    endwin();
}

void TerminalDisplay::drawModule(const std::shared_ptr<ICoreModule>& module, int startY, int startX, int width, int height) {
    // Draw frame
    mvhline(startY, startX, 0, width);
    mvhline(startY + height, startX, 0, width);
    mvvline(startY, startX, 0, height);
    mvvline(startY, startX + width, 0, height);
    mvaddch(startY, startX, ACS_ULCORNER);
    mvaddch(startY, startX + width, ACS_URCORNER);
    mvaddch(startY + height, startX, ACS_LLCORNER);
    mvaddch(startY + height, startX + width, ACS_LRCORNER);

    // Module title
    std::string moduleTitle = module->getName();
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(startY, startX + (width - moduleTitle.length()) / 2, "%s", moduleTitle.c_str());
    attroff(COLOR_PAIR(2) | A_BOLD);

    // Module content
    const auto& moduleData = module->getData();
    int row = startY + 1;

    for (const auto& entry : moduleData) {
        if (row >= startY + height) break;

        std::string key = entry.first + ":";
        std::string value = entry.second;

        attron(A_BOLD);
        mvprintw(row, startX + 2, "%s", key.c_str());
        attroff(A_BOLD);

        // Check if the value is a percentage
        if (value.length() > 1 && value.back() == '%') {
            try {
                // Parse the percentage value
                double percentage = std::stod(value.substr(0, value.length() - 1));

                // Determine the color based on the percentage
                int colorPair;
                if (percentage < 50.0) {
                    colorPair = 4; // Green for low values
                } else if (percentage < 80.0) {
                    colorPair = 5; // Yellow for medium values
                } else {
                    colorPair = 6; // Red for high values
                }

                // Display the numeric value first
                attron(COLOR_PAIR(colorPair));
                std::string numericDisplay = value + " ";
                mvprintw(row, startX + 3 + key.length(), "%s", numericDisplay.c_str());

                // Calculate progress bar width (use remaining space)
                int availableWidth = width - (4 + key.length() + numericDisplay.length());
                int barWidth = availableWidth - 2; // Leave some margin

                if (barWidth > 5) { // Only draw if we have reasonable space
                    int filledWidth = static_cast<int>((percentage / 100.0) * barWidth);

                    // Draw the progress bar
                    mvprintw(row, startX + 3 + key.length() + numericDisplay.length(), "[");
                    for (int i = 0; i < barWidth; i++) {
                        if (i < filledWidth) {
                            addch('/');
                        } else {
                            addch(' ');
                        }
                    }
                    printw("]");
                }
                attroff(COLOR_PAIR(colorPair));
            } catch (const std::invalid_argument&) {
                // If conversion fails, just display the value normally
                attron(COLOR_PAIR(3));
                mvprintw(row, startX + 3 + key.length(), "%s", value.c_str());
                attroff(COLOR_PAIR(3));
            }
        } else {
            // For non-percentage values, display normally
            attron(COLOR_PAIR(3));
            mvprintw(row, startX + 3 + key.length(), "%s", value.c_str());
            attroff(COLOR_PAIR(3));
        }

        row++;
    }
}

void TerminalDisplay::calculateLayout() {
    getmaxyx(stdscr, layout.rows, layout.cols);

    // Détermine combien de modules peuvent tenir sur une ligne
    layout.modulesPerRow = std::max(1, layout.cols / 50); // Chaque module a besoin d'environ 50 colonnes
    
    layout.moduleWidth = (layout.cols / layout.modulesPerRow) - 3;
    layout.moduleHeight = (layout.rows - 4) / 2; // 2 rangées de modules
}

void TerminalDisplay::showModuleSelector() {
    int startY = 2;
    int startX = (COLS - 40) / 2; // Centrer le menu
    int width = 40;
    int height = modules.size() + 4;

    // Draw menu frame
    mvhline(startY, startX, 0, width);
    mvhline(startY + height, startX, 0, width);
    mvvline(startY, startX, 0, height);
    mvvline(startY, startX + width, 0, height);
    mvaddch(startY, startX, ACS_ULCORNER);
    mvaddch(startY, startX + width, ACS_URCORNER);
    mvaddch(startY + height, startX, ACS_LLCORNER);
    mvaddch(startY + height, startX + width, ACS_LRCORNER);

    // Menu title
    std::string menuTitle = "Module Selector";
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(startY, startX + (width - menuTitle.length()) / 2, "%s", menuTitle.c_str());
    attroff(COLOR_PAIR(2) | A_BOLD);

    // Instructions
    mvprintw(startY + 1, startX + 2, "Select modules to display:");
    
    // List modules
    for (size_t i = 0; i < modules.size(); i++) {
        // Check if this module is currently selected in the menu
        bool isSelected = (i == selectedModuleIndex);
        
        // Check if this module is visible
        bool isVisible = moduleVisibility[modules[i]->getName()];
        
        // Set appropriate attributes
        if (isSelected) attron(COLOR_PAIR(7));
        
        // Show check box status
        std::string checkStatus = isVisible ? "[X]" : "[ ]";
        mvprintw(startY + 2 + i, startX + 2, "%s %s", checkStatus.c_str(), modules[i]->getName().c_str());
        
        if (isSelected) attroff(COLOR_PAIR(7));
    }
    
    // Footer instructions
    mvprintw(startY + height - 1, startX + 2, "Space/Enter: toggle, M/Q: exit menu");
}

void TerminalDisplay::toggleModuleVisibility(int index) {
    if (index >= 0 && index < static_cast<int>(modules.size())) {
        std::string moduleName = modules[index]->getName();
        moduleVisibility[moduleName] = !moduleVisibility[moduleName];
    }
}
