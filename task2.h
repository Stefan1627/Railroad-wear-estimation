/*CALMAC Stefan-314CC*/
#ifndef TASK2_H
#define TASK2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_SIZE 1000
#define MAX_NAME_LENGTH 50
#define INF INT_MAX

typedef struct Railroad2_0 {
    int city_index;
    int route_index;
    int remains;
    int cost;
    struct Railroad2_0 *next;
}Railroad2_0;

typedef struct List2_0 {
    Railroad2_0 *head;
}List2_0;

/**
 * functie pentru sortarea dupa indici a vectorului
 * @param cost_array - vectorul de costuri
 * @param prev - vectorul ce contine ordinea indicilor vectorului sortat
 * @param num_cities - numarul total de orase
*/
void sort_cost(int *cost_array, int *prev, int num_cities);

/**
 * functie pentru retinerea drumurilor pe care dorim sa le pastram
 * @param a - lista de adiacenta a grafului
 * @param path - drumul care trebuie pastrat
*/
void save_paths(List2_0 **a, int *path);

/**
 * functie pentru stergerea elementelor nule din vector
 * @param print_order - vectorul cu rutele
 * @param size - marimea vectorului
*/
int delete_null_elements(char **print_order, int size);

/**
 * functie pentru adaugarea unui nod nou in lista
 * @param a - lista de adiacenta a grafului
 * @param s - indicele nodului sursa
 * @param city - indicele nodului destinatie
 * @param route - indicele rutei corespunzatoare
 * @param cost - costul rutei 
*/
void add_node2_0(List2_0 **a, int s, int city, int route, int cost);

/**
 * functie pentru crearea listei de adiacenta
 * @param a - lista de adiacenta a grafului
 * @param in - fisierul cu datele de intrare
 * @param m - numarul de muchii
 * @param city_names - vectorul de orase
 * @param num_cities - numarul total de orase
*/
void create_adj_list(List2_0 **a, FILE *in, int m,
                     char **city_names, int *num_cities);

/**
 * functie pentru aflarea celui mai scurt drum pentru fiecare nod
 * @param a - lista de adiacenta a grafului
 * @param start - indicele nodului de start
 * @param num_cities - numarul total de orase
 * @param cost_array - vectorul de costuri
 * @param paths - vectorul de drumuri
*/
void dijkstra(List2_0 **a, int start, int num_cities,
              int *cost_array, int *prev, int **paths);

/**
 * functie pentru retinerea muchilor dorite
 * @param a - lista de adiacenta a grafului
 * @param paths - vectorul de drumuri
 * @param prev - vectorul cu costul drumurilor sortate
 * @param k - numarul maxim de muchii care trebuie pastrate
*/
void optimize_railroad(List2_0 **a, int **paths, int *prev, int k);

/**
 * functie pentru afisarea rezultatului final
 * @param a - lista de adiacenta a grafului
 * @param out - fisierul de iesire
 * @param num_cities - numarul total de orase
 * @param city_names - vectorul de orase
 * @param t - numarul de muchii pastrate
 * @param m - numarul total de muchii
*/
void print2_0(List2_0 **a, FILE *out, int num_cities,
              char **city_names, int t, int m);

/**
 * @param in - fisier de intrare
 * @param out - fisier de iesire
*/
void task2(FILE *in, FILE *out);

#endif /* TASK2_H */