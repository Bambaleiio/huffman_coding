//
//  Decompress.cpp
//  Huffman Coding
//
//  Created by Arseny Kozlov on 20.02.2025.
//


#include "Huffman.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

void decompressFile(const char* compressedFile, const char* outputFile);

int main() {
    const char* compressedFile = "Output.txt";
    const char* decompressedFile = "Decompressed.txt";

    auto start = std::chrono::high_resolution_clock::now();

    decompressFile(compressedFile, decompressedFile);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Decompression time: " << duration.count() << " seconds\n";

    return 0;
}


// Формат сжатого файла: [256 байт таблицы частот] -> [tail (int)] -> [len (int)] -> [len байт сжатых данных]
void decompressFile(const char* compressedFile, const char* outputFile) {
    
    // read compressed file
    std::ifstream in(compressedFile, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening compressed file: " << compressedFile << "\n";
        return;
    }
    
    unsigned char freq[MAX_SYMBOLS] = {0};
    in.read(reinterpret_cast<char*>(freq), MAX_SYMBOLS * sizeof(unsigned char));
    
    int tail = 0, len = 0;
    in.read(reinterpret_cast<char*>(&tail), sizeof(tail));
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    
    std::vector<char> compressedData(len);
    in.read(compressedData.data(), len);
    in.close();
    
    std::string bitString;
    bitString.reserve(len * 8);
    for (int i = 0; i < len; ++i) {
        char byte = compressedData[i];
        for (int b = 7; b >= 0; --b) {
            if (i == len - 1 && tail != 0 && b > (tail - 1)) continue;
            bitString.push_back((byte & (1 << b)) ? '1' : '0');
        }
    }
    
    // rebuild tree
    Node* list = nullptr;
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (freq[i] == 0) continue;
        Node* newNode = new Node((unsigned char)i, freq[i]);
        addToList(list, newNode);
    }
    while (list && list->next) {
        Node* left = list;
        Node* right = list->next;
        list = right->next;
        Node* parent = new Node(left, right);
        addToList(list, parent);
    }
    Node* root = list;
    
    std::string decompressed;
    Node* current = root;
    for (char bit : bitString) {
        if (bit == '0')
            current = current->left;
        else
            current = current->right;
        
        if (current->isSymb) {
            decompressed.push_back(current->symb);
            current = root;
        }
    }
    
    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening output file: " << outputFile << "\n";
        freeTree(root);
        return;
    }
    out.write(decompressed.c_str(), decompressed.size());
    out.close();
    
    freeTree(root);
}
