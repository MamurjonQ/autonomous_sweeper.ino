// Ultrasonic sensor pins: {Left, Front, Right}
const int YL[] = {10, 12, A1}; // Trig pins
const int EC[] = {11, 13, A2}; // Echo pins

long cm(int i) {
  digitalWrite(YL[i], LOW);
  delayMicroseconds(2);
  digitalWrite(YL[i], HIGH);
  delayMicroseconds(10);
  digitalWrite(YL[i], LOW);

  long duration = pulseIn(EC[i], HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void side(int en, int a, int b, int v) {
  digitalWrite(a, v > 0);
  digitalWrite(b, v < 0);
  analogWrite(en, constrain(abs(v), 0, 255));
}

void drive(int l, int r) {
  side(5, 2, 3, l); // left: D5 enable, D2/D3 direction
  side(6, 7, 8, r); // right: D6 enable, D7/D8 direction
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(YL[i], OUTPUT);
    pinMode(EC[i], INPUT);
  }
  for (int p = 2; p <= 9; p++) {
    pinMode(p, OUTPUT);
  }
  analogWrite(9, 170); // brush motor on, about 67%
}

void loop() {
  long l = cm(0), f = cm(1), r = cm(2);
  Serial.print(l); Serial.print(" ");
  Serial.print(f); Serial.print(" ");
  Serial.println(r);

  if (f < 15 || (l < 15 && r < 15)) {
    drive(-150, -150);
    delay(400);
    int d = l > r ? -1 : 1;
    drive(150 * d, -150 * d);
    delay(800);
  } 
  else if (l < 12) {
    drive(180, 100);
  } 
  else if (r < 12) {
    drive(100, 180);
  } 
  else {
    drive(170, 170);
  }
  
  delay(30);
}
