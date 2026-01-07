
// --- HW3-2 解答程式碼 ---

// 定義腳位
const int ledPin = 3;    // 作業要求 LED 在 D3
const int buttonPin = 2; // 按鈕接在 D2

// 變數設定
int mode = 0;            // 目前的模式 (0=恆亮, 1=慢閃, 2=中閃, 3=快閃)
int lastButtonState = HIGH; // 上一次按鈕的讀數 (預設 INPUT_PULLUP 為 HIGH)
int currentButtonState;     // 目前按鈕的讀數

// 閃爍計時用變數
unsigned long previousMillis = 0; // 記錄上次閃爍切換的時間
int ledState = LOW;               // 記錄目前燈是亮還是暗

// 設定三種閃爍速度 (毫秒)
const long intervalSlow = 1000;   // 慢閃 (1秒)
const long intervalMedium = 500;  // 中閃 (0.5秒)
const long intervalFast = 100;    // 快閃 (0.1秒)

void setup() {
  pinMode(ledPin, OUTPUT);
  // 使用 INPUT_PULLUP，這樣按鈕另一端只要接 GND 即可，不需外接電阻
  pinMode(buttonPin, INPUT_PULLUP); 
  
  // 初始狀態：恆亮
  digitalWrite(ledPin, HIGH); 
  Serial.begin(9600); // 開啟監控視窗方便除錯
}

void loop() {
  // 1. 讀取按鈕狀態
  currentButtonState = digitalRead(buttonPin);

  // 2. 偵測按鈕是否被「按下」（從 HIGH 變 LOW）
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50); // 簡單的去彈跳 (Debounce)
    if (digitalRead(buttonPin) == LOW) {
      mode++; // 切換到下一個模式
      
      // 如果超過模式 3，回到模式 0
      if (mode > 3) {
        mode = 0;
      }
      
      Serial.print("Mode changed to: ");
      Serial.println(mode);
      
      // 切換模式時，重置 LED 狀態以免卡在滅掉
      if (mode == 0) {
        digitalWrite(ledPin, HIGH);
      }
    }
  }
  
  // 更新按鈕狀態紀錄
  lastButtonState = currentButtonState;

  // 3. 根據模式執行對應動作
  unsigned long currentMillis = millis(); // 取得目前開機時間

  switch (mode) {
    case 0: // 模式 0: 恆亮
      digitalWrite(ledPin, HIGH);
      break;

    case 1: // 模式 1: 慢閃
      updateBlink(currentMillis, intervalSlow);
      break;

    case 2: // 模式 2: 不快不慢的閃爍 (中閃)
      updateBlink(currentMillis, intervalMedium);
      break;

    case 3: // 模式 3: 快閃
      updateBlink(currentMillis, intervalFast);
      break;
  }
}

// 用來控制閃爍的函式
void updateBlink(unsigned long currentTime, long interval) {
  if (currentTime - previousMillis >= interval) {
    // 儲存最後一次切換時間
    previousMillis = currentTime;

    // 切換 LED 狀態 (亮變暗，暗變亮)
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}