#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 50
#define MAX_ALFABETO 10
#define MAX_TRANSICIONES 200
#define MAX_CADENA 100

typedef struct {
    char origen[10];
    char simbolo[10];
    char destino[10];
} Transicion;


typedef struct {
    char estados[MAX_ESTADOS][10];
    int num_estados;

    char alfabeto[MAX_ALFABETO][10];
    int num_alfabeto;

    Transicion transiciones[MAX_TRANSICIONES];
    int num_transiciones;

    char estado_inicial[10];

    char estados_finales[MAX_ESTADOS][10];
    int num_finales;
} Automata;

int es_final(Automata *a, const char *estado) {
    for (int i = 0; i < a->num_finales; i++) {
        if (strcmp(a->estados_finales[i], estado) == 0)
            return 1;
    }
    return 0;
}


char *transicion(Automata *a, const char *estado, const char *simbolo) {
    for (int i = 0; i < a->num_transiciones; i++) {
        if (strcmp(a->transiciones[i].origen, estado) == 0 &&
            strcmp(a->transiciones[i].simbolo, simbolo) == 0) {
            return a->transiciones[i].destino;
        }
    }
    return NULL;
}


int acepta(Automata *a, const char *cadena) {
    char estado_actual[10];
    strcpy(estado_actual, a->estado_inicial);

    for (int i = 0; i < strlen(cadena); i++) {
        char simbolo[2] = { cadena[i], '\0' };
        char *nuevo_estado = transicion(a, estado_actual, simbolo);

        if (nuevo_estado == NULL) {
            printf("No hay transicion definida para (%s, %s)\n", estado_actual, simbolo);
            return 0;
        }
        strcpy(estado_actual, nuevo_estado);
    }
    return es_final(a, estado_actual);
}


int split_line(char *linea, char tokens[][10], const char *delim) {
    int count = 0;
    char *token = strtok(linea, delim);
    while (token != NULL) {
        strcpy(tokens[count++], token);
        token = strtok(NULL, delim);
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <archivo_automata> <archivo_cadenas>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("No se pudo abrir el archivo de automata.\n");
        return 1;
    }

    Automata a;
    a.num_estados = a.num_alfabeto = a.num_transiciones = a.num_finales = 0;

    char linea[1024];


    fgets(linea, sizeof(linea), f);
    linea[strcspn(linea, "\n")] = 0;
    a.num_estados = split_line(linea, a.estados, ",");


    fgets(linea, sizeof(linea), f);
    linea[strcspn(linea, "\n")] = 0;
    a.num_alfabeto = split_line(linea, a.alfabeto, ",");


    fgets(linea, sizeof(linea), f);
    linea[strcspn(linea, "\n")] = 0;

    char partes[MAX_TRANSICIONES][50];
    int num = split_line(linea, partes, ",");
    for (int i = 0; i < num; i++) {
        char izq[20], der[20];
        sscanf(partes[i], "(%[^:]:%[^)])=%s", 
               a.transiciones[a.num_transiciones].origen, 
               a.transiciones[a.num_transiciones].simbolo, 
               a.transiciones[a.num_transiciones].destino);
        a.num_transiciones++;
    }

    fgets(linea, sizeof(linea), f);
    linea[strcspn(linea, "\n")] = 0;
    strcpy(a.estado_inicial, linea);

    fgets(linea, sizeof(linea), f);
    linea[strcspn(linea, "\n")] = 0;
    a.num_finales = split_line(linea, a.estados_finales, ",");

    fclose(f);

    printf("Automata cargado correctamente.\n");

    // Leer archivo de cadenas
    f = fopen(argv[2], "r");
    if (!f) {
        printf("No se pudo abrir el archivo de cadenas.\n");
        return 1;
    }

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = 0;
        if (strlen(linea) == 0) continue;

        int resultado = acepta(&a, linea);
        printf("%s -> %s\n", linea, resultado ? "ACEPTADA" : "RECHAZADA");
    }

    fclose(f);
    return 0;
}
