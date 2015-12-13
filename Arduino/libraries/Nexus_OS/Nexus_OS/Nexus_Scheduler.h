//
// Nexus_Scheduler.h
//

#ifndef __Nexus_Scheduler__
#define __Nexus_Scheduler__

namespace Nexus {

    struct TimeoutEvent {

        TimeoutEvent() { }

    };

    struct TaskEvent {

        enum Type
        {
            ChildExited = 1
        };

        TaskEvent(Task* task, Type type)
         : task(task), type(type)
        { }

        TaskEvent() { }

        Task* task;
        Type  type;

    };

    /**

    Coordinates adding, removing and scheduling coros and tasks

    */
    
    class Scheduler {

      public:

        Scheduler() : _idle(0), _total(0), _time(0) { }

        void addCoro(Coro* coro)
        {
            coro->_next = _coros;

            _coros = coro;
        }

        void addTask(Task* task, Task* parent = NULL)
        {
            task->_parent = parent;
            task->_next = _tasks;

            _tasks = task;
        }

        // void addTask(Task* newTask)
        // {
        //     Task* task = _tasks;

        //     while (task == NULL)
        //     {
        //         task = task->getNext();
        //     }

        //     if (task)
        //         task->_next = newTask;
        //     else
        //         _tasks = newTask;
        // }

        Task* getTasks() { return _tasks; }
        unsigned getLoad() { return _load; }

        void send(Task* task, const Message& message)
        {
            if (!task->_sleep)
            {
                task->_run(task, message);
            }
        }

        void tick(uint32_t msecs)
        {
            int v; availableMemory = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);

            Message message = Message(TimeoutEvent());

            for (Coro* prev = NULL, * coro = _coros; coro != NULL; coro = coro->getNext())
            {
                coro->_run(coro, message);
            }

            int count = 0;

            for (Task* prev = NULL, * task = _tasks; task != NULL; task = task->getNext())
            {
                if (msecs >= task->_timeout)
                {
                    task->_run(task, message);
                }
                else _idle += 1;

                _total += 1;

                if (task->_context == NULL)
                {
                    if (task->getParent())
                    {
                        task->getParent()->send(TaskEvent(task, TaskEvent::ChildExited));
                    }

                    if (prev)
                        prev->_next = task->_next;
                    else
                        _tasks = (Task *)task->_next;

                    if (prev) task = prev;
                }
                else prev = task;

                count += 1;
            }

            if (millis() > _time)
            {
                _load = (100 - (_idle * 100 / _total)) * count;
                _idle = _total = 0;

                _time = millis() + 1000;
            }
        }

      private:

        Coro* _coros;
        Task* _tasks;
        unsigned _load;
        unsigned long _idle;
        unsigned long _total;
        unsigned long _time;

    };

    extern class Scheduler Scheduler;

}

#endif
