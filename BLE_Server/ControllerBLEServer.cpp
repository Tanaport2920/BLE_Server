//#include "robotWireless.h"

#include "ControllerBLEServer.h"

// サーバー側のコールバッククラス
class ControllerServerCallbacks : public BLEServerCallbacks {
  void onDisconnect(BLEServer* pServer) override {
    Serial.println("Client disconnected. Restart advertising...");
    // 切断されたのでアドバタイズ再開
    BLEDevice::startAdvertising();
  }
};

ControllerBLEServer::ControllerBLEServer() {
  m_pServer = nullptr;
  m_pCharacteristic = nullptr;
  memset(&m_controllerData, 0, sizeof(m_controllerData));
}

ControllerBLEServer::~ControllerBLEServer() {
}

// BLEサーバの初期化・開始
void ControllerBLEServer::begin(const char* deviceName,
                                const char* serviceUUID,
                                const char* characteristicUUID)
{
  // BLE初期化
  BLEDevice::init(deviceName);

  // サーバを作成
  m_pServer = BLEDevice::createServer();

  // コールバック登録
  m_pServer->setCallbacks(new ControllerServerCallbacks());

  // サービスを作成
  BLEService* pService = m_pServer->createService(serviceUUID);

  // キャラクタスティックを作成
  //   読み込み(READ)と通知(NOTIFY)を使えるようにする
  m_pCharacteristic = pService->createCharacteristic(
    characteristicUUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  // サービス開始
  pService->start();

  // アドバタイズ開始
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceUUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();

  Serial.println("[ControllerBLEServer] BLE server started (Controller).");
}

// コントローラ入力を更新し、通知する
void ControllerBLEServer::setControllerInput(bool bt_a, bool bt_b, bool bt_x, bool bt_y,
                                             int16_t x, int16_t y)
{
  // ボタンをビットフラグにまとめる
  //  bit0=>A, bit1=>B, bit2=>X, bit3=>Y
  uint8_t buttons = 0;
  if(bt_a) buttons |= 0x01;
  if(bt_b) buttons |= 0x02;
  if(bt_x) buttons |= 0x04;
  if(bt_y) buttons |= 0x08;

  m_controllerData.buttons = buttons;
  m_controllerData.x = x;
  m_controllerData.y = y;

  // 通知
  notifyControllerData();
}

// 実際にCharacteristicを書き換えてNotifyする
void ControllerBLEServer::notifyControllerData() {
  if (!m_pCharacteristic) return;

  // ControllerDataをCharacteristicへセット
  m_pCharacteristic->setValue((uint8_t*)&m_controllerData, sizeof(m_controllerData));
  m_pCharacteristic->notify(); // 接続中のクライアントに通知
}

void ControllerBLEServer::update() {
  // 特に継続処理がなければ何もしない
}
