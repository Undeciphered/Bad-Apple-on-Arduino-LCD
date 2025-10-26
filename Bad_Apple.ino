#include <LiquidCrystal.h>
#include <avr/wdt.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void reset() {

  wdt_enable(WDTO_15MS);
  for (;;);
}

void setup() {

  Serial.begin(57600);
  lcd.begin(16, 2);
  lcd.clear();
  while (Serial.available() == 0);
  int handshake_receive_buffer = Serial.read();
  if (handshake_receive_buffer == 6) {
    uint8_t handshake_send_buffer{ 7 };
    Serial.write(handshake_send_buffer);
  } else {
    reset();
  }
}

uint8_t screen_segment_buffer[8];
uint8_t buffer[64];
uint8_t byte_count{ 0 };
unsigned long since_last_data{0};

void loop() {

  while (Serial.available() > 0 && byte_count < 64) {
    buffer[byte_count++] = Serial.read();
    since_last_data = millis();
  }

  if (byte_count == 64) {

    for (int segment = 0; segment < 8; segment++) {
      for (int line = 0; line < 8; line++) {
        screen_segment_buffer[line] = buffer[8 * segment + line];
      }
      lcd.createChar(segment, screen_segment_buffer);
    }
    lcd.setCursor(0, 0);
    for (int i = 0; i < 4; i++) lcd.write(byte(i));
    lcd.setCursor(0, 1);
    for (int i = 0; i < 4; i++) lcd.write(byte(i+4));
    byte_count = 0;
  }

  if (millis() - since_last_data >= 1000 && since_last_data != 0) reset();
}
