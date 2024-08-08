#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

/* Retorna true si ambos strings son iguales */
static bool string_equals(char *string1, char *string2) {
  return !strcmp(string1, string2);
}

static bool check_arguments(int argc, char **argv) {
  if (argc != 3) {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    exit(1);
  }
  return true;
}

//funciones adicionales


int main(int argc, char **argv) {
  check_arguments(argc, argv);

  int last_amount;
  bool last_is_buy;

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  Heap *buy_heap = heap_create(10, 1); // max para órdenes de compra
  Heap *sell_heap = heap_create(10, 0); // min  para órdenes de venta

  int arrival_counter = 1; // contador de llegada de órdenes

  /* Leer eventos */
  int N_EVENTS;
  fscanf(input_file, "%d", &N_EVENTS);

  for (int i = 0; i < N_EVENTS; i++) {
    char command[32];
    fscanf(input_file, "%s", command);

    if (string_equals(command, "STATUS")) {
    if (buy_heap->size == 0 && sell_heap->size == 0) {
        fprintf(output_file, "Mercado inactivo: No hay registros de transacciones.\n");
    } else {
        fprintf(output_file, "Estado del mercado\n");
        if (buy_heap->size > 0) {
            Order *buy_order = buy_heap->nodes[0];
            int min_arrival = buy_order->arrival;
            // buscar si hay otra orden de igual monto pero menor arrival
            for (int i = 1; i < buy_heap->size; i++) {
                if (buy_heap->nodes[i]->amount == buy_order->amount && buy_heap->nodes[i]->arrival < min_arrival) {
                    min_arrival = buy_heap->nodes[i]->arrival;
                    buy_order = buy_heap->nodes[i];
                }
            }
            fprintf(output_file, "\t%d por %d\n", buy_order->amount, buy_order->account_number);
        }
        if (buy_heap->size > 0 && sell_heap->size > 0) {
            Order *buy_order = buy_heap->nodes[0];
            Order *sell_order = sell_heap->nodes[0];
            fprintf(output_file, "\tSPREAD %d\n", sell_order->amount - buy_order->amount);
        }
        if (sell_heap->size > 0) {
            Order *sell_order = sell_heap->nodes[0];
            int min_arrival = sell_order->arrival;
            // buscar si hay otra orden de igual monto pero menor arrival
            for (int i = 1; i < sell_heap->size; i++) {
                if (sell_heap->nodes[i]->amount == sell_order->amount && sell_heap->nodes[i]->arrival < min_arrival) {
                    min_arrival = sell_heap->nodes[i]->arrival;
                    sell_order = sell_heap->nodes[i];
                }
            }
            fprintf(output_file, "\t%d por %d\n", sell_order->amount, sell_order->account_number);
        }
    }


    } else if (string_equals(command, "SELL") || string_equals(command, "BUY")) {
      int amount, account_number;
      fscanf(input_file, "%d %d", &amount, &account_number);
      
      if (string_equals(command, "SELL")) {
        heap_insert(sell_heap, arrival_counter++, amount, account_number);
        last_is_buy = false;
        fprintf(output_file, "User %d vende a monto %d\n", account_number, amount);
        last_amount = amount;
      } else {
        heap_insert(buy_heap, arrival_counter++, amount, account_number);
        last_is_buy = true;
        fprintf(output_file, "User %d compra a monto %d\n", account_number, amount);
        last_amount = amount;
      }

      // intentar ejecutar la orden registrada
      while (buy_heap->size > 0 && sell_heap->size > 0 && buy_heap->nodes[0]->amount >= sell_heap->nodes[0]->amount) {
        Order *buy_order = heap_extract(buy_heap);
        Order *sell_order = heap_extract(sell_heap);

    int executed_amount;

    if (last_is_buy) {
      executed_amount = last_amount;
      fprintf(output_file, "Orden Ejecutada: %d -> %d: %d\n", sell_order->account_number, buy_order->account_number, executed_amount);
    } else {
      executed_amount = last_amount;
        fprintf(output_file, "Orden Ejecutada: %d -> %d: %d\n", sell_order->account_number, buy_order->account_number, executed_amount);
      }}
    }
  }

  // Cerrar los archivos
  fclose(input_file);
  fclose(output_file);

  // Liberar memoria del heap
  heap_free(buy_heap);
  heap_free(sell_heap);

  return 0;
}
