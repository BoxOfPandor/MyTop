#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "IDisplay.hpp"
#include <vector>
#include <memory>
#include <map>

class Display : public IDisplay {
protected:
    std::vector<std::shared_ptr<ICoreModule>> modules;
    std::map<std::string, bool> moduleVisibility;
    bool exitFlag;

public:
    Display() : exitFlag(false) {}
    virtual ~Display() = default;
    
    void addModule(std::shared_ptr<ICoreModule> module) override {
        modules.push_back(module);
        moduleVisibility[module->getName()] = true;
    }
    
    bool shouldExit() const override {
        return exitFlag;
    }
};

#endif // DISPLAY_HPP
