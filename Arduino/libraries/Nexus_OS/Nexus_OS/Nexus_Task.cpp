//
// Nexus_Task.cpp
//

#include "../Nexus_OS.h"

namespace Nexus {
    
    void Task::send(const Message& message)
    {
        Scheduler.send(this, message);
    }

    Terminal *Task::getTerminal()
    {
        for (Task *task = this; task != NULL; task = task->getParent())
        {
            if (task->getParent() == NULL) return static_cast<Terminal *>(task);
        }

        return NULL;
    }

    Stream& Task::getStream()
    {
        return getTerminal()->getStream();
    }

}
