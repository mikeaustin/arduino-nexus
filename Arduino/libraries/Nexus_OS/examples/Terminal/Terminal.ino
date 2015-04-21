//
// Terminal.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

class Client : public Task {

  public:

    Client() : Task(TaskHelper<Client>::run, F("Client")) { }

    void run(const Message& message)
    {
        option<KeyEvent> keyEvent;

        task_enter;

        pinMode(13, OUTPUT);

        for (;;)
        {
            task_wait();

            if (keyEvent = message.get<KeyEvent>())
            {
                switch (keyEvent->key)
                {
                    case KeyEvent::KeyEscape: getStream().print(F("<ESC>")); break;
                    case KeyEvent::KeyUp:     getStream().print(F("<UP>")); break;
                    case KeyEvent::KeyDown:   getStream().print(F("<DOWN>")); break;
                    case KeyEvent::KeyLeft:   getStream().print(F("<LEFT>")); break;
                    case KeyEvent::KeyRight:  getStream().print(F("<RIGHT>")); break;
                    default:                  getStream().print((char)keyEvent->key);
                }
            }
        }

        task_exit;
    }

};

Terminal console(Serial);

namespace {

    void stream(Coro* coro, const Message& message)
    {
        if (Serial.available() > 0)
        {
            console.send(StreamEvent::Create(Serial));
        }
    }

}

Coro messenger = Coro(&stream);
Client client = Client();

void setup()
{
    Serial.begin(9600);

    Serial.println(F("Type stuff. And try the arrow keys and escape key."));

    Scheduler.addCoro(&messenger);

    Scheduler.addTask(&console, NULL);
    Scheduler.addTask(&client, &console);

    console.setForegroundTask(&client);
}

void loop()
{
    Scheduler.tick(millis());
}
