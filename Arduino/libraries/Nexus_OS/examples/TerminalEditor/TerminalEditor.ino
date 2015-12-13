//
// Terminal.ino
//

#include <Nexus_OS.h>

using namespace Nexus;

class Editor : public Task {

  public:

    Editor() : Task(TaskHelper<Editor>::run, F("Editor")),
      _row(0), _col(0)
    { }

    void run(const Message& message)
    {
        option<KeyEvent> keyEvent;

        task_enter;

        pinMode(13, OUTPUT);

        displayBuffer();

        for (;;)
        {
            displayInfo();

            task_wait();

            if (keyEvent = message.get<KeyEvent>())
            {
                switch (keyEvent->key)
                {
                    break; case KeyEvent::KeyEnter:  insertNewline();
                    break; case KeyEvent::KeyDelete: deleteCharacter();
                    break; case KeyEvent::KeyEscape: displayBuffer();
                    break; case KeyEvent::KeyUp:     
                    break; case KeyEvent::KeyDown:   
                    break; case KeyEvent::KeyLeft:   
                    break; case KeyEvent::KeyRight:  
                    break; default:                  insertCharacter(keyEvent->key);
                }
            }
        }

        task_exit;
    }

    void insertCharacter(char c)
    {
        if (c < 32) return;

        if (_buffer.push(c))
        {
            getStream() << c;

            _col++;
        }
    }

    int rowSize(int i)
    {
        int size = 0;

        while (i > 0 && _buffer[--i] != '\n')
        {
            ++size;
        }

        return size;
    }

    void deleteCharacter()
    {
        if (_buffer.pop())
        {
            if (_col > 0)
            {
                getStream() << F("\b \b");
                --_col;
            }
            else
            {
                int size = rowSize(_buffer.size());

                getStream() << F("\e[A");
                if (size > 0)
                {
                    getStream() << F("\e[");
                    getStream() << size;
                    getStream() << F("C");
                }

                --_row;
                _col = size;
            }

        }
    }

    void insertNewline()
    {
        if (_buffer.push('\n'))
        {
            getStream() << F("\r\n");

            _row++;
            _col = 0;
        }
    }

    void displayBuffer()
    {
        getStream() << F("\e[H\e[2J");

        getStream() << F("[Simple ANSI Editor]       No cursor key support yet        Press ESC to refresh") << endl;
        getStream() << F("--------------------------------------------------------------------------------");

        getStream() << F("\e[23;0H");
        getStream() << F("--------------------------------------------------------------------------------");

        getStream() << F("\e[3;0H");

        for (int i = 0; i < _buffer.size(); i++)
        {
            if (_buffer[i] == '\n')
                getStream() << F("\r\n");
            else
                getStream() << _buffer[i];
        }
    }

    int digits(int number)
    {
        int digits = 0;

        do
        {
            number /= 10; digits++;
        }
        while (number != 0);

        return digits;
    }

    void displayInfo()
    {
        getStream() << F("\e[s\e[?25l");

        getStream() << F("\e[24;0H") << F("\e[K") << _row + 1 << ' ' << _col + 1;
        getStream() << F("\e[24;77H");
        for (int i = 0; i < 4 - digits(_buffer.size()); ++i)
            getStream() << ' ';
        getStream() << _buffer.size();

        getStream() << F("\e[u\e[?25h");
    }

  private:

    Buffer<1000> _buffer;
    uint8_t      _row, _col;

};

Terminal console(Serial);
Editor editor = Editor();

namespace {

    void stream(Coro* coro, const Message& message)
    {
        if (Serial.available() > 0)
        {
            console.send(StreamEvent(Serial));
        }
    }

}

Coro messenger = Coro(&stream);

void setup()
{
    Serial.begin(9600);

    Scheduler.addCoro(&messenger);

    Scheduler.addTask(&console, NULL);
    Scheduler.addTask(&editor, &console);

    console.setForegroundTask(&editor);
}

void loop()
{
    Scheduler.tick(millis());
}
