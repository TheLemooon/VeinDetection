#include <WiFi.h>
#include <esp_camera.h>
#include <WiFiClient.h>
#include <Arduino.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* serverIP = "your_pc_ip";  // PC IP address (Receiver)
const int serverPort = 8080;

WiFiServer server(serverPort);

//void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.println("starting");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 15;
  config.pin_d1 = 2;
  config.pin_d2 = 4;
  config.pin_d3 = 0;
  config.pin_d4 = 5;
  config.pin_d5 = 18;
  config.pin_d6 = 19;
  config.pin_d7 = 21;
  config.pin_xclk = 22;
  config.pin_pclk = 23;
  config.pin_vsync = 25;
  config.pin_href = 26;
  config.pin_sscb_sda = 27;
  config.pin_sscb_scl = 32;
  config.pin_pwdn = 33;
  config.pin_reset = -1;
  config.xclk_freq_hz = 1000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("Camera initialization failed");
    return;
  }

  // Start server to send image
  server.begin();
  Serial.println("Camera ready");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Failed to capture image");
      return;
    }

    // Send the captured image to the client
    client.write(fb->buf, fb->len);
    esp_camera_fb_return(fb);

    delay(100);
  }
  else
  {
    Serial.println("server unavailable");
  }
}
