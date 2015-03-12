//
// Nexus_Task.h
//

#ifndef __Nexus_Task__
#define __Nexus_Task__

#include <Arduino.h>
#include <limits.h>

#define MERGE_LINE(line) label_##line
#define LABEL_LINE(line) MERGE_LINE(line)

#define coro_enter   if (_context) goto *_context;
#define coro_yield() _context = &&LABEL_LINE(__LINE__); return; LABEL_LINE(__LINE__):
#define coro_exit    _context = NULL; return;

#define task_enter        coro_enter
#define task_sleep(msecs) _timeout = millis() + msecs; _sleep = true; coro_yield()
#define task_yield()      _timeout = 0; _sleep = false; coro_yield()
#define task_wait()       _timeout = LONG_MAX; _sleep = false; coro_yield()
#define task_wait4(msecs)       _timeout = millis() + msecs; _sleep = false; coro_yield()
#define task_exit         coro_exit

namespace Nexus {

    /**

    Used by the scheduler to provide cooperative multi-threading

    */

    class Coro {

      public:

        friend class Scheduler;

        typedef void (*TickFunc)(Coro& coro, const Message& message);

        Coro(TickFunc run) : _run(run), _context(NULL), _next(NULL) { }

        Coro* getNext() { return _next; }

      protected:

        TickFunc _run;

        void *_context;
        Coro *_next;

    };

    /**

    Helper class to avoid virtual function vtable memory overhead

    */

    template<typename T>
    struct TaskHelper {

        static void run(Coro& task, const Message& message)
        {
            static_cast<T *>(&task)->run(message);
        }

    };

    /**

    A named coro that supports sleeping for a set amount of time

    */

    class Task : public Coro {

      public:

        friend class Scheduler;

        Task(TickFunc run, symbol name) : Coro(run),
          _name(name), _timeout(0), _sleep(false)
        { }

        void send(const Message& message);

        Task* getNext() { return static_cast<Task*>(Coro::getNext()); }

      protected:

        const symbol _name;
        uint32_t     _timeout : 31;
        bool         _sleep : 1;

    };

}

#endif