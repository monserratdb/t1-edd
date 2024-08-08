#pragma once
#include "heads.h"

// basado con partes iguales de https://github.com/IIC2133-PUC/Talleres/tree/master/trees/bst/trees/src/trees

typedef struct node_t {
    int value;
    struct node_t *left_child;
    struct node_t *right_child;
    int height; 
} Node;

Node* node_create(Head *head);

Node* insert_node(Node* root, Node* node);

int max(int a, int b);

int height(Node* node);

Node* right_rotate(Node* y);

Node* left_rotate(Node* x);

int get_balance(Node* node);


Node* find_node(Node* root, int id);
