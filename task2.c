/*CALMAC Stefan-314CC*/
#include "task2.h"
#include "task1.h"

// sortam dupa indici cost_array
void sort_cost(int *cost_array, int *prev, int num_cities)
{
    int i, j, temp;
    for (i = 0; i < num_cities; i++) {
        prev[i] = i;
    }

    // bubble sort
    for (i = 0; i < num_cities - 1; i++) {
        for (j = 0; j < num_cities - i - 1; j++) {
            if (cost_array[prev[j]] > cost_array[prev[j + 1]]) {
                temp = prev[j];
                prev[j] = prev[j + 1];
                prev[j + 1] = temp;
            }
        }
    }
}

// functie pentru retinerea drumurilor pe care dorim sa le pastram
void save_paths(List2_0 **a, int *path)
{
    Railroad2_0 *current;
    int i = 0;
    if (path[i] != -1) {
        while (path[i + 1] != -1) {
            // cautam elementul din lista corespunzator
            current = a[path[i]]->head;
            while (current->city_index != path[i + 1]) {
                current = current->next;
            }

            // verificam daca elementul gasit este cel din input
            if (current->route_index < 0) {
                current = a[path[i + 1]]->head;
                while (current->city_index != path[i]) {
                    current = current->next;
                }
                current->remains = 1;
            }
            else
                current->remains = 1;

            i++;
        }
    }
}

// functie pentru eliminarea elementelor nule din arrayul pentru
// rutele pe care le pastram
int delete_null_elements(char **print_order, int size)
{
    int new_size = 0;
    int i;
    // mutam toate elementele nenule la stanga
    for (i = 0; i < size; ++i) {
        if (strlen(print_order[i]) > 2) {
            strcpy(print_order[new_size++], print_order[i]);
        }
    }

    // returnam noua marime a array-ului
    return new_size;
}

// functie pentru adaugarea unui nod in lista de adiacenta
void add_node2_0(List2_0 **a, int s, int city, int route, int cost)
{
    Railroad2_0 *dest, *tmp;

    // alocarea memoriei pentru nodul destinatie
    dest = (Railroad2_0 *)malloc(sizeof(Railroad2_0));
    dest->city_index = city;
    dest->route_index = route;
    dest->remains = 0;
    dest->cost = cost;
    dest->next = NULL;

    if (a[s]->head == NULL)
        a[s]->head = dest;
    else {
        // traversam lista pentru a adauga nodul al sfarsitul listei
        tmp = a[s]->head;

        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        // adaugam nodul in lista
        tmp->next = dest;
    }
}

// functie pentru crearea listei de adiacenta
void create_adj_list(List2_0 **a, FILE *in, int m,
                     char **city_names, int *num_cities)
{
    int i, exist_1, exist_2, cost;
    int route_index;
    char *tok, word[MAX_SIZE];

    for (i = 0; i < m; i++) {
        fgets(word, MAX_SIZE, in);

        // extragem primul oras din string
        tok = strtok(word, " ");

        // aflam/atribuim un numar corespunzator orasului
        exist_1 = check_exist(city_names, tok, *num_cities);
        if (exist_1 == -1) {
            strcpy(city_names[*num_cities], tok);
            exist_1 = *num_cities;
            (*num_cities)++;
        }

        // extragem al doilea oras din string si ii aflam pozitia
        tok = strtok(NULL, " ");
        exist_2 = check_exist(city_names, tok, *num_cities);
        if (exist_2 == -1) {
            strcpy(city_names[*num_cities], tok);
            exist_2 = *num_cities;
            (*num_cities)++;
        }

        tok = strtok(NULL, " ");
        cost = atoi(tok);

        // adaugam nodurile in lista
        route_index = i + 1;
        add_node2_0(a, exist_1, exist_2, route_index, cost);
        add_node2_0(a, exist_2, exist_1, route_index * -1, cost);
    }
}

