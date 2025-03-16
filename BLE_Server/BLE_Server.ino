/*
  Controller側 (Server) サンプル
  ABXYボタン、アナログスティックの入力を setControllerInput() で更新し、
  BLE Notify でロボット側(クライアント)へ送信
*/

#include <Arduino.h>
// BLE関連（サーバ）のラッピング
#include <ControllerBLEServer.h>

// UUID (ロボットと一致させる)
#define MY_SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define MY_CHARACTERISTIC_UUID "abcdef01-1234-5678-1234-56789abcdef0"

// コントローラ(BLEサーバ)
ControllerBLEServer myController;

// テスト用の擬似入力状態
bool bt_a = false, bt_b = false, bt_x = false, bt_y = false;
int16_t x = 0, y = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Controller (Server) starting...");

  // BLEサーバ開始
  myController.begin(
    "MyControllerServer",     // アドバタイズ名
    MY_SERVICE_UUID,          // サービスUUID
    MY_CHARACTERISTIC_UUID    // キャラUUID
  );
}

void loop() {
  // 0.5秒ごとにテスト更新
  static unsigned long prevMs = 0;
  if (millis() - prevMs > 30) {
    prevMs = millis();

    // ABボタンをトグルしてみる
    bt_a = !bt_a; 
    bt_b = !bt_b;
    bt_x = !bt_x; 
    bt_y = !bt_y;

    // 入力をライブラリにセット => Notifyで送信される
    myController.setControllerInput(bt_a, bt_b, bt_x, bt_y, x, y);

    Serial.print("Updated controller input. A=");
    Serial.print(bt_a); 
    Serial.print(", B=");
    Serial.print(bt_b);
    Serial.print(", X=");
    Serial.print(bt_x);
    Serial.print(", Y=");
    Serial.println(bt_y);
  }

  // ライブラリ更新
  myController.update();
}
