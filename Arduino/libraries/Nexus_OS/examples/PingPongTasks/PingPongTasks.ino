//
// PingPongTasks.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

struct SquareData {

    static SquareData Create(Task* sender, int value)
    {
        SquareData data = { sender, value };  return data;
    }

    Task* sender;
    int   value;

};

class Pong : public Task {

  public:

    Pong() : Task(TaskHelper<Pong>::run, F("Pong")) { }

    void run(const Message& message)
    {
        task_enter;

        pinMode(13, OUTPUT);

        for (;;)
        {
            task_wait();

            if (data = message.get<SquareData>())
            {
                task_yield(); // Allow sender to wait()

                data->sender->send(SquareData::Create(this, data->value * data->value));
            }
        }

        task_exit;
    }

  private:

    option<SquareData> data;

};

class Ping : public Task, TaskHelper<Ping> {

  public:

    Ping(Task* pong) : Task(TaskHelper<Ping>::run, F("Ping")),
      pong(pong)
    { }

    void run(const Message& message)
    {
        option<SquareData> data;

        task_enter;

        for (i = 1; i <= 5; i++)
        {
            task_yield(); // Allow sender to wait()

            pong->send(SquareData::Create(this, i));

            task_wait();

            if (data = message.get<SquareData>())
            {
                Serial.println(data->value);
            }
        }

        task_exit;
    }

  private:

    Task* pong;
    int   i;

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
