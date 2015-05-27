//
// Message.h
//

/**

Proviedes run time type information, used by messages and events

*/

template<typename Type>
struct TypeInfo {

    static const void* GetType() { return reinterpret_cast<const void*>(&GetType); }

    static void Archive(const void* data, Stream& stream) { }

};

namespace Nexus {

    /*

    Used to synchronously send data between tasks or the scheduler

    */

    class Message {

      public:

        template<typename DataType>
        Message(const DataType& data)
         : _type(TypeInfo<DataType>::GetType()), _data((void*) &data), _archive(&TypeInfo<DataType>::Archive)
        { }

        template<typename DataType>
        option<DataType> get() const
        {
            if (TypeInfo<DataType>::GetType() == _type)
            {
                return option<DataType>(*static_cast<const DataType*>(_data));
            }

            return option<DataType>();
        }

        const void* getType() const { return _type; }
        const void* getData() const { return _data; }

        void archive(Stream& stream) const
        {
            stream.write((uint16_t) _type);

            if (_archive) _archive(_data, stream);
        }

      private:

        const void* _type;
        const void* _data;

        void (*_archive)(const void* data, Stream& stream);

    };

}
