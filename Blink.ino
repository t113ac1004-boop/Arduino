
// ----- Q5.3 -----
const byte PIN_R = 9, PIN_G = 10, PIN_B = 11;
const byte BTN   = 2;
const bool COMMON_ANODE = true;

void setRGB(uint8_t r, uint8_t g, uint8_t b){
  if (COMMON_ANODE){ r=255-r; g=255-g; b=255-b; }
  analogWrite(PIN_R, r); analogWrite(PIN_G, g); analogWrite(PIN_B, b);
}
void ledOff(){ setRGB(0,0,0); }

// 模式狀態
int mode = 1;                 // 1/2/3（一般模式）
int returnToPrev = -1;        // 記錄長按前的模式；-1 表未進長按
unsigned long tMode = 0;
int idx = 0; bool blinkOn = false;

// 按鍵
const unsigned long DEBOUNCE_MS = 30;
const unsigned long TAP_MS      = 500;   // 短按上限
const unsigned long LONG_MS     = 1500;  // 長按門檻（題目要求）
int stable = HIGH, lastRead = HIGH;
unsigned long lastEdge = 0, pressStart = 0;

// 呼吸參數（紅藍交替）
bool breathing = false, redPhase = true;
unsigned long tBreath = 0;
int breathVal = 0, breathDir = +1;       // 0..255

void resetModeTimers(unsigned long now){
  tMode = now; idx = 0; blinkOn = false;
}

void runNormal(unsigned long now){
  if (mode == 1){
    if (now - tMode >= 2000){ tMode = now; idx = (idx + 1) % 3; }
    if (idx==0)      setRGB(0,0,255);
    else if (idx==1) setRGB(0,255,255);
    else             setRGB(0,255,0);
  }else if (mode == 2){
    if (now - tMode >= 500){ tMode = now; blinkOn = !blinkOn; }
    if (blinkOn) setRGB(0,255,255); else ledOff();
  }else{ // 3
    if (now - tMode >= 500){ tMode = now; blinkOn = !blinkOn; }
    if (blinkOn) setRGB(0,255,0); else ledOff();
  }
}

void runBreath(unsigned long now){
  const unsigned long STEP_MS = 10;
  if (now - tBreath >= STEP_MS){
    tBreath = now;
    breathVal += breathDir;
    if (breathVal >= 255){ breathVal = 255; breathDir = -1; }
    else if (breathVal <= 0){ breathVal = 0; breathDir = +1; redPhase = !redPhase; }
  }
  if (redPhase) setRGB(breathVal,0,0);
  else          setRGB(0,0,breathVal);
}

void setup(){
  pinMode(PIN_R,OUTPUT); pinMode(PIN_G,OUTPUT); pinMode(PIN_B,OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
  ledOff(); tMode = millis();
}

void loop(){
  unsigned long now = millis();

  // 按鍵去抖
  int rd = digitalRead(BTN);
  if (rd != lastRead) lastEdge = now;
  if (now - lastEdge > DEBOUNCE_MS){
    if (rd != stable){
      stable = rd;
      if (stable == LOW){                    // 按下
        pressStart = now;
        // 先熄燈（規格：按住時熄滅，直到進入長按呼吸為止）
        ledOff();
      }else{                                 // 放開
        unsigned long dur = now - pressStart;
        if (breathing){
          // 結束長按呼吸 → 回到長按前的模式（或改成 2 代表固定回黃燈閃爍）
          breathing = false;
          mode = (returnToPrev == -1 ? 2 : returnToPrev);
          returnToPrev = -1;
          resetModeTimers(now);
        }else if (dur < TAP_MS){
          // 短按：切換模式
          mode = (mode % 3) + 1;
          resetModeTimers(now);
        } // 介於 TAP_MS ~ LONG_MS：單純按住，不做事
      }
    }
  }
  lastRead = rd;

  // 仍在按住：判斷是否跨過長按門檻
  if (stable == LOW && !breathing){
    if (now - pressStart >= LONG_MS){
      breathing = true;
      returnToPrev = mode;         // 記住原模式
      breathVal = 0; breathDir = +1; redPhase = true; tBreath = now;
    }else{
      ledOff();                    // 長按前的按住期間維持熄滅
      return;                      // 不跑一般模式
    }
  }

  // 驅動顯示
  if (breathing) runBreath(now);
  else           runNormal(now);
}

