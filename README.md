# Hệ Thống Quản Lý Người Dùng và Ví Điểm Thưởng

## Giới thiệu thành viên tham gia:

| **Họ Tên** | **Mã sinh viên** | **Vai trò** | **Công việc được giao** |
| --- | --- | --- | --- |
| Nguyễn Năng Huy | K24DTCN201 | Lập trình viên | Thiết kế cấu trúc hệ thống, lập trình các chức năng: Menu, đăng nhập, đăng ký, quản lý người dùng (admin) ,tích hợp OTP, |
| Bùi Thị Minh Hải | K24DTCN189 | Lập trình viên | Hợp nhất mã nguồn, Giới thiệu và bảo vệ sản phẩm, Testing & Quality Assurance |
| Lê Minh C | K24DTCNxxx | Lập trình viên |  |
| Nguyễn Văn C | K24DTCNxxx | Lập trình viên |  |

Đây là một chương trình C++ đơn giản mô phỏng hệ thống quản lý người dùng và ví điểm thưởng, tích hợp các tính năng bảo mật cơ bản như băm mật khẩu (SHA-256) và xác thực hai yếu tố (TOTP) sử dụng thư viện OpenSSL và công cụ OATH Toolkit.

## Cấu trúc chương trình

```
+------------------------+
|       main.cpp         |
| - main()               |
| - Menu chính           |
|   - Đăng nhập          |
|   - Đăng ký            |
|   - Thoát              |
| - Menu người dùng      |
|   - Xem thông tin      |
|   - Cập nhật thông tin |
|   - Đổi mật khẩu       |
|   - Quản lý ví         |
|   - Chức năng admin    |
+------------------------+
          |
          | Sử dụng
          v
+---------------------+       +---------------------+
|      user.h         |<----->|      wallet.h       |
| - Lớp User          |       | - Lớp Wallet        |
|   - username        |       |   - walletID        |
|   - password        |       |   - balance         |
|   - fullName        |       |   - transactionLog  |
|   - email, phone    |       | - Hàm ví            |
|   - role, walletID  |       |   - save/load       |
|   - oathSecretKey   |       |   - transferPoints  |
| - Hàm người dùng    |       |   - viewWallet      |
|   - save/load       |       |   - depositPoints   |
|   - registerUser    |       |                     |
|   - login           |       |                     |
|   - changePassword  |       |                     |
|   - updateProfile   |       |                     |
|   - viewUsers       |       |                     |
|   - adminEditUser   |       |                     |
+---------------------+       +---------------------+
          |                           |
          | Triển khai                | Triển khai
          v                           v
+------------------------+       +-----------------------+
|      user.cpp          |       |      wallet.cpp       |
| - Triển khai User      |       | - Triển khai Wallet   |
|   functions            |       |   functions           |
| - Xử lý:               |       | - Xử lý:              |
|   - Đọc/ghi tệp        |       |   - Đọc/ghi tệp       |
|   - Băm mật khẩu       |       |   - Thao tác ví       |
|   - Sinh OTP           |       |   - Nhật ký giao dịch |
|   - Quản lý người dùng |       |                       |
+------------------------+       +-----------------------+
          |                           |
          | Đọc/Ghi                   | Đọc/Ghi
          v                           v
+--------------------------+       +---------------------+
|   data/users.txt         |       |  data/wallets.txt   |
| - Lưu dữ liệu người dùng |       | - Lưu dữ liệu ví    |
| - Định dạng: CSV         |       | - Định dạng: CSV    |
| - Sao lưu: .bak          |       | - Sao lưu: .bak     |
+--------------------------+       +---------------------+

```

## Phân tích và đặc tả chức năng

### Mục tiêu hệ thống
Hệ thống quản lý điểm thưởng được thiết kế để quản lý tài khoản người dùng và ví điểm thưởng, hỗ trợ các hoạt động như đăng ký, đăng nhập, chuyển điểm, và quản lý thông tin người dùng với bảo mật cao thông qua OTP và băm mật khẩu.

