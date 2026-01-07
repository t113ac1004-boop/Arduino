const int rledPin = 6 ;  // 從 8 改到 6，因為 6 支援 PWM
const int gledPin = 9 ; 
const int bledPin = 10 ;
const int buttonPin = 7;

int mood = 10;
const int neutralMood = 10;
bool buttonPressed = false;

unsigned long touchedTimer = 0;
unsigned long reducedTimer = 0;
const long unTouchInterval = 5000;
const long reducedInterval = 1000;

void setup(){
  // 使用 INPUT_PULLUP 避免雜訊，按鈕另一端接 GND
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(rledPin, OUTPUT);
  pinMode(gledPin, OUTPUT);
  pinMode(bledPin, OUTPUT);
}

void loop(){
  showLEDState(mood);

  // 因為用了 INPUT_PULLUP，LOW 代表按下
  int buttonState = digitalRead(buttonPin);

  if(buttonState == LOW && !buttonPressed){
    mood = mood + 1;
    if(mood > 20) mood = 20;
    touchedTimer = millis();
    buttonPressed = true;
    delay(50); // 簡單的去彈跳避震
  }
  
  if(buttonState == HIGH && buttonPressed){
    buttonPressed = false;
  }

  unsigned long currentTimer = millis();
  
  // 5秒沒摸，且每隔1秒減少一次
  if(currentTimer - touchedTimer > unTouchInterval){
    if(currentTimer - reducedTimer > reducedInterval){
      if(mood > 0) mood--; 
      reducedTimer = currentTimer;
    }
  }
}

void showLEDState(int state){
  float brightnessInterval = 255.0 / 10.0;
  
  if(state >= neutralMood){
    // 越開心：紅色固定，綠色漸強，藍色漸弱
    analogWrite(rledPin, 255);
    analogWrite(gledPin, (int)(brightnessInterval * (state - neutralMood)));
    analogWrite(bledPin, (int)(255 - brightnessInterval * (state - neutralMood)));
  } else {
    // 越難過：藍色固定，綠色漸強，紅色漸弱
    analogWrite(bledPin, 255);
    analogWrite(gledPin, (int)(brightnessInterval * (neutralMood - state)));
    analogWrite(rledPin, (int)(255 - brightnessInterval * (neutralMood - state)));
  }
}