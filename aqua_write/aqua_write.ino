#include <SD.h>

////
// Arduino side of RAM.
////

#define CE_0 49
#define WE_0 47
#define OE_0 45

#define AD0_0 43
#define AD1_0 41
#define AD2_0 39
#define AD3_0 37
#define AD4_0 35
#define AD5_0 33
#define AD6_0 31
#define AD7_0 29
#define AD8_0 27
#define AD9_0 25

#define DATA0_0 34
#define DATA1_0 32
#define DATA2_0 30
#define DATA3_0 28
#define DATA4_0 26
#define DATA5_0 24
#define DATA6_0 22
#define DATA7_0 23

#define BUTTON 7

File myFile;
int idx = 0;
char stringArray[10];
int addr = 0;

void setup() {
  pinMode(CE_0, OUTPUT);
  pinMode(WE_0, OUTPUT);
  pinMode(OE_0, OUTPUT);

  digitalWrite(CE_0, HIGH);
  digitalWrite(WE_0, HIGH);
  digitalWrite(OE_0, HIGH);

  pinMode(AD0_0, OUTPUT);
  pinMode(AD1_0, OUTPUT);
  pinMode(AD2_0, OUTPUT);
  pinMode(AD3_0, OUTPUT);
  pinMode(AD4_0, OUTPUT);
  pinMode(AD5_0, OUTPUT);
  pinMode(AD6_0, OUTPUT);
  pinMode(AD7_0, OUTPUT);
  pinMode(AD8_0, OUTPUT);
  pinMode(AD9_0, OUTPUT);

  pinMode(DATA0_0, OUTPUT);
  pinMode(DATA1_0, OUTPUT);
  pinMode(DATA2_0, OUTPUT);
  pinMode(DATA3_0, OUTPUT);
  pinMode(DATA4_0, OUTPUT);
  pinMode(DATA5_0, OUTPUT);
  pinMode(DATA6_0, OUTPUT);
  pinMode(DATA7_0, OUTPUT);

  pinMode(BUTTON, INPUT);
    
  Serial.begin(9600);

  printSwLockDisableInstructions();
}

void loop() {  
  if (digitalRead(BUTTON) == LOW) {
    
    int next;
    int pageNum = 0;
    
    addr = 0;
    writeLoader(pageNum);

    if (!SD.begin(53)) {
      while (true);
    }
    
    myFile = SD.open("data.txt");
  
    if (myFile) {
      while ((next = myFile.read()) != -1) {
          char nextChar = (char) next;
          
          if (nextChar == '\n') {
              stringArray[idx] = '\0';
  
              // One complete line of the data file is ready at this point.
              writeByteToRam(atoi(stringArray));
              readByteFromRam();
              addr += 1;
              
              idx = 0;

              // One page complete.
              if (addr == 1024) {
                Serial.println("Page loaded. Transfer to RAM, then press button to continue.");
                printRamTransferInstructions();
                while (digitalRead(BUTTON) == HIGH) {}
                
                Serial.println("Continuing data load...");
                addr = 0;
                pageNum++;
                writeLoader(pageNum);
              }
              
          } else {
              stringArray[idx] = nextChar;
              idx += 1;
          }
      }
  
      myFile.close();
    }

    Serial.println("All SD data loaded. Transfer it to RAM.");
    printRamTransferInstructions();

    delay(2000); // Prevent button bounce.
  }
}

void writeByteToRam(int data) {
  // Set address and data.
  digitalWrite(AD0_0, bitRead(addr, 0));
  digitalWrite(AD1_0, bitRead(addr, 1));
  digitalWrite(AD2_0, bitRead(addr, 2));
  digitalWrite(AD3_0, bitRead(addr, 3));
  digitalWrite(AD4_0, bitRead(addr, 4));
  digitalWrite(AD5_0, bitRead(addr, 5));
  digitalWrite(AD6_0, bitRead(addr, 6));
  digitalWrite(AD7_0, bitRead(addr, 7));
  digitalWrite(AD8_0, bitRead(addr, 8));
  digitalWrite(AD9_0, bitRead(addr, 9));

  digitalWrite(DATA0_0, bitRead(data, 0));
  digitalWrite(DATA1_0, bitRead(data, 1));
  digitalWrite(DATA2_0, bitRead(data, 2));
  digitalWrite(DATA3_0, bitRead(data, 3));
  digitalWrite(DATA4_0, bitRead(data, 4));
  digitalWrite(DATA5_0, bitRead(data, 5));
  digitalWrite(DATA6_0, bitRead(data, 6));
  digitalWrite(DATA7_0, bitRead(data, 7));

  delay(1);
  digitalWrite(CE_0, LOW);
  digitalWrite(WE_0, LOW);
  delay(1);
  digitalWrite(WE_0, HIGH);
  digitalWrite(CE_0, HIGH);
  delay(1);

  Serial.print("Byte written:\t");
  Serial.print(data);
  Serial.print("\tAddress:\t");
  Serial.println(addr);
}

