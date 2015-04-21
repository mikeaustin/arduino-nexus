//
// Message.h
//

namespace Nexus {

    /**

    Proviedes run time type information, used by messages and events

    */

    template<typename Type>
    struct TypeInfo {

        static const uint16_t ID = 0;

        static void* GetType() { return reinterpret_cast<void*>(&GetType); }

    };

    /*

    Used to synchronously send data between tasks or the scheduler

    */

    class Message {

      public:

        template<typename DataType>
        Message(const DataType & event)
         : _data(&event), _type(TypeInfo<DataType>::GetType()), _id(TypeInfo<DataType>::ID)
        { }

        template<typename DataType>
        option<DataType> get() const
        {
            if (TypeInfo<DataType>::ID == _id || (TypeInfo<DataType>::ID == 0 && TypeInfo<DataType>::GetType() == _type))
            {
                    return option<DataType>(*static_cast<const DataType*>(_data));
            }

            return option<DataType>();
        }

      private:

        const void*    _data;
        const void*    _type;
        const uint16_t _id;

    };

}
