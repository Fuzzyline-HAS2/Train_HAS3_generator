#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// ==========================================
// 사용자 설정 (USER CONFIGURATION)
// ==========================================

// 1. 와이파이 설정
const char *ssid = "badland_ruins";
const char *password = "Code3824@";

// 2. 펌웨어 다운로드 주소
const char *firmware_url = "https://raw.githubusercontent.com/Fuzzyline-HAS2/"
                           "generator/hyoju/update.bin";

// 버전 정보 파일 URL (version.txt)
const char *version_url = "https://raw.githubusercontent.com/Fuzzyline-HAS2/"
                          "generator/hyoju/version.txt";

// 3. 디버그 및 버전 정보
#define CURRENT_FIRMWARE_VERSION 1

// ==========================================
#endif
