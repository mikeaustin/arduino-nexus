//
// Blink.ino
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

        for (;;)
        {
            digitalWrite(13, HIGH);
            task_sleep(500);

            digitalWrite(13, LOW);
            task_sleep(500);
        }

        task_exit;
    }

};

Blink blink;

void setup()
{
    Scheduler.addTask(&blink);
}

void loop()
{
    Scheduler.tick(millis());
}
