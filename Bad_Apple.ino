void setup() {
  Serial.begin(9600);
}

void loop() {


  while (Serial.available() == 0);
  String receive_buffer = Serial.readStringUntil('\n');
  Serial.write("Hello World!\n");
}