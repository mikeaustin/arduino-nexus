//
// BlinkSerialCoro.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

class Blink : public Task {

  public:

    Blink() : Task(&TaskHelper<Blink>::run, F("Blink")) { }

    void run(const Message& message)
    {
        task_enter;

        pinMode(13, OUTPUT);

        for (;;)
        {
            task_wait();

            for (i = 0; i < 5; ++i)
            {
                digitalWrite(13, HIGH);

                task_sleep(500);

                digitalWrite(13, LOW);

                task_sleep(500);
            }
        }

        task_exit;
    }

  private:

    uint8_t i;

};

Blink blink;

namespace {

    void stream(Coro& coro, const Message& message)
    {
        while (Serial.available() > 0)
        {
            Serial.read();

            blink.send(Message(Event()));
        }
    }

}

Coro messenger = Coro(&stream);

void setup()
{
    Serial.begin(9600);

    Scheduler.addCoro(&messenger);
    Scheduler.addTask(&blink);
}

void loop()
{
    Scheduler.tick(millis());
}
