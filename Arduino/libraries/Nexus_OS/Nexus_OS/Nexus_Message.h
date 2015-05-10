//
// Message.h
//

/**

Proviedes run time type information, used by messages and events

*/

struct Bundle {

    Bundle(const void* type, const void* data, const uint8_t size)
     : data(data), type(type), size(size)
    { }

    const void*   data;
    const void*   type;
    const uint8_t size;

};

template<typename Type>
struct TypeInfo {

    static const void* GetType() { return reinterpret_cast<const void*>(&GetType); }

    static const Bundle Archive(const Type& data)
    {
        return Bundle(GetType(), &data, sizeof(Type));
    }

};

namespace Nexus {

    /*

    Used to synchronously send data between tasks or the scheduler

    */

    class Message {

      public:

        // Message(const void* type, const void* data, size_t size)
        //  : _data(data), _type(type), _size(size), _bundle(Bundle(type, data, size))
        // { }

        template<typename Type>
        Message(const Type& bundle)
         : _bundle(bundle)
        { }

        template<typename DataType>
        option<DataType> get() const
        {
            if (TypeInfo<DataType>::GetType() == _bundle.type)
            {
                return option<DataType>(*static_cast<const DataType*>(_bundle.data));
            }

            return option<DataType>();
        }

        const void* getData() const { return _bundle.data; }
        const void* getType() const { return _bundle.type; }
        uint8_t     getSize() const { return _bundle.size; }

      private:

        const Bundle _bundle;

    };

}
