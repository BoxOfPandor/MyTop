#ifndef SFMLDISPLAY_HPP
#define SFMLDISPLAY_HPP

#include "IDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <functional>

class SFMLDisplay : public IDisplay {
public:
    SFMLDisplay();
    ~SFMLDisplay();

    void init() override;
    void addModule(std::shared_ptr<ICoreModule> module) override;
    void refresh() override;
    void handleInput() override;
    bool shouldExit() const override;
    void cleanup() override;

private:
    void drawModule(const std::shared_ptr<ICoreModule>& module, float x, float y, float width, float height);
    void drawProgressBar(float x, float y, float width, float height, float percentage);
    void showModuleSelector();
    void toggleModuleVisibility(int index);
    sf::Color getColorForPercentage(float percentage) const;

    // Fenêtre principale et ressources graphiques
    sf::RenderWindow window;
    sf::Font font;
    sf::Font defaultFont; // Police par défaut fallback
    bool fontLoaded;
    
    // Gestion des modules
    std::vector<std::shared_ptr<ICoreModule>> modules;
    std::map<std::string, bool> moduleVisibility;
    bool exitFlag;
    int refreshRate; // en millisecondes
    sf::Clock refreshClock;
    
    // Interface
    bool selectorMode;
    int selectedModuleIndex;
    
    // Disposition des éléments
    struct Layout {
        float moduleWidth;
        float moduleHeight;
        int modulesPerRow;
    } layout;
};

#endif // SFMLDISPLAY_HPP
