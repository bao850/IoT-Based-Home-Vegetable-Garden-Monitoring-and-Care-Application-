#include "camera_module.h"
#define CAMERA_MODEL_ESP32S3_CAM_LCD
#include "camera_pins.h"
#include <WiFiClient.h>

bool initCameraModule(){
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer   = LEDC_TIMER_0;

    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size   = FRAMESIZE_XGA;
    config.xclk_freq_hz = 20000000;
    config.jpeg_quality = 8;
    config.fb_count     = 1;

    return esp_camera_init(&config) == ESP_OK;
}

String extractSecureURL(String payload){
    String key = "\"secure_url\":\"";
    int s = payload.indexOf(key);
    if(s < 0) return "";
    s += key.length();
    int e = payload.indexOf("\"", s);
    if(e < 0) return "";
    return payload.substring(s, e);
}

String uploadToCloudinary(camera_fb_t* fb, String cloudName, String preset, String folder){
    WiFiClient client;
    String host = "api.cloudinary.com";
    String url  = "/v1_1/" + cloudName + "/image/upload";

    if(!client.connect(host.c_str(), 80)){
        return "";
    }

    String boundary = "----ESP32BOUNDARY";
    String fileName = "esp32_" + String(millis()) + ".jpg";

    String bodyStart = "";
    bodyStart += "--" + boundary + "\r\n";
    bodyStart += "Content-Disposition: form-data; name=\"upload_preset\"\r\n\r\n";
    bodyStart += preset + "\r\n";

    bodyStart += "--" + boundary + "\r\n";
    bodyStart += "Content-Disposition: form-data; name=\"folder\"\r\n\r\n";
    bodyStart += folder + "\r\n";

    bodyStart += "--" + boundary + "\r\n";
    bodyStart += "Content-Disposition: form-data; name=\"file\"; filename=\"" + fileName + "\"\r\n";
    bodyStart += "Content-Type: image/jpeg\r\n\r\n";

    String bodyEnd = "\r\n--" + boundary + "--\r\n";
    int len = bodyStart.length() + fb->len + bodyEnd.length();

    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: " + host);
    client.println("Connection: close");
    client.println("Content-Type: multipart/form-data; boundary=" + boundary);
    client.println("Content-Length: " + String(len));
    client.println();

    client.print(bodyStart);
    client.write(fb->buf, fb->len);
    client.print(bodyEnd);

    String res;
    while(client.connected() || client.available()){
        if(client.available()) res += client.readString();
    }
    client.stop();
    return res;
}

String captureImageThenUpload(String cloudName, String preset, String folder){

    // ⭐ WARM-UP CAMERA (FIX ẢNH XANH)
    for(int i = 0; i < 2; i++){
        camera_fb_t* tmp = esp_camera_fb_get();
        if(tmp) esp_camera_fb_return(tmp);
        delay(200);
    }

    // ⭐ FRAME THẬT
    camera_fb_t* fb = esp_camera_fb_get();
    if(!fb) return "";

    String raw = uploadToCloudinary(fb, cloudName, preset, folder);
    esp_camera_fb_return(fb);

    return extractSecureURL(raw);
}
