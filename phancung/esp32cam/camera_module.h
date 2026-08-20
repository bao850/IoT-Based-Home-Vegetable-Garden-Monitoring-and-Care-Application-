#pragma once
#include <Arduino.h>
#include <esp_camera.h>

bool initCameraModule();
String uploadToCloudinary(camera_fb_t* fb, String cloudName, String preset, String folder);
String extractSecureURL(String payload);
String captureImageThenUpload(String cloudName, String preset, String folder);
