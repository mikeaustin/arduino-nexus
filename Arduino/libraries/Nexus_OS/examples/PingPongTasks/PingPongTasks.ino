//
// PingPongTasks.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

struct SqrData : public Event {

    SqrData(Task *sender = NULL, int value = 0) : sender(sender), value(value) { }
    Task *sender;
    int   value;

};

class Pong : public Task {

  public:

    Pong() : Task(&TaskHelper<Pong>::run, F("Pong")) { }

    void run(const Message& message)
    {
        task_enter;

        pinMode(13, OUTPUT);

        for (;;)
        {
            task_wait();

            if (data = message.get<SqrData>())
            {
                task_send(data.sender, SqrData(this, data.value * data.value));
            }
        }

        task_exit;
    }

  private:

    SqrData data;

};

class Ping : public Task, TaskHelper<Ping> {

  public:

    Ping(Task *pong) : Task(&TaskHelper<Ping>::run, F("Ping")),
      pong(pong)
    { }

    void run(const Message& message)
    {
        task_enter;

        for (i = 1; i <= 5; i++)
        {
            task_send(pong, Message(SqrData(this, i)));

            task_wait();

            if (const SqrData& data = message.get<SqrData>())
            {
                Serial.println(data.value);
            }
        }

        task_exit;
    }

  private:

    Task    *pong;
    SqrData  data;
    int      i;

};

Pong pong;
Ping ping(&pong);

void setup()
{
    Serial.begin(9600);

    Scheduler.addTask(&ping);
    Scheduler.addTask(&pong);
}

void loop()
{
    Scheduler.tick(millis());
}
