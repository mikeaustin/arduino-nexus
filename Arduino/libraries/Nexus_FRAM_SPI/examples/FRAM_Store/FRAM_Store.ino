#include <Nexus_FRAM_SPI.h>
#include <SPI.h>

class RAM_Disk {
  public:
    void write(int16_t addr, uint8_t data) { }
    uint8_t read(int16_t addr) { }
    bool read(int16_t addr, void *data, size_t size) { }
};

template<typename API>
class Store {

  public:

    template<typename Data>
    void put(uint16_t key, Data data) { api.write(key, data); }

    template<typename Data>
    Data find(uint16_t key, Data data = Data())
    {
        return api.read(key, &data, sizeof(Data)), data;
    }
    
  private:

    API api;

};

class Contact {
};

Store<RAM_Disk> ramDisk;

void setup() {
  // put your setup code here, to run once:

    Contact contact = ramDisk.find<Contact>(100);
}

void loop() {
  // put your main code here, to run repeatedly:

}
