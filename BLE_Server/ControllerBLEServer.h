#ifndef CONTROLLER_BLE_SERVER_H
#define CONTROLLER_BLE_SERVER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLECharacteristic.h>

/*
  コントローラが公開するデータの構造体
  - buttons: ABXYボタンをビットフラグで表す( bit0=>A, bit1=>B, bit2=>X, bit3=>Y )
  - x, y: アナログスティックの値(int16_t)とする
*/

// (ボタンのステータスやアナログスティックなど、必要に応じて拡張する)
struct ControllerData {
  uint8_t buttons;
  int16_t x;
  int16_t y;
};

class ControllerBLEServer {
public:
  ControllerBLEServer();
  ~ControllerBLEServer();

  // BLEサーバの初期化・開始
  //   deviceName        : アドバタイズ名
  //   serviceUUID       : サービスUUID
  //   characteristicUUID: キャラクタスティックUUID
  void begin(const char* deviceName,
             const char* serviceUUID,
             const char* characteristicUUID);

  // コントローラ入力を更新し、BLEで通知する
  void setControllerInput(bool bt_a, bool bt_b, bool bt_x, bool bt_y, int16_t x, int16_t y);

  // ループ内で継続処理が必要なら呼ぶ
  void update();

private:
  // 内部で使う関数：Characteristic にデータを書き込み & Notify
  void notifyControllerData();

private:
  BLEServer*        m_pServer;
  BLECharacteristic* m_pCharacteristic;

  ControllerData    m_controllerData;
};

#endif
