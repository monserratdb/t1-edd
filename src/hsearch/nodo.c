#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "nodo.h"

// basado con partes iguales de https://github.com/IIC2133-PUC/Talleres/tree/master/trees/bst/trees/src/trees

Node* node_create(Head *head){
    Node* node = (Node*)calloc(1, sizeof(Node));
    if (node == NULL) {
        printf("Error: No se pudo asignar memoria para el nodo\n");
        exit(EXIT_FAILURE);
    }
    node->value = head->id;
    node->height = 1; // height de un nuevo nodo es 1
    return node;
}

Node* insert_node(Node* root, Node* node){
    if (root == NULL) {
        return node;
    }
    
    if (node->value < root->value) {
        root->left_child = insert_node(root->left_child, node);
    } else if (node->value > root->value) {
        root->right_child = insert_node(root->right_child, node);
    } else {
        return root;
    }
    // actualizar height
    root->height = 1 + max(height(root->left_child), height(root->right_child));
    // balance para ver si hay que rotar
    int balance = get_balance(root);
    // rotación simple a derecha
    if (balance > 1 && node->value < root->left_child->value) {
        return right_rotate(root);
    }
    //  rotación simple a left
    if (balance < -1 && node->value > root->right_child->value) {
        return left_rotate(root);
    }
    //  rotación doble a derecha
    if (balance > 1 && node->value > root->left_child->value) {
        root->left_child = left_rotate(root->left_child);
        return right_rotate(root);
    }
    //  rotación doble a left
    if (balance < -1 && node->value < root->right_child->value) {
        root->right_child = right_rotate(root->right_child);
        return left_rotate(root);
    }

    return root;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

Node* right_rotate(Node* y) {
    Node* x = y->left_child;
    Node* T2 = x->right_child;
    // rotar
    x->right_child = y;
    y->left_child = T2;
    // actualizar height
    y->height = max(height(y->left_child), height(y->right_child)) + 1;
    x->height = max(height(x->left_child), height(x->right_child)) + 1;

    return x;
}

Node* left_rotate(Node* x) {
    Node* y = x->right_child;
    Node* T2 = y->left_child;
    // rotar
    y->left_child = x;
    x->right_child = T2;
    // actualizar height
    x->height = max(height(x->left_child), height(x->right_child)) + 1;
    y->height = max(height(y->left_child), height(y->right_child)) + 1;

    return y;
}

int get_balance(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left_child) - height(node->right_child);
}


Node* find_node(Node* root, int id) {
    Node* actual_node = root;
    while (actual_node) {
        if (actual_node->value == id) {
            return actual_node; // devolver el encontrado
        }
        if (actual_node->value > id) {
            actual_node = actual_node->left_child;
        } else {
            actual_node = actual_node->right_child;
        }
    }
    return NULL; // NULL si no se encontró
}
