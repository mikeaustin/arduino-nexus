//
// Blink.ino
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
const void* TypeInfo<DrawPoint>::GetType() { return (void*) 1; }


struct DrawString {

    DrawString(const char* string)
     : string(string)
    { }

    const char* string;

};

template<>
const void* TypeInfo<DrawString>::GetType() { return (void*) 2; }

template<>
const Bundle TypeInfo<DrawString>::Archive(const DrawString& data)
{
    return Bundle(GetType(), data.string, strlen(data.string));
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
        stream.write((uint16_t) message.getType());
        stream.write(message.getSize());
        stream.write((char*) message.getData(), message.getSize());
    }

  private:

    uint16_t _port;

};


class Draw : public Task {

  public:

    Draw() : Task(TaskHelper<Draw>::run, F("Draw")) { }

    void run(const Message& message)
    {
        Proxy windowServer(10);

        Stream& stream = Serial;

        task_enter;
        
        for (;;)
        {
            windowServer.send(DrawPoint(random(1024), random(768)));
            windowServer.send(DrawString("Nexus OS"));
        }

        task_exit;
    }

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
