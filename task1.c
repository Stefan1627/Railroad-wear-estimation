/*CALMAC Stefan-314CC*/
#include "task1.h"

// functie pentru extragerea indicelui corespunzator orasului
int check_exist(char **city_names, char *str, int num_cities)
{
    int i;
    for (i = 0; i < num_cities; i++) {
        if (strcmp(city_names[i], str) == 0) {
            return i;
        }
    }
    return -1;
}

// functie pentru aflarea celui mai mare vecin
float get_max_neigh(List **a, float max_used, int i)
{
    Railroad *current;
    current = a[i]->head;
    while (current != NULL) {
        if (current->route_index < 0) {
            if (current->sections[current->num_sections - 1] > max_used) {
                max_used = current->sections[current->num_sections - 1];
            }
        }
        else {
            if (current->sections[0] > max_used) {
                max_used = current->sections[0];
            }
        }
        current = current->next;
    }
    return max_used;
}

// functie pentru copierea listei
List **copy_lists(List **a, int size)
{
    // alocarea memoriei pentru noul vector de liste
    List **copy = (List **)malloc(size * sizeof(List *));

    // trecem prin fiecare lista din original
    for (int i = 0; i < size; i++) {
        // alocam memoria pentru noua lista
        copy[i] = (List *)malloc(sizeof(List));
        if (copy[i] == NULL) {
            // verificam daca s-a alocat cu succes
            for (int j = 0; j < i; j++) {
                free(copy[j]);
            }
            free(copy);
            return NULL;
        }
        copy[i]->head = NULL;

        // copiem lista
        Railroad *current = a[i]->head;
        Railroad **copy_current = &(copy[i]->head);
        while (current != NULL) {
            *copy_current = (Railroad *)malloc(sizeof(Railroad));
            if (*copy_current == NULL) {
                for (int k = 0; k <= i; k++) {
                    Railroad *to_free = copy[k]->head;
                    while (to_free != NULL) {
                        Railroad *next = to_free->next;
                        if (to_free->sections != NULL) {
                            free(to_free->sections);
                        }
                        free(to_free);

                        to_free = next;
                    }
                    free(copy[k]);
                }
                free(copy);
                return NULL;
            }

            // copiem datele din lista
            (*copy_current)->city_index = current->city_index;
            (*copy_current)->num_sections = current->num_sections;
            (*copy_current)->route_index = current->route_index;

            if (current->sections != NULL) {
                (*copy_current)->sections =
                    (float *)malloc(current->num_sections * sizeof(float));

                if ((*copy_current)->sections == NULL) {
                    for (int k = 0; k <= i; k++) {
                        Railroad *to_free = copy[k]->head;
                        while (to_free != NULL) {
                            Railroad *next = to_free->next;

                            if (to_free->sections != NULL) {
                                free(to_free->sections);
                            }

                            free(to_free);
                            to_free = next;
                        }
                        free(copy[k]);
                    }
                    free(copy);
                    return NULL;
                }
                memcpy((*copy_current)->sections, current->sections,
                       current->num_sections * sizeof(float));
            }
            else {
                (*copy_current)->sections = NULL;
            }

            // mutam pe urmatorul nod
            current = current->next;
            copy_current = &((*copy_current)->next);
        }
        *copy_current = NULL;
    }

    return copy;
}

// functie pentru adaugarea unui nod in lista de adiacenta
void add_node(List **a, int s, int d, int j, int route, char *str)
{
    int i;
    char *tok;
    Railroad *dest, *tmp;

    // alocam memoria pentru nodul destinatie
    dest = (Railroad *)malloc(sizeof(Railroad));
    dest->city_index = d;
    dest->sections = (float *)malloc(j * sizeof(float));
    dest->num_sections = j;
    dest->route_index = route;
    tok = strtok(str, " ");
    dest->sections[0] = atof(tok);
    for (i = 1; i < j; i++) {
        tok = strtok(NULL, " ");
        dest->sections[i] = atof(tok);
    }
    dest->next = NULL;

    if (a[s]->head == NULL)
        a[s]->head = dest;
    else {
        // traversam lista pana la final
        tmp = a[s]->head;

        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        // adaugam nodul al finalul listei
        tmp->next = dest;
    }
}

