// ===== 腳位定義（依照你的圖，完全不改） =====
const int buttonPin = 2;   // 按鈕 IN 接 D2

const int redPin   = 3;    // R 接 D3
const int greenPin = 4;    // G 接 D4
const int bluePin  = 5;    // B 接 D5

// 顏色狀態
int colorState = 0;

// 按鈕狀態紀錄（用來避免連續觸發）
bool lastButtonState = LOW;

void setup() {
  pinMode(buttonPin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // 一開始全部關燈
  turnOffLED();
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // 偵測「剛按下去」那一瞬間
  if (buttonState == HIGH && lastButtonState == LOW) {
    colorState++;
    if (colorState > 6) {
      colorState = 0;
    }

    setColor(colorState);

    delay(200); // 簡單防彈跳
  }

  lastButtonState = buttonState;
}

// ===== 設定顏色（共陽 LED：LOW = 亮） =====
void setColor(int state) {
  switch (state) {
    case 0: // 紅
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      break;

    case 1: // 綠
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      break;

    case 2: // 藍
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      break;

    case 3: // 黃（紅+綠）
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      break;

    case 4: // 紫（紅+藍）
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      break;

    case 5: // 白（全亮）
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      break;

    case 6: // 全關
      turnOffLED();
      break;
  }
}

// ===== 關燈 =====
void turnOffLED() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}
