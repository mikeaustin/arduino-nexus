//
// Message.h
//

/**

Proviedes run time type information, used by messages and events

*/

template<typename Type>
struct TypeInfo {

    static const void* GetType() { return reinterpret_cast<const void*>(&GetType); }

};

namespace Nexus {

    /*

    Used to synchronously send data between tasks or the scheduler

    */

    class Message {

      public:

        //template<typename DataType>
        explicit Message(const void* type, const void* data, size_t size)
         : _data(data), _type(type), _size(size)
        { }

        // template<typename DataType>
        // explicit Message(const DataType& event)
        //  : _data(&event), _type(TypeInfo<DataType>::GetType()), _size(sizeof(DataType))
        // {
        //     // Require DataType be a POD type

        //     DataType data = { };
        // }

        template<typename DataType>
        option<DataType> get() const
        {
            if (TypeInfo<DataType>::GetType() == _type)
            {
                    return option<DataType>(*static_cast<const DataType*>(_data));
            }

            return option<DataType>();
        }

        const void* getData() const { return _data; }
        const void* getType() const { return _type; }
        uint8_t     getSize() const { return _size; }

      private:

        const void*   _data;
        const void*   _type;
        const uint8_t _size;

    };

}

template<typename Type>
struct DistObject {

    static Nexus::Message Archive(const Type& value)
    {
        Nexus::Message message(TypeInfo<Type>::GetType(), &value, sizeof(Type));

        return message;
    }

};