// functie pentru acutalizarea elementelor din lista
void list_update(List **a, int num_cities)
{
    int i, j;
    Railroad *current_from, *current_to;

    for (i = 0; i < num_cities; i++) {
        current_from = a[i]->head;
        while (current_from != NULL) {
            if (current_from->route_index > 0) {
                current_to = a[current_from->city_index]->head;
                while (current_to->city_index != i) {
                    current_to = current_to->next;
                }

                for (j = 0; j < current_to->num_sections; j++) {
                    current_to->sections[j] = current_from->sections[j];
                }
            }
            current_from = current_from->next;
        }
    }
}

// functie pentru procesarea fiecarei linii din input
void read_line(List **a, char *word, char **city_names,
               int *num_cities, int i)
{
    int j, x, y;
    char *tok, aux[MAX_SIZE];

    // extragem primul oras si ii aflam indicele
    tok = strtok(word, " ");
    x = check_exist(city_names, tok, *num_cities);
    if (x == -1) {
        strcpy(city_names[*num_cities], tok);
        x = *num_cities;
        (*num_cities)++;
    }

    // extragem al doilea oras si ii aflam indicele
    tok = strtok(NULL, " ");
    y = check_exist(city_names, tok, *num_cities);
    if (y == -1) {
        strcpy(city_names[*num_cities], tok);
        y = *num_cities;
        (*num_cities)++;
    }

    // numarul de tronsoane
    tok = strtok(NULL, " ");
    j = atoi(tok);

    // vectorul de tronsoane
    tok = strtok(NULL, "");
    strcpy(aux, tok);

    // adaugam nodurile in lista
    add_node(a, x, y, j, i, tok);
    add_node(a, y, x, j, i * -1, aux);
}

// functie pentru modificarea listei la trcerea anilor
void year_traversal(List **a, int k, int num_cities)
{
    int i, j, m;
    float max_used;
    Railroad *current, *current_orig;
    List **a_copy = copy_lists(a, num_cities);

    for (i = 0; i < k; i++) {
        max_used = 0;
        for (j = 0; j < num_cities; j++) {
            current = a_copy[j]->head;
            current_orig = a[j]->head;

            while (current != NULL && current_orig != NULL) {
                if (current->route_index > 0) {
                    // modificam elementele care nu sunt capete de linie
                    for (m = 0; m < current->num_sections; m++) {
                        if (current->sections[m] > 0) {
                            current->sections[m] *= 2;
                        }
                        else if (m != 0 && m != current->num_sections - 1) {

                            max_used = current_orig->sections[m - 1];
                            if (max_used < current_orig->sections[m + 1])
                                max_used = current_orig->sections[m + 1];
                            current->sections[m] = max_used / 2;
                        }

                        if (current->sections[m] > MAX_PERCENTAGE) {
                            current->sections[m] = MAX_PERCENTAGE;
                        }
                    }

                    // modificam capatul 0 al tronsoanelor
                    if (current->sections[0] == 0) {
                        max_used = current_orig->sections[1];
                        max_used = get_max_neigh(a, max_used, j);
                        current->sections[0] = max_used / 2;
                    }

                    // modificam ultimul tronson
                    if (current->sections[current->num_sections - 1] == 0) {
                        max_used =
                            current_orig->sections[current->num_sections - 2];

                        max_used =
                            get_max_neigh(a, max_used, current->city_index);

                        current->sections[current->num_sections - 1] =
                            max_used / 2;
                    }
                }
                current = current->next;
                current_orig = current_orig->next;
            }
        }

        // actualizam elementele in liste
        for (j = 0; j < num_cities; j++) {
            current = a_copy[j]->head;
            current_orig = a[j]->head;
            while (current != NULL && current_orig != NULL) {
                for (m = 0; m < current->num_sections; m++) {
                    current_orig->sections[m] = current->sections[m];
                }
                current = current->next;
                current_orig = current_orig->next;
            }
        }
        list_update(a, num_cities);
    }

    // eliberam memoria alocata pentru copie
    for (i = 0; i < num_cities; i++) {
        Railroad *current = a_copy[i]->head;
        while (current != NULL) {
            Railroad *next = current->next;
            free(current->sections);
            free(current);
            current = next;
        }
        free(a_copy[i]);
    }
    free(a_copy);
}

