#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Định nghĩa các chân kết nối với L298N
// Động cơ A - Bánh bên trái
const int ENA = 13; // Chân điều khiển tốc độ động cơ A
const int IN1 = 12; // Chân điều khiển chiều quay động cơ A
const int IN2 = 14; // Chân điều khiển chiều quay động cơ A

// Động cơ B - Bánh bên phải
const int ENB = 25; // Chân điều khiển tốc độ động cơ B
const int IN3 = 27; // Chân điều khiển chiều quay động cơ B
const int IN4 = 26; // Chân điều khiển chiều quay động cơ B

// Định nghĩa tốc độ động cơ (0-255)
const int SPEED = 200; // Tốc độ mặc định

// Biến lưu lệnh điều khiển từ Serial
char command;

// Thông tin WiFi
const char *ssid = "Đức Bình";     // THAY ĐỔI: Tên WiFi của bạn
const char *password = "12345678"; // THAY ĐỔI: Mật khẩu WiFi của bạn

// Cấu hình IP tĩnh - dùng cho mạng hotspot điện thoại
IPAddress staticIP(172, 20, 10, 3); // Địa chỉ IP bạn muốn dùng
IPAddress gateway(172, 20, 10, 1);  // Gateway (thường là IP của điện thoại phát WiFi)
IPAddress subnet(255, 255, 255, 0); // Subnet mask cho mạng hotspot
IPAddress dns(8, 8, 8, 8);          // DNS của Google

// Cổng Web Server
WebServer server(80);

// Điều khiển xe đi tiến
void moveForward()
{
  // Điều khiển động cơ trái
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, SPEED);

  // Điều khiển động cơ phải
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, SPEED);
}

// Điều khiển xe đi lùi
void moveBackward()
{
  // Điều khiển động cơ trái
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, SPEED);

  // Điều khiển động cơ phải
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, SPEED);
}

// Điều khiển xe rẽ trái
void turnLeft()
{
  // Động cơ trái dừng hoặc quay chậm
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  // Động cơ phải quay tiến
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, SPEED);
}

// Điều khiển xe rẽ phải
void turnRight()
{
  // Động cơ trái quay tiến
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, SPEED);

  // Động cơ phải dừng hoặc quay chậm
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

