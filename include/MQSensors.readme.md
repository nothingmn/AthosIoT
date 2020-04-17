MQ Series of Sensors
----

- MQ-2 - Methane, Butane, LPG, Smoke    
    - [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    - [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
    - [Smoke - Supported](https://en.wikipedia.org/wiki/Smoke)    
    - [Hydrogen - Supported](https://en.wikipedia.org/wiki/Hydrogen)    
- MQ-3 - Alcohol, Ethanol, Smoke    
    - [Alcohol mG/l - Supported](https://en.wikipedia.org/wiki/Alcohol)    
    - [Alcohol ppm - Supported](https://en.wikipedia.org/wiki/Alcohol)    
- MQ-4 - Methane, CNG Gas    
    - [Compressed Natural Gas (CNG) - **NOT Supported**](https://en.wikipedia.org/wiki/CNG)    
    - [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
- MQ-5 - Natural gas, LPG    
    - [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    - [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
- MQ-6 - LPG, butane    
    - [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    - [Butane - **NOT Supported**](https://en.wikipedia.org/wiki/Butane)    
- MQ-7 - Carbon Monoxide    
    - [LPG - Supported](https://en.wikipedia.org/wiki/Carbon_Monoxide)    
- MQ-8 - Hydrogen Gas    
    - [Hydrogen - Supported](https://en.wikipedia.org/wiki/Hydrogen)    
- MQ-9 - Carbon Monoxide, flammable gasses    
    - [LPG - Supported](https://en.wikipedia.org/wiki/Liquefied_petroleum_gas)    
    - [Methane - Supported](https://en.wikipedia.org/wiki/Methane)    
    - [Carbon Monoxide - Supported](https://en.wikipedia.org/wiki/Carbon_Monoxide)    
- MQ135 - Air Quality   (CO2)    
    - [Carbon Dioxide - Supported](https://en.wikipedia.org/wiki/Carbon_Dioxide)    

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

