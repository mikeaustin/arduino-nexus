//
// Nexus.h
//

#include <Arduino.h>

#include "Nexus_OS/Nexus_Symbol.h"
#include "Nexus_OS/Nexus_Message.h"
#include "Nexus_OS/Nexus_Task.h"
#include "Nexus_OS/Nexus_Scheduler.h"
#include "Nexus_OS/Nexus_Terminal.h"

namespace Nexus {

    extern class Scheduler Scheduler;

}

/*

9  Bytes
19 Bytes    First 'new'

*/
