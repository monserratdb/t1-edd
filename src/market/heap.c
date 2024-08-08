#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

//copia taller https://github.com/IIC2133-PUC/Talleres/tree/master/heaps/priority-queue/queue

Heap *heap_create(int capacity, int type) {
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->type = type; // 0 es max(compra), 1 es min (venta)
    heap->nodes = (Order **)malloc(capacity * sizeof(Order *));
    //printf("heap creado con capacidad: %d y tipo %d\n", capacity, type);
    //printf("insertando en el heap: %d por %d\n", capacity, type);
    return heap;
}

void heap_free(Heap *heap) {
    for (int i = 0; i < heap->size; i++) {
        free(heap->nodes[i]);
    }
    free(heap->nodes);
    free(heap);
}

void heap_insert(Heap *heap, int arrival, int amount, int account_number) {
    if (heap->size == heap->capacity) {
        // Expand heap if necessary
        heap->capacity *= 2;
        heap->nodes = (Order **)realloc(heap->nodes, heap->capacity * sizeof(Order *));
    }
    Order *order = (Order *)malloc(sizeof(Order));
    order->arrival = arrival;
    order->amount = amount;
    order->account_number = account_number;
    heap->nodes[heap->size] = order;
    percolate_up(heap, heap->size);
    heap->size++;
}




Order *heap_extract(Heap *heap) {
    if (heap->size == 0) {
        return NULL;
    }
    int min_or_max = 0;
    for (int i = 1; i < heap->size; i++) {
        if ((heap->type == 0 && heap->nodes[i]->amount < heap->nodes[min_or_max]->amount) ||
            (heap->type == 1 && heap->nodes[i]->amount > heap->nodes[min_or_max]->amount) ||
            (heap->nodes[i]->amount == heap->nodes[min_or_max]->amount && heap->nodes[i]->arrival < heap->nodes[min_or_max]->arrival)) {
            min_or_max = i;
        }
    }
    Order *min_order = heap->nodes[min_or_max];
    heap->nodes[min_or_max] = heap->nodes[heap->size - 1];
    heap->size--;
    percolate_down(heap, min_or_max);
    return min_order;
}



void heapify(Heap *heap) {
    for (int i = heap->size / 2; i >= 0; i--) {
        percolate_down(heap, i);
    }
}

void percolate_up(Heap *heap, int index) {
    int parent = (index - 1) / 2;
    if (index > 0 && (
        (heap->type == 0 && heap->nodes[index]->amount < heap->nodes[parent]->amount) ||
        (heap->type == 1 && heap->nodes[index]->amount > heap->nodes[parent]->amount))) {
        Order *temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[parent];
        heap->nodes[parent] = temp;
        percolate_up(heap, parent);
    }
}

void percolate_down(Heap *heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int min_or_max = index;

    if (left < heap->size && (
        (heap->type == 0 && heap->nodes[left]->amount < heap->nodes[min_or_max]->amount) ||
        (heap->type == 1 && heap->nodes[left]->amount > heap->nodes[min_or_max]->amount))) {
        min_or_max = left;
    }

    if (right < heap->size && (
        (heap->type == 0 && heap->nodes[right]->amount < heap->nodes[min_or_max]->amount) ||
        (heap->type == 1 && heap->nodes[right]->amount > heap->nodes[min_or_max]->amount))) {
        min_or_max = right;
    }

    if (min_or_max != index) {
        Order *temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[min_or_max];
        heap->nodes[min_or_max] = temp;
        percolate_down(heap, min_or_max);
    }
}