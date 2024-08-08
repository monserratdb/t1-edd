#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "heads.h"
#include "nodo.h"

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
int compare_heads_by_x(const void *a, const void *b) {
    Head **head1 = (Head **)a;
    Head **head2 = (Head **)b;
    return ((*head1)->x - (*head2)->x);
}

double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int compare_heads_by_coordinates(const void *a, const void *b) {
    Head **head1 = (Head **)a;
    Head **head2 = (Head **)b;
    if ((*head1)->x != (*head2)->x) {
        return (*head1)->x - (*head2)->x;
    }
    return (*head1)->y - (*head2)->y;
}


int main(int argc, char **argv) {
  check_arguments(argc, argv);

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  /* Construcción de las estructuras */

  int N_HEADS;
  int result;
  result = fscanf(input_file, "%d", &N_HEADS);
  if (result != 1) {
    printf("Error leyendo la cantidad de cabezas\n");
    return 1;
  }
  Head *heads = calloc(N_HEADS, sizeof(Head));

  /* Inicializar árboles de búsqueda */

  Node* id_tree = NULL; // Árbol AVL para búsqueda por ID
  Node* year_tree = NULL; //arbol para busqueda por year
  Node* year_district_tree = NULL;

  /* Guardar cabezas */
  for (int i = 0; i < N_HEADS; i++) {
    Head *h = &heads[i];
    result = fscanf(input_file, "%d\n", &N_HEADS);
    result = fscanf(input_file, "SAVE %d %s %d %d %d %d\n", &h->id, h->name, &h->year, &h->region, &h->x, &h->y);
    if (result != 6) {
      printf("Error al leer los datos de la cabeza %d\n", i);
      return 1;
    }
    // Agregar a árboles de búsqueda
    id_tree = insert_node(id_tree, node_create(h));
    year_tree = insert_node(year_tree, node_create(h));
    year_district_tree = insert_node(year_district_tree, node_create(h));
  }

  int N_BUSQUEDAS;
  result = fscanf(input_file, "%d\n", &N_BUSQUEDAS);
  if (result != 1) {
    printf("Error leyendo la cantidad de eventos\n");
    return 1;
  }

  /* Búsquedas */
  char command[32];
  for (int i = 0; i < N_BUSQUEDAS; i++) {
    result = fscanf(input_file, "%s\n", command);
    if (result != 1) {
      printf("Error al leer la búsqueda %d\n", i);
      return 1;
    }

    if (string_equals(command, "WITH-ID")) {
    int id;
    result = fscanf(input_file, "%d", &id);
    if (result != 1) {
        printf("Error al leer el ID de la búsqueda WITH-ID\n");
        return 1;
    }
    Node* found_node = find_node(id_tree, id);
    if (found_node) {
        fprintf(output_file, "WITH-ID %d: 1\n", id);
        Head *found_head = NULL;
        // buscamos la cabeza
        for (int j = 0; j < N_HEADS; j++) {
            if (heads[j].id == id) {
                found_head = &heads[j];
                break;
            }
        }
        if (found_head) {
            fprintf(output_file, "%d %s\n", found_head->id, found_head->name);
        }
    } else {
        fprintf(output_file, "WITH-ID %d: 0\n", id);
    }

    } else if (string_equals(command, "WITH-YEAR")) {
    int year;
    result = fscanf(input_file, "%d", &year);
    if (result != 1) {
        printf("Error al leer el año de la búsqueda WITH-YEAR\n");
        return 1;
    }
    // contador
    int count = 0;
    // iteramos
    for (int j = 0; j < N_HEADS; j++) {
        if (heads[j].year == year) {
            count++;
        }
    }
    fprintf(output_file, "WITH-YEAR %d: %d\n", year, count);
    // iteramos again
    for (int j = 0; j < N_HEADS; j++) {
        if (heads[j].year == year) {
            fprintf(output_file, "%d %s\n", heads[j].id, heads[j].name);
        }
    }

    } else if (string_equals(command, "WITH-YEAR-DISTRICT")) {
    int year, district;
    result = fscanf(input_file, "%d %d", &year, &district);
    if (result != 2) {
        printf("Error al leer el año y el distrito de la búsqueda WITH-YEAR-DISTRICT\n");
        return 1;
    }
    // counter
    int count = 0;
    // iteramos
    for (int j = 0; j < N_HEADS; j++) {
        if (heads[j].year == year && heads[j].region == district) {
            count++;
        }
    }
    fprintf(output_file, "WITH-YEAR-DISTRICT %d %d: %d\n", year, district, count);
    // iteramos again
    for (int j = 0; j < N_HEADS; j++) {
        if (heads[j].year == year && heads[j].region == district) {
            fprintf(output_file, "%d %s\n", heads[j].id, heads[j].name);
        }
    }

    } else if (string_equals(command, "IN-X-RANGE")) {
    int A, B;
    result = fscanf(input_file, "%d %d", &A, &B);
    if (result != 2) {
        printf("Error al leer los límites del rango en la búsqueda IN-X-RANGE\n");
        return 1;
    }
    Head *temp_heads[N_HEADS];
    int count = 0;
    // iteramos
    for (int j = 0; j < N_HEADS; j++) {
        if (heads[j].x >= A && heads[j].x < B) {
            temp_heads[count++] = &heads[j];
        }
    }
    // ordenamos c qsort
    qsort(temp_heads, count, sizeof(Head*), compare_heads_by_x);
    fprintf(output_file, "IN-X-RANGE %d %d: %d\n", A, B, count);
    for (int j = 0; j < count; j++) {
        fprintf(output_file, "%d %s\n", temp_heads[j]->id, temp_heads[j]->name);
    }

    } else if (string_equals(command, "IN-CIRCLE")) {
    int x, y, r;
    result = fscanf(input_file, "%d %d %d", &x, &y, &r);
    if (result != 3) {
        printf("Error al leer las coordenadas y el radio del círculo en la búsqueda IN-CIRCLE\n");
        return 1;
    }
    Head *temp_heads[N_HEADS];
    int count = 0;

    for (int j = 0; j < N_HEADS; j++) {
        if (distance(x, y, heads[j].x, heads[j].y) <= r) {
            temp_heads[count++] = &heads[j];
        }
    }
   
    qsort(temp_heads, count, sizeof(Head*), compare_heads_by_coordinates);
    fprintf(output_file, "IN-CIRCLE %d %d %d: %d\n", x, y, r, count);
    for (int j = 0; j < count; j++) {
        fprintf(output_file, "%d %s\n", temp_heads[j]->id, temp_heads[j]->name);
    }


    } else {
      printf("BÚSQUEDA NO ENCONTRADA: %s\n", command);
    }
  }

  /* Cerramos los archivos */
  fclose(input_file);
  fclose(output_file);

  /* Liberación de memoria */
  free(heads);

  return 0;
}
