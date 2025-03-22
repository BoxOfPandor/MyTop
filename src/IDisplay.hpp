#ifndef IDISPLAY_HPP
#define IDISPLAY_HPP

#include "core/ICoreModule.hpp"
#include <vector>
#include <memory>

class IDisplay {
public:
    virtual ~IDisplay() = default;
    
    // Initialiser l'affichage
    virtual void init() = 0;
    
    // Rafraîchir l'affichage avec les données actuelles des modules
    virtual void refresh() = 0;
    
    // Nettoyer/fermer l'affichage
    virtual void cleanup() = 0;
    
    // Ajouter un module à l'affichage
    virtual void addModule(std::shared_ptr<ICoreModule> module) = 0;
    
    // Vérifier si l'utilisateur veut quitter
    virtual bool shouldExit() const = 0;
    
    // Gérer les entrées utilisateur
    virtual void handleInput() = 0;
};

#endif // IDISPLAY_HPP