// functie pentru aflarea celui mai scurt drum pentru fiecare nod
void dijkstra(List2_0 **a, int start, int num_cities,
              int *cost_array, int *prev, int **paths)
{
    int visited[num_cities];
    for (int i = 0; i < num_cities; i++) {
        cost_array[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }

    cost_array[start] = 0;

    for (int i = 0; i < num_cities; i++) {
        int min_cost_array = INT_MAX;
        int min_index = -1;

        for (int j = 0; j < num_cities; j++) {
            if (!visited[j] && cost_array[j] < min_cost_array) {
                min_cost_array = cost_array[j];
                min_index = j;
            }
        }

        if (min_index == -1) {
            break;
        }

        visited[min_index] = 1;

        Railroad2_0 *current = a[min_index]->head;
        while (current != NULL) {
            int neighbor = current->city_index;
            int cost = current->cost;
            if (cost_array[min_index] + cost < cost_array[neighbor]) {
                cost_array[neighbor] = cost_array[min_index] + cost;
                prev[neighbor] = min_index;

                int k = 0;
                int temp = neighbor;
                while (temp != -1) {
                    paths[neighbor][k++] = temp;
                    temp = prev[temp];
                }

                // adaugam marcaj pentru sfarsitul drumului
                paths[neighbor][k] = -1;
            }
            current = current->next;
        }
    }
}

// functie pentru retinerea muchilor dorite
void optimize_railroad(List2_0 **a, int **paths, int *prev, int k)
{
    int i;
    for (i = 1; i < k + 1; i++) {
        save_paths(a, paths[prev[i]]);
    }
}

// functie pentru printarea rezultatului final
void print2_0(List2_0 **a, FILE *out, int num_cities,
              char **city_names, int t, int m)
{
    int i, j;
    char **print_order = (char **)malloc((m + 1) * sizeof(char *));
    for (i = 0; i < m + 1; i++) {
        print_order[i] = (char *)malloc(100 * sizeof(char));
        print_order[i][0] = '\0';
    }
    Railroad2_0 *current;

    fprintf(out, "%d\n", t);

    // adaugam intr-un array elementele pe care le retinem la
    // indicele corespunzator
    for (i = 0; i < num_cities; i++) {
        current = a[i]->head;
        while (current != NULL) {
            if (current->remains == 1) {
                j = current->route_index;
                strcat(print_order[j], city_names[i]);
                strcat(print_order[j], " ");
                strcat(print_order[j], city_names[current->city_index]);
            }
            current = current->next;
        }
    }

    // eliminam elementele nenule si printam array-ul
    j = delete_null_elements(print_order, m + 1);
    for (i = 0; i < j; i++) {
        fprintf(out, "%s\n", print_order[i]);
    }

    // eliberam memoria pentru array-ul ajutator
    for (i = 0; i < m + 1; i++) {
        free(print_order[i]);
    }
    free(print_order);
}

void task2(FILE *in, FILE *out)
{
    int k, m, i;
    int num_cities = 0;
    char word[MAX_SIZE], start_city[MAX_NAME_LENGTH];
    char **city_names;

    fgets(word, MAX_SIZE, in);
    word[strlen(word) - 1] = '\0';
    strcpy(start_city, word);

    fgets(word, MAX_SIZE, in);
    k = atoi(word);

    fgets(word, MAX_SIZE, in);
    m = atoi(word);

    city_names = (char **)malloc(m * 2 * sizeof(char *));
    for (i = 0; i < m * 2; i++) {
        city_names[i] = (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
    }

    List2_0 **a = (List2_0 **)malloc(m * 2 * sizeof(List2_0 *));
    for (i = 0; i < m * 2; i++) {
        a[i] = (List2_0 *)malloc(sizeof(List2_0));
        a[i]->head = NULL;
    }

    // creem lista de adiacenta
    create_adj_list(a, in, m, city_names, &num_cities);

    // modificam numarul de muchii care trebuie pastrate daca este nevoie
    if (k > num_cities - 1) {
        k = num_cities - 1;
    }

    int *cost_array = (int *)malloc(num_cities * sizeof(int));
    int *prev = (int *)malloc(num_cities * sizeof(int));
    int **paths = (int **)malloc(num_cities * sizeof(int *));
    for (i = 0; i < num_cities; i++) {
        paths[i] = (int *)malloc((num_cities + 1) * sizeof(int));

        // initializam cu temrinatorul de drum
        paths[i][0] = -1;
    }

    // aflam numarul corespunzator orasului de start
    start_city[strlen(start_city) - 1] = '\0';
    int start_index = check_exist(city_names, start_city, num_cities);

    // extragem array-ul de costuri si drumurile pentru fiecare nod
    dijkstra(a, start_index, num_cities, cost_array, prev, paths);

    // sortam drumurile in fucntie de costuri
    sort_cost(cost_array, prev, num_cities);

    // retinem doar muchiile dorite
    optimize_railroad(a, paths, prev, k);

    // printam rezultatul
    print2_0(a, out, num_cities, city_names, k, m);

    // eliberarea memoriei utilizate
    free(cost_array);
    free(prev);
    for (i = 0; i < num_cities; i++) {
        free(paths[i]);
    }
    free(paths);
    for (i = 0; i < m * 2; i++) {
        free(city_names[i]);
    }
    free(city_names);
    for (i = 0; i < m * 2; i++) {
        Railroad2_0 *current = a[i]->head;
        while (current != NULL) {
            Railroad2_0 *next = current->next;
            free(current);
            current = next;
        }
        free(a[i]);
    }
    free(a);
}
