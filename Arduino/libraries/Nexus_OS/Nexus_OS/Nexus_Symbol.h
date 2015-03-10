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

        symbol(const __FlashStringHelper *string) :
          _string(reinterpret_cast<const char *>(string))
        { }

      private:

        const char *_string;

    };

}

#endif
