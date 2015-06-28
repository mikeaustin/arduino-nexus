//
// Nexus.h
//

#include <Arduino.h>

template<typename T, typename U>
const T pgm_ptr(const U& addr)
{
    return (const T)pgm_read_ptr(&addr);
}

extern int __heap_start, *__brkval;
extern int availableMemory;

namespace Nexus {

    // inline Stream& operator <<(Stream& stream, const __FlashStringHelper* string)
    // {
    //     return stream.print(string), stream;
    // }

    // inline Stream& operator <<(Stream& stream, char value)
    // {
    //     return stream.print(value), stream;
    // }

    // inline Stream& operator <<(Stream& stream, int value)
    // {
    //     return stream.print(value), stream;
    // }

    // inline Stream& operator <<(Stream& stream, unsigned long value)
    // {
    //     return stream.print(value), stream;
    // }

    template<typename DataType>
    inline Stream& operator <<(Stream& stream, const DataType value)
    {
        return stream.print(value), stream;
    }

    inline Stream& operator <<(Stream& stream, void* pointer)
    {
        return stream.print(reinterpret_cast<intptr_t>(pointer)), stream;
    }

    inline Stream& operator <<(Stream& stream, Stream& (*func)(Stream&))
    {
        return func(stream);
    }
    
    inline Stream& endl(Stream& stream)
    {
        stream.println(F(""));

        return stream;
    }

    inline int digits(int number)
    {
        int digits = 0;

        do
        {
            number /= 10; digits++;
        }
        while (number != 0);

        return digits;
    }

    template<typename T>
    struct Column {

        enum Align { Left, Right } align;

        T       value;
        uint8_t width;

    };

    template<typename T>
    inline Stream& operator <<(Stream& stream, const Column<T>& column)
    {
        switch (column.align)
        {
            break; case Column<T>::Left:
            {
                stream.print(column.value);

                for (int i = 0; i < column.width - column.value.size(); ++i)
                {
                    stream.print(' ');
                }
            }
            break; case Column<T>::Right:
            {
                for (int i = 0; i < column.width - column.value.size(); ++i)
                {
                    stream.print(' ');
                }

                stream.print(column.value);
            }
        }

        return stream;
    }

    template<>
    inline Stream& operator <<(Stream& stream, const Column<int>& column)
    {
        switch (column.align)
        {
            break; case Column<int>::Left:
            {
                stream.print(column.value);

                for (int i = 0; i < column.width - digits(column.value); ++i)
                {
                    stream.print(' ');
                }
            }
            break; case Column<int>::Right:
            {
                for (int i = 0; i < column.width - digits(column.value); ++i)
                {
                    stream.print(' ');
                }

                stream.print(column.value);
            }
        }

        return stream;
    }

    template<typename T>
    inline Column<T> column(T value, uint8_t width)
    {
        Column<T> column = { Column<T>::Left, value, width };

        return column;
    }

    template<typename T>
    inline Column<T> right(T value, uint8_t width)
    {
        Column<T> column = { Column<T>::Right, value, width };

        return column;
    }

}

#include "Nexus_OS/Nexus_Symbol.h"
#include "Nexus_OS/Nexus_Tuple.h"
#include "Nexus_OS/Nexus_Message.h"
#include "Nexus_OS/Nexus_Task.h"
#include "Nexus_OS/Nexus_Scheduler.h"
#include "Nexus_OS/Nexus_Terminal.h"
#include "Nexus_OS/Nexus_Shell.h"
#include "Nexus_OS/Nexus_Storage.h"

#include "Nexus_OS/Nexus_Commands.h"

/*

9  Bytes
19 Bytes    First 'new'

*/
