// wiring: 3 separate reistors for each anode on pwm pins, cathodes together on gnd

const int PIN_R = 5; 
const int PIN_B = 6;
const int PIN_G = 3;

int r = 100;
int b = 100;
int g = 100;

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_B, OUTPUT);
}

void loop() {
  r = random(0, 155);
  b = random(0, 155);
  g = random(0, 155);

  analogWrite(PIN_R, r);
  analogWrite(PIN_B, b);
  analogWrite(PIN_G, g);

  delay(500);
}
