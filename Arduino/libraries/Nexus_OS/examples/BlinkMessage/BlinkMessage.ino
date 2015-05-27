//
// BlinkMessage.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

class LED : public Task {

  public:

    LED() : Task(TaskHelper<LED>::run, F("LED")) { }

    void run(const Message& message)
    {
        task_enter;

        pinMode(13, OUTPUT);

        for (;;)
        {
            task_wait();
            digitalWrite(13, HIGH);

            task_wait();
            digitalWrite(13, LOW);
        }

        task_exit;
    }

};

class Blink : public Task, TaskHelper<Blink> {

  public:

    Blink(Task* led)
     : Task(TaskHelper<Blink>::run, F("Blink")),
       led(led)
    { }

    void run(const Message& message)
    {
        task_enter;

        for (;;)
        {
            led->send(0);
            task_sleep(500);

            led->send(0);
            task_sleep(500);
        }

        task_exit;
    }

  private:

    Task* led;

};

LED   led;
Blink blink(&led);

void setup()
{
    Scheduler.addTask(&blink);
    Scheduler.addTask(&led);
}

void loop()
{
    Scheduler.tick(millis());
}
