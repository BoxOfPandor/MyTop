#include "SFMLDisplay.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

SFMLDisplay::SFMLDisplay() 
    : exitFlag(false), 
      refreshRate(1000), 
      selectorMode(false), 
      selectedModuleIndex(0),
      fontLoaded(false) {
    // Initialisation de la police par défaut
    defaultFont.loadFromFile("/System/Library/Fonts/Menlo.ttc");  // Une police système macOS
}

SFMLDisplay::~SFMLDisplay() {
    cleanup();
}

void SFMLDisplay::init() {
    // Créer la fenêtre
    window.create(sf::VideoMode(1024, 768), "MyTop System Monitor", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    
    // Charger la police
    fontLoaded = font.loadFromFile("resources/DejaVuSansMono.ttf");
    if (!fontLoaded) {
        std::cerr << "Failed to load font. Using default font." << std::endl;
    }
    
    // Configurer la disposition
    layout.moduleWidth = 320;
    layout.moduleHeight = 240;
    layout.modulesPerRow = 3;
    
    refreshClock.restart();
}

void SFMLDisplay::addModule(std::shared_ptr<ICoreModule> module) {
    modules.push_back(module);
    moduleVisibility[module->getName()] = true;
}

void SFMLDisplay::refresh() {
    // Vérifier s'il est temps de rafraîchir les données
    if (refreshClock.getElapsedTime().asMilliseconds() >= refreshRate) {
        for (auto& module : modules) {
            if (moduleVisibility[module->getName()]) {
                module->updateData();
            }
        }
        refreshClock.restart();
    }
    
    window.clear(sf::Color(30, 30, 30));
    
    // Titre principal
    sf::Text titleText;
    titleText.setFont(fontLoaded ? font : defaultFont);
    titleText.setString("MyTop System Monitor");
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition((window.getSize().x - titleText.getLocalBounds().width) / 2, 10);
    window.draw(titleText);
    
    if (selectorMode) {
        showModuleSelector();
    } else {
        // Afficher les modules
        int visibleCount = std::count_if(moduleVisibility.begin(), moduleVisibility.end(), 
                                        [](const auto& pair) { return pair.second; });
        
        int modulesPerRow = layout.modulesPerRow;
        float startX = 20;
        float startY = 50;
        
        int visibleIndex = 0;
        for (size_t i = 0; i < modules.size(); i++) {
            if (moduleVisibility[modules[i]->getName()]) {
                int row = visibleIndex / modulesPerRow;
                int col = visibleIndex % modulesPerRow;
                
                float x = startX + col * (layout.moduleWidth + 20);
                float y = startY + row * (layout.moduleHeight + 20);
                
                drawModule(modules[i], x, y, layout.moduleWidth, layout.moduleHeight);
                visibleIndex++;
            }
        }
    }
    
    // Instructions
    sf::Text instructionsText;
    instructionsText.setFont(fontLoaded ? font : defaultFont);
    instructionsText.setString("Press M to toggle module selector | +/- to adjust refresh rate | ESC to quit");
    instructionsText.setCharacterSize(14);
    instructionsText.setFillColor(sf::Color(200, 200, 200));
    instructionsText.setPosition(10, window.getSize().y - 30);
    window.draw(instructionsText);
    
    // Rafraîchir l'écran
    window.display();
}

void SFMLDisplay::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            exitFlag = true;
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (selectorMode) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                    case sf::Keyboard::M:
                        selectorMode = false;
                        break;
                    case sf::Keyboard::Up:
                        selectedModuleIndex = (selectedModuleIndex - 1 + modules.size()) % modules.size();
                        break;
                    case sf::Keyboard::Down:
                        selectedModuleIndex = (selectedModuleIndex + 1) % modules.size();
                        break;
                    case sf::Keyboard::Space:
                    case sf::Keyboard::Return:
                        toggleModuleVisibility(selectedModuleIndex);
                        break;
                    default:
                        break;
                }
            } else {
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        exitFlag = true;
                        break;
                    case sf::Keyboard::M:
                        selectorMode = true;
                        break;
                    case sf::Keyboard::Add:
                    case sf::Keyboard::Equal:
                        refreshRate = std::max(100, refreshRate - 100);
                        break;
                    case sf::Keyboard::Subtract:
                    case sf::Keyboard::Dash:
                        refreshRate = std::min(5000, refreshRate + 100);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

bool SFMLDisplay::shouldExit() const {
    return exitFlag;
}

void SFMLDisplay::cleanup() {
    if (window.isOpen()) {
        window.close();
    }
}

void SFMLDisplay::drawModule(const std::shared_ptr<ICoreModule>& module, float x, float y, float width, float height) {
    // Fond du module
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color(100, 100, 100));
    window.draw(background);
    
    // Titre du module
    sf::Text titleText;
    titleText.setFont(fontLoaded ? font : defaultFont);
    titleText.setString(module->getName());
    titleText.setCharacterSize(18);
    titleText.setFillColor(sf::Color(120, 200, 120));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(x + 10, y + 5);
    window.draw(titleText);
    
    // Ligne de séparation
    sf::RectangleShape separator(sf::Vector2f(width - 20, 1));
    separator.setPosition(x + 10, y + 30);
    separator.setFillColor(sf::Color(100, 100, 100));
    window.draw(separator);
    
    // Contenu du module
    const auto& moduleData = module->getData();
    float contentY = y + 40;
    float labelX = x + 15;
    float valueX = x + width / 2;
    
    for (const auto& entry : moduleData) {
        if (contentY > y + height - 25) break;  // Éviter de déborder
        
        std::string key = entry.first + ":";
        std::string value = entry.second;
        
        // Clé
        sf::Text keyText;
        keyText.setFont(fontLoaded ? font : defaultFont);
        keyText.setString(key);
        keyText.setCharacterSize(14);
        keyText.setFillColor(sf::Color::White);
        keyText.setPosition(labelX, contentY);
        window.draw(keyText);
        
        // Valeur
        sf::Text valueText;
        valueText.setFont(fontLoaded ? font : defaultFont);
        valueText.setCharacterSize(14);
        
        // Vérifier si c'est un pourcentage
        if (value.length() > 1 && value.back() == '%') {
            try {
                // Extraire le pourcentage
                float percentage = std::stof(value.substr(0, value.length() - 1));
                
                // Afficher le texte de pourcentage
                valueText.setString(value);
                valueText.setFillColor(getColorForPercentage(percentage));
                valueText.setPosition(valueX, contentY);
                window.draw(valueText);
                
                // Dessiner une barre de progression
                if (width > 200) {  // Seulement si on a assez d'espace
                    drawProgressBar(valueX + 70, contentY + 7, width - valueX - 90, 12, percentage);
                }
            } catch (const std::invalid_argument&) {
                // Si la conversion échoue, afficher normalement
                valueText.setString(value);
                valueText.setFillColor(sf::Color(200, 200, 200));
                valueText.setPosition(valueX, contentY);
                window.draw(valueText);
            }
        } else {
            // Valeur normale
            valueText.setString(value);
            valueText.setFillColor(sf::Color(200, 200, 200));
            valueText.setPosition(valueX, contentY);
            window.draw(valueText);
        }
        
        contentY += 25;
    }
}

