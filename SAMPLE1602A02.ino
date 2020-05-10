// ヘッダ
#include <LiquidCrystal.h>
#include <Bounce2.h>
// 定数
const int BUTTON = 7;           //プッシュボタンを接続するピン
const int BUTTON_PUSH = LOW;    //ボタン押下状態(INPUT_PULLUP)
// 変数
String inputString = "";         // 受信文字列
bool stringComplete = false;  // 文字列受信完了フラグ

int val = 0;   //入力ピンの値
int oldVal = 0; //過去値の保持用
int state = 0; //LED状態(1:ON,0:OFF)
// バウンスのインスタンスを生成
Bounce debouncer = Bounce();
// LCDの設定
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);   //LCDモジュールの初期設定(16文字,2行)
  lcd.print("SERIAL TEST!");  //LCDの１行目の出力メッセージ
  Serial.begin(9600);   //通信速度(9600bps)
  inputString.reserve(200);   //受信文字列エリアは200文字
  pinMode(BUTTON, INPUT_PULLUP);
  debouncer.attach(BUTTON);
  debouncer.interval(5); // interval in ms
}

void loop() {

  debouncer.update();     // デバウンサをupdate()する
  val = debouncer.read();    //デバウンサ経由でボタン状態を取得
  if ((val == BUTTON_PUSH) && (oldVal != BUTTON_PUSH)) {      // ボタン押下状態の場合
    Serial.println("hello,world!");
  }
  oldVal = val;         //現在の状態をストア
  
  lcd.setCursor(0, 1);
  if(stringComplete == true) {
    Lcd_Print16(inputString);
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

// １６文字LCD用のPRINTルーチン
void Lcd_Print16(String s) {
  s.trim();   //ゴミ取り
  s.concat("                ");   //16文字分のスペースを連結
  lcd.print(s);
}

// シリアルイベント（受信）
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
