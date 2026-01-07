// --- HW3-2 第四題 (Section Four) 解答 ---
// 這是修改自 Arduino 內建範例 "Button"

// 定義腳位
const int buttonPin = 2;     // 按鈕連接到 D2
const int ledPin =  3;       // 題目要求：LED 連接到 D3 (原範例是 13)

// 變數
int buttonState = 0;         // 讀取按鈕狀態用

void setup() {
  // 初始化 LED 腳位為輸出
  pinMode(ledPin, OUTPUT);
  
  // 初始化按鈕腳位為輸入
  // 【重要修改】這裡改用 INPUT_PULLUP，這樣你按鈕接線只要接到 GND 即可
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // 讀取按鈕狀態
  buttonState = digitalRead(buttonPin);

  // 檢查按鈕是否被按下
  // 因為使用了 INPUT_PULLUP，按下時會讀到 LOW (低電位)
  if (buttonState == LOW) {
    // 按下時，打開 LED
    digitalWrite(ledPin, HIGH);
  } else {
    // 放開時，關閉 LED
    digitalWrite(ledPin, LOW);
  }
}