#### Mô tả chức năng
* Hệ thống bao gồm các chức năng chính sau:
  - Đăng ký tài khoản:
    + Mô tả: Cho phép người dùng hoặc quản trị viên tạo tài khoản mới.
    + Đầu vào: Tên đăng nhập, mật khẩu (tự nhập hoặc tự động sinh), họ tên, email, số điện thoại.
    + Đầu ra: Tài khoản được tạo với ví điểm thưởng liên kết, khóa bí mật OTP được cung cấp.
    + Yêu cầu bảo mật: Mật khẩu được băm bằng SHA-256, hỗ trợ xác thực hai yếu tố (2FA) qua OTP.
  - Đăng nhập:
    + Mô tả: Xác thực người dùng để truy cập hệ thống.
    + Đầu vào: Tên đăng nhập, mật khẩu, mã OTP.
    + Đầu ra: Truy cập thành công hoặc thông báo lỗi.
    + Yêu cầu bảo mật: Kiểm tra mật khẩu băm và mã OTP hợp lệ.
  - Quản lý thông tin cá nhân:
    + Mô tả: Cho phép người dùng xem và cập nhật thông tin cá nhân (họ tên, email, số điện thoại).
    + Đầu vào: Thông tin mới, mã OTP xác nhận.
    + Đầu ra: Thông tin được cập nhật hoặc thông báo lỗi.
  - Đổi mật khẩu:
    + Mô tả: Cho phép người dùng thay đổi mật khẩu.
    + Đầu vào: Mật khẩu hiện tại, mật khẩu mới, mã OTP xác nhận.
    + Đầu ra: Mật khẩu được cập nhật hoặc thông báo lỗi.
  - Quản lý ví điểm thưởng:
    + Mô tả: Cho phép xem số dư, lịch sử giao dịch, và chuyển điểm giữa các ví.
    + Đầu vào: Số điểm cần chuyển, ID ví đích, mã OTP xác nhận.
    + Đầu ra: Giao dịch thành công hoặc thông báo lỗi.
  - Quản lý người dùng (dành cho admin):
    + Mô tả: Admin có thể xem danh sách người dùng, tạo tài khoản mới, và chỉnh sửa thông tin người dùng.
    + Đầu vào: Tên đăng nhập, thông tin cần chỉnh sửa.
    + Đầu ra: Danh sách người dùng hoặc thông tin được cập nhật.

#### Phân tích yêu cầu
* Yêu cầu chức năng:
  - Hỗ trợ xác thực hai yếu tố (2FA) qua OTP.
  - Lưu trữ dữ liệu người dùng và ví an toàn trong tệp users.txt và wallets.txt.
  - Giao diện dòng lệnh thân thiện, dễ sử dụng.

#### Yêu cầu Hệ thống

* Để biên dịch và chạy chương trình này cần:
  - Trình biên dịch C++: Hỗ trợ C++11 trở lên (ví dụ: g++ hoặc clang++).
  - Thư viện OpenSSL: Bao gồm cả header files (thường là gói "libssl-dev" hoặc "openssl-devel"). Cần thiết cho việc băm mật khẩu.
  - Công cụ OATH Toolkit: Cần được cài đặt và có sẵn trong PATH của hệ thống. Cần thiết cho việc xác thực mã OTP 2FA.

## Hướng dẫn Cài đặt và chạy

### 1. Clone Repository
```shell
git clone https://github.com/mokaa1102/hethongquanly.git
cd hethongquanly
```
### 2. Cài đặt Dependencies

- Trên Ubuntu/Debian:
```shell
sudo apt update
sudo apt install build-essential libssl-dev oathtool
```
- Trên macOS (sử dụng Homebrew):
```shell
brew update
brew install openssl oathtool pkg-config
```
- Trên Windows:
Sử dụng Windows Subsystem for Linux (WSL) và làm theo hướng dẫn cho Ubuntu/Debian.

### 3. Biên dịch chương trình

Sử dụng "Makefile" được cung cấp để biên dịch:
```shell
make
```
Câu lệnh này sẽ tạo ra một file thực thi có tên "hethongquanly" trong thư mục gốc của chương trình.
```shell
g++ src/main.cpp -o hethongquanly -std=c++11 -lssl -lcrypto
```
### 4. Chạy Chương trình
```shell
./hethongquanly
```
- Lần chạy đầu tiên: Chương trình sẽ kiểm tra và nếu không có tài khoản "admin", nó sẽ tự động tạo một tài khoản "admin" với mật khẩu mặc định (admin123) và hiển thị OATH Secret Key.
  Ghi lại OATH Secret Key này và thêm vào ứng dụng xác thực của bạn (One Time Password, Google Authenticator, Microsoft Authenticator,...). Bạn sẽ cần mã OTP từ ứng dụng này để đăng nhập.