void readByteFromRam() {
  // Set address and data.
  digitalWrite(AD0_0, bitRead(addr, 0));
  digitalWrite(AD1_0, bitRead(addr, 1));
  digitalWrite(AD2_0, bitRead(addr, 2));
  digitalWrite(AD3_0, bitRead(addr, 3));
  digitalWrite(AD4_0, bitRead(addr, 4));
  digitalWrite(AD5_0, bitRead(addr, 5));
  digitalWrite(AD6_0, bitRead(addr, 6));
  digitalWrite(AD7_0, bitRead(addr, 7));
  digitalWrite(AD8_0, bitRead(addr, 8));
  digitalWrite(AD9_0, bitRead(addr, 9));

  pinMode(DATA0_0, INPUT);
  pinMode(DATA1_0, INPUT);
  pinMode(DATA2_0, INPUT);
  pinMode(DATA3_0, INPUT);
  pinMode(DATA4_0, INPUT);
  pinMode(DATA5_0, INPUT);
  pinMode(DATA6_0, INPUT);
  pinMode(DATA7_0, INPUT);

  delay(1);
  digitalWrite(CE_0, LOW);
  digitalWrite(OE_0, LOW);
  delay(1);

  String temp = String(digitalRead(DATA7_0)) + String(digitalRead(DATA6_0)) + String(digitalRead(DATA5_0)) + String(digitalRead(DATA4_0)) + String(digitalRead(DATA3_0)) + String(digitalRead(DATA2_0)) + String(digitalRead(DATA1_0)) + String(digitalRead(DATA0_0));
  int temp_i = strtol( temp.c_str(), NULL, 2 );
  Serial.print("Byte read:\t");
  Serial.println(temp_i);
  
  digitalWrite(OE_0, HIGH);
  digitalWrite(CE_0, HIGH);
  delay(1);

  pinMode(DATA0_0, OUTPUT);
  pinMode(DATA1_0, OUTPUT);
  pinMode(DATA2_0, OUTPUT);
  pinMode(DATA3_0, OUTPUT);
  pinMode(DATA4_0, OUTPUT);
  pinMode(DATA5_0, OUTPUT);
  pinMode(DATA6_0, OUTPUT);
  pinMode(DATA7_0, OUTPUT);
}

// The loader is machine code that sits at the start of the dual-port RAM chip
// memory and is used to copy the remainder of the contents to the correct SRAM location.
// This allows multiple pages of 1004 bytes (1024 minus loader size) to be copied in to memory for larger programs.
void writeLoader(int pageNum) {
  // Z80 machine code for the loader.
  uint8_t loader [] = {33, 20, 252, 1, 0, 64, 17, 238, 3, 126, 2, 35, 3, 27, 122, 179, 194, 9, 252, 201};

  // If this isn't the first page of data to load, adjust the address we are loading to (add 1004 per additional page).
  if (pageNum > 0) {
    int startLoc = 16384 + (1004 * pageNum);
    loader[4] = startLoc % 256;         // Low byte.
    loader[5] = int(startLoc / 256);    // High byte.
  }

  for (uint8_t val : loader) {
    writeByteToRam(val);
    readByteFromRam();
    addr += 1;
  }
}

void printRamTransferInstructions() {
  Serial.println("To load data to RAM:");
  Serial.println("poke 14341,252");
  Serial.println("poke 14340,0");
  Serial.println("x=usr(0)");
}

void printSwLockDisableInstructions() {
  Serial.println("Before starting, disable the Aquarius software lock:");

  Serial.println("poke 14510,63");
  Serial.println("poke 14412,63");
  Serial.println("poke 14530,63");

  Serial.println("10 print 1");
  Serial.println("run");

  Serial.println("poke 15250,175");
  Serial.println("poke 15251,211");
  Serial.println("poke 15252,255");
  Serial.println("poke 15253,201");

  Serial.println("poke 14341,59");
  Serial.println("poke 14340,146");
  Serial.println("x=usr(0)");

  Serial.println("poke 14510,191");
  Serial.println("poke 14412,191");
  Serial.println("poke 14530,191");

  Serial.println("run");
}
