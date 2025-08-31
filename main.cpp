#include <cstdlib>
#include <iostream>
#include <string>
#include <set>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iomanip>

#include <iostream>
#include <iomanip>
#include <cstdlib>

void showHelp() {
    system("cat /proc/acpi/ibm/fan");

    std::cout << std::left << std::setw(30) << "\nFLAGS" 
              << std::left << std::setw(15) << "DEFAULT" << "\n";

    struct Option {
        const char* flags;
        const char* def;
    } options[] = {
        {"-h, --help", ""},
        {"-s, --stats <seconds>", "2"},
        {"-l, --level <level>", "auto"},
        {"-m, --mode <enable/disable>", "enable"},
        {"-w, --watchdog <value>", "0"}
    };

    for (auto &opt : options) {
        std::cout << std::left << std::setw(30) << opt.flags
                  << std::left << std::setw(15) << opt.def << "\n";
    }

    std::cout << "\n>\u001B]8;;https://github.com/NotiLo-A/MaxFan/\u0007Sourse\u001B]8;;\u0007<\n";
}



int main(int argc, char* argv[]) {
    if (geteuid() != 0) {
        std::cerr << "run as sudo\n";
        return 1;
    }

    std::ifstream fanFile("/proc/acpi/ibm/fan");
    if (!fanFile.is_open()) {
        std::cerr << "Are you using a Thinkpad?\n";
        return 1;
    }

    if (argc == 1) {
        std::cout << "sudo maxfan --help" << std::endl;
        return 0;
    }

    bool showHelpFlag = false;
    int stats = 2;
    std::string level = "auto";
    std::string mode = "enable";
    int watchdog = 0;
    bool statsSet = false;
    bool levelSet = false;
    bool modeSet = false;
    bool watchdogSet = false;

    std::set<std::string> validLevels = {
        "0","1","2","3","4","5","6","7",
        "auto","disengaged","full-speed"
    };

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            showHelpFlag = true;
        }
        else if (arg == "-s" || arg == "--stats") {
            if (i + 1 < argc && isdigit(argv[i + 1][0])) {
                stats = std::atoi(argv[++i]);
            } 
            statsSet = true;
        }
        else if (arg == "-l" || arg == "--level") {
            if (i + 1 < argc && validLevels.count(argv[i + 1])) {
                level = argv[++i];
            } else if (i + 1 < argc) {
                showHelp();
                return 1;
            }
            levelSet = true;
        } 
        else if (arg == "-m" || arg == "--mode") {
            if (i + 1 < argc && (std::string(argv[i + 1]) == "enable" || std::string(argv[i + 1]) == "disable")) {
                mode = argv[++i];  
            } else if (i + 1 < argc) {
                showHelp();
                return 1;
            }
            modeSet = true;
        }
        else if (arg == "-w" || arg == "--watchdog") {
            if (i + 1 < argc) {
                int value = std::atoi(argv[i + 1]);
                if (value >= 0 && value <= 120) {
                    watchdog = value;
                    ++i;
                } else {
                    showHelp();
                    return 1;
                }
            }
            watchdogSet = true;
        } 
        else {
            if (validLevels.count(argv[i])) {
                level = argv[i];
                levelSet = true;
            } else {
                showHelp();
                return 1;
            }
        }
    }

    if (showHelpFlag) {
        showHelp();
        return 0;
    }

    if (levelSet) {
        std::string cmd = "echo level '" + level + "' | sudo tee /proc/acpi/ibm/fan > /dev/null";
        system(cmd.c_str());
        std::cout << "Level: " << level << std::endl;
        return 0;
    }

    if (watchdogSet) {
        std::string cmd = "echo watchdog '" + std::to_string(watchdog) + "' | sudo tee /proc/acpi/ibm/fan > /dev/null";
        system(cmd.c_str());
        std::cout << "Watchdog: " << watchdog << std::endl;
        return 0;
    }

    if (modeSet) {
        std::string cmd = "echo '" + mode + "' | sudo tee /proc/acpi/ibm/fan > /dev/null"; 
        system(cmd.c_str());
        std::cout << "Mode: " << mode << std::endl;
    }

    if (statsSet) {
        std::string cmd = "watch -n '" + std::to_string(stats) + "' \"grep -E '^(status|speed|level):' /proc/acpi/ibm/fan\""; 
        system(cmd.c_str());
    }

    return 0;
}
