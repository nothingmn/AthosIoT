MQ Series of Sensors
----

MQ-2 - Methane, Butane, LPG, Smoke    **- Fully Supported**    
MQ-3 - Alcohol, Ethanol, Smoke    **- Alcohol supported, NOT Ethanol nor Smoke**    
MQ-4 - Methane, CNG Gas    **- CNG Not supported, Methane is supported**    
MQ-5 - Natural gas, LPG    **- Fully Supported**    
MQ-6 - LPG, butane    **- LPG Supported, NOT Butane**    
MQ-7 - Carbon Monoxide    **- Fully Supported**    
MQ-8 - Hydrogen Gas    **- Fully Supported**    
MQ-9 - Carbon Monoxide, flammable gasses    **- Fully Supported**    
MQ135 - Air Quality   (CO2) **- Fully Supported**    

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

