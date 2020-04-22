MQ Series of Sensors
----

1. MQ-2 - Methane, Butane, LPG, Smoke    
    1. [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    1. [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
    1. [Smoke - Supported](https://en.wikipedia.org/wiki/Smoke)    
    1. [Hydrogen - Supported](https://en.wikipedia.org/wiki/Hydrogen)    
    - ![BMP280](assets/dht22.jpg)
1. MQ-3 - Alcohol, Ethanol, Smoke    
    1. [Alcohol mG/l - Supported](https://en.wikipedia.org/wiki/Alcohol)    
    1. [Alcohol ppm - Supported](https://en.wikipedia.org/wiki/Alcohol)    
    - ![BMP280](assets/dht22.jpg)
1. MQ-4 - Methane, CNG Gas    
    1. [Compressed Natural Gas (CNG) - **NOT Supported**](https://en.wikipedia.org/wiki/CNG)    
    1. [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
    - ![BMP280](assets/dht22.jpg)
1. MQ-5 - Natural gas, LPG    
    1. [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    1. [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
    - ![BMP280](assets/dht22.jpg)
1. MQ-6 - LPG, butane    
    1. [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    1. [Butane - **NOT Supported**](https://en.wikipedia.org/wiki/Butane)    
    - ![BMP280](assets/dht22.jpg)
1. MQ-7 - Carbon Monoxide    
    1. [LPG - Supported](https://en.wikipedia.org/wiki/Carbon_Monoxide)    
    - ![BMP280](assets/dht22.jpg)
1. MQ-8 - Hydrogen Gas    
    1. [Hydrogen - Supported](https://en.wikipedia.org/wiki/Hydrogen)    
    - ![BMP280](assets/dht22.jpg)
1. MQ-9 - Carbon Monoxide, flammable gasses    
    1. [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    1. [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
    1. [Carbon Monoxide - Supported](https://en.wikipedia.org/wiki/Carbon_Monoxide)    
    - ![BMP280](assets/dht22.jpg)
1. MQ135 - Air Quality   (CO2)    
    1. [Carbon Dioxide - Supported](https://en.wikipedia.org/wiki/Carbon_Dioxide)    

All are typically 4 pin, with this setup:    

**MQ   -> ESP8266**    
__________    
A0    ->  A0    
D1    ->  NOT USED    
GRND  ->  GRND    
VCC   ->  VCC    


_____    

All sensors are integrated using the TroykaMQ Library, found here:    
https://github.com/amperka/TroykaMQ    

