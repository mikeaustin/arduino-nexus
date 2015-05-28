//
// Nexus_OS.cpp
//

//#include "Nexus_OS.h"

#include "Nexus_OS/Nexus_Task.cpp"

int availableMemory;

namespace Nexus {

    class Scheduler Scheduler;

    const char Commands::help_name[] PROGMEM = "help";
    const char Commands::info_name[] PROGMEM = "info";
    const char Commands::tasks_name[] PROGMEM = "tasks";

}
