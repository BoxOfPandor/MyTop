/*
** EPITECH PROJECT, 2025
** B_PDG_300_LIL_3_1_PDGRUSH3_thibault_pouch
** File description:
** main.cpp
*/

#include "core/CoreSys.hpp"
#include "core/CoreCPU.hpp"
#include "core/CoreRAM.hpp"
#include "core/CoreBat.hpp"
#include "core/CoreNet.hpp"
#include "core/CoreTime.hpp"
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <csignal>

bool running = true;

void signal_handler(int signal) {
    running = false;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    // Setup signal handler for clean exit
    signal(SIGINT, signal_handler);
    
    // Create vector to store modules
    std::vector<std::unique_ptr<CoreModule>> modules;
    
    // Add modules to vector
    modules.push_back(std::make_unique<CoreSysModule>());
    modules.push_back(std::make_unique<CoreCPU>());
    modules.push_back(std::make_unique<CoreRAM>());
    modules.push_back(std::make_unique<CoreBatModule>());
    //modules.push_back(std::make_unique<CoreNetModule>());
    modules.push_back(std::make_unique<CoreTime>());
    
    // Main loop - updates every second
    while (running) {
        clearScreen();
        
        std::cout << "MyTop - System Monitor (Press Ctrl+C to exit)" << std::endl;
        std::cout << "=========================================" << std::endl << std::endl;
        
        // Update and display each module
        for (auto& module : modules) {
            module->updateData();
            module->printModule();
            std::cout << std::endl;
        }
        
        // Wait 1 second before updating again
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\nExiting MyTop..." << std::endl;
    return 0;
}
