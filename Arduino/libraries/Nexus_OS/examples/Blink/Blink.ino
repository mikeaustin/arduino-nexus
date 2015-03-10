//
// Blink.ino
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

        for (i = 0; i < 5; ++i)
        {
            digitalWrite(13, HIGH);

            task_sleep(500);

            digitalWrite(13, LOW);
            task_sleep(500);
        }

        task_exit;
    }

  private:

    uint8_t i;

};

void setup()
{
    Serial.begin(9600);

    Scheduler.addTask(new Blink());
}

void loop()
{
    Scheduler.tick(millis());
}
