#include <ads1231.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("READY");
	
	ads1231_init();
}

void loop()
{
	long mg;
    
    mg=ads1231_get_milligrams();
    if(mg > ADS1231_ERR)
    {
		Serial.print("Timeout, code ");
		Serial.println(mg);
	}
	else
		Serial.println(mg);
}
