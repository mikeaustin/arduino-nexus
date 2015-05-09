//
// Blink.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

struct DrawPoint {

    static DrawPoint Create(uint16_t x, uint16_t y)
    {
        DrawPoint data = { x, y };

        return data;
    }

    uint16_t x;
    uint16_t y;

};

template<>
const void* TypeInfo<DrawPoint>::GetType() { return (void*) 1; }


struct DrawString {

    static DrawString Create(const char* string)
    {
        DrawString data = { string };

        return data;
    }

    const char* string;

};

template<>
const void* TypeInfo<DrawString>::GetType() { return (void*) 2; }

template<>
struct DistObject<DrawString> {

    static Nexus::Message Archive(const DrawString& value)
    {
        Message message(TypeInfo<DrawString>::GetType(), value.string, strlen(value.string));

        return message;
    }

};


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
            // DrawPoint drawPoint = { random(1024), random(768) };

            // stream.write(10);
            // stream.write(TypeInfo<DrawPoint>::ID);
            // stream.write(sizeof (DrawPoint));
            // stream.write((char*) &drawPoint, sizeof (DrawPoint));

            windowServer.send(DrawPoint::Create(random(1024), random(768)));

            //DrawPoint data[] = { 10, 20, 30, 40 };

            // stream.write(10);
            // stream.write(TypeInfo<DrawString>::ID);
            // stream.write(strlen("Nexus OS"));
            // stream.write("Nexus OS", strlen("Nexus OS"));

            windowServer.send(DrawString::Create("Nexus OS"));
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
