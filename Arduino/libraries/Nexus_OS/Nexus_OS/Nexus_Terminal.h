//
// Terminal.h
//

namespace Nexus {
    
    struct StreamEvent {

        StreamEvent(Stream& stream)
         : stream(stream)
        { }

        StreamEvent() : stream(*reinterpret_cast<Stream*>(0)) { }

        Stream& stream;

    };

    struct KeyEvent {

        enum Key
        {
            KeyEnter = 10, KeyDelete = 8, KeyEscape = 27,
            KeyUp = 30, KeyDown = 31, KeyLeft = 28, KeyRight = 29
        };

        KeyEvent(int key)
         : key(key)
        { }

        KeyEvent() { }

        char key;

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

                if (message.get<StreamEvent>())
                {
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
                                    break; case 'A': key = KeyEvent::KeyUp;
                                    break; case 'B': key = KeyEvent::KeyDown;
                                    break; case 'C': key = KeyEvent::KeyRight;
                                    break; case 'D': key = KeyEvent::KeyLeft;
                                }
                            }
                        }
                        else key = 27;
                    }
                    else key = c;

                    if (_task) _task->send(KeyEvent(key));
                }
            }

            task_exit;
        }

      private:

        Stream& _stream;
        Task*   _task;

    };

}

    // struct Messenger : public Coro {

    //     Messenger(Task* task, Stream& stream) : Coro(&run),
    //       task(task), stream(stream)
    //     { }

    //     Task*   task;
    //     Stream& stream;

    //     static void run(Coro* coro_, const Message& message)
    //     {
    //         Messenger* coro = (Messenger*) coro_;

    //         if (coro->stream.available() > 0)
    //         {
    //             coro->task->send(StreamEvent(coro->stream));
    //         }
    //     }

    // };
