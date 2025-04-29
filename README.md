# Hệ Thống Quản Lý Người Dùng và Ví Điểm Thưởng

Đây là một chương trình C++ đơn giản mô phỏng hệ thống quản lý người dùng và ví điểm thưởng, tích hợp các tính năng bảo mật cơ bản như băm mật khẩu (SHA-256) và xác thực hai yếu tố (TOTP) sử dụng thư viện OpenSSL và công cụ "oathtool".

## Tính năng

- Quản lý Người dùng:
    * Đăng ký tài khoản mới (người dùng tự đăng ký hoặc admin tạo).
    * Đăng nhập an toàn với mật khẩu đã được băm và mã OTP (TOTP 2FA).
    * Yêu cầu đổi mật khẩu lần đầu nếu mật khẩu được tạo tự động bởi admin.
    * Thay đổi mật khẩu (yêu cầu mật khẩu hiện tại và OTP).
    * Xem thông tin cá nhân.
    * Cập nhật thông tin cá nhân (Họ tên, Email, SĐT - yêu cầu OTP).
- Quản lý Ví điểm:
    * Mỗi người dùng có một ví điểm riêng.
    * Xem số dư và lịch sử giao dịch của ví.
    * Chuyển điểm giữa các ví (yêu cầu OTP để xác nhận).
- Chức năng Admin:
    * Xem danh sách tất cả người dùng.
    * Tạo tài khoản mới cho người dùng (với mật khẩu tự động sinh).
    * Chỉnh sửa thông tin người dùng (Họ tên, Email, SĐT, Vai trò).
    * Tự động tạo tài khoản "admin" mặc định nếu chưa tồn tại khi khởi chạy lần đầu.
- Bảo mật:
    * Mật khẩu được băm bằng SHA-256 trước khi lưu trữ.
    * Xác thực hai yếu tố (2FA) dựa trên TOTP khi đăng nhập, sử dụng khóa bí mật OATH và công cụ "oathtool".
    * Sử dụng OTP cho các hành động như đổi mật khẩu, cập nhật hồ sơ, chuyển điểm.
- Lưu trữ Dữ liệu:
    * Thông tin người dùng được lưu vào file "users.txt".
    * Thông tin ví và lịch sử giao dịch được lưu vào file "wallets.txt".
    * Tự động tạo file backup (.bak) mỗi khi lưu dữ liệu.
    * Tự động đọc từ file backup nếu file chính bị lỗi hoặc không tồn tại.

## Yêu cầu Hệ thống

Để biên dịch và chạy chương trình này cần:

1.  Trình biên dịch C++: Hỗ trợ C++11 trở lên (ví dụ: g++ hoặc clang++).
2.  Thư viện OpenSSL: Bao gồm cả header files (thường là gói "libssl-dev" hoặc "openssl-devel"). Cần thiết cho việc băm mật khẩu.
3.  Công cụ oathtool: Cần được cài đặt và có sẵn trong PATH của hệ thống. Cần thiết cho việc xác thực mã OTP 2FA.

## Hướng dẫn Cài đặt và Chạy

### 1. Clone Repository

git clone [https://github.com/ten-cua-ban/he-thong-quan-ly.git](https://github.com/ten-cua-ban/he-thong-quan-ly.git)
cd he-thong-quan-ly

### 2. Cài đặt Dependencies

- Trên Ubuntu/Debian:
sudo apt update
sudo apt install build-essential libssl-dev oathtool

- Trên macOS (sử dụng Homebrew):
brew update
brew install openssl oathtool pkg-config

- Trên Windows:
Sử dụng Windows Subsystem for Linux (WSL) và làm theo hướng dẫn cho Ubuntu/Debian.

### 3. Biên dịch chương trình

Sử dụng "Makefile" được cung cấp để biên dịch:

make

Câu lệnh này sẽ tạo ra một file thực thi có tên "hethongquanly" trong thư mục gốc của chương trình.

g++ src/main.cpp -o hethongquanly -std=c++11 -lssl -lcrypto

### 4. Chạy Chương trình

./hethongquanly

- Lần chạy đầu tiên: Chương trình sẽ kiểm tra và nếu không có tài khoản "admin", nó sẽ tự động tạo một tài khoản "admin" với mật khẩu mặc định (admin123) và hiển thị OATH Secret Key.
  Ghi lại OATH Secret Key này và thêm vào ứng dụng xác thực của bạn (Google Authenticator, Authy,...). Bạn sẽ cần mã OTP từ ứng dụng này để đăng nhập.
- Đăng nhập lần đầu: Đăng nhập với "admin" và mật khẩu "admin123". Nhập mã OTP từ ứng dụng xác thực. Hệ thống sẽ yêu cầu bạn đổi mật khẩu ngay lập tức.
- Lưu trữ dữ liệu: Các file "users.txt" và "wallets.txt" (cùng các file ".bak") sẽ được tạo/cập nhật trong cùng thư mục bạn chạy file "hethongquanly".

## Cấu trúc chương trình

|-- src/
|   |-- main.cpp      # Mã nguồn C++ (main.cpp)
|
|-- data
|
|-- .gitignore        # Các file/thư mục bị Git bỏ qua
|
|-- README.md         # File này
|
|-- Makefile          # Hướng dẫn biên dịch tự động

## Lưu ý về Bảo mật

- OTP Mô phỏng: Việc sử dụng "generateSimulatedOTP()" cho các chức năng như đổi mật khẩu, cập nhật hồ sơ, chuyển tiền. Nó chỉ in OTP ra màn hình thay vì gửi qua SMS, Email hoặc yêu cầu OTP từ ứng dụng xác thực như lúc đăng nhập.
- Lưu trữ OATH Key: Khóa bí mật OATH được lưu trữ dưới dạng văn bản gốc trong "users.txt".
- Lưu trữ Dữ liệu: Sử dụng file text để dữ liệu.
- Randomness: "srand(time(0) + rand())" nguồn tạo số ngẫu nhiên cho mục đích mật mã.
