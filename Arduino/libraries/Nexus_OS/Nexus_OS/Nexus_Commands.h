//
// Commands.h
//

#include <Arduino.h>

#ifndef __Nexus_Commands__
#define __Nexus_Commands__

namespace Nexus {

    inline Stream& operator <<(Stream& stream, symbol symbol)
    {
        stream.print(symbol._string);

        return stream;
    }

    inline Stream& operator <<(Stream& stream, const void* pointer)
    {
        return stream.print(reinterpret_cast<intptr_t>(pointer)), stream;
    }

    inline Stream& operator <<(Stream& stream, Stream& (*func)(Stream&))
    {
        return func(stream);
    }
    
    inline Stream& endl(Stream& stream)
    {
        stream.println(F(""));

        return stream;
    }

    struct Column {

        symbol  sym;
        uint8_t width;

    };

    inline Stream& operator <<(Stream& stream, const Column& column)
    {
        stream.print(column.sym);

        for (int i = 0; i < column.width - column.sym.size(); ++i)
        {
            stream.print(' ');
        }

        return stream;
    }

    inline Column column(symbol symbol, uint8_t width)
    {
        Column column = { symbol, width };

        return column;
    }

    struct Commands {

        static const char help_name[] PROGMEM;
        static const char reset_name[] PROGMEM;
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

        static Task* reset(Task* parent)
        {
            void (*reset)() = 0;

            reset();

            return NULL;
        }

        static Task* tasks(Task* parent)
        {
            Stream& stream = parent->getStream();

            for (Task* task = Scheduler.getTasks(); task != NULL; task = task->getNext())
            {
                stream << column(task->getName(), 16) << task << endl;
            }

            return NULL;
        }

    };

}

#endif
