#include <iostream>
#include <cstring>
#include "huffmanCompression.h"
using namespace std;

// Constructor - Khởi tạo treeRoot thành nullptr
HuffmanCompression::HuffmanCompression() {
    treeRoot = nullptr;
}

// Destructor - Giải phóng bộ nhớ cho Huffman Tree
HuffmanCompression::~HuffmanCompression() {
    if (treeRoot == nullptr)
        return;
    queue<TreeNode*> q;
    q.push(treeRoot);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        if (node->left != nullptr)
            q.push(node->left);
        if (node->right != nullptr)
            q.push(node->right);
        delete(node);
    }
    treeRoot = nullptr;
}

// Tính toán trọng số (tần số) của mỗi ký tự trong dữ liệu
void HuffmanCompression::calcWeight(const unsigned char* dataPtr, uint32_t dataSize,
                                    unordered_map<unsigned char, uint32_t >& weightMap) {
    for (uint32_t i = 0; i < dataSize; i++) {
        if (weightMap.find(dataPtr[i]) == weightMap.end()) {
            weightMap[dataPtr[i]] = 1;
        } else {
            weightMap[dataPtr[i]]++;
        }
    }
}

// Xây dựng cây Huffman từ map trọng số
void HuffmanCompression::buildTree() {
    while (nodeQueue.size() > 1) {
        TreeNode* rightNode = nodeQueue.top();
        nodeQueue.pop();
        TreeNode* leftNode = nodeQueue.top();
        nodeQueue.pop();
        auto parentNode = new TreeNode(0, rightNode->weight + leftNode->weight);
        parentNode->left = leftNode;
        parentNode->right = rightNode;
        nodeQueue.push(parentNode);
    }
    treeRoot = nodeQueue.top();
}

// Thực hiện DFS trên cây để tạo mã Huffman
void HuffmanCompression::dfs(TreeNode* node, hfmCodeBitSet& path,
                             unordered_map<unsigned char, hfmCodeBitSet> &resCodeMap) {
    if (node->left == nullptr && node->right == nullptr) {
        resCodeMap[node->val] = path;
        return;
    }
    path.append(0);
    dfs(node->left, path, resCodeMap);
    path.pop_back();
    path.append(1);
    dfs(node->right, path, resCodeMap);
    path.pop_back();
}

// Lấy mã Huffman cho mỗi ký tự
void HuffmanCompression::getCodeMap(unordered_map<unsigned char, hfmCodeBitSet> &resCodeMap) {
    if (treeRoot == nullptr)
        return;
    hfmCodeBitSet path;
    dfs(treeRoot, path, resCodeMap);
}

// Mã hóa dữ liệu đầu vào bằng các mã Huffman
void HuffmanCompression::generateEncodedOutput(const unsigned char *rawDataPtr, uint32_t rawDataSize,
                                               const unordered_map<unsigned char, hfmCodeBitSet> &resCodeMap,
                                               unsigned char* &outputDataPtr, uint32_t outputDataBitSize) {
    outputDataPtr = new unsigned char[outputDataBitSize / 8 + 1];
    memset(outputDataPtr, 0, outputDataBitSize / 8 + 1);
    uint32_t outputBitOffset = 0;
    uint32_t codeBitLength = 0;
    uint32_t byteOff, bitOffInByte;
    uint32_t i, j;

    // Tạo dữ liệu đầu ra được mã hóa bằng cách tra cứu bản đồ mã Huffman
    for (i = 0; i < rawDataSize; i++) {
        const hfmCodeBitSet& code = resCodeMap.at(rawDataPtr[i]);
        codeBitLength = code.length();
        for (j = 0; j < codeBitLength; j++) {
            byteOff = (j + outputBitOffset) / 8;
            bitOffInByte = (j + outputBitOffset) % 8;
            outputDataPtr[byteOff] |= (code[j] << bitOffInByte);
        }
        outputBitOffset += codeBitLength;
    }
    if (outputBitOffset != outputDataBitSize) {
        throw runtime_error("HuffmanCompression::generateEncodedOutput error: bitCount not match");
    }
}

