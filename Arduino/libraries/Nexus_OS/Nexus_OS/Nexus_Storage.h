//
// Nexus_Storage.h
//

#ifndef __Nexus_Storage__
#define __Nexus_Storage__

#ifdef _ADAFRUIT_FRAM_SPI_H_

namespace Nexus {

    #include <SPI.h>
    #include <Adafruit_FRAM_SPI.h>

    template<int CS_Pin>
    class FRAM_Driver {

      public:

        FRAM_Driver() : _fram(CS_Pin) { }

        void initialize()
        {
            if (!_fram.begin())
            {
                Serial.println(F("No SPI FRAM found."));
            }
        }

        uint8_t write(uint16_t offset, const uint8_t *value, size_t count)
        {
            _fram.writeEnable(true);
            _fram.write(offset, value, count);
            _fram.writeEnable(false);

            return count;
        }

        uint8_t read(uint16_t offset, uint8_t *value, size_t count)
        {
            _fram.read(offset, value, count);

            return count;
        }

      private:

        Adafruit_FRAM_SPI _fram;

    };

}

#endif

namespace Nexus {

    template<typename Driver, int Size>
    class Storage {

      public:

        Storage() : _ready(false) { }

        Driver& getDriver();

        void format();

        template<typename Value>
        void put(const char *key, const Value& value);

        template<typename Value>
        Value get(const char *key);

      protected:

        void setLastOffset(uint16_t lastOffset)
        {
            _driver.write(Size - 2, reinterpret_cast<uint8_t *>(&lastOffset), 2);
        }

        uint16_t getLastOffset()
        {
            uint16_t lastOffset;

            _driver.read(Size - 2, reinterpret_cast<uint8_t *>(&lastOffset), 2);

            return lastOffset;
        }

      private:

        Driver _driver;
        bool   _ready;

    };

}

#endif

#include "Nexus_Storage.cpp"
