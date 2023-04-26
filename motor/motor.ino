const int motor_A1 = 9;
const int motor_A2 = 8;
const int motor_B1 = 10;
const int motor_B2 = 11;
const int IR_R = A1;
const int IR_M = A3;
const int IR_L = A5;
int IR_L_data;
int IR_M_data;
int IR_R_data;

char input;

void setup() {
  pinMode(motor_A1, OUTPUT);
  pinMode(motor_A2, OUTPUT);
  pinMode(motor_B1, OUTPUT);
  pinMode(motor_B2, OUTPUT);
  pinMode(IR_L, INPUT);
  pinMode(IR_M, INPUT);
  pinMode(IR_R, INPUT);
  Serial.begin(9600);
}

void loop() {
  IR_L_data = digitalRead(IR_L);
  IR_M_data = digitalRead(IR_M);
  IR_R_data = digitalRead(IR_R);

  bool is_turned = false;

  if (Serial.available() > 0) {
    input = Serial.read();

    if (IR_L_data == 1 or IR_M_data == 1 or IR_R_data == 1) {
      if (input == 'D') {  // 보행자가 없을 때는 정상 IR 센서 동작
        drive();
      } else if (input == 'T') {  // 보행자가 있을 때는 Turn 함수 호출 -> IR센서 값이 0 0 0
        stop();
      }
    } else if (IR_L_data == 0 and IR_M_data == 0 and IR_R_data == 0) {  // 라인을 벗어낫을 때
      if (input == 'L') {
        left();
      } else if (input == 'R') {
        right();
      } else if (input == 'F') {
        forward();
      }
    }
  }
}

void drive() {
  if (IR_L_data == 0 and IR_M_data == 1 and IR_R_data == 0) {
    forward();
  } else if (IR_L_data == 1 and IR_M_data == 0 and IR_R_data == 0) {
    left();
  } else if (IR_L_data == 0 and IR_M_data == 0 and IR_R_data == 1) {
    right();
  } else if (IR_L_data == 1 and IR_R_data == 1) {
    stop();
  }
}

void right() {
  analogWrite(motor_A1, 255);
  digitalWrite(motor_A2, LOW);
  analogWrite(motor_B1, 220);
  digitalWrite(motor_B2, LOW);
}

void left() {
  analogWrite(motor_A1, 220);
  digitalWrite(motor_A2, LOW);
  analogWrite(motor_B1, 255);
  digitalWrite(motor_B2, LOW);
}

void forward() {
  analogWrite(motor_A1, 255);
  digitalWrite(motor_A2, LOW);
  analogWrite(motor_B1, 255);
  digitalWrite(motor_B2, LOW);
}

void backward() {
  digitalWrite(motor_A1, LOW);
  digitalWrite(motor_A2, HIGH);
  digitalWrite(motor_B1, LOW);
  digitalWrite(motor_B2, HIGH);
}

void stop() {
  digitalWrite(motor_A1, LOW);
  digitalWrite(motor_A2, LOW);
  digitalWrite(motor_B1, LOW);
  digitalWrite(motor_B2, LOW);
}

void turn() {
  milliright(500);
  milliforward(500);
  millileft(500);
}

void milliright(unsigned long x){
  unsigned long current_time = millis();
  unsigned long interval = x;
  while (millis() - current_time < interval) { // 현재 시간과의 차가 500 밀리초 미만일 때까지 오른쪽으로 회전합니다.
    right();
    if (Serial.available() > 0 && Serial.read() == 'D') {
      stop(); // D신호가 오면 모터를 멈추고 함수 종료
      return;
    }
  }
}
void millileft(unsigned long x){
  unsigned long current_time = millis();
  unsigned long interval = x;
  while (millis() - current_time < interval) { // 현재 시간과의 차가 500 밀리초 미만일 때까지 오른쪽으로 회전합니다.
    left();
    if (Serial.available() > 0 && Serial.read() == 'D') {
      stop(); // D신호가 오면 모터를 멈추고 함수 종료
      return;
    }
  }
}
void milliforward(unsigned long x){
  unsigned long current_time = millis();
  unsigned long interval = x;
  while (millis() - current_time < interval) { // 현재 시간과의 차가 500 밀리초 미만일 때까지 오른쪽으로 회전합니다.
    forward();
    if (Serial.available() > 0 && Serial.read() == 'D') {
      stop(); // D신호가 오면 모터를 멈추고 함수 종료
      return;
    }
  }
}