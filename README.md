# ĐỒ ÁN MÔN HỌC HỆ NHÚNG (IT4210): Chuột máy tính cân bằng
## GIỚI THIỆU
- **Mô tả đề bài:**
  Dự án này thực hiện việc xây dựng một chuột máy tính không dây sử dụng vi điều khiển ESP32 kết hợp với module cảm biến MPU6050, tận dụng khả năng của thư viện BleMouse HID để mô phỏng chuột Bluetooth. Hệ thống cho phép điều khiển con trỏ chuột trên máy tính bằng cách sử dụng các góc nghiêng của thiết bị, được đo từ cảm biến gia tốc và con quay hồi chuyển theo ba trục: yaw, pitch, và roll.
- **Tính năng chính của sản phẩm:**
  - **Điều khiển chuột bằng cảm biến chuyển động**: Sử dụng module MPU6050 để đo góc nghiêng theo ba trục (yaw, pitch, roll) và điều khiển con trỏ chuột trên máy tính qua kết nối Bluetooth.
    - Di chuyển con trỏ (X, Y):
      - Góc pitch được quy đổi thành tốc độ di chuyển theo trục X.
      - Góc roll được quy đổi thành tốc độ di chuyển theo trục Y.
    - Chức năng cuộn (Scroll): Góc yaw được ánh xạ để thực hiện thao tác cuộn lên hoặc xuống.
    - Ngưỡng chuyển động: Chỉ gửi lệnh di chuyển hoặc cuộn khi góc nghiêng vượt quá ngưỡng xác định (0.5 độ), giúp loại bỏ chuyển động không mong muốn.
    - Tạm dừng khi nằm ngang: Tự động vô hiệu hóa chuyển động chuột khi thiết bị nằm trên mặt phẳng (góc pitch và roll nhỏ hơn 2 độ), tránh thao tác ngoài ý muốn.
- **Bộ lọc EMA (Exponential Moving Average)**: Áp dụng bộ lọc EMA với hệ số alpha (0.3) để làm mượt dữ liệu góc nghiêng, đảm bảo chuyển động chuột ổn định và chính xác.
- **Nút bấm vật lý**:
  - Click trái: Sử dụng nút bấm trên GPIO 4 để thực hiện thao tác click trái.
  - Click phải: Sử dụng nút bấm trên GPIO 5 để thực hiện thao tác click phải.
  - Debouncing: Cơ chế chống nhiễu tín hiệu nút bấm với thời gian debounce 10ms, đảm bảo nhận diện chính xác trạng thái nhấn/nhả.
- **Kết nối Bluetooth HID**: Sử dụng thư viện BleMouse để kết nối không dây với máy tính, mô phỏng chuột HID chuẩn, tương thích với hầu hết các hệ điều hành.
- **Cấu hình cảm biến MPU6050**:
  - Phạm vi gia tốc: ±2g, đảm bảo độ nhạy phù hợp cho các chuyển động nhỏ.
  - Phạm vi con quay hồi chuyển: ±250°/s, hỗ trợ đo góc yaw chính xác.
  - Băng thông lọc: 44Hz, giảm nhiễu tín hiệu từ cảm biến.
