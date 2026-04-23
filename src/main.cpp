#include <Adafruit_NeoPixel.h>

// Function prototypes
void setup();
void loop();
// 핀 설정
#define LED_PIN       15   // 네오픽셀 데이터 핀
#define LED_COUNT     8    // 네오픽셀 개수
#define ENC_CLK       22   // 엔코더 CLK
#define ENC_DT        23   // 엔코더 DT

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// 상태 변수
int speedDelay = 100;      // 애니메이션 속도 (ms)
int currentPixel = 0;      // 현재 켜질 픽셀 위치
int lastCLK;               // 엔코더 이전 상태
unsigned long lastAnimTime = 0; // 마지막 애니메이션 업데이트 시간

void setup() {
    // Initialize serial communication at 115200 bits per second:
    Serial.begin(115200);
    
    // Add your setup code here:
    pinMode(LED_BUILTIN, OUTPUT);
    // 엔코더 핀 설정
    pinMode(ENC_CLK, INPUT_PULLUP);
    pinMode(ENC_DT, INPUT_PULLUP);
    lastCLK = digitalRead(ENC_CLK);

    // 네오픽셀 초기화
    strip.begin();
    strip.show();
    strip.setBrightness(50); // 기본 밝기

    Serial.println("NeoPixel Speed Control Ready");
}

void handleEncoder() {
    int currentCLK = digitalRead(ENC_CLK);

    // 엔코더 회전 감지 (CLK 상태 변화)
    if (currentCLK != lastCLK && currentCLK == LOW) {
        // 시계 방향/반시계 방향 판단
        if (digitalRead(ENC_DT) != currentCLK) {
            speedDelay -= 10; // 속도 증가 (지연 시간 감소)
        } else {
            speedDelay += 10; // 속도 감소 (지연 시간 증가)
        }

        // 범위 제한 (10ms ~ 500ms)
        speedDelay = constrain(speedDelay, 10, 500);

        Serial.print("Current Delay: ");
        Serial.print(speedDelay);
        Serial.println(" ms");
    }
    lastCLK = currentCLK;
}

void handleAnimation() {
    // 설정된 speedDelay 시간이 지났는지 확인 (Non-blocking delay)
    if (millis() - lastAnimTime >= (unsigned long)speedDelay) {
        lastAnimTime = millis();

        // 모든 LED를 끄고 현재 위치의 LED만 켜기
        strip.clear();
        strip.setPixelColor(currentPixel, strip.Color(0, 255, 100)); // 초록색 계열
        strip.show();

        // 다음 픽셀로 이동
        currentPixel++;
        if (currentPixel >= LED_COUNT) {
            currentPixel = 0;
        }
    }
}

void loop() {
    // Add your main code here, to run repeatedly:
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    handleEncoder();    // 엔코더 입력 확인
    handleAnimation();  // 애니메이션 실행
}