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

        symbol(const __FlashStringHelper* string) :
          //_string(reinterpret_cast<const char *>(string))
          _string(string)
        { }

        //operator const char *() const { return _string; }
        operator const __FlashStringHelper*() const { return _string; }

        uint8_t size() const { return strlen_P((const char*)_string); }

        bool operator ==(const char* string) const { return strcmp_P(string, (const char*)_string) == 0; }

      private:

        const __FlashStringHelper* _string;

    };

}

#endif
