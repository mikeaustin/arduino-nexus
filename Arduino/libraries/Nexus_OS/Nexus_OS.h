//
// Nexus.h
//

#include <Arduino.h>

template<typename T, typename U>
const T pgm_ptr(const U& addr)
{
    return (const T)pgm_read_ptr(&addr);
}

#include "Nexus_OS/Nexus_Symbol.h"
#include "Nexus_OS/Nexus_Tuple.h"
#include "Nexus_OS/Nexus_Message.h"
#include "Nexus_OS/Nexus_Task.h"
#include "Nexus_OS/Nexus_Scheduler.h"
#include "Nexus_OS/Nexus_Terminal.h"
#include "Nexus_OS/Nexus_Shell.h"
#include "Nexus_OS/Nexus_Storage.h"

#include "Nexus_OS/Nexus_Commands.h"

/*

9  Bytes
19 Bytes    First 'new'

*/
