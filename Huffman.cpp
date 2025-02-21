//
//  Huffman.cpp
//  Huffman Coding
//
//  Created by Arseny Kozlov on 21.02.2025.
//

#include "Huffman.h"
#include <cstring>

Node::Node(unsigned char c, unsigned int f)
    : symb(c), isSymb(true), freq(f), left(nullptr), right(nullptr), next(nullptr) {}

Node::Node(Node* l, Node* r)
    : symb(0), isSymb(false), freq(l->freq + r->freq), left(l), right(r), next(nullptr)
{
    std::fill(code, code + CODE_SIZE, 0);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// приоритетная очередь
void addToList(Node* &head, Node* node) {
    if (!head || node->freq < head->freq) {
        node->next = head;
        head = node;
    } else {
        Node* temp = head;
        while (temp->next && temp->next->freq <= node->freq) {
            temp = temp->next;
        }
        node->next = temp->next;
        temp->next = node;
    }
}

// налево "0", направо – "1"
void generateCodes(Node* root, const std::string &code, std::string codeTable[MAX_SYMBOLS]) {
    if (!root) return;
    
    if (root->isSymb) {
        codeTable[root->symb] = code;
    }
    generateCodes(root->left, code + "0", codeTable);
    generateCodes(root->right, code + "1", codeTable);
}

// -----------------------------------------------------
// DEBUG
void printQueue(Node* list) {
    while(list) {
        std::cout << list->symb << " -> ";
        list = list->next;
    }
    std::cout << '\n';
}

void printTree(Node* root, int indent) {
    if (!root)
        return;
    
    // Вывод правого поддерева
    printTree(root->right, indent + 4);
    
    if (indent)
        std::cout << std::setw(indent) << " ";
    
    if (root->isSymb)
        std::cout << root->symb << "(" << root->freq << ")" << "\n";
    else
        std::cout << "*" << "(" << root->freq << ")" << "\n";
    
    // Вывод левого поддерева
    printTree(root->left, indent + 4);
}
// -----------------------------------------------------
