#include <Arduino.h>
#include <TFT_eSPI.h>

// Initialize the Display
TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Use GPIO 4 to turn on the backlight for LilyGO T-Display
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  
  tft.println("16-bit HIL System");
  tft.drawFastHLine(0, 30, 240, TFT_WHITE);
}

void loop() {
  // Generate random 16-bit numbers (0 to 65535)
  uint16_t valA = random(1000, 10000); 
  uint16_t valB = random(100, 5000);
  uint8_t opcode = 0; // 0 for Addition

  // --- STEP 1: SEND DATA TO PYTHON BRIDGE ---
  // Send 1 byte for Opcode
  Serial.write(opcode);
  
  // Send 2 bytes for valA (High byte then Low byte)
  Serial.write((valA >> 8) & 0xFF);
  Serial.write(valA & 0xFF);
  
  // Send 2 bytes for valB (High byte then Low byte)
  Serial.write((valB >> 8) & 0xFF);
  Serial.write(valB & 0xFF);

  // --- STEP 2: UPDATE DISPLAY ---
  tft.fillRect(0, 40, 240, 100, TFT_BLACK); // Clear data area
  tft.setCursor(0, 50);
  tft.setTextColor(TFT_WHITE);
  tft.printf("A: %u\n", valA);
  tft.printf("B: %u (Op: ADD)\n", valB);
  tft.setTextColor(TFT_BLUE);
  tft.println("Waiting for RTL Core...");

  // --- STEP 3: WAIT FOR 16-BIT RESULT ---
  // We expect 2 bytes back from the Python/Verilog simulation
  long timeout = millis();
  while (Serial.available() < 2 && (millis() - timeout < 2000)) {
    // Wait for 2 seconds max
  }

  if (Serial.available() >= 2) {
    uint8_t highByte = Serial.read();
    uint8_t lowByte = Serial.read();
    uint16_t result = (highByte << 8) | lowByte;

    tft.setCursor(0, 110);
    tft.setTextColor(TFT_YELLOW);
    tft.printf("CORE RESULT: %u", result);
  } else {
    tft.setCursor(0, 110);
    tft.setTextColor(TFT_RED);
    tft.println("TIMEOUT: CORE OFFLINE");
  }

  delay(4000); // 4-second test cycle
}