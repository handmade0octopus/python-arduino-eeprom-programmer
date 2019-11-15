#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13
#define BUFFER_SIZE 1024

unsigned int chipSize = 0x8000; // Change if you want to erease smaller chip. This size is for 32KB 28C256
byte programData[BUFFER_SIZE];

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


void programEEPROM(byte data[], unsigned int dataSize, unsigned int offset) {
  for (unsigned int address = offset; address < dataSize + offset; address += 1) {
    writeEEPROM(address, data[address - offset]);
  }
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (unsigned int base = 0; base < chipSize; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%04x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

void sendContents() {
for (unsigned int base = 0; base < chipSize; base += 1) {
      Serial.write(readEEPROM(base));    
  }
}

void ereaseEEPROM(boolean output) {
  // Erase entire EEPROM
  unsigned long startTime = millis();
  if(output) Serial.print("Erasing EEPROM");
  int progress = 0;
  for (unsigned int address = 0; address < chipSize; address += 1) {
    writeEEPROM(address, 0xFF);
    if (address % (chipSize/100) == 0) {
      if(output) Serial.print(".");
      if (address % ((chipSize/100)*10) == 0) {
        unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - startTime;
        int minutes = elapsedTime/60000;
        int seconds = elapsedTime/1000 - minutes*60;
        if(output) Serial.println();
        char buf[80];
        sprintf(buf, "%03d%s Time elapsed: %01d min %02d s", progress, "%", minutes, seconds);
        progress += 10;
        if(output) Serial.println(buf);
      }
    }
  }
  if(output) Serial.println("Done ereasing EEPROM");
}



void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(9600);
  

  
  // Uncomment those if you want to have additional functionality.
  
  // ereaseEEPROM(true); - ereases EEPROM to 0xFF and shows (or not) what stage the process is in
  // printContents(); - prints contents of the ROM
}


void loop() {
   // Reads chip size to be programmed
   if(Serial.available() > 0) {
    delay(30);
    if (Serial.available() == 1) {
        chipSize = Serial.read()*256;
        delay(30);
        Serial.write(chipSize/256);
    }
    Serial.flush();

    // After a delay starts programming rom by BUFFER_SIZE as Arduino nano only have 2048 byte memorry
    delay(300);
    for(unsigned int offset = 0; offset < chipSize; offset += BUFFER_SIZE) {
      Serial.readBytes(programData, BUFFER_SIZE);
      programEEPROM(programData, BUFFER_SIZE, offset);
      Serial.println(offset);
      Serial.flush();
    }

    // Waits and sends full rom bin to the reader
    delay(200);
    sendContents();
    delay(200);
   }
    
  
  
}
