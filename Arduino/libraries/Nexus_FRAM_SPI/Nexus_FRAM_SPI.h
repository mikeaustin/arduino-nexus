//
// Nexus_FRAM_SPI.h
//

#include <Arduino.h>
#include <SPI.h>

namespace Nexus {

 //
 // class SPI_Base
 //

 // Bit-banging implementation

    template<uint8_t CS_Pin, uint8_t CLK_Pin, uint8_t MISO_Pin, uint8_t MOSI_Pin>
    class SPI_Base {
        
      public:

        void initialize()
        {
            pinMode(CLK_Pin, OUTPUT);
            pinMode(MOSI_Pin, OUTPUT);
            pinMode(MISO_Pin, INPUT);
        }

        uint8_t transfer(uint8_t data)
        {
            uint8_t reply = 0;

            for (int i = 7; i >= 0; --i)
            {
                reply <<= 1;

                digitalWrite(CLK_Pin, LOW);
                digitalWrite(MOSI_Pin, data & (1 << i));
                digitalWrite(CLK_Pin, HIGH);

                if (digitalRead(MISO_Pin)) 
                    reply |= 1;
            }

            return reply;
        }

    };

 // Hardware SPI specialization

    template<uint8_t CS_Pin>
    class SPI_Base<CS_Pin, -1, -1, -1> {

      public:
      
        void initialize()
        {
            SPI.begin();

#ifdef __SAM3X8E__
            SPI.setClockDivider (9); // 9.3 MHz
#else
            SPI.setClockDivider (SPI_CLOCK_DIV2); // 8 MHz
#endif

            SPI.setDataMode(SPI_MODE0);
        }
        
        uint8_t transfer(uint8_t byte)
        {
            return SPI.transfer(byte);
        };

    };

 //
 // class FRAM_SPI
 //

    template<uint8_t CS_Pin, uint8_t CLK_Pin = -1, uint8_t MISO_Pin = -1, uint8_t MOSI_Pin = -1>
    class FRAM_SPI : public SPI_Base<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin> {

      public:

        enum Opcodes
        {
            Opcode_WREN   = 0b0110,     /* Write Enable Latch */
            Opcode_WRDI   = 0b0100,     /* Reset Write Enable Latch */
            Opcode_RDSR   = 0b0101,     /* Read Status Register */
            Opcode_WRSR   = 0b0001,     /* Write Status Register */
            Opcode_READ   = 0b0011,     /* Read Memory */
            Opcode_WRITE  = 0b0010,     /* Write Memory */
            Opcode_RDID   = 0b10011111  /* Read Device ID */
        };

        FRAM_SPI() : SPI_Base<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin>(),
          _initialized(false)
        { }

     // Initialize pins and SPI

        void begin();

     // Read and write to memory

        void    write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr);

     // Get the device identifier

        void    getDeviceID(uint8_t *mfgID, uint16_t *prodID);

     // Get and set the status register

        uint8_t getStatusRegister();
        void    setStatusRegister(uint8_t reg);

      private:

        bool _initialized;

    };

}

#include "Nexus_FRAM_SPI.hpp"
