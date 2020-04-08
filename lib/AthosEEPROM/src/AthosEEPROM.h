//AthosEEPROM.h
#ifndef ATH_EEPROM_h
#define ATH_EEPROM_h

#include "AthosHelpers.h"
#include <PubSubClient.h>

class AthosEEPROM {
    public:
        AthosEEPROM();
        StorageValues readEEPROMData(void);
        void writeEEPROMData(StorageValues config);
        void wipeEEPROM(void);
        StorageValues EEPROM_setup(void);
        
    private:
};
#endif
