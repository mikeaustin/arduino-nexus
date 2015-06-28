//
// TerminalShell.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

class Blink : public Task {

  public:

    Blink() : Task(TaskHelper<Blink>::run, F("Blink")) { }

    void run(const Message& message)
    {
        task_enter;

        pinMode(13, OUTPUT);

        for (i = 0; i < 5; i++)
        {
            digitalWrite(13, HIGH);
            task_sleep(500);

            digitalWrite(13, LOW);
            task_sleep(500);
        }

        task_exit;
    }

    int i;

};

namespace {

    const char blink_name[] PROGMEM = "blink";

    Task* blink(Task* parent)
    {
        Blink *blink = new Blink();

        Scheduler.addTask(blink, parent);

        return blink;
    }

}

const Command commands[] PROGMEM =
{
    Commands::help_name, Commands::help,
    Commands::info_name, Commands::info,
    Commands::tasks_name, Commands::tasks,
    blink_name, blink,
    NULL, NULL
};

Terminal console(Serial);
Shell shell(commands);

namespace {

    void stream(Coro* coro, const Message& message)
    {
        if (Serial.available() > 0)
        {
            console.send(StreamEvent(Serial));
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
