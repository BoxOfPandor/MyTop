#include "TerminalDisplay.hpp"
#include "SFMLDisplay.hpp"
#include "core/CoreSys.hpp"
#include "core/CoreCPU.hpp"
#include "core/CoreRAM.hpp"
#include "core/CoreBat.hpp"
#include "core/CoreNet.hpp"
#include "core/CoreTime.hpp"
#include <memory>
#include <iostream>
#include <string>

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -t, --terminal   Use terminal interface (default)" << std::endl;
    std::cout << "  -g, --graphical  Use graphical interface (SFML)" << std::endl;
    std::cout << "  -h, --help       Display this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    // Par défaut, utiliser l'interface terminal
    bool useGraphical = false;
    
    // Analyser les arguments de ligne de commande
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-g" || arg == "--graphical") {
            useGraphical = true;
        } else if (arg == "-t" || arg == "--terminal") {
            useGraphical = false;
        } else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Créer l'interface d'affichage appropriée
    std::unique_ptr<IDisplay> display;
    if (useGraphical) {
        display = std::make_unique<SFMLDisplay>();
    } else {
        display = std::make_unique<TerminalDisplay>();
    }
    
    display->init();

    // Ajouter les modules
    display->addModule(std::make_shared<CoreSysModule>());
    display->addModule(std::make_shared<CoreCPU>());
    display->addModule(std::make_shared<CoreRAM>());
    display->addModule(std::make_shared<CoreBatModule>());
    display->addModule(std::make_shared<CoreNetModule>());
    display->addModule(std::make_shared<CoreTime>());

    // Boucle principale
    while (!display->shouldExit()) {
        display->refresh();
        display->handleInput();
    }

    // Nettoyer avant de quitter
    display->cleanup();

    return 0;
}
