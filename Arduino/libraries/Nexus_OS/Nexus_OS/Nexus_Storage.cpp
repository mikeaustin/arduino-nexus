//
// Nexus_Storage.cpp
//

namespace Nexus {

    template<typename Driver, int Size>
    Driver& Storage<Driver, Size>::getDriver()
    {
        if (!_ready)
        {
            _driver.setup();

            _ready = true;
        }

        return _driver;
    }

    template<typename Driver, int Size>
    void Storage<Driver, Size>::format()
    {
        setLastOffset(0);
    }

    template<typename Driver, int Size>
    template<typename Value>
    void Storage<Driver, Size>::put(const char *key, const Value& value)
    {
        Driver& driver = getDriver();

        const uint8_t keySize = strlen(key);
        const uint8_t valueSize = sizeof(Value);

        uint16_t offset = getLastOffset();

        offset += driver.write(offset, reinterpret_cast<const uint8_t *>(&keySize), 1);
        offset += driver.write(offset, reinterpret_cast<const uint8_t *>(key), keySize);
        offset += driver.write(offset, reinterpret_cast<const uint8_t *>(&value), valueSize);

        setLastOffset(offset);
    }

    template<typename Driver, int Size>
    template<typename Value>
    Value Storage<Driver, Size>::get(const char *key)
    {
        Driver& driver = getDriver();

        uint16_t lastOffset = getLastOffset();
        uint16_t offset = 0;

        while (offset < lastOffset)
        {
            uint8_t keySize;
            char    key_[keySize];

            offset += driver.read(offset, reinterpret_cast<uint8_t *>(&keySize), 1);
            offset += driver.read(offset, reinterpret_cast<uint8_t *>(key_), keySize);

            if (memcmp(key, key_, keySize) == 0)
            {
                Value value;

                offset += driver.read(offset, reinterpret_cast<uint8_t *>(&value), sizeof(Value));

                return value;
            }
            else offset += sizeof(Value);
        }

        return Value();
    }

}
