//
// Nexus_Shell.h
//

namespace Nexus {

    typedef Task* (*CommandThunk)(Task* parent);

    struct Command {

        //Command(const char * name, CommandThunk command) : name(name), command(command) { }

        const char*  name;
        CommandThunk command;

    };

    class Buffer {

      public:

        Buffer() : _index(0) { }

        bool push(char c)
        {
            if (_index < sizeof(_buffer) - 1)
            {
                _buffer[_index++] = c;

                return true;
            }

            return false;
        }

        bool pop()
        {
            if (_index > 0)
            {
                _index--;

                return true;
            }

            return false;
        }

        const char *value()
        {
            _buffer[_index] = '\0';

            return _buffer;
        }

        size_t size()
        {
            return _index;
        }

        void reset()
        {
            _index = 0;
        }

      private:

        char    _buffer[30 + 1];
        uint8_t _index;

    };

    class Shell : public Task {

      public:

        Shell(const Command* commands) : Task(TaskHelper<Shell>::run, F("Shell")),
          _commands(commands)
        { }

        const Command* getCommands() { return _commands; }

        void run(const Message& message)
        {
            option<KeyEvent> keyEvent;

            Task* task;

            task_enter;

            getStream().print(F("] "));

            for (;;)
            {
                task_wait();

                if (keyEvent = message.get<KeyEvent>())
                {
                    switch (keyEvent->key)
                    {
                        break; case KeyEvent::KeyEnter:
                        {
                            getStream().println(F(""));

                            if (_buffer.size() > 0)
                            {
                                task = executeCommand(_buffer.value());
                            }

                            getStream().print(F("] "));
                        }
                        break; case KeyEvent::KeyDelete:
                        {
                            deleteCharacter();
                        }
                        break; default:
                        {
                            if (keyEvent->key >= 32 && keyEvent->key < 127)
                            {
                                insertCharacter(keyEvent->key);
                            }
                        }
                    }
                }
            }

            task_exit;
        }

        void insertCharacter(char c)
        {
            if (_buffer.push(c))
            {
                getStream().print(c);
            }
        }

        void deleteCharacter()
        {
            if (_buffer.pop())
            {
                getStream().print(F("\b \b"));
            }
        }

        CommandThunk findCommand(const char* name)
        {
            for (const Command* command = _commands; pgm_ptr<void*>(command->name) != NULL; ++command)
            {
                //const char *commandName = (const char *)pgm_read_word(&command->name);
                const char* commandName = pgm_ptr<const char*>(command->name);
                const symbol commandName2 = pgm_ptr<const __FlashStringHelper*>(command->name);
                const CommandThunk thunk = (CommandThunk)pgm_read_word(&command->command);

                if (commandName2 == name) return thunk;
                //if (strcmp_P(name, commandName) == 0) return thunk;
                //if (strcmp_P(name, command->name) == 0) return command->command;
                //if (command->name == name) return command->command;
            }

            return NULL;
        }

        Task *executeCommand(const char* name)
        {
            Task* task;

            if (CommandThunk thunk = findCommand(name))
            {
                task = thunk(this);
            }
            else getStream().println(F("Command not found"));

            _buffer.reset();

            return NULL;
        }

      private:

        const Command* _commands;
        Buffer   _buffer;

    };

}
