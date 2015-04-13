//
// StorageFRAM.ino
//

#include <SPI.h>
#include <Adafruit_FRAM_SPI.h>

#include <Nexus_OS.h>

using namespace Nexus;

Storage<FRAM_Driver<10>, 0x2000> fram;

void setup()
{
    Serial.begin(9600);

    Serial.println(F("Formatting FRAM..."));

    fram.format();

    Serial.println(F("Done."));

    fram.put("foo", 10L);
    fram.put("bar", 20L);

    Serial.println(fram.get<long>("foo"));
    Serial.println(fram.get<long>("bar"));
}

void loop()
{
    Scheduler.tick(millis());
}
