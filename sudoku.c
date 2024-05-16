#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n) {
    int check[10];

    // Validar filas
    for (int i = 0; i < 9; i++) {
        memset(check, 0, sizeof(check));  // Inicializar check a 0
        for (int j = 0; j < 9; j++) {
            int num = n->sudo[i][j];
            if (num != 0) {
                if (check[num] == 1) return 0;
                check[num] = 1;
            }
        }
    }

    // Validar columnas
    for (int j = 0; j < 9; j++) {
        memset(check, 0, sizeof(check));  // Inicializar check a 0
        for (int i = 0; i < 9; i++) {
            int num = n->sudo[i][j];
            if (num != 0) {
                if (check[num] == 1) return 0;
                check[num] = 1;
            }
        }
    }

    // Validar submatrices 3x3
    for (int k = 0; k < 9; k++) {
        memset(check, 0, sizeof(check));  // Inicializar check a 0
        for (int p = 0; p < 9; p++) {
            int i = 3 * (k / 3) + (p / 3);
            int j = 3 * (k % 3) + (p % 3);
            int num = n->sudo[i][j];
            if (num != 0) {
                if (check[num] == 1) return 0;
                check[num] = 1;
            }
        }
    }

    return 1;
}


List* get_adj_nodes(Node* n) {
    List* list = createList();
    int i = 0, j = 0;
    int found = 0;

    // Encontrar la primera casilla vacía
    for (i = 0; i < 9 && !found; i++) {
        for (j = 0; j < 9 && !found; j++) {
            if (n->sudo[i][j] == 0) {
                found = 1;
                i--; // Ajustar el índice después del bucle
                j--;
            }
        }
    }

    if (!found) {
        return list; // No hay casillas vacías
    }

    // Generar nodos adyacentes
    for (int num = 1; num <= 9; num++) {
        Node* new_node = copy(n);
        new_node->sudo[i][j] = num;
        if (is_valid(new_node)) {
            pushBack(list, new_node);
        } else {
            free(new_node);
        }
    }
    return list;
}


int is_final(Node* n) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) return 0;
        }
    }
    return 1;
}

Node* DFS(Node* initial, int* cont) {
    Stack* S = createStack();
    push(S, initial);
    *cont = 0;

    while (!is_empty(S)) {
        Node* current = top(S);
        pop(S);
        (*cont)++;

        if (is_final(current)) {
            return current;
        }

        List* adj_nodes = get_adj_nodes(current);
        for (Node* adj_node = front(adj_nodes); adj_node != NULL; adj_node = next(adj_nodes)) {
            push(S, adj_node);
        }

        clean(adj_nodes);
        free(current);
    }

    return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/