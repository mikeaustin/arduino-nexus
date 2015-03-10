//
// Message.h
//

namespace Nexus {

    template<typename Type>
    struct TypeInfo {

        static void *GetType() { return reinterpret_cast<void *>(&GetType); }

    };
    
    class Event {

      public:

        Event(bool valid = true) : _valid(valid) { }

        operator bool() const
        {
            return _valid;
        }

        bool _valid;

    };

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
