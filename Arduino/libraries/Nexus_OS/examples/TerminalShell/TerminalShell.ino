//
// TerminalShell.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

namespace {

    const char ledOn_name[] PROGMEM = "led on";
    const char ledOff_name[] PROGMEM = "led off";

    Task *ledOn(Task *parent)
    {
        digitalWrite(13, HIGH);
        parent->getStream().println(F("LED enabled."));

        return NULL;
    }

    Task *ledOff(Task *parent)
    {
        digitalWrite(13, LOW);
        parent->getStream().println(F("LED disabled."));

        return NULL;
    }

}

const Command commands[] PROGMEM =
{
    Commands::help_name, Commands::help,
    Commands::reset_name, Commands::reset,
    Commands::tasks_name, Commands::tasks,
    ledOn_name, ledOn,
    ledOff_name, ledOff,
    NULL, NULL
};

Terminal console(Serial);
Shell shell(commands);

namespace {

    void stream(Coro *coro, const Message& message)
    {
        if (Serial.available() > 0)
        {
            Scheduler.send(&console, Message(StreamEvent(Serial)));
        }
    }

}

Coro messenger = Coro(&stream);

void setup()
{
    pinMode(13, OUTPUT);

    Serial.begin(9600);

    Serial.println(F("Nexus OS 0.1 | 2015 Mike Austin"));
    Serial.println(F("Type 'help' for a list of commands"));

    Scheduler.addCoro(&messenger);

    Scheduler.addTask(&console);
    Scheduler.addTask(&shell, &console);

    console.setForegroundTask(&shell);
}

void loop()
{
    Scheduler.tick(millis());
}
