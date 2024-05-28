#include <cstdio>
#include <cstdint>
#include "image.h"

// Constructor - Khởi tạo một đối tượng Image với dữ liệu và thông tin hình ảnh
Image::Image(unsigned char *data, unsigned char *info,
             uint32_t width, uint32_t height, uint32_t rawSize, uint32_t dataSize) {
    this -> data = data;
    this -> imageInfo = info;
    this -> width = width;
    this -> height = height;
    this -> rawDataSize = rawSize;
    this -> dataSize = dataSize;
}

// Destructor - Giải phóng bộ nhớ
Image::~Image() {
    delete [] data;
    delete [] imageInfo;
}

// Trả về kích thước dữ liệu gốc
unsigned int Image::getRawDataSize() {
    return rawDataSize;
}

// Trả về chiều cao của hình ảnh
uint32_t Image::getHeight() {
    return height;
}

// Trả về chiều rộng của hình ảnh
uint32_t Image::getWidth() {
    return width;
}

// Trả về kích thước dữ liệu
uint32_t Image::getDataSize() {
    return dataSize;
}

// Trả về con trỏ đến dữ liệu
unsigned char *Image::getData() {
    return data;
}

// Trả về con trỏ đến thông tin hình ảnh
unsigned char *Image::getImageInfo() {
    return imageInfo;
}
