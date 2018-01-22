// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

//Particle Photon ID: 3a0038000547343138333038

//defined NeoPixel strip & motion sensor.
#define PIXEL_PIN D0
#define PIXEL_COUNT 9
#define PIXEL_TYPE WS2812B
#define PROX_SENS A5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_PIN);

// integer for counting motion sensor activations
int count = 0;

// converting number of neopixels to integer
int pixels = PIXEL_COUNT;

// soil moisture sensor
int soilVal = 0; // maxx value = 4095
int soilPin1 = A0;
int soilPower = D2;

// number of pixels lit up for feedback
int rate;

// rating states
int state1 = 0000; //      < 899
int state2 = 1300; // 900 < 1699
int state3 = 2000; // 1700 < 2599

int state4 = 2600; // 2600 < 2799
int state5 = 2800; // 2800 < 2924
int state6 = 2925; // 2925 < 2999

int state7 = 3000; // 3000 < 3029
int state8 = 3030; // 3030 < 3049
int state9 = 3050; // 3050 <

// proximenty sensor
int proxThreshold = 1000;

// interval timer
int minute = 1;
const long readingInterval = 60000 * minute;
unsigned long previousMillis = 0;

// colours
// uint32_t = strip.Color();
uint32_t green = strip.Color(255,0,0);
uint32_t red = strip.Color(0,255,0);
uint32_t blue = strip.Color(0,0,255);
uint32_t yellow = strip.Color(255,255,0);
uint32_t off = strip.Color(0,0,0);



void setup() {

    Time.zone(+10);

    // initiate neopixels
    strip.begin();
    strip.show();

    Serial.begin(9600);

    //soil moisture sensor initiate
    pinMode(soilPower, OUTPUT);
    digitalWrite(soilPower, LOW);

//     readSoil();
//     rateSoil(soilVal);
//     result(soilVal, count, rate);
}



void loop() {

    proxDetect();

    checkSoil();

}



void checkSoil() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > readingInterval) {
        previousMillis = currentMillis;
        readSoil();
        rateSoil(soilVal);
        result(soilVal, count, rate);
        count = 0;
    }
}

void proxDetect() {
    int readProx = analogRead(PROX_SENS);
    if (readProx > proxThreshold) {
        readSoil();
        sendFeedback(soilVal);
        Particle.publish("Detection Read", String(soilVal));
        count++;
    }
}

void rateSoil(int reading) {
    rate = PIXEL_COUNT;

    if (reading >= state9) {
        rate;
    } else if (reading > state8) {
        rate = rate - 1;
    } else if (reading > state7) {
        rate = rate - 2;

    } else if (reading > state6) {
        rate = rate - 3;
    } else if (reading > state5) {
        rate = rate - 4;
    } else if (reading > state4) {
        rate = rate - 5;

    } else if (reading > state3) {
        rate = rate - 6;
    } else if (reading > state2) {
        rate = rate - 7;
    } else {
        rate = rate - 8;
    }
}

void readSoil() {
    digitalWrite(soilPower, HIGH);
    delay(10);
    soilVal = analogRead(soilPin1);
    digitalWrite(soilPower, LOW);
}

void sendFeedback(int reading) {

    pixels = PIXEL_COUNT;


    for (int i = 0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, 0,0,0);
        strip.show();
    }
    if (reading >= state9) {
        setStripColor(blue, pixels);
    } else if (reading > state8) {
        pixels = pixels - 1;
        setStripColor(blue, pixels);
    } else if (reading > state7) {
        pixels = pixels - 2;
        setStripColor(blue, pixels);

    } else if (reading > state6) {
        pixels = pixels - 3;
        setStripColor(green, pixels);
    } else if (reading > state5) {
        pixels = pixels - 4;
        setStripColor(green, pixels);
    } else if (reading > state4) {
        pixels = pixels - 5;
        setStripColor(green, pixels);

    } else if (reading > state3) {
        pixels = pixels - 6;
        setStripColor(red, pixels);
    } else if (reading > state2) {
        pixels = pixels - 7;
        setStripColor(red, pixels);
    } else {
        pixels = pixels - 8;
        setStripColor(red, pixels);
    }
    delay(5000);
    clearStrip();
}

void result(int read, int counter, int rate) {
    Particle.publish("Activated", String(counter));
    Particle.publish("Rating", String(rate));
    Particle.publish("READING", String(read));
}

void setStripColor(uint32_t colour, int lit) {
    for (int i = 0; i < lit; i++) {
        strip.setPixelColor(i, colour);
    }
    strip.show();
}

void clearStrip() {
    for (int i = 0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, off);
    }
    strip.show();
}
