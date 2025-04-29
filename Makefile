# Trinh bien dich C++
CXX = g++

# Co bien dich
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Co lien ket thu vien OpenSSL
LDFLAGS = -lssl -lcrypto

# Thu muc chua ma nguon
SRC_DIR = src

# Ten file thuc thi
EXECUTABLE = hethongquanly

# Tim tat ca cac file .cpp trong thu muc nguon (src)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Tao ten file object (.o) tuong ung tu file nguon
OBJECTS = $(SOURCES:.cpp=.o)

# Bien dich file thuc thi
all: $(EXECUTABLE)

# Tao file thuc thi tu file object
$(EXECUTABLE): $(OBJECTS)
$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)
@echo "Bien dich hoan tat. Chay chương trinh bang lenh: ./$(EXECUTABLE)"

# Bien dich file nguon (.cpp) thanh file object (.o)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
$(CXX) $(CXXFLAGS) -c $< -o $@

# Don dep cac file da bien dich
clean:
rm -f $(OBJECTS) $(EXECUTABLE)
@echo "Don dep hoan tat."

# Tranh xung dot ten file
.PHONY: all clean
