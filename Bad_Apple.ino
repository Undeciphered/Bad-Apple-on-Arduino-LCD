#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {

  Serial.begin(9600);
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

uint8_t receive_buffer

void loop() {
  
  while (Serial.available() == 0);
  receive_buffer = Serial.read();
  lcd.setCursor(0, 0);
  lcd.print(receive_buffer);
  lcd.print("     ");
}
