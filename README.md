# MyTop - System Monitoring Tool

## Overview
MyTop is a versatile system monitoring tool that provides real-time insights into your system's performance. It displays information about CPU usage, memory utilization, battery status, network statistics, and more. The application offers both terminal-based and graphical interfaces, making it suitable for various environments and user preferences.

## Features
- **Modular Architecture**: Different system components are represented as individual modules
- **Dual Interface**: Choose between terminal (ncurses) or graphical (SFML) interface
- **Real-time Statistics**: Continuously updated system information
- **Module Selection**: Enable/disable specific monitoring modules
- **Customizable Refresh Rate**: Adjust data refresh frequency

## System Modules
MyTop includes the following monitoring modules:
- **System Info**: Hostname, username, kernel version, and shell
- **CPU Info**: CPU model, frequency, core count, and per-core usage
- **RAM Info**: Memory usage, including free, used, active, and inactive memory
- **Battery Info**: Battery status and percentage (if available)
- **Network Info**: Network interface status, MAC addresses, and data transfer statistics
- **Time Info**: Current date/time and system uptime

## Installation
### Prerequisites
#### For macOS:
```bash
# Install SFML and ncurses
brew install sfml@2 ncurses
```

#### For Linux:
```bash
# Install SFML and ncurses
sudo apt-get install libsfml-dev libncurses-dev
```

### Building from Source
1. Clone the repository:
```bash
git clone https://github.com/yourusername/mytop.git
cd mytop
```

2. Edit the Makefile based on your system:
For macOS, use the provided Makefile.
For Linux, modify the SFMLFLAGS in the Makefile:
```
# Change from
SFMLFLAGS = -I$(SFML_PATH)/include -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# To
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
```

3. Build the application:
```bash
make
```

## Usage
### Basic Usage
Run MyTop with the terminal interface (default):
```bash
./mytop
```
Run MyTop with the graphical interface:
```bash
./mytop -g
```

### Command Line Arguments
```
Usage: ./mytop [options]
Options:
  -t, --terminal   Use terminal interface (default)
  -g, --graphical  Use graphical interface (SFML)
  -h, --help       Display this help message
```

### Controls
#### Terminal Interface
- `q` / `Q`: Quit application
- `m` / `M`: Toggle module selector
- `+` / `-`: Adjust refresh rate
- Space / Enter: Toggle module visibility (in selector mode)
- Arrow keys: Navigate in selector mode

#### Graphical Interface
- `Esc`: Quit application
- `m` / `M`: Toggle module selector
- `+` / `-`: Adjust refresh rate
- Space / Enter: Toggle module visibility (in selector mode)
- Arrow keys: Navigate in selector mode

## Project Architecture
MyTop follows a modular and extensible architecture:

### Core Components
- **ICoreModule Interface**: Base interface for all system monitoring modules
- **CoreModule**: Abstract base class implementing common module functionality
- **Specialized Modules**: CPU, RAM, Battery, etc. each implementing specific monitoring features

### Display Components
- **IDisplay Interface**: Common interface for display methods
- **TerminalDisplay**: Implementation using ncurses for terminal-based UI
- **SFMLDisplay**: Implementation using SFML for graphical UI

### Main Program Flow
1. Parse command-line arguments to determine display mode
2. Initialize the appropriate display (Terminal or SFML)
3. Create and register system monitoring modules
4. Enter the main loop:
   - Update system data
   - Refresh the display
   - Handle user input
5. Clean up resources on exit

## Extending MyTop
You can extend MyTop by adding new monitoring modules:
1. Create a new class that inherits from `CoreModule`
2. Implement the required `updateData()` method
3. Add your module to the main program

Example:
```cpp
class MyNewModule : public CoreModule {
public:
    MyNewModule() : CoreModule("My New Module") {}
    
    void updateData() override {
        data.clear();
        data.push_back({"Key1", "Value1"});
        data.push_back({"Key2", "Value2"});
    }
};

// In main.cpp
display->addModule(std::make_shared<MyNewModule>());
```

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments
- SFML for the graphics library
- ncurses for the terminal interface
- All contributors to the project