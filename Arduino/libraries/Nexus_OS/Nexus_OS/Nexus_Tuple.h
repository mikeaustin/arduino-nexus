//
// Nexus_Tuple.h
//

#ifndef __Nexus_Tuple__
#define __Nexus_Tuple__

namespace Nexus {

    // template<typename T1, typename T2 = void>
    // struct tuple {

    //     tuple(T1 _1, T2 _2)
    //      : _1(_1), _2(_2)
    //     { }

    //     T1 _1;
    //     T2 _2;

    // };

    // template<typename T1>
    // struct tuple<T1, void> {

    //     tuple(T1 _1)
    //      : _1(_1)
    //     { }

    //     T1 _1;

    // };

    // template<typename T1, typename T2>
    // tuple<T1, T2> make_tuple(T1 _1, T2 _2) {

    //     return tuple<T1, T2>(_1, _2);

    // }


    template<typename T>
    class option {

      public:

        option()
         : _ptr(NULL)
        { }

        option(const T& value)
         : _value(value), _ptr(&_value)
        {
            _ptr = &_value;
        }

        option& operator =(const option& option)
        {
            _value = option._value;
            _ptr   = option._ptr;

            return *this;
        }

        operator bool() const { return _ptr != NULL; }
        const T *operator ->() const { return &_value; }
        T operator *() { return _value; }

      private:

        T  _value;
        T* _ptr;

    };

}

#endif