// verificam daca rutele indeplinesc conditiile impuse
void verify_tol(List **a, float l, int *accepted, int num_cities)
{
    int i, j;
    float sum = 0, result = 0;
    Railroad *current;
    for (i = 0; i < num_cities; i++) {
        current = a[i]->head;
        while (current != NULL) {
            if (current->route_index > 0) {
                sum = 0;
                for (j = 0; j < current->num_sections; j++) {
                    sum += current->sections[j];
                }

                result = sum / current->num_sections;
                if (result < l) {
                    accepted[current->route_index] = 1;
                }
            }
            current = current->next;
        }
    }
}

// functie pentru afisarea rezultatului final
void print(List **a, int r, int k, int *accepted, int num_cities,
           char **city_names, FILE *out)
{
    int i, j = 1;
    char tmp[10];

    char **final_list = (char **)malloc((r + 1) * sizeof(char *));
    for (i = 0; i < r + 1; i++) {
        final_list[i] = (char *)malloc(100 * sizeof(char));
        final_list[i][0] = '\0';
    }

    // adaugam in final_list fiecare ruta la indicele corespunzator
    Railroad *current;
    for (i = 0; i < num_cities; i++) {
        current = a[i]->head;
        while (current != NULL) {
            if (current->route_index > 0) {
                j = current->route_index;
                strcat(final_list[j], city_names[i]);
                strcat(final_list[j], " ");
                strcat(final_list[j], city_names[current->city_index]);
                strcat(final_list[j], " ");
                sprintf(tmp, "%d ", current->num_sections);
                strcat(final_list[j], tmp);
                for (k = 0; k < current->num_sections; k++) {
                    sprintf(tmp, "%.2f ", current->sections[k]);
                    strcat(final_list[j], tmp);
                }
            }
            current = current->next;
        }
    }

    // printam rezultatul si eliberam memoria
    for (i = 1; i < r + 1; i++) {
        final_list[i][strlen(final_list[i]) - 1] = '\0';
        fprintf(out, "%s\n", final_list[i]);
        free(final_list[i]);
    }
    free(final_list[0]);
    free(final_list);

    // printam rutele care trebuie pastrate
    for (k = 0; k < r + 1; k++) {
        if (accepted[k] == 1)
            fprintf(out, "%d ", k);
    }
}

void task1(FILE *in, FILE *out)
{
    int r, k;
    int i;
    int num_cities = 0;
    int *accepted;
    float l;
    char **city_names;
    char word[MAX_SIZE];

    fgets(word, MAX_SIZE, in);
    r = atoi(word);

    fgets(word, MAX_SIZE, in);
    k = atoi(word);

    fgets(word, MAX_SIZE, in);
    l = atoi(word);

    city_names = (char **)malloc(r * 2 * sizeof(char *));
    for (i = 0; i < r * 2; i++) {
        city_names[i] = (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
    }

    List **a = (List **)malloc(r * 2 * sizeof(List *));
    for (i = 0; i < r * 2; i++) {
        a[i] = (List *)malloc(sizeof(List));
        a[i]->head = NULL;
    }

    accepted = (int *)calloc(r + 10, sizeof(int));

    // creem lista de adiacenta
    for (i = 0; i < r; i++) {
        fgets(word, MAX_SIZE, in);
        read_line(a, word, city_names, &num_cities, i + 1);
    }

    // modificam lista odata cu trecerea anilor
    year_traversal(a, k, num_cities);

    // verificam si retinem rutele care trebuie pastrate
    verify_tol(a, l, accepted, num_cities);

    // afisam rezultatul
    print(a, r, k, accepted, num_cities, city_names, out);

    // eliberam memoria utilizata
    for (i = 0; i < r * 2; i++) {
        Railroad *current = a[i]->head;
        while (current != NULL) {
            Railroad *next = current->next;
            free(current->sections);
            free(current);
            current = next;
        }
        free(a[i]);
    }
    free(a);
    free(accepted);

    for (i = 0; i < r * 2; i++) {
        free(city_names[i]);
    }
    free(city_names);
}