//
// Nexus_FRAM_SPI.cpp
//

namespace Nexus {

 //
 // class FRAM_SPI
 //

 // Initialize pins and SPI

    template<uint8_t CS_Pin, uint8_t CLK_Pin, uint8_t MISO_Pin, uint8_t MOSI_Pin>
    void FRAM_SPI<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin>::begin()
    {
        pinMode(CS_Pin, OUTPUT);
        digitalWrite(CS_Pin, HIGH);

        this->initialize();

        uint8_t mfgID; uint16_t prodID;
        getDeviceID(&mfgID, &prodID);
        
        if (mfgID != 0x04 || prodID != 0x0302)
        {
            return false;
        }
        
        return _initialized = true;
    }

 // Read and write to memory

    template<uint8_t CS_Pin, uint8_t CLK_Pin, uint8_t MISO_Pin, uint8_t MOSI_Pin>
    void FRAM_SPI<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin>::write(uint16_t addr, uint8_t data)
    {
        digitalWrite(CS_Pin, LOW);
  
        this->transfer(Opcode_WRITE);
        this->transfer(highByte(addr)); // (uint8_t)(addr >> 8 & 0xFF)
        this->transfer(lowByte(addr));  // (uint8_t)(addr >> 0 & 0xFF)
        this->transfer(data);

        digitalWrite(CS_Pin, HIGH);
    }

    template<uint8_t CS_Pin, uint8_t CLK_Pin, uint8_t MISO_Pin, uint8_t MOSI_Pin>
    uint8_t FRAM_SPI<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin>::read(uint16_t addr)
    {
        digitalWrite(CS_Pin, LOW);

        this->transfer(Opcode_READ);
        this->transfer(highByte(addr));
        this->transfer(lowByte(addr));
        uint8_t data = this->transfer(0);

        digitalWrite(CS_Pin, HIGH);

        return data;
    }

 // Get the device identifier

    template<uint8_t CS_Pin, uint8_t CLK_Pin, uint8_t MISO_Pin, uint8_t MOSI_Pin>
    void FRAM_SPI<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin>::getDeviceID(uint8_t *mfgID, uint16_t *prodID)
    {
        uint8_t array[4], results;
        
        digitalWrite(CS_Pin, LOW);
        
        this->transfer(Opcode_RDID);
        array[0] = this->transfer(0);
        array[1] = this->transfer(0);
        array[2] = this->transfer(0);
        array[3] = this->transfer(0);
        
        digitalWrite(CS_Pin, HIGH);

        *mfgID  = (array[0]);
        *prodID = (array[2] << 8) + array[3];
    }

 // Get and set the status register

    template<uint8_t CS_Pin, uint8_t CLK_Pin, uint8_t MISO_Pin, uint8_t MOSI_Pin>
    uint8_t FRAM_SPI<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin>::getStatusRegister()
    {
        uint8_t reg = 0;

        digitalWrite(CS_Pin, LOW);

        this->transfer(Opcode_RDSR);
        reg = this->transfer(0);

        digitalWrite(CS_Pin, HIGH);

        return reg;
    }

    template<uint8_t CS_Pin, uint8_t CLK_Pin, uint8_t MISO_Pin, uint8_t MOSI_Pin>
    void FRAM_SPI<CS_Pin, CLK_Pin, MISO_Pin, MOSI_Pin>::setStatusRegister(uint8_t reg)
    {
        digitalWrite(CS_Pin, LOW);

        this->transfer(Opcode_WRSR);
        this->transfer(reg);

        digitalWrite(CS_Pin, HIGH);
    }

}
