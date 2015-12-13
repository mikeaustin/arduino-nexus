//
// Symbol
//

#ifndef __Nexus_Symbol__
#define __Nexus_Symbol__

namespace Nexus {
    
    /**

    Make Flash strings easier to work with and promote their use

    */

    class symbol {

      public:

        friend Stream& operator <<(Stream& stream, symbol symbol);

        symbol(const __FlashStringHelper* string = NULL) :
          _string(string)
        { }

        operator const __FlashStringHelper*() const { return _string; }

        uint8_t size() const { return strlen_P(reinterpret_cast<const char*>(_string)); }

        bool operator ==(const char* string) const
        {
            return strcmp_P(string, reinterpret_cast<const char*>(_string)) == 0;
        }

      private:

        const __FlashStringHelper* _string;

    };

    inline Stream& operator <<(Stream& stream, symbol symbol)
    {
        return stream.print(symbol._string), stream;
    }

}

#endif
