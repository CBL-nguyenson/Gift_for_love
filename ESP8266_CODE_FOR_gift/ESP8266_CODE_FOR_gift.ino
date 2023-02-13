#define BLYNK_TEMPLATE_ID "TMPLsrKTdMLH"
#define BLYNK_TEMPLATE_NAME "Gift for love"
#define BLYNK_AUTH_TOKEN "FjBl2_gt2PD2pcFoqQIjpTATge_iwrgE"
#define BLYNK_PRINT Serial
#define BLYNK_FIRMWARE_VERSION        "0.1.1"  //phiên bản phần mềm


#include "Connect_Wifi_and_Device.h"
#include "Blynk_Control.h"

char auth[] = "FjBl2_gt2PD2pcFoqQIjpTATge_iwrgE";


void setup() {
  Serial.begin(9600);
  Setup_Run(); // function to start Pins
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  if (!wifiManager.autoConnect("#DTQT")) {
    Serial.println("KẾT NỐI THẤT BẠI ĐANG RESET ESP CHỜ.....");
    ESP.reset();  //Nếu kết nối thất bại, thử kết nối lại bằng cách reset thiết bị
    delay(500);
  }

  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());   //Kết nối vào Server

}

void loop() {
  Blynk.run();
  bt_port.tick();    //Thư viện nút nhấn
  Loop_Run();
}
