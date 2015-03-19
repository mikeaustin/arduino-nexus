//
// Message.h
//

namespace Nexus {

    /**

    Proviedes run time type information , used by messages and events

    */

    template<typename Type>
    struct TypeInfo {

        static void *GetType() { return reinterpret_cast<void *>(&GetType); }

    };

    /**

    An encapsulated option data type, passed to tasks via messages

    */
    
    class Event {

      public:

        Event(bool valid = true) : _valid(valid) { }

        operator bool() const
        {
            return _valid;
        }

        bool _valid;

    };

    /*

    Used to synchronously send data between tasks or the scheduler

    */

    class Message {

      public:

        static Event None;

        template<typename DataType>
        Message(const DataType& event) : _event(event), _type(TypeInfo<DataType>::GetType())
        { }

        template<typename DataType>
        const DataType& get() const
        {
            if (TypeInfo<DataType>::GetType() == _type)
            {
                return static_cast<const DataType&>(_event);
            }

            return static_cast<const DataType&>(None);
        }

      private:

        const Event&  _event;
        const void   *_type;

    };

}
