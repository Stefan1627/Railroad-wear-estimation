/*CALMAC Stefan-314CC*/
#ifndef TASK1_H
#define TASK1_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 1000
#define MAX_NAME_LENGTH 50
#define MAX_PERCENTAGE 100

typedef struct Railroad {
    int city_index;
    int num_sections;
    int route_index;
    float *sections;
    struct Railroad *next;
} Railroad;

typedef struct List {
    Railroad *head;
} List;

/**
 * functie pentru verificarea daca un oras exista sau trebuie memorat
 * @param city_names - numele tuturor oraselor
 * @param str - orasul de cautat
 * @param num_cities - numarul total de orase
*/
int check_exist(char **city_names, char *str, int num_cities);

/**
 * functie pentru determinarea celui mai mare tronson vecin
 * @param a - lista de adiacenta a grafului
 * @param max_used - valoarea maxima a vecinilor
 * @param i - indicele orasului
*/
float get_max_neigh(List **a, float max_used, int i);

/**
 * functie pentru copierea unei liste
 * @param a - lista de adiacenta a grafului
 * @param size - marimea listei
*/
List **copy_lists(List **a, int size);

/**
 * functie pentru adaugarea unui nod nou in lista de adiacenta
 * @param a - lista de adiacenta a grafului
 * @param s - indicele nodului sursa
 * @param d - indicele nodului destinatie
 * @param j - numarul de tronsoane
 * @param route - indicele rutei corespunzatoare
 * @param str - valorile fiecarui tronson
*/
void add_node(List **a, int s, int d, int j, int route, char *str);

/**
 * functie pentru actualizarea elementelor listei
 * @param a - lista de adiacenta a grafului
 * @param num_cities - numarul total de orase
*/
void list_update(List **a, int num_cities);

/**
 * functie pentru citirea rutelor si formarea liste de adiacenta
 * @param a - lista de adiacenta a grafului
 * @param word - linia cu toate datele despre o ruta
 * @param city_names - numele tuturor oraselor
 * @param num_cities - numarul total de orase
 * @param i - indexul rutei respective
*/
void read_line(List **a, char *word, char **city_names,
               int *num_cities, int i);

/**
 * functie pentru modificarea tronsoanelor pentru fiecare an
 * @param a - lista de adiacenta a grafului
 * @param k - numarul de ani
 * @param num_cities - numarul total de orase
*/
void year_traversal(List **a, int k, int num_cities);

/**
 * functie pentru verificarea gradului de uzura al tronsoanelor
 * @param a - lista de adiacenta grafului
 * @param l - gradul de uzura maxim acceptat
 * @param accepted - evidenta rutelor care trebuie pastrate
 * @param num_cities - numarul total de orase
*/
void verify_tol(List **a, float l, int *accepted, int num_cities);

/**
 * functie pentru afisarea rezultatului
 * @param a - lista de adiacenta a grafului
 * @param r - numarul de rute
 * @param k - numarul de ani
 * @param accepted - evidenta rutelor care trebuie pastrate
 * @param num_cities - numarul total de orase
 * @param city_names - toate numele oraselor
 * @param out - fisier iesire
*/
void print(List **a, int r, int k, int *accepted, int num_cities,
           char **city_names, FILE *out);

/**
 * @param in - fisier intrare
 * @param out - fisier iesire
*/
void task1(FILE *in, FILE *out);

#endif /* TASK1_H */