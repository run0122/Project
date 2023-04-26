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
  //IR 센서 값을 읽어 출력해주는 코드
  IR_L_data = digitalRead(IR_L);
  IR_M_data = digitalRead(IR_M);
  IR_R_data = digitalRead(IR_R);

  Serial.print(IR_L_data);
  Serial.print("-");
  Serial.print(IR_M_data);
  Serial.print("-");
  Serial.println(IR_R_data);

  if (Serial.available() > 0) {
    char input = Serial.read();

    if (IR_L_data == 1 or IR_M_data == 1 or IR_R_data == 1) {
      if (input == 'D') {  // 보행자가 없을 때는 정상 IR 센서 동작
        drive();
      } else if (input == 'T') {  // 보행자가 있을 때는 Turn 함수 호출 -> IR센서 값이 0 0 0
        turn();
      }
    }
    else if (IR_L_data == 0 and IR_M_data == 0 and IR_R_data == 0) {       // 라인을 벗어낫을 때
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
  delay(15);
  digitalWrite(motor_A1, HIGH);
  digitalWrite(motor_A2, LOW);
  digitalWrite(motor_B1, LOW);
  digitalWrite(motor_B2, LOW);
}

void left() {
  delay(15);
  digitalWrite(motor_A1, LOW);
  digitalWrite(motor_A2, LOW);
  digitalWrite(motor_B1, HIGH);
  digitalWrite(motor_B2, LOW);
}

void forward() {
  delay(15);
  digitalWrite(motor_A1, HIGH);
  digitalWrite(motor_A2, LOW);
  digitalWrite(motor_B1, HIGH);
  digitalWrite(motor_B2, LOW);
}

void backward() {
  delay(15);
  digitalWrite(motor_A1, LOW);
  digitalWrite(motor_A2, HIGH);
  digitalWrite(motor_B1, LOW);
  digitalWrite(motor_B2, HIGH);
}

void stop() {
  delay(15);
  digitalWrite(motor_A1, LOW);
  digitalWrite(motor_A2, LOW);
  digitalWrite(motor_B1, LOW);
  digitalWrite(motor_B2, LOW);
}

void turn() {
  right();
  delay(500);
  forward();
  delay(500);
  left();
  delay(500);
}