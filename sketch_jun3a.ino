#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BleMouse.h>

// Khởi tạo đối tượng
Adafruit_MPU6050 mpu;
BleMouse bleMouse("Balance Mouse", "ESP32", 100);

// Định nghĩa chân cho nút bấm
const int leftButtonPin = 4;  // GPIO 4 cho nút click trái
const int rightButtonPin = 5; // GPIO 5 cho nút click phải

// Trạng thái nút trước đó và thời gian debouncing
int lastLeftButtonState = HIGH;
int lastRightButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 10; // Thời gian debouncing (ms)

// Hệ số ánh xạ và ngưỡng
const float k = 25.0; // Hệ số cơ bản cho chuyển động X, Y
const float m = 0.2; // Hệ số cho scroll
const float threshold = 0.5; // Ngưỡng góc để bỏ qua chuyển động nhỏ
const float flatThreshold = 2.0; // Ngưỡng để xác định "nằm trên mặt phẳng"
const float alpha = 0.3; // Hệ số EMA (0 < alpha < 1)

// Biến lưu góc đã lọc
float filteredPitch = 0.0;
float filteredRoll = 0.0;

void setup() {
  // Khởi tạo Serial để debug
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // Chờ Serial khởi tạo
  }
  
  // Khởi tạo I2C
  Wire.begin();
  
  // Khởi tạo MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050!");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // ±2g
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);      // ±250°/s
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);   // Tăng băng thông
  
  // Khởi tạo Bluetooth HID
  bleMouse.begin();
  Serial.println("Bluetooth Mouse started");

  // Cấu hình chân nút bấm với pull-up nội bộ
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
}

void updateFilter(float pitch, float roll) {
  // Bộ lọc EMA: filtered = alpha * new + (1 - alpha) * old
  filteredPitch = alpha * pitch + (1 - alpha) * filteredPitch;
  filteredRoll = alpha * roll + (1 - alpha) * filteredRoll;
}

void loop() {
  Serial.print("GPIO 4 (D?): "); Serial.println(digitalRead(4));
  Serial.print("GPIO 5 (D?): "); Serial.println(digitalRead(5));
  // Kiểm tra kết nối Bluetooth
  if (bleMouse.isConnected()) {
    // Đọc dữ liệu từ MPU6050
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    // Tính góc pitch và roll
    float pitch = atan2(a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;
    float roll = atan2(a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;
    float yaw = g.gyro.z * 0.01; // Tích phân đơn giản cho yaw
    
    // Áp dụng bộ lọc EMA
    updateFilter(pitch, roll);
    
    // Kiểm tra trạng thái "nằm trên mặt phẳng"
    bool isFlat = (abs(filteredPitch) < flatThreshold && abs(filteredRoll) < flatThreshold);
    
    // Ánh xạ góc thành chuyển động chuột
    int8_t mouse_dx = 0, mouse_dy = 0, scroll = 0;
    if (!isFlat) {
      if (abs(filteredRoll) > threshold) {
        mouse_dx = (int8_t)(k * filteredPitch / 90.0);
      }
      if (abs(filteredPitch) > threshold) {
        mouse_dy = (int8_t)(k * filteredRoll / 90.0);
      }
      if (abs(yaw) > threshold) {
        scroll = (int8_t)(m * yaw / 90.0);
      }
    }
    
    // Đọc trạng thái nút bấm với debouncing
    int leftButtonState = digitalRead(leftButtonPin);
    int rightButtonState = digitalRead(rightButtonPin);
    unsigned long currentTime = millis();
    Serial.println(leftButtonState);
    Serial.println(rightButtonState);
    if (currentTime - lastDebounceTime > debounceDelay) {
      // Xử lý nút click trái
      if (leftButtonState == LOW && lastLeftButtonState == HIGH) {
        bleMouse.press(MOUSE_LEFT);
        Serial.println("Left click pressed");
      }
      if (leftButtonState == HIGH && lastLeftButtonState == LOW) {
        bleMouse.release(MOUSE_LEFT);
        Serial.println("Left click released");
      }

      // Xử lý nút click phải
      if (rightButtonState == LOW && lastRightButtonState == HIGH) {
        bleMouse.press(MOUSE_RIGHT);
        Serial.println("Right click pressed");
      }
      if (rightButtonState == HIGH && lastRightButtonState == LOW) {
        bleMouse.release(MOUSE_RIGHT);
        Serial.println("Right click released");
      }

      lastDebounceTime = currentTime;
    }

    // Cập nhật trạng thái trước đó
    lastLeftButtonState = leftButtonState;
    lastRightButtonState = rightButtonState;
    
    // Giới hạn giá trị trong phạm vi HID (-127 đến 127)
    mouse_dx = constrain(mouse_dx, -127, 127);
    mouse_dy = constrain(mouse_dy, -127, 127);
    scroll = constrain(scroll, -127, 127);
    
    // Gửi báo cáo chuột
    bleMouse.move(mouse_dx, mouse_dy, scroll);
  }
  
  delay(10); // Cập nhật mỗi 10ms (~200Hz)
}