- Đăng nhập lần đầu: Đăng nhập với "admin" và mật khẩu "admin123". Nhập mã OTP từ ứng dụng xác thực. Hệ thống sẽ yêu cầu bạn đổi mật khẩu ngay lập tức.
- Lưu trữ dữ liệu: Các file "users.txt" và "wallets.txt" (cùng các file ".bak") sẽ được tạo/cập nhật trong cùng thư mục bạn chạy file "hethongquanly".

## Cấu trúc chương trình
```
hethongquanly/
│
├── include/              # Thư mục chứa các file Header (.h)
│   │
│   ├── user.h            # Khai báo lớp User và các hàm liên quan đến User
│   │
│   └── wallet.h          # Khai báo lớp Wallet và các hàm liên quan đến Ví (wallet)
│
├── src/                  # Thư mục chứa các file mã nguồn (.cpp)
│   │
│   ├── main.cpp          # Chứa hàm main(), vòng lặp menu chính/phụ, điều phối ứng dụng
│   │
│   ├── user.cpp          # Chứa định nghĩa/implementation các hàm đã khai báo trong user.h
│   │
│   └── wallet.cpp        # Chứa định nghĩa/implementation các hàm đã khai báo trong wallet.h
│
├── data/                 # Thư mục chứa file dữ liệu (Cần tạo thư mục này thủ công. File users.txt, wallets.txt, .bak sẽ được tạo/đọc ở đây)
│
├── .gitignore            # File liệt kê các mục git sẽ bỏ qua
│
├── README.md             # File giới thiệu, mô tả, hướng dẫn cài đặt và sử dụng
│
└── Makefile              # File chứa các quy tắc để biên dịch (lệnh 'make')
```

- OTP: Việc sử dụng "generateSimulatedOTP()" cho các chức năng như đổi mật khẩu, cập nhật hồ sơ, chuyển tiền. Nó chỉ in OTP ra màn hình thay vì gửi qua SMS, Email hoặc yêu cầu OTP từ ứng dụng xác thực như lúc đăng nhập.
- Lưu trữ OATH Key: Khóa bí mật OATH được lưu trữ dưới dạng văn bản gốc trong "users.txt".
- Lưu trữ Dữ liệu: Sử dụng file text để dữ liệu.
- Randomness: "srand(time(0) + rand())" nguồn tạo số ngẫu nhiên cho mục đích mật mã.

## Các thao tác thực hiện
* Menu chính:
1. Đăng nhập: Nhập tên đăng nhập, mật khẩu, và mã OTP.
2. Đăng ký: Nhập thông tin để tạo tài khoản mới (tên đăng nhập, mật khẩu, họ tên, email, số điện thoại).
3. Thoát: Kết thúc chương trình.

* Menu người dùng (sau khi đăng nhập):
1. Xem thông tin cá nhân: Hiển thị thông tin tài khoản.
2. Cập nhật thông tin cá nhân: Cập nhật họ tên, email, số điện thoại (yêu cầu OTP).
3. Đổi mật khẩu: Đổi mật khẩu mới (yêu cầu mật khẩu hiện tại và OTP).
4. Quản lý ví điểm thưởng:
   - Xem thông tin ví & lịch sử giao dịch: Hiển thị số dư và lịch sử.
   - Chuyển điểm: Nhập ID ví đích, số điểm, và OTP để chuyển.
   - Quay lại menu trước.
5. Đăng xuất.

* Menu admin:
1. Xem danh sách người dùng: Hiển thị danh sách tất cả người dùng.
2. Tạo tài khoản mới: Tạo tài khoản cho người dùng khác (mật khẩu tự động sinh).
3. Chỉnh sửa tài khoản người dùng: Cập nhật thông tin người dùng (họ tên, email, số điện thoại, vai trò).
4. Đăng xuất.

## Tài liệu tham khảo

- Bài tập có sử dụng các tài liệu và nguồn tham khảo sau:
  + OpenSSL Documentation: https://www.openssl.org/docs/
    Được sử dụng để triển khai băm mật khẩu bằng SHA-256.
  + C++ Standard Library Reference: https://en.cppreference.com/w/
    Tham khảo cho các thư viện chuẩn như ```<vector>, <string>, <fstream>,...```
  + OATH Toolkit (oathtool): https://www.nongnu.org/oath-toolkit/
    Hướng dẫn cài đặt và sử dụng oathtool cho OTP.
