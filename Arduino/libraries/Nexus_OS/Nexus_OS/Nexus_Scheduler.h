//
// Nexus_Scheduler.h
//

#ifndef __Nexus_Scheduler__
#define __Nexus_Scheduler__

namespace Nexus {

    /**

    Coordinates adding, removing and scheduling coros and tasks

    */

    struct TimeoutEvent {

        static TimeoutEvent Create()
        {
            TimeoutEvent event = { };

            return event;
        }

    };
    
    class Scheduler {

      public:

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

        void send(Task* task, const Message& message)
        {
            if (!task->_sleep)
            {
                task->_run(task, message);
            }
        }

        void tick(uint32_t msecs)
        {
            Message message = TypeInfo<TimeoutEvent>::Archive(TimeoutEvent::Create());

            for (Coro* prev = NULL, * coro = _coros; coro != NULL; coro = coro->getNext())
            {
                coro->_run(coro, message);
            }

            for (Task* prev = NULL, * task = _tasks; task != NULL; task = task->getNext())
            {
                if (msecs >= task->_timeout)
                {
                    task->_run(task, message);
                }

                if (task->_context == NULL)
                {
                    if (prev)
                        prev->_next = task->_next;
                    else
                        _tasks = (Task *)task->_next;

                    if (prev) task = prev;
                }
                else prev = task;
            }
        }

      private:

        Coro* _coros;
        Task* _tasks;

    };

    extern class Scheduler Scheduler;

}

#endif
