#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {

  Serial.begin(57600);
  lcd.begin(16, 2);
  lcd.clear();
  while (Serial.available() == 0);
  int handshake_receive_buffer = Serial.read();
  if(handshake_receive_buffer == 6) {
    uint8_t handshake_send_buffer{7};
    Serial.write(handshake_send_buffer);
  } else {
    return;
  }
}

uint8_t receive_buffer{0};
uint8_t screen_segment_buffer[8];

void loop() {

  for (int segment = 0; segment < 8; segment++) {
    for (int line = 0; line < 8; line++) {

      while (Serial.available() == 0);
      receive_buffer = Serial.read();
      screen_segment_buffer[line] = receive_buffer;
    }
    lcd.createChar(segment, screen_segment_buffer);
  }

  lcd.clear();
  for (int row = 0; row <= 1; row++) {
    lcd.setCursor(0, row);
    for(int i = 0; i < 4; i++) {
      lcd.write(byte(i + row * 4));
    }
  }
}
