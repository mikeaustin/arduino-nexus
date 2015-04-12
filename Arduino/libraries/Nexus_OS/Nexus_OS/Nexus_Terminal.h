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
            KeyEnter = 10, KeyDelete = 8, KeyEscape = 27,
            KeyUp = 30, KeyDown = 31, KeyLeft = 28, KeyRight = 29
        };

        KeyEvent(int key = 0) : key(key) { }

        int key;

    };

    class Terminal : public Task {

      public:

        Terminal(Stream& stream) : Task(&TaskHelper<Terminal>::run, F("Terminal")),
          _stream(stream), _task(NULL)
        { }

        void setForegroundTask(Task *foregroundTask)
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

                if (c == 13) continue;

                if (c == 10)
                {
                    key = KeyEvent::KeyEnter;
                }
                if (c == 27)
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

                if (_task) Scheduler.send(_task, Message(KeyEvent(key)));
            }

            task_exit;
        }

      private:

        Stream&  _stream;
        Task    *_task;

    };

}