- **Tần suất cập nhật cao**: Cập nhật dữ liệu cảm biến và gửi báo cáo chuột mỗi 10ms (~200Hz), đảm bảo phản hồi nhanh và mượt mà.
- **Debug qua Serial**: Hiển thị trạng thái nút bấm, dữ liệu cảm biến và các thông tin kết nối Bluetooth qua cổng Serial (9600 baud) để hỗ trợ phát triển và kiểm tra.
- **Ảnh chụp lắp mạch:**
  ![Mach](https://github.com/user-attachments/assets/cc82df22-2cf1-4ea0-b7fb-6a1c13413e46)
## TÁC GIẢ

- **Tên nhóm:** THD
- **Thành viên trong nhóm:**
  |STT|Họ tên|MSSV|Công việc|
  |--:|--|--|--|
  |1|Hoàng Minh Đức|20215566|Tối ưu hóa code xử lý dữ liệu cảm biến MPU6050 và bộ lọc EMA để đảm bảo chuyển động chuột mượt mà.|
  |2|Trần Đức Thiện|20210815|Cấu hình và debug kết nối Bluetooth HID, đảm bảo tương thích với máy tính và phản hồi nhanh.|
  |3|Phạm Khánh Hưng|20210414|Thiết kế và kiểm tra phần cứng, bao gồm kết nối ESP32, MPU6050 và các nút bấm trên GPIO 4, 5.|
  
## MÔI TRƯỜNG HOẠT ĐỘNG

- **Module CPU/Dev kit:** ESP32
- **Các kit, module phần cứng được sử dụng:**
    - **ESP32 DevKitC:** Bao gồm vi điều khiển ESP32-WROOM-32, tích hợp module Bluetooth và Wi-Fi, các chân GPIO để kết nối ngoại vi.
    - **MPU6050:** Module cảm biến gia tốc và con quay hồi chuyển 6 trục, cung cấp dữ liệu gia tốc (accelerometer) và con quay (gyroscope) để tính toán góc yaw, pitch, và roll.
    - **Nút bấm (Push Buttons):** Hai nút bấm đơn giản dùng cho chức năng click trái và click phải của chuột.
- **Phần mềm và Công cụ phát triển:**
    - **Arduino IDE:** Môi trường phát triển tích hợp (IDE) để viết mã, biên dịch và nạp firmware cho ESP32.
    - **Thư viện Adafruit MPU6050:** Thư viện hỗ trợ giao tiếp với module MPU6050 qua giao thức I2C, cung cấp API để đọc dữ liệu cảm biến.
    - **Thư viện BleMouse:** Thư viện HID Bluetooth để biến ESP32 thành thiết bị chuột Bluetooth, hỗ trợ các chức năng di chuyển, click và scroll.
    - **PlatformIO (tùy chọn):** Có thể được sử dụng thay thế Arduino IDE để quản lý dự án và thư viện.
    - **Trình biên dịch ESP32 Arduino Core:** Cung cấp môi trường lập trình Arduino cho ESP32, tích hợp trong Arduino IDE.

## SƠ ĐỒ KẾT NỐI (SCHEMATIC)

Sơ đồ kết nối chi tiết giữa các chân của ESP32 và các module ngoại vi:

| ESP32 Pin     | Module/Chức năng                              | Ghi chú                                                              |
|---------------|----------------------------------------------|----------------------------------------------------------------------|
| `SCL (GPIO 22)` | MPU6050 - Chân SCL                          | Kết nối với chân SCL của MPU6050 để giao tiếp I2C.                   |
| `SDA (GPIO 21)` | MPU6050 - Chân SDA                          | Kết nối với chân SDA của MPU6050 để giao tiếp I2C.                   |
| `3V3`         | MPU6050 - Chân cấp nguồn (VCC)              | Cấp nguồn hoạt động 3.3V cho module MPU6050.                        |
| `GND`         | MPU6050 - Chân nối đất (GND)                | Kết nối chân GND của MPU6050 với GND chung của hệ thống.             |
| `GPIO 4`      | Nút bấm - Click trái                        | Kết nối với nút bấm, sử dụng pull-up nội bộ, nhấn xuống GND (LOW).   |
| `GPIO 5`      | Nút bấm - Click phải                        | Kết nối với nút bấm, sử dụng pull-up nội bộ, nhấn xuống GND (LOW).   |
| `GND`         | Nút bấm - Chân nối đất (GND)                | Kết nối chân còn lại của cả hai nút bấm với GND chung của hệ thống.  |

## ĐẶC TẢ HÀM
/**
 * @brief Hàm khởi tạo hệ thống.
 * 
 * - Khởi tạo Serial với baudrate 9600 để debug
 * - Thiết lập kết nối I2C
 * - Cấu hình MPU6050:
 *   - Phạm vi gia tốc: ±2G
 *   - Phạm vi gyro: ±250°/s
 *   - Băng thông bộ lọc: 44Hz
 * - Khởi tạo Bluetooth HID Mouse với tên "Balance Mouse"
 * - Cấu hình chân nút bấm (GPIO 4 và 5) với chế độ INPUT_PULLUP
 */

void setup();

/**
 * @brief Hàm chính chạy trong vòng lặp vô hạn.
 * 
 * - Kiểm tra kết nối Bluetooth
 * - Đọc dữ liệu từ cảm biến MPU6050 (gia tốc, gyro)
 * - Tính toán góc nghiêng (pitch, roll) từ dữ liệu gia tốc
 * - Tính giá trị yaw từ dữ liệu gyro
 * - Áp dụng bộ lọc EMA để làm mượt dữ liệu góc
 * - Kiểm tra trạng thái "nằm phẳng" của thiết bị
 * - Ánh xạ góc nghiêng thành chuyển động chuột:
 *   - Roll → Di chuyển trục X
 *   - Pitch → Di chuyển trục Y
 *   - Yaw → Cuộn (scroll)
 * - Xử lý sự kiện nút bấm với cơ chế debounce
 * - Gửi lệnh di chuyển chuột qua Bluetooth
 * - Delay 10ms giữa các lần lặp
 */

void loop();

/**
 * @brief Áp dụng bộ lọc EMA (Exponential Moving Average) cho góc pitch và roll.
 * @param pitch Góc pitch mới (độ)
 * @param roll Góc roll mới (độ)
 * 
 * Công thức lọc:
 * filtered = alpha * new_value + (1 - alpha) * old_value
 * Với alpha = 0.3
 * 
 * Kết quả lọc được lưu vào biến toàn cục:
 * - filteredPitch
 * - filteredRoll
 */

void updateFilter(float pitch, float roll);
## KẾT QUẢ

https://github.com/user-attachments/assets/3bdd9d26-07f6-47c1-a557-f2ecee0b9478
