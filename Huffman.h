//
//  Huffman.h
//  Huffman Coding
//
//  Created by Arseny Kozlov on 21.02.2025.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

const int CODE_SIZE = 8;
const int MAX_SYMBOLS = 256;

typedef union bit2char {
    char symb;
    struct bit {
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
    } mbit;
} BIT2CHAR;

struct Node {
    unsigned char symb;
    bool isSymb;
    unsigned int freq;
    unsigned char code[CODE_SIZE];
    
    Node *left, *right, *next;
    
    // Конструктор для листа
    Node(unsigned char c, unsigned int f);
    
    // Конструктор для внутреннего узла
    Node(Node* l, Node* r);
};

void freeTree(Node* root);
void addToList(Node* &head, Node* node);
void generateCodes(Node* root, const std::string &code, std::string codeTable[MAX_SYMBOLS]);

// Функции для отладки
void printQueue(Node* list);
void printTree(Node* root, int indent = 0);

#endif
