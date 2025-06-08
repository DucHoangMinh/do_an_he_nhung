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


