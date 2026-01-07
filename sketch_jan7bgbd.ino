// 腳位定義
const int ledPin = 3;      // 加油棒 LED (支援 PWM 的腳位)
const int buttonPin = 2;   // 切換模式按鈕

// 變數設定
int mode = 0;              // 0:恆亮, 1:呼吸, 2:爆閃, 3:關閉
unsigned long prevMs = 0;
int brightness = 0;        // 呼吸燈亮度
int fadeAmount = 5;        // 呼吸燈變化速度

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  checkButton(); // 檢查是否按了按鈕

  unsigned long now = millis();

  if (mode == 0) {
    // --- 模式 0：恆亮 ---
    analogWrite(ledPin, 255);
  } 
  else if (mode == 1) {
    // --- 模式 1：柔和呼吸燈 ---
    if (now - prevMs >= 30) {
      prevMs = now;
      analogWrite(ledPin, brightness);
      brightness += fadeAmount;
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount; // 轉向
      }
    }
  } 
  else if (mode == 2) {
    // --- 模式 2：狂熱爆閃 (Strobe) ---
    if (now - prevMs >= 50) { // 50ms 閃一次，極快
      prevMs = now;
      digitalWrite(ledPin, !digitalRead(ledPin));
    }
  } 
  else {
    // --- 模式 3：關閉模式 (省電) ---
    digitalWrite(ledPin, LOW);
  }
}

// 簡單的按鈕偵測副程式
void checkButton() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(buttonPin);
  
  if (lastState == HIGH && currentState == LOW) { // 按下的瞬間
    mode = (mode + 1) % 4; // 四種狀態循環
    delay(200); // 簡單的防彈跳
  }
  lastState = currentState;
}