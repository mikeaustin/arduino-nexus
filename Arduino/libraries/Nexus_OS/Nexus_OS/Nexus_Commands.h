//
// Commands.h
//

#include <Arduino.h>

#ifndef __Nexus_Commands__
#define __Nexus_Commands__

namespace Nexus {

    struct Commands {

        static const char help_name[] PROGMEM;
        static const char info_name[] PROGMEM;
        static const char tasks_name[] PROGMEM;

        static Task* help(Task* parent)
        {
            Stream& stream = parent->getStream();

            Shell* shell = static_cast<Shell*>(parent);

            for (const Command* command = shell->getCommands(); pgm_ptr<void*>(command->name) != NULL; ++command)
            {
                const symbol name = pgm_ptr<const __FlashStringHelper*>(command->name);

                stream << name << endl;
            }

            return NULL;
        }

        static Task* info(Task* parent)
        {
            Stream& stream = parent->getStream();

            stream << F("Up for ") << millis() / 1000 << F(" seconds.") << endl;
            stream << availableMemory << F(" bytes free.") << endl;

            return NULL;
        }

        static Task* tasks(Task* parent)
        {
            Stream& stream = parent->getStream();

            for (Task* task = Scheduler.getTasks(); task != NULL; task = task->getNext())
            {
                //stream << column(task->getName(), 16) << (void*) task << endl;
                parent->getTerminal()->send(PrintEvent(task->getName(), task->getID()));
            }

            return NULL;
        }

    };

}

#endif
