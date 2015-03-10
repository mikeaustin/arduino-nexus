//
// Nexus_Task.cpp
//

#include "../Nexus_OS.h"

namespace Nexus {
    
    void Task::send(const Message& message)
    {
        Scheduler.send(*this, message);
    }

}
