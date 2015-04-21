//
// Terminal.h
//

namespace Nexus {
    
    struct StreamEvent {

        static StreamEvent Create(Stream& stream)
        {
            StreamEvent event = { &stream }; return event;
        }

        Stream* stream;

    };

    template<>
    struct TypeInfo<StreamEvent> : public TypeInfo<void> {

        static const uint16_t ID = 1;

    };

    struct KeyEvent {

        enum Key
        {
            KeyEnter = 10, KeyDelete = 8, KeyEscape = 27,
            KeyUp = 30, KeyDown = 31, KeyLeft = 28, KeyRight = 29
        };

        static KeyEvent Create(int key = 0)
        {
            KeyEvent event = { key }; return event;
        }

        int key;

    };

    template<>
    struct TypeInfo<KeyEvent> : public TypeInfo<void> {

        static const uint16_t ID = 2;

    };

    class Terminal : public Task {

      public:

        Terminal(Stream& stream) : Task(TaskHelper<Terminal>::run, F("Terminal")),
          _stream(stream), _task(NULL)
        { }

        void setForegroundTask(Task* foregroundTask)
        {
            _task = foregroundTask;
        }

        Stream& getStream() { return _stream; }

        void run(const Message& message)
        {
            int c, key;

            task_enter;

            for (;;)
            {
                task_wait();

                c = _stream.read();

                if (c == 10) continue;

                if (c == 13)
                {
                    key = KeyEvent::KeyEnter;
                }
                else if (c == 8 || c == 127)
                {
                    key = KeyEvent::KeyDelete;
                }
                else if (c == 27)
                {
                    task_wait4(10);

                    if (message.get<StreamEvent>())
                    {
                        c = _stream.read();

                        if (c == '[')
                        {
                            task_wait();

                            c = _stream.read();

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

                if (_task) _task->send(Message(KeyEvent::Create(key)));
            }

            task_exit;
        }

      private:

        Stream& _stream;
        Task*   _task;

    };

}
