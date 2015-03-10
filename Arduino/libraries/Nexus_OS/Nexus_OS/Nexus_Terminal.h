//
// Terminal.h
//

namespace Nexus {
    
    struct StreamEvent : public Event {

        StreamEvent(Stream& stream) : stream(stream) { }

        Stream& stream;

    };

    struct KeyEvent : public Event {

        enum Key
        {
            None = 256, KeyEnter, KeyDelete, KeyEscape,
            KeyUp, KeyDown, KeyLeft, KeyRight
        };

        KeyEvent(int key = None) : key(key) { }

        int key;

    };

    class Terminal : public Task {

      public:

        Terminal(Stream& stream) : Task(&TaskHelper<Terminal>::run, F("Terminal")),
          _stream(stream)
        { }

        void run(const Message& message)
        {
            //StreamEvent streamEvent;

            task_enter;

            for (;;)
            {
                int key;

                task_wait();

                int c = _stream.read();

                if (c == 27)
                {
                    task_wait4(10);

                    if (message.get<StreamEvent>())
                    {
                        int c = _stream.read();

                        if (c == '[')
                        {
                            task_wait();

                            int c = _stream.read();

                            switch (c)
                            {
                                case 'A': key = KeyEvent::KeyUp; break;
                                case 'B': key = KeyEvent::KeyDown; break;
                                case 'C': key = KeyEvent::KeyRight; break;
                                case 'D': key = KeyEvent::KeyLeft; break;
                            }
                        }
                    }
                    else key = 27;
                }
                else key = c;

                Serial.println(key);
                Message(KeyEvent(key));
            }

            task_exit;
        }

      private:

        Stream&  _stream;
        Task    *_task;

    };

}
