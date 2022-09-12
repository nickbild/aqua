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

////
// Aquarius side of RAM.
////

#define CE_1 A15
#define WE_1 A14
#define OE_1 A13

#define AD0_1 A12
#define AD1_1 A11
#define AD2_1 A10
#define AD3_1 A9
#define AD4_1 A8
#define AD5_1 A7
#define AD6_1 A6
#define AD7_1 A5
#define AD8_1 A4
#define AD9_1 A3

#define DATA0_1 A2
#define DATA1_1 A1
#define DATA2_1 A0
#define DATA3_1 8
#define DATA4_1 9
#define DATA5_1 10
#define DATA6_1 11
#define DATA7_1 12

long addr = 0;

void setup() {
  pinMode(CE_0, OUTPUT);
  pinMode(WE_0, OUTPUT);
  pinMode(OE_0, OUTPUT);

  pinMode(CE_1, OUTPUT);
  pinMode(WE_1, OUTPUT);
  pinMode(OE_1, OUTPUT);


  digitalWrite(CE_0, HIGH);
  digitalWrite(WE_0, HIGH);
  digitalWrite(OE_0, HIGH);

  digitalWrite(CE_1, HIGH);
  digitalWrite(WE_1, HIGH);
  digitalWrite(OE_1, HIGH);

  
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

  pinMode(AD0_1, OUTPUT);
  pinMode(AD1_1, OUTPUT);
  pinMode(AD2_1, OUTPUT);
  pinMode(AD3_1, OUTPUT);
  pinMode(AD4_1, OUTPUT);
  pinMode(AD5_1, OUTPUT);
  pinMode(AD6_1, OUTPUT);
  pinMode(AD7_1, OUTPUT);
  pinMode(AD8_1, OUTPUT);
  pinMode(AD9_1, OUTPUT);


  pinMode(DATA0_0, OUTPUT);
  pinMode(DATA1_0, OUTPUT);
  pinMode(DATA2_0, OUTPUT);
  pinMode(DATA3_0, OUTPUT);
  pinMode(DATA4_0, OUTPUT);
  pinMode(DATA5_0, OUTPUT);
  pinMode(DATA6_0, OUTPUT);
  pinMode(DATA7_0, OUTPUT);

  pinMode(DATA0_1, INPUT);
  pinMode(DATA1_1, INPUT);
  pinMode(DATA2_1, INPUT);
  pinMode(DATA3_1, INPUT);
  pinMode(DATA4_1, INPUT);
  pinMode(DATA5_1, INPUT);
  pinMode(DATA6_1, INPUT);
  pinMode(DATA7_1, INPUT);

  
  Serial.begin(9600);
}

void loop() {
  uint8_t val = 0;

  for (int i=0; i<1024; i++) {
    addr = i;
    writeByteToRam(val);
    readByteFromRam();

    val++;
  }

  delay(99999999);
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
  digitalWrite(AD0_1, bitRead(addr, 0));
  digitalWrite(AD1_1, bitRead(addr, 1));
  digitalWrite(AD2_1, bitRead(addr, 2));
  digitalWrite(AD3_1, bitRead(addr, 3));
  digitalWrite(AD4_1, bitRead(addr, 4));
  digitalWrite(AD5_1, bitRead(addr, 5));
  digitalWrite(AD6_1, bitRead(addr, 6));
  digitalWrite(AD7_1, bitRead(addr, 7));
  digitalWrite(AD8_1, bitRead(addr, 8));
  digitalWrite(AD9_1, bitRead(addr, 9));

  delay(1);
  digitalWrite(CE_1, LOW);
  digitalWrite(OE_1, LOW);
  delay(1);

  String temp = String(digitalRead(DATA7_1)) + String(digitalRead(DATA6_1)) + String(digitalRead(DATA5_1)) + String(digitalRead(DATA4_1)) + String(digitalRead(DATA3_1)) + String(digitalRead(DATA2_1)) + String(digitalRead(DATA1_1)) + String(digitalRead(DATA0_1));
  int temp_i = strtol( temp.c_str(), NULL, 2 );
  Serial.print("Byte read:\t");
  Serial.println(temp_i);
  
  digitalWrite(OE_1, HIGH);
  digitalWrite(CE_1, HIGH);
  delay(1);
}