// Dừng cả hai động cơ
void stopMotors()
{
  // Dừng động cơ trái
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  // Dừng động cơ phải
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

// Hàm thực thi lệnh điều khiển
void executeCommand(char cmd)
{
  switch (cmd)
  {
  case 'F': // Đi tiến
    Serial.println("Đi tiến");
    moveForward();
    break;
  case 'B': // Đi lùi
    Serial.println("Đi lùi");
    moveBackward();
    break;
  case 'L': // Rẽ trái
    Serial.println("Rẽ trái");
    turnLeft();
    break;
  case 'R': // Rẽ phải
    Serial.println("Rẽ phải");
    turnRight();
    break;
  case 'S': // Dừng lại
    Serial.println("Dừng lại");
    stopMotors();
    break;
  default:
    break;
  }
}

// Tạo HTML cho trang web điều khiển
String HTML_PAGE = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 RC Car Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin: 0;
            padding: 20px;
            background-color: #f0f0f0;
        }
        .control-panel {
            max-width: 500px;
            margin: 0 auto;
            background-color: #ffffff;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }
        h1 {
            color: #333;
        }
        .button-container {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            grid-gap: 10px;
            margin: 20px 0;
        }
        .control-btn {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 15px 0;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 5px;
            width: 100%;
        }
        .stop-btn {
            background-color: #f44336;
            grid-column: 2;
        }
        .forward-btn {
            grid-column: 2;
        }
        .backward-btn {
            grid-column: 2;
        }
        .left-btn {
            grid-column: 1;
        }
        .right-btn {
            grid-column: 3;
        }
        .status {
            margin-top: 20px;
            padding: 10px;
            background-color: #f1f1f1;
            border-radius: 5px;
        }
    </style>
</head>
<body>
    <div class="control-panel">
        <h1>ESP32 RC Car Control</h1>
        <div class="button-container">
            <div></div>
            <button class="control-btn forward-btn" ontouchstart="sendCommand('F')" ontouchend="sendCommand('S')" onmousedown="sendCommand('F')" onmouseup="sendCommand('S')">UP</button>
            <div></div>
            
            <button class="control-btn left-btn" ontouchstart="sendCommand('L')" ontouchend="sendCommand('S')" onmousedown="sendCommand('L')" onmouseup="sendCommand('S')">LEFT</button>
            <button class="control-btn stop-btn" onclick="sendCommand('S')">STOP</button>
            <button class="control-btn right-btn" ontouchstart="sendCommand('R')" ontouchend="sendCommand('S')" onmousedown="sendCommand('R')" onmouseup="sendCommand('S')">RIGHT</button>
            
            <div></div>
            <button class="control-btn backward-btn" ontouchstart="sendCommand('B')" ontouchend="sendCommand('S')" onmousedown="sendCommand('B')" onmouseup="sendCommand('S')">DOWN</button>
            <div></div>
        </div>
        <div class="status" id="status">Status: Waiting</div>
    </div>

    <script>
        function sendCommand(command) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?cmd=" + command, true);
            xhr.send();
            
            // Cập nhật trạng thái
            var statusText = "Status: ";
            switch(command) {
                case 'F': statusText += "Going UP"; break;
                case 'B': statusText += "Going DOWN"; break;
                case 'L': statusText += "Going LEFT"; break;
                case 'R': statusText += "Going RIGHT"; break;
                case 'S': statusText += "STOPPED"; break;
                default: statusText += "Waiting"; break;
            }
            document.getElementById("status").innerHTML = statusText;
        }
    </script>
</body>
</html>
)=====";

// Xử lý khi có request tới trang chủ
void handleRoot()
{
  server.send(200, "text/html", HTML_PAGE);
}

// Xử lý khi có request tới đường dẫn /action
void handleAction()
{
  String cmd = server.arg("cmd");
  if (cmd.length() > 0)
  {
    executeCommand(cmd.charAt(0));
    server.send(200, "text/plain", "OK");
  }
  else
  {
    server.send(400, "text/plain", "Bad Request");
  }
}

// Xử lý khi không tìm thấy đường dẫn
void handleNotFound()
{
  server.send(404, "text/plain", "Not Found");
}

void setup()
{
  // Khởi tạo cổng Serial để nhận lệnh điều khiển
  Serial.begin(9600);

  // Cấu hình các chân điều khiển động cơ là OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ban đầu, dừng tất cả động cơ
  stopMotors();

  Serial.println("ESP32 RC Car với L298N");

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối WiFi");

  // Đợi kết nối WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Đã kết nối WiFi với IP: ");
  Serial.println(WiFi.localIP());

  // Cấu hình các đường dẫn cho web server
  server.on("/", handleRoot);
  server.on("/action", handleAction);
  server.onNotFound(handleNotFound);

  // Khởi động web server
  server.begin();
  Serial.println("Web Server đã khởi động");
  Serial.println("Mở trình duyệt và truy cập địa chỉ IP ở trên để điều khiển xe");

  Serial.println("Bạn vẫn có thể sử dụng các phím sau trên Serial để điều khiển:");
  Serial.println("F: Đi tiến");
  Serial.println("B: Đi lùi");
  Serial.println("L: Rẽ trái");
  Serial.println("R: Rẽ phải");
  Serial.println("S: Dừng lại");
}

void loop()
{
  // Xử lý các request từ web server
  server.handleClient();

  // Kiểm tra nếu có dữ liệu từ Serial
  if (Serial.available() > 0)
  {
    command = Serial.read();
    executeCommand(command);
  }

  // Tạm dừng một chút để tránh xử lý quá nhanh
  delay(10);
}