void SFMLDisplay::drawProgressBar(float x, float y, float width, float height, float percentage) {
    // Fond de la barre
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(70, 70, 70));
    background.setOutlineThickness(1);
    background.setOutlineColor(sf::Color(100, 100, 100));
    window.draw(background);
    
    // Partie remplie de la barre
    float fillWidth = (percentage / 100.0f) * width;
    if (fillWidth > 0) {
        sf::RectangleShape fill(sf::Vector2f(fillWidth, height));
        fill.setPosition(x, y);
        fill.setFillColor(getColorForPercentage(percentage));
        window.draw(fill);
    }
}

sf::Color SFMLDisplay::getColorForPercentage(float percentage) const {
    if (percentage < 50.0f) {
        return sf::Color(0, 200, 0);  // Vert pour faible utilisation
    } else if (percentage < 80.0f) {
        return sf::Color(240, 200, 0);  // Jaune pour utilisation moyenne
    } else {
        return sf::Color(240, 50, 50);  // Rouge pour utilisation élevée
    }
}

void SFMLDisplay::showModuleSelector() {
    // Fond du sélecteur
    float selectorWidth = 350;
    float selectorHeight = std::min(400.0f, static_cast<float>(modules.size() * 40 + 100));
    float x = (window.getSize().x - selectorWidth) / 2;
    float y = (window.getSize().y - selectorHeight) / 2;
    
    sf::RectangleShape background(sf::Vector2f(selectorWidth, selectorHeight));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(60, 60, 60));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color(120, 120, 120));
    window.draw(background);
    
    // Titre du sélecteur
    sf::Text titleText;
    titleText.setFont(fontLoaded ? font : defaultFont);
    titleText.setString("Module Selector");
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color(220, 220, 220));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(x + (selectorWidth - titleText.getLocalBounds().width) / 2, y + 15);
    window.draw(titleText);
    
    // Instructions
    sf::Text instructionsText;
    instructionsText.setFont(fontLoaded ? font : defaultFont);
    instructionsText.setString("Select modules to display");
    instructionsText.setCharacterSize(16);
    instructionsText.setFillColor(sf::Color(180, 180, 180));
    instructionsText.setPosition(x + 20, y + 50);
    window.draw(instructionsText);
    
    // Liste des modules
    float listY = y + 80;
    for (size_t i = 0; i < modules.size(); i++) {
        bool isSelected = (i == static_cast<size_t>(selectedModuleIndex));
        bool isVisible = moduleVisibility[modules[i]->getName()];
        
        // Fond de l'élément sélectionné
        if (isSelected) {
            sf::RectangleShape selectedBg(sf::Vector2f(selectorWidth - 40, 30));
            selectedBg.setPosition(x + 20, listY);
            selectedBg.setFillColor(sf::Color(80, 100, 120));
            window.draw(selectedBg);
        }
        
        // Checkbox
        sf::RectangleShape checkbox(sf::Vector2f(20, 20));
        checkbox.setPosition(x + 30, listY + 5);
        checkbox.setFillColor(sf::Color(40, 40, 40));
        checkbox.setOutlineThickness(1);
        checkbox.setOutlineColor(sf::Color(150, 150, 150));
        window.draw(checkbox);
        
        if (isVisible) {
            sf::RectangleShape check(sf::Vector2f(14, 14));
            check.setPosition(x + 33, listY + 8);
            check.setFillColor(sf::Color(100, 200, 100));
            window.draw(check);
        }
        
        // Nom du module
        sf::Text moduleText;
        moduleText.setFont(fontLoaded ? font : defaultFont);
        moduleText.setString(modules[i]->getName());
        moduleText.setCharacterSize(16);
        moduleText.setFillColor(isSelected ? sf::Color::White : sf::Color(200, 200, 200));
        moduleText.setPosition(x + 60, listY + 5);
        window.draw(moduleText);
        
        listY += 40;
    }
    
    // Instructions en bas
    sf::Text footerText;
    footerText.setFont(fontLoaded ? font : defaultFont);
    footerText.setString("Space: toggle | Enter: confirm | ESC: cancel");
    footerText.setCharacterSize(14);
    footerText.setFillColor(sf::Color(150, 150, 150));
    footerText.setPosition(x + 20, y + selectorHeight - 30);
    window.draw(footerText);
}

void SFMLDisplay::toggleModuleVisibility(int index) {
    if (index >= 0 && index < static_cast<int>(modules.size())) {
        std::string moduleName = modules[index]->getName();
        moduleVisibility[moduleName] = !moduleVisibility[moduleName];
    }
}
