
#include <SPI.h>
#include <spieeprom.h>

#define csPin 5;
#define EEPROM_MAX_READ_ADDRESS 0x010//0x13F0

//müssen für korrekte Funktion auf High gestellt werden
#define invHOLD_GPIO 16
#define invWP_GPIO 17
SPIEEPROM eeprom;


byte data[EEPROM_MAX_READ_ADDRESS];

void setup()
{
	Serial.begin(115200);
	delay(1000);

	pinMode(invHOLD_GPIO, OUTPUT);
	pinMode(invWP_GPIO, OUTPUT);
	digitalWrite(invHOLD_GPIO, HIGH);
	digitalWrite(invWP_GPIO, HIGH);

	eeprom = SPIEEPROM(1);	//SPIEEPROM von type 24Bit-Adresse (type 1) erstellen
	eeprom.setup();
	SPI.setFrequency(100000);	//SPI-Frequenz einstellen
}


void loop()
{
	// --- Start new scan on button press
	Serial.println("Drueggsch nen Knop gehts los");

	// --- Read all to make sure no old inputs are in buffer
	while (Serial.peek() != -1)
	{
		Serial.read();
	}

	// --- wait for press
	while (!Serial.available()) {
		delay(20);
	}

	if ((char)Serial.read() == 'w') {
		unsigned long startWrite = millis();

		for (long l = 0; l < EEPROM_MAX_READ_ADDRESS; l++)
		{
			data[l] = 0xAA;
		}

		eeprom.write_bytes(0x00000, data, EEPROM_MAX_READ_ADDRESS);

		Serial.print("Schreibedauer: ");
		Serial.println(millis() - startWrite);

		for (long l = 0; l < EEPROM_MAX_READ_ADDRESS; l++)
		{
			data[l] = 0x00;
		}
		return;
	}

	/*for (long l = 0; l < EEPROM_MAX_READ_ADDRESS; l++)
	{
		if (l % 16 == 0) {
			Serial.println();
			Serial.print("0x");
			Serial.print(l, HEX);
			Serial.print(":  -  ");
		}
		Serial.print(data[l] < 0x10 ? " 0" : " ");
		Serial.print(data[l], HEX);
		Serial.print(" ");
	}
	Serial.println();
	*/

	//Startzeit des Lesevorgangs festhalten
	unsigned long timeStart = millis();

	Serial.print("Startzeit: ");
	Serial.print(timeStart);

	//vom EEPROM lesen
	eeprom.read_bytes(0x00000, data, EEPROM_MAX_READ_ADDRESS);

	//für das lesen benötigte Zeit berechnen
	unsigned long timeRead = millis() - timeStart;
	
	//gelesenen Daten im Stile eines Hexdumps ausgeben
	for (int l = 0; l < EEPROM_MAX_READ_ADDRESS; l++)
	{
		//neue Zeile mit Startadresse beginnen
		if (l % 16 == 0) {
			Serial.println();
			Serial.print("0x");
			if ((int)data[l] < 0x10000)
				Serial.print("0");
			if ((int)data[l] < 0x1000)
				Serial.print("0");
			if ((int)data[l] < 0x100)
				Serial.print("0");
			if ((int)data[l] < 0x10)
				Serial.print("0");
			//Serial.print(((long)data[l] < (long)0x10000 ? "0" : ""));
			//Serial.print(((long)data[l] < (long)0x1000 ? "0" : ""));
			//Serial.print(((long)data[l] < (long)0x100 ? "0" : ""));
			//Serial.print(((long)data[l] < (long)0x10 ? "0" : ""));
			Serial.print(l, HEX);
			Serial.print(":  -  ");
		}
		Serial.print(data[l] < 0x10 ? ",0" : ",");
		Serial.print(data[l], HEX);
		//Serial.print(" ");
	}
	Serial.println();

	for (long l = 0; l < EEPROM_MAX_READ_ADDRESS; l++)
	{
		data[l] = 0x00;
	}

	Serial.print("Fuer das lesen benoetigte Zeit: ");
	Serial.print(timeRead);
	Serial.println("ms");
	Serial.println();

	delay(500);
}
