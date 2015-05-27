
//
// Processing.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

struct DrawPoint {

    DrawPoint(uint16_t x, uint16_t y)
     : x(x), y(y)
    { }

    const uint16_t x;
    const uint16_t y;

};

template<>
const void* TypeInfo<DrawPoint>::GetType() { return (const void*) 1; }

template<>
void TypeInfo<DrawPoint>::Archive(const void* data_, Stream& stream)
{
    const DrawPoint* data = (const DrawPoint*) data_;

    stream.write(4);

    stream.write((char*) &data->x, 2);
    stream.write((char*) &data->y, 2);
}


struct DrawString {

    DrawString(uint16_t x, uint16_t y, const char* string)
     : x(x), y(y), string(string)
    { }

    const uint16_t x;
    const uint16_t y;

    const char* string;

};

template<>
const void* TypeInfo<DrawString>::GetType() { return (const void*) 2; }

template<>
void TypeInfo<DrawString>::Archive(const void* data_, Stream& stream)
{
    const DrawString* data = (const DrawString*) data_;

    stream.write(4 + strlen(data->string));

    stream.write((char*) &data->x, 2);
    stream.write((char*) &data->y, 2);

    stream.write(data->string, strlen(data->string));
}


class Proxy : public Task {

  public:

    Proxy(uint16_t port)
     : Task(TaskHelper<Proxy>::run, F("Proxy")),
       _port(port)
    { }

    void run(const Message& message)
    {
        Stream& stream = Serial;

        stream.write(_port);

        message.archive(stream);
    }

  private:

    uint8_t _port;

};


class Draw : public Task {

  public:

    Draw() : Task(TaskHelper<Draw>::run, F("Draw")) { }

    void run(const Message& message)
    {
        Proxy windowServer(10);

        Stream& stream = Serial;

        task_enter;

        for (i = 0;; i++)
        {
            windowServer.send(DrawPoint(random(1024), random(768)));

            if (i % 1000 == 0)
            {
                windowServer.send(DrawString(random(1024), random(768), "Nexus OS"));
            }

            task_yield();
        }

        task_exit;
    }

    int i;

};


void setup()
{
    Serial.begin(9600);

    Scheduler.addTask(new Draw());

    Serial.write('A');
}

void loop()
{
    Scheduler.tick(millis());
}
