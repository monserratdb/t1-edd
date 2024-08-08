#ifndef HEAP_H
#define HEAP_H
//copia taller https://github.com/IIC2133-PUC/Talleres/tree/master/heaps/priority-queue/queue

typedef struct {
    int arrival;
    int amount;
    int account_number;
} Order;

typedef struct {
    Order **nodes;
    int size;
    int capacity;
    int type; // 0 para min heap, 1 para max heap
} Heap;

Heap *heap_create(int capacity, int type);
void heap_free(Heap *heap);
void heap_insert(Heap *heap, int arrival, int amount, int account_number);
Order *heap_extract(Heap *heap);
void percolate_up(Heap *heap, int index);
void percolate_down(Heap *heap, int index);

#endif
