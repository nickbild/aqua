#include <SD.h>

// SRAM Pin Mappings
#define AD0 22
#define AD1 24
#define AD2 26
#define AD3 28
#define AD4 30
#define AD5 32
#define AD6 34
#define AD7 36
#define AD8 38
#define AD9 40
#define AD10 42
#define AD11 44
#define AD12 46
#define AD13 48
#define AD14 49

#define DATA0 45
#define DATA1 43
#define DATA2 41
#define DATA3 39
#define DATA4 37
#define DATA5 35
#define DATA6 33
#define DATA7 31

#define WE 4
#define OE 5
#define CE 6
// END: SRAM Pin Mappings

#define BUTTON 8
#define BUS_CONNECT 12

File myFile;
int index = 0;
char stringArray[20];
int addr = 0;

void setup() {
  if (!SD.begin(53)) {
    while (true);
  }

  Serial.begin(9600);

  pinMode(AD0, OUTPUT);
  pinMode(AD1, OUTPUT);
  pinMode(AD2, OUTPUT);
  pinMode(AD3, OUTPUT);
  pinMode(AD4, OUTPUT);
  pinMode(AD5, OUTPUT);
  pinMode(AD6, OUTPUT);
  pinMode(AD7, OUTPUT);
  pinMode(AD8, OUTPUT);
  pinMode(AD9, OUTPUT);
  pinMode(AD10, OUTPUT);
  pinMode(AD11, OUTPUT);
  pinMode(AD12, OUTPUT);
  pinMode(AD13, OUTPUT);
  pinMode(AD14, OUTPUT);

  pinMode(DATA0, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(DATA3, OUTPUT);
  pinMode(DATA4, OUTPUT);
  pinMode(DATA5, OUTPUT);
  pinMode(DATA6, OUTPUT);
  pinMode(DATA7, OUTPUT);

  pinMode(WE, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(OE, OUTPUT);

  pinMode(BUTTON, INPUT);
  pinMode(BUS_CONNECT, OUTPUT);

  digitalWrite(WE, HIGH);
  digitalWrite(CE, LOW);
  digitalWrite(OE, HIGH);
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    digitalWrite(BUS_CONNECT, LOW);  // Take SRAM off Aquarius bus.
    prepareBusDisconnect();
    
    myFile = SD.open("data.txt");
  
    int next;
  
    if (myFile) { 
      while ((next = myFile.read()) != -1) {
          char nextChar = (char) next;
          
          if (nextChar == '\n') {
              stringArray[index] = '\0';
  
              // One complete line of the data file is ready at this point.
              writeByteToRam(atoi(stringArray));
              readByteFromRam();
              addr += 1;
              
              index = 0;
          } else {
              stringArray[index] = nextChar;
              index += 1;
          }
      }
  
      myFile.close();
    }

    prepareBusConnect();
    digitalWrite(BUS_CONNECT, HIGH);  // Put SRAM back on Aquarius bus.

    delay(2000); // Prevent button bounce.
  }

}

void writeByteToRam(int data) {
  // Set address and data.
  digitalWrite(AD0, bitRead(addr, 0));
  digitalWrite(AD1, bitRead(addr, 1));
  digitalWrite(AD2, bitRead(addr, 2));
  digitalWrite(AD3, bitRead(addr, 3));
  digitalWrite(AD4, bitRead(addr, 4));
  digitalWrite(AD5, bitRead(addr, 5));
  digitalWrite(AD6, bitRead(addr, 6));
  digitalWrite(AD7, bitRead(addr, 7));
  digitalWrite(AD8, bitRead(addr, 8));
  digitalWrite(AD9, bitRead(addr, 9));
  digitalWrite(AD10, bitRead(addr, 10));
  digitalWrite(AD11, bitRead(addr, 11));
  digitalWrite(AD12, bitRead(addr, 12));
  digitalWrite(AD13, bitRead(addr, 13));
  digitalWrite(AD14, bitRead(addr, 14));

  digitalWrite(DATA0, bitRead(data, 0));
  digitalWrite(DATA1, bitRead(data, 1));
  digitalWrite(DATA2, bitRead(data, 2));
  digitalWrite(DATA3, bitRead(data, 3));
  digitalWrite(DATA4, bitRead(data, 4));
  digitalWrite(DATA5, bitRead(data, 5));
  digitalWrite(DATA6, bitRead(data, 6));
  digitalWrite(DATA7, bitRead(data, 7));

  delay(1);

  digitalWrite(WE, LOW);
  delay(1);
  
  digitalWrite(WE, HIGH);
  delay(1);

  Serial.print("Byte written: ");
  Serial.println(data);
}

void readByteFromRam() {
  digitalWrite(AD0, bitRead(addr, 0));
  digitalWrite(AD1, bitRead(addr, 1));
  digitalWrite(AD2, bitRead(addr, 2));
  digitalWrite(AD3, bitRead(addr, 3));
  digitalWrite(AD4, bitRead(addr, 4));
  digitalWrite(AD5, bitRead(addr, 5));
  digitalWrite(AD6, bitRead(addr, 6));
  digitalWrite(AD7, bitRead(addr, 7));
  digitalWrite(AD8, bitRead(addr, 8));
  digitalWrite(AD9, bitRead(addr, 9));
  digitalWrite(AD10, bitRead(addr, 10));
  digitalWrite(AD11, bitRead(addr, 11));
  digitalWrite(AD12, bitRead(addr, 12));
  digitalWrite(AD13, bitRead(addr, 13));
  digitalWrite(AD14, bitRead(addr, 14));

  pinMode(DATA0, INPUT);
  pinMode(DATA1, INPUT);
  pinMode(DATA2, INPUT);
  pinMode(DATA3, INPUT);
  pinMode(DATA4, INPUT);
  pinMode(DATA5, INPUT);
  pinMode(DATA6, INPUT);
  pinMode(DATA7, INPUT);

  delay(1);

  digitalWrite(OE, LOW);
  delay(1);

  String temp = String(digitalRead(DATA7)) + String(digitalRead(DATA6)) + String(digitalRead(DATA5)) + String(digitalRead(DATA4)) + String(digitalRead(DATA3)) + String(digitalRead(DATA2)) + String(digitalRead(DATA1)) + String(digitalRead(DATA0));
  int temp_i = strtol( temp.c_str(), NULL, 2 );
  
  delay(1);
  digitalWrite(OE, HIGH);
  delay(1);

  Serial.print("Byte read: ");
  Serial.println(temp_i);

  pinMode(DATA0, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(DATA3, OUTPUT);
  pinMode(DATA4, OUTPUT);
  pinMode(DATA5, OUTPUT);
  pinMode(DATA6, OUTPUT);
  pinMode(DATA7, OUTPUT);
}

void prepareBusConnect() {
  pinMode(AD0, INPUT);
  pinMode(AD1, INPUT);
  pinMode(AD2, INPUT);
  pinMode(AD3, INPUT);
  pinMode(AD4, INPUT);
  pinMode(AD5, INPUT);
  pinMode(AD6, INPUT);
  pinMode(AD7, INPUT);
  pinMode(AD8, INPUT);
  pinMode(AD9, INPUT);
  pinMode(AD10, INPUT);
  pinMode(AD11, INPUT);
  pinMode(AD12, INPUT);
  pinMode(AD13, INPUT);
  pinMode(AD14, INPUT);

  pinMode(DATA0, INPUT);
  pinMode(DATA1, INPUT);
  pinMode(DATA2, INPUT);
  pinMode(DATA3, INPUT);
  pinMode(DATA4, INPUT);
  pinMode(DATA5, INPUT);
  pinMode(DATA6, INPUT);
  pinMode(DATA7, INPUT);

  pinMode(WE, INPUT);
  pinMode(OE, INPUT);
}

void prepareBusDisconnect() {
  pinMode(AD0, OUTPUT);
  pinMode(AD1, OUTPUT);
  pinMode(AD2, OUTPUT);
  pinMode(AD3, OUTPUT);
  pinMode(AD4, OUTPUT);
  pinMode(AD5, OUTPUT);
  pinMode(AD6, OUTPUT);
  pinMode(AD7, OUTPUT);
  pinMode(AD8, OUTPUT);
  pinMode(AD9, OUTPUT);
  pinMode(AD10, OUTPUT);
  pinMode(AD11, OUTPUT);
  pinMode(AD12, OUTPUT);
  pinMode(AD13, OUTPUT);
  pinMode(AD14, OUTPUT);

  pinMode(DATA0, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(DATA3, OUTPUT);
  pinMode(DATA4, OUTPUT);
  pinMode(DATA5, OUTPUT);
  pinMode(DATA6, OUTPUT);
  pinMode(DATA7, OUTPUT);

  pinMode(WE, OUTPUT);
  pinMode(OE, OUTPUT);
}
