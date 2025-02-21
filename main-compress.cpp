//
//  main.cpp
//  Huffman Coding
//
//  Created by Arseny Kozlov on 20.02.2025.
//

#include "Huffman.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <chrono>


int main() {
    
    // COMPRESS
    auto start = std::chrono::high_resolution_clock::now();
    
    const char* inputFile = "Input.txt";
//    const char* inputFile = "sample3.bmp";
    const char* outputFile = "Output.txt";

    unsigned char freq[MAX_SYMBOLS] = {0};

    // read input file
    FILE* file = fopen(inputFile, "rb");
    if (!file) {
        std::cout << "Error opening input file\n";
        return 1;
    }
    
    fseek(file, 0L, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < length; ++i) {
        freq[(unsigned char)fgetc(file)]++;
    }
    fclose(file);
    
    // build tree
    Node* list = nullptr;
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (!freq[i]) continue;
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
    std::string codeTable[MAX_SYMBOLS];
    generateCodes(root, "", codeTable);

    // read input file fully
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cout << "Error opening input file for second read\n";
        return 1;
    }
    std::ostringstream oss;
    oss << inFile.rdbuf();
    std::string fileData = oss.str();
    inFile.close();

    
    std::string bitString;
    bitString.reserve(fileData.size() * 8);
    for (unsigned char c : fileData) {
        bitString += codeTable[c];
    }

    const int BIT8 = 8;
    // static_cast from any pointer to void pointer and vice versa
    int totalBits = static_cast<int>(bitString.size());
    int tail = totalBits % BIT8;
    int count = totalBits / BIT8;
    int len = count + (tail ? 1 : 0);

    std::vector<char> compressedData(len, 0);
    for (int i = 0; i < len; ++i) {
        BIT2CHAR symb;
        symb.symb = 0;
        for (int b = 0; b < BIT8; ++b) {
            int pos = i * BIT8 + b;
            if (pos < totalBits && bitString[pos] == '1') {
                switch (b) {
                    case 0: symb.mbit.b1 = 1; break;
                    case 1: symb.mbit.b2 = 1; break;
                    case 2: symb.mbit.b3 = 1; break;
                    case 3: symb.mbit.b4 = 1; break;
                    case 4: symb.mbit.b5 = 1; break;
                    case 5: symb.mbit.b6 = 1; break;
                    case 6: symb.mbit.b7 = 1; break;
                    case 7: symb.mbit.b8 = 1; break;
                }
            }
        }
        compressedData[i] = symb.symb;
    }

    // open output file
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cout << "Error opening output file\n";
        return 1;
    }
    
    
    // tail -> len -> data
    /* reinterpret_cast 
        In C++, reinterpret_cast is a casting operator that converts a pointer of one data type into a pointer of another data type, regardless of the relationship between the types. It essentially reinterprets the bit pattern of the value as a new type. It can also convert between pointer types and integral types.
        reinterpret_cast is considered the most dangerous cast in C++ because it bypasses type checking and allows for potentially unsafe conversions. It should be used with extreme caution and only when necessary, as misuse can lead to undefined behavior, data corruption, and program crashes.
        C++
    */
    outFile.write(reinterpret_cast<const char*>(&tail), sizeof(tail));
    outFile.write(reinterpret_cast<const char*>(&len),  sizeof(len));
    outFile.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
    outFile.close();

    std::cout << "Original file size: " << length << " bytes\n";
    std::cout << "Compressed file size: " << compressedData.size() << " bytes\n";
    std::cout << "Compression ratio: " << (double)compressedData.size() / length * 100 << "%\n";
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Compression time: " << duration.count() << " seconds\n";
    
    freeTree(root);
    
    return 0;
}
