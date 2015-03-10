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

        }

        task_exit;
    }

  private:

    uint8_t i;

};

Terminal terminal(Serial);
//Blink blink;

namespace {

    void stream(Coro& coro, const Message& message)
    {
        if (Serial.available() > 0)
        {
            terminal.send(Message(StreamEvent(Serial)));
        }
    }

}

Coro messenger = Coro(&stream);

void setup()
{
    Serial.begin(9600);

    Serial.println("Arduino OS 0.1 | 2015 Mike Austin");

    Scheduler.addCoro(&messenger);
    //Scheduler.addTask(&blink);
    Scheduler.addTask(&terminal);
}

void loop()
{
    Scheduler.tick(millis());
}
