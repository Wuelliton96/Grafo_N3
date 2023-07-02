#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_VERTICES 100

// Estrutura para representar um grafo
typedef struct {
    int vertices;
    int matrizAdj[MAX_VERTICES][MAX_VERTICES];
} Grafo;

// Função para inicializar o grafo
void inicializarGrafo(Grafo* grafo, int vertices) {
    grafo->vertices = vertices;
    int i, j;
    for (i = 0; i < vertices; i++) {
        for (j = 0; j < vertices; j++) {
            grafo->matrizAdj[i][j] = 0;
        }
    }
}

// Função auxiliar para converter uma letra para um número (índice do vértice)
int letraParaIndice(char letra) {
    return letra - 'A';
}

// Função auxiliar para converter um número (índice do vértice) para uma letra
char indiceParaLetra(int indice) {
    return indice + 'A';
}

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Grafo* grafo, char origem, char destino, int peso) {
    int indiceOrigem = letraParaIndice(origem);
    int indiceDestino = letraParaIndice(destino);
    grafo->matrizAdj[indiceOrigem][indiceDestino] = peso;
}

// Função auxiliar para encontrar o vértice com a menor distância
int encontrarMenorDistancia(int distancias[], int visitados[], int vertices) {
    int i, min = INT_MAX, min_indice;
    for (i = 0; i < vertices; i++) {
        if (visitados[i] == 0 && distancias[i] <= min) {
            min = distancias[i];
            min_indice = i;
        }
    }
    return min_indice;
}
// Função para imprimir o caminho mínimo encontrado
void imprimirCaminho(int pais[], int destino) {
    if (pais[destino] == -1) {
        printf("%c", indiceParaLetra(destino));
        return;
    }
    imprimirCaminho(pais, pais[destino]);
    printf(" -> %c", indiceParaLetra(destino));
}

// Função que implementa o algoritmo de Dijkstra para encontrar o menor caminho
void dijkstra(Grafo* grafo, char origem, char destino) {
    int distancias[MAX_VERTICES];
    int visitados[MAX_VERTICES];
    int pais[MAX_VERTICES];

    int i, j;
    for (i = 0; i < grafo->vertices; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = 0;
        pais[i] = -1;
    }

    int indiceOrigem = letraParaIndice(origem);
    int indiceDestino = letraParaIndice(destino);

    distancias[indiceOrigem] = 0;

    for (i = 0; i < grafo->vertices - 1; i++) {
        int u = encontrarMenorDistancia(distancias, visitados, grafo->vertices);
        visitados[u] = 1;

        for (j = 0; j < grafo->vertices; j++) {
            if (!visitados[j] && grafo->matrizAdj[u][j] && distancias[u] != INT_MAX && distancias[u] + grafo->matrizAdj[u][j] < distancias[j]) {
                distancias[j] = distancias[u] + grafo->matrizAdj[u][j];
                pais[j] = u;
            }
        }
    }
    printf("Menor caminho entre %c e %c: ", origem, destino);
    imprimirCaminho(pais, indiceDestino);
    printf("\nDistancia total: %d\n", distancias[indiceDestino]);
}

int main() {
    Grafo grafo;
    int vertices, peso;
    char origem, destino;

    FILE *arquivo = fopen("grafo.txt", "r"); // Abrindo o arquivo para leitura
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(arquivo, "%d", &vertices);

    inicializarGrafo(&grafo, vertices);

    while (fscanf(arquivo, " %c %c %d", &origem, &destino, &peso) != EOF) {
        adicionarAresta(&grafo, origem, destino, peso);
    }

    fclose(arquivo);
    
    printf("Digite o ponto de origem: ");
    scanf(" %c", &origem);
    printf("Digite o ponto de destino: ");
    scanf(" %c", &destino);

    dijkstra(&grafo, origem, destino);

    return 0;
}