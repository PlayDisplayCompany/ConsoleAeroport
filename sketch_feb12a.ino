#define BTN1 3
#define BTN2 4
#define BTN3 5
#define BTN4 6
#define BTN5 7
#define BTN6 8

#define ENC_A 10
#define ENC_B 9

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char ubyte;

void setup() {
  // put your setup code here, to run once:
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  pinMode(BTN4, INPUT);
  pinMode(BTN5, INPUT);
  pinMode(BTN6, INPUT);
  
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  //pinMode(E1, OUTPUT);
  Serial.begin(115200);
}
ubyte pv[] = {0, 0, 0, 0, 0, 0};
bool updateBtns() {
  uint sbtn1 = digitalRead(BTN1);
  uint sbtn2 = digitalRead(BTN2);
  uint sbtn3 = digitalRead(BTN3);
  uint sbtn4 = digitalRead(BTN4);
  uint sbtn5 = digitalRead(BTN5);
  uint sbtn6 = digitalRead(BTN6);
  
  if (sbtn1 != pv[0] || 
  sbtn2 != pv[1] || 
  sbtn3 != pv[2] || 
  sbtn4 != pv[3] || 
  sbtn5 != pv[4] || 
  sbtn6 != pv[5] ) {
    pv[0] = sbtn1;
    pv[1] = sbtn2;
    pv[2] = sbtn3;
    pv[3] = sbtn4;
    pv[4] = sbtn5;
    pv[5] = sbtn6;
    return true;
  } else return false;
  return false;
}

int encoderSpeed = 0;

void sendData() {
  byte spd[] = {encoderSpeed & 0xFF, (encoderSpeed >> 8) & 0xFF};
  ubyte ln[] = {10};
//  Serial.print(String(" ")+pv[0]+" "+pv[1]+" "+pv[2]+" "+pv[3]+" "+pv[4]+" "+pv[5]+" ");
//  Serial.println(spd[0]);
  Serial.write(pv, 6);
  Serial.write(spd, 2);
  Serial.write(ln, 1);
  Serial.flush();
}

uint pa = 0;
uint pb = 0;

int8_t read_encoder()
{
  static uint enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  int a = digitalRead(ENC_A);
  int b = digitalRead(ENC_B);
//  Serial.println(String("test ")+a+" "+b);
  int index = pa+(a<<1)+(pb<<2)+(b<<3);
  
  pa = a;
  pb = b;
  return enc_states[index];
}

ulong lastTime = 0;
ulong deltaTime = 0;
ulong currentTime = 0;
void loop() {
  currentTime = millis();
  
  uint tmpData = read_encoder();
  if (tmpData) {

    int spd = 2/(currentTime - deltaTime);
    encoderSpeed += tmpData * spd;
    deltaTime = currentTime;
  }

  if (updateBtns()) {
    sendData();
  }

  if ((millis() - lastTime) > 100) {
    lastTime = currentTime;

    sendData();
//    analogWrite(E1, abs(encoderSpeed));
//    Serial.println(encoderSpeed);
//    Serial.flush();
    encoderSpeed = 0;
  }
  delay(1);
}

