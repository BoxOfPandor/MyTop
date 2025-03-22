#include "TerminalDisplay.hpp"
#include "core/CoreSys.hpp"
#include "core/CoreCPU.hpp"
#include "core/CoreRAM.hpp"
#include "core/CoreBat.hpp"
#include "core/CoreNet.hpp"
#include "core/CoreTime.hpp"
#include <memory>

int main() {
    // Créer l'interface d'affichage
    auto display = std::make_unique<TerminalDisplay>();
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
