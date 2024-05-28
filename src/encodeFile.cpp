#include "encodeFile.h"

// Kích thước mỗi phần tử trong weight map, bao gồm 1 byte cho val và 4 byte cho weight
const uint32_t EncodeFile::WeightMapElementSize = 5;  // Mỗi phần tử chứa 1 + 4 = 5 byte (1 byte cho val, 4 byte cho weight)

// Constructor - Khởi tạo EncodeFile với dữ liệu đầu vào
EncodeFile::EncodeFile(unsigned char *imageInfo,
                       unsigned char *weightMapData,
                       unsigned char *data,
                       uint32_t dataBitSize, uint32_t weightMapValCount) {
    this -> imageInfo = imageInfo;
    this -> weightMapData = weightMapData;
    this -> data = data;
    this -> dataBitSize = dataBitSize;
    this -> weightMapValCount = weightMapValCount;
    this -> weightMapArr = vector<pair<unsigned char, uint32_t>>(weightMapValCount);
    generateWeightMapArr();  // Tạo mảng weight map từ dữ liệu weight map
}

// Destructor - Giải phóng bộ nhớ
EncodeFile::~EncodeFile() {
    delete [] imageInfo;
    delete [] weightMapData;
    delete [] data;
}

// Tạo mảng weight map từ dữ liệu weight map
void EncodeFile::generateWeightMapArr() {
    uint32_t valIndex;
    for (valIndex = 0; valIndex < weightMapValCount; valIndex++) {
        weightMapArr[valIndex].first = weightMapData[valIndex * WeightMapElementSize];  // Đặt giá trị
        weightMapArr[valIndex].second =
                *reinterpret_cast<uint32_t *>(weightMapData + (valIndex * WeightMapElementSize + 1));  // Đặt trọng số
    }
}

// Trả về mảng weight map
vector<pair<unsigned char, uint32_t>>& EncodeFile::getWeightMapArr() {
    return weightMapArr;
}

// Trả về kích thước dữ liệu
uint32_t EncodeFile::getDataBitSize() {
    return dataBitSize;
}

// Trả về con trỏ đến dữ liệu
unsigned char *EncodeFile::getData() {
    return data;
}

// Trả về con trỏ đến thông tin hình ảnh
unsigned char *EncodeFile::getImageInfo() {
    return imageInfo;
}
