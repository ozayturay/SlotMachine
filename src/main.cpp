#include <Arduino.h>
#include <MaxMatrix.h>
#include <avr/pgmspace.h>

PROGMEM const unsigned char CH[] = {
8, 8, B00000110, B01101111, B11110110, B01100100, B00100100, B00101000, B00110000, B00100000, // 0: cherry
8, 8, B00000000, B00111100, B01111110, B11111101, B01100010, B00111100, B00000000, B00000000, // 1: lemon
8, 8, B00111100, B01111110, B11111111, B11111111, B11111101, B11111101, B01100010, B00111100, // 2: orange
8, 8, B00000000, B00011110, B01111111, B11111101, B11111010, B11100100, B01111000, B00000000, // 3: plum
8, 8, B00011000, B11111111, B01111010, B01111010, B01111010, B01111010, B00110100, B00011000, // 4: bell
8, 8, B00011000, B00111100, B00111100, B01111110, B01111110, B00011000, B00010000, B00100000, // 5: grape
8, 8, B00011100, B00111110, B01111101, B11111010, B11110100, B11101000, B01010000, B00100000, // 6: melon
8, 8, B00011000, B00011000, B00011000, B00011000, B00110000, B01100000, B01111110, B01111110, // 7: seven
8, 8, B01000010, B00100100, B00011000, B00111100, B11111111, B00011000, B00010000, B00010000, // 8: star
8, 8, B00111100, B01000010, B10011001, B10100101, B10000001, B10100101, B01000010, B00111100, // 9: joker
};


// Dot Matrix pin definitions
#define DOTMATRIX_DIN 4
#define DOTMATRIX_CS  3
#define DOTMATRIX_CLK 2
#define DOTMATRIX_DISPLAY_COUNT 1
MaxMatrix dot_matrix(DOTMATRIX_DIN, DOTMATRIX_CS, DOTMATRIX_CLK, DOTMATRIX_DISPLAY_COUNT);

byte xicht_happy[] = {8, 8, B01010101, B10101010, B00000000, B00011000, B00011000, B00000000, B10101010, B01010101};

// Scrollingtext helpers
byte buffer[10];
int cylinder[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 7, 7, 7, 5, 3, 4, 5, 3, 5, 8};
int cylinderStart = 0; // starting position for cylinder

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Put extracted character on Display
/*
String, co se posle je vlastne "valec" se symboly. kazde cislo je jiny symbol

obalit fci printchar nejak , aby brala dany string, co ma vypsat
ten by se porad opakoval...
Tzn. ze pokud je char posledni v rade, tak dalsi char je prvni char.

a bylo by tam pocet "otoceni", coz by bylo pocet shiftu, co to udela.
podle otoceni najdes v arrayi dany symbol a dokazes ho vytahnout.

pak naky zpomalovani a zrychlovani pri otoceni, to az pri dalsi iteraci

priklad.
string je "0123456789"
pocet shiftu je 4
tkaze symbol na kterym se to zastavi je "3"

string je "0123456789"
pocet shiftu je 18
symbol bude "7"

string je "8743987098564839863"
pocet shiftu je 31
symbol bude "6"

*/
int rollTheCylinder(int cylinderArr[], int arrSize, int shiftSpeed, int shiftCount, int startingIndex = -1){
	int realIndex = 0;

	if (startingIndex != -1) {
		realIndex = startingIndex;
	}

	for (int mainCount = 0; mainCount < shiftCount; mainCount++) // shift x times
	{

		if (realIndex >= arrSize) { // endless shift thru the cylinder array
      realIndex = realIndex - arrSize;
    }

		// display symbol to hidden place
		memcpy_P(buffer, CH + 10 * cylinderArr[realIndex], 10);
		dot_matrix.writeSprite(9, 0, buffer); // writes to 9th position because of 1px space
		dot_matrix.setColumn(9 + buffer[0], 0);

		for (int i = 0; i < buffer[0] + 1; i++) // shift from hidden place
		{
			delay(shiftSpeed);
			dot_matrix.shiftLeft(false, false);
		}

		realIndex++;

	}

	return realIndex;
}


void setup() {
    Serial.begin(115200);

    // Initialize MaxMatrix
    dot_matrix.init(); // module initialize
    dot_matrix.setIntensity(1); // dot matix intensity 0-15
    dot_matrix.clear();

    dot_matrix.writeSprite(0, 0, xicht_happy);
}

void loop() {

		delay(3000);

		int val = rollTheCylinder(cylinder, 20, 100, 4);

		delay(4000);

		val = rollTheCylinder(cylinder, 20, 100, 7, val);

		delay(1000);

		val = rollTheCylinder(cylinder, 20, 100, 5, val);

    while(1){
        /* endless stop */
    }

}