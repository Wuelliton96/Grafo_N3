#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100

typedef struct {
    int vertices;
    int arestas[MAX_VERTICES][MAX_VERTICES];
} Grafo;

void iniciarGrafo(Grafo* grafo, int vertices) {
    grafo->vertices = vertices;
    int i, j;
    for (i = 0; i < vertices; i++) {
        for (j = 0; j < vertices; j++) {
            grafo->arestas[i][j] = 0;
        }
    }
}

void adicionarAresta(Grafo* grafo, int origem, int destino, int peso) {
    grafo->arestas[origem][destino] = peso;
	grafo->arestas[destino][origem] = peso;
    printf("| Origem %d -> Destino %d =  %d    | \n", origem, destino, peso);
}

int menorDistancia(int distancias[], int visitados[], int vertices) {
    int i, min = INT_MAX, min_indice;
    for (i = 0; i < vertices; i++) {
        if (visitados[i] == 0 && distancias[i] <= min) {
            min = distancias[i];
            min_indice = i;
        }
    }
    return min_indice;
}

void imprimirCaminho(int anterior[], int destino) {
    if (anterior[destino] == -1) {
        printf("%d", destino);
        return;
    }
    imprimirCaminho(anterior, anterior[destino]);
    printf(" -> %d", destino);
}

void dijkstra(Grafo* grafo, int origem, int destino) {
    int distancias[MAX_VERTICES];
    int visitados[MAX_VERTICES];
    int anterior[MAX_VERTICES];

    int i, j;
    for (i = 0; i < grafo->vertices; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = 0;
        anterior[i] = -1;
    }
    
    distancias[origem] = 0; // não tem distancia e por isso que tem que inicia por 0
    
    for (i = 0; i < grafo->vertices - 1; i++) {
        int u = menorDistancia(distancias, visitados, grafo->vertices);
        visitados[u] = 1;

        for (j = 0; j < grafo->vertices; j++) {
            if (!visitados[j] && grafo->arestas[u][j] && distancias[u] != INT_MAX && distancias[u] + grafo->arestas[u][j] < distancias[j]) {
                distancias[j] = distancias[u] + grafo->arestas[u][j];
                anterior[j] = u;
            }
        }
    }
    printf("Menor caminho entre %d e %d: ", origem, destino);
    imprimirCaminho(anterior, destino);
    printf("\nDistancia percorrida: %d\n", distancias[destino]);
}

int main() {
    Grafo grafo;
    int vertices, peso;
    int origem, destino;

    FILE *arquivo = fopen("grafo.txt", "r");
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(arquivo, "%d", &vertices);

    iniciarGrafo(&grafo, vertices);

    while (fscanf(arquivo, "%d %d %d", &origem, &destino, &peso) != EOF) {
        adicionarAresta(&grafo, origem, destino, peso);
    }
    fclose(arquivo);    
    printf("\nDigite o ponto de origem: ");
    scanf("%d", &origem);
    printf("Digite o ponto de destino: ");
    scanf("%d", &destino);

    dijkstra(&grafo, origem, destino);

    return 0;
}