// Giải mã dữ liệu đã mã hóa bằng Cây Huffman
void HuffmanCompression::generateDecodedOutput(const unsigned char *rawDataPtr, uint32_t rawDataBitSize,
                                               unsigned char *&outputDataPtr, uint32_t outputDataSize) {
    outputDataPtr = new unsigned char[outputDataSize + 1];
    memset(outputDataPtr, 0, outputDataSize + 1);
    uint32_t bitOffInByte = 0;
    uint32_t byteOff = 0;
    uint32_t rawDataBitOff;
    uint32_t outputDataOff = 0;
    TreeNode *curNode = treeRoot;

    // Tạo dữ liệu đầu ra đã giải mã bằng cách duyệt Cây Huffman
    for (rawDataBitOff = 0; rawDataBitOff < rawDataBitSize; rawDataBitOff++) {
        if (curNode->left == nullptr && curNode->right == nullptr) {
            outputDataPtr[outputDataOff++] = curNode->val;
            curNode = treeRoot;
        }
        byteOff = rawDataBitOff / 8;
        bitOffInByte = rawDataBitOff % 8;
        if ((rawDataPtr[byteOff] >> bitOffInByte) & 1) {
            if (curNode == nullptr || curNode->right == nullptr) {
                throw runtime_error("HuffmanCompression::generateDecodedOutput error: bit not match");
            } else {
                curNode = curNode->right;
            }
        } else {
            if (curNode == nullptr || curNode->left == nullptr) {
                throw runtime_error("HuffmanCompression::generateDecodedOutput error: bit not match");
            } else {
                curNode = curNode->left;
            }
        }
    }
    if (curNode->left == nullptr && curNode->right == nullptr) {
        outputDataPtr[outputDataOff++] = curNode->val;
        curNode = treeRoot;
    }
    if (outputDataOff != outputDataSize) {
        throw runtime_error("HuffmanCompression::generateDecodedOutput error: outputData overflow");
    }
}

// Mã hóa dữ liệu đầu vào thành dữ liệu mã Huffman
void HuffmanCompression::getEncodedData(const unsigned char *rawDataPtr, uint32_t rawDataSize,
                                        unordered_map<unsigned char, uint32_t > &dstWeightMap,
                                        unsigned char *&outputDataPtr, uint32_t &outputDataBitSize) {
    calcWeight(rawDataPtr, rawDataSize, dstWeightMap);  // Tính toán trọng số
    generateEncodedNodeQueue(dstWeightMap);  // Tạo hàng đợi nút từ trọng số

    buildTree();  // Xây dựng Cây Huffman
    unordered_map<unsigned char, hfmCodeBitSet> resCodeMap;
    getCodeMap(resCodeMap);  // Lấy các mã Huffman
    outputDataBitSize = calcEncodedOutputSize(dstWeightMap, resCodeMap);  // Tính kích thước đầu ra đã mã hóa
    generateEncodedOutput(rawDataPtr, rawDataSize, resCodeMap, outputDataPtr, outputDataBitSize);  // Tạo đầu ra đã mã hóa
}

// Giải mã dữ liệu đã mã hóa Huffman thành dữ liệu gốc
void HuffmanCompression::getDecodedData(const unsigned char *rawDataPtr, uint32_t rawDataBitSize,
                                        const vector<pair<unsigned char, uint32_t >> &srcWeightMapArr,
                                        unsigned char *&outputDataPtr, uint32_t &outputDataSize) {
    generateDecodedNodeQueue(srcWeightMapArr);  // Tạo hàng đợi nút từ mảng trọng số
    buildTree();  // Xây dựng Cây Huffman
    outputDataSize = calcDecodedOutputSize(srcWeightMapArr);  // Tính kích thước đầu ra giải mã
    generateDecodedOutput(rawDataPtr, rawDataBitSize, outputDataPtr, outputDataSize);  // Tạo đầu ra giải mã
}

// Tính toán tổng kích thước bit của dữ liệu đã mã hóa
uint32_t HuffmanCompression::calcEncodedOutputSize(const unordered_map<unsigned char, uint32_t > &weightMap,
                                                   const unordered_map<unsigned char, hfmCodeBitSet> &resCodeMap) {
    uint32_t bitsCount = 0;
    unsigned char val;
    uint32_t weight, codeLength;

    // Tính toán tổng số bit yêu cầu để mã hóa
    for (const auto &ele : weightMap) {
        val = ele.first;
        weight = ele.second;
        codeLength = static_cast<uint32_t >(resCodeMap.at(val).length());
        bitsCount += weight * codeLength;
    }
    return bitsCount;
}

// Tính toán kích thước đầu ra giải mã từ mảng trọng số
uint32_t HuffmanCompression::calcDecodedOutputSize(const vector<pair<unsigned char, uint32_t >> &srcWeightMapArr) {
    uint32_t bytesCount = 0;
    for (const auto &ele : srcWeightMapArr) {
        bytesCount += ele.second;
    }
    return bytesCount;
}

// Tạo hàng đợi nút từ map trọng số (dùng cho mã hóa)
void HuffmanCompression::generateEncodedNodeQueue(const unordered_map<unsigned char, uint32_t > &weightMap) {
    for (const auto &ele : weightMap) {
        auto newNode = new TreeNode(ele.first, ele.second);
        nodeQueue.push(newNode);
    }
}

// Tạo hàng đợi nút từ mảng trọng số (dùng cho giải mã)
void HuffmanCompression::generateDecodedNodeQueue(const vector<pair<unsigned char, uint32_t >> &weightMapArr) {
    for (const auto &ele : weightMapArr) {
        auto newNode = new TreeNode(ele.first, ele.second);
        nodeQueue.push(newNode);
    }
}
