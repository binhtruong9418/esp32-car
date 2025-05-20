# ESP32 RC Car với Module L298N

Dự án xe điều khiển từ xa (RC Car) sử dụng ESP32 và module điều khiển động cơ L298N, cho phép điều khiển xe thông qua giao diện web trên thiết bị di động hoặc máy tính.

## Tính năng

- Điều khiển xe qua WiFi thông qua giao diện web trực quan
- Hỗ trợ các thao tác: đi tiến, đi lùi, rẽ trái, rẽ phải và dừng lại
- Tương thích với cả thiết bị di động và máy tính
- Hiển thị trạng thái hoạt động hiện tại của xe

## Sơ đồ kết nối

### Kết nối ESP32 với Module L298N

| ESP32 | L298N | Chức năng |
|-------|-------|-----------|
| GPIO 13 | ENA | Điều khiển tốc độ động cơ A (bánh trái) |
| GPIO 12 | IN1 | Điều khiển chiều quay động cơ A |
| GPIO 14 | IN2 | Điều khiển chiều quay động cơ A |
| GPIO 25 | ENB | Điều khiển tốc độ động cơ B (bánh phải) |
| GPIO 27 | IN3 | Điều khiển chiều quay động cơ B |
| GPIO 26 | IN4 | Điều khiển chiều quay động cơ B |
| GND | GND | Nối đất |
| - | +12V | Nguồn ngoài cho động cơ (pin hoặc adapter) |
| 5V/3.3V | +5V | Nếu bạn muốn cấp nguồn cho ESP32 từ module L298N |

### Kết nối Động cơ

- Động cơ A (Bánh bên trái): Kết nối với cổng OUT1 và OUT2 của module L298N
- Động cơ B (Bánh bên phải): Kết nối với cổng OUT3 và OUT4 của module L298N

## Cài đặt và Cấu hình

### Yêu cầu phần cứng

- ESP32 Development Board
- Module L298N
- 2 động cơ DC
- Khung xe (có thể tự chế hoặc mua sẵn)
- Pin hoặc nguồn điện (9-12V cho động cơ)
- Dây jumper để kết nối

### Yêu cầu phần mềm

- Arduino IDE
- Thư viện ESP32 cho Arduino IDE
- Thư viện WiFi cho ESP32
- Thư viện WebServer cho ESP32

### Cấu hình WiFi

Mở file code và tìm các dòng sau để thay đổi thông tin WiFi của bạn:

```cpp
// Thông tin WiFi
const char *ssid = "Đức Bình";     // THAY ĐỔI: Tên WiFi của bạn
const char *password = "12345678"; // THAY ĐỔI: Mật khẩu WiFi của bạn
```

Nếu bạn muốn sử dụng IP tĩnh (cho mạng hotspot điện thoại), hãy kiểm tra và điều chỉnh cấu hình sau:

```cpp
// Cấu hình IP tĩnh - dùng cho mạng hotspot điện thoại
IPAddress staticIP(172, 20, 10, 3); // Địa chỉ IP bạn muốn dùng
IPAddress gateway(172, 20, 10, 1);  // Gateway (thường là IP của điện thoại phát WiFi)
IPAddress subnet(255, 255, 255, 0); // Subnet mask cho mạng hotspot
IPAddress dns(8, 8, 8, 8);          // DNS của Google
```

### Điều chỉnh tốc độ động cơ

Bạn có thể điều chỉnh tốc độ mặc định của động cơ bằng cách thay đổi giá trị SPEED:

```cpp
// Định nghĩa tốc độ động cơ (0-255)
const int SPEED = 200; // Tốc độ mặc định
```

## Cách sử dụng

1. Cài đặt và nạp code vào ESP32 qua Arduino IDE
2. Mở Serial Monitor để xem thông tin kết nối
3. Sau khi ESP32 kết nối WiFi thành công, bạn sẽ thấy địa chỉ IP của nó
4. Mở trình duyệt trên điện thoại hoặc máy tính và truy cập địa chỉ IP đó
5. Sử dụng giao diện web để điều khiển xe:
   - Nút UP: Điều khiển xe đi tiến
   - Nút DOWN: Điều khiển xe đi lùi
   - Nút LEFT: Điều khiển xe rẽ trái
   - Nút RIGHT: Điều khiển xe rẽ phải
   - Nút STOP: Dừng xe

## Điều khiển qua Serial Monitor

Bạn cũng có thể điều khiển xe thông qua Serial Monitor bằng cách gửi các ký tự sau:
- F: Đi tiến
- B: Đi lùi
- L: Rẽ trái
- R: Rẽ phải
- S: Dừng lại

## Gỡ lỗi

Nếu bạn gặp vấn đề với dự án, hãy kiểm tra:

1. Nguồn điện: Đảm bảo pin/nguồn điện đủ mạnh cho động cơ
2. Kết nối: Kiểm tra lại các kết nối giữa ESP32, L298N và động cơ
3. WiFi: Đảm bảo ESP32 kết nối thành công với WiFi
4. Địa chỉ IP: Nếu sử dụng IP tĩnh, đảm bảo cấu hình phù hợp với mạng của bạn

## Cải tiến có thể thực hiện

- Thêm điều khiển tốc độ động cơ qua giao diện web
- Thêm cảm biến khoảng cách để tránh va chạm
- Tích hợp camera để xem hình ảnh trực tiếp
- Thêm chế độ tự lái hoặc điều khiển theo lộ trình định sẵn

## Giấy phép

[Thêm thông tin giấy phép dự án của bạn]

## Tác giả

[Thêm thông tin tác giả]
