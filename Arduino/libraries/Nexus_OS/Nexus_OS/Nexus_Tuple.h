//
// Nexus_Tuple.h
//

#ifndef __Nexus_Tuple__
#define __Nexus_Tuple__

namespace Nexus {

    template<typename T1, typename T2 = void>
    struct tuple : public Event {
        tuple(T1 _1, T2 _2)
          : _1(_1), _2(_2)
        { }

        T1 _1;
        T2 _2;
    };

    template<typename T1>
    struct tuple<T1, void> : public Event {
        tuple(T1 _1)
          : _1(_1)
        { }

        T1 _1;
    };

    template<typename T1, typename T2>
    tuple<T1, T2> make_tuple(T1 _1, T2 _2) {
        return tuple<T1, T2>(_1, _2);
    }

    template<typename T>
    class option {

      public:

        option()
          : _ptr(NULL)
        { }

        option(const T& value)
          : _value(value), _ptr(&_value)
        { }

        operator bool() { return _ptr != NULL; }
        const T value() { return _value; }

      private:

        T  _value;
        T *_ptr;

    };

}

#endif
