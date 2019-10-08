#ifndef TRABALHOGRAFOS_GRAFO_H
#define TRABALHOGRAFOS_GRAFO_H


#include <vector>
#include "No.h"
#include "ListaArestas.h"

class Grafo {
public:
	Grafo(int ordem, bool orientado, bool ponderado_aresta, bool ponderado_vertice);
    ~Grafo();


    void insereNo(int idNo);
    void removeNo(int idNo);
    No* buscaNo(int idNo);
    void addAresta(int idVertice1, int idVertice2, int peso);
    void removeAresta(int idVertice1, int idVertice2);
    void imprimirVertices();
    void imprimirArestas();
    void grauEntrada(int idNo);
    void grauSaida(int idNo);
    bool existeAresta(int idVertice1, int idVertice2);
    float getPesoAresta(int idVertice1, int idVertice2);
    bool existeVertice(int idVertice);
    bool ehPonderadoVertice();
    bool ehPonderadoAresta();
    int getOrdem();
    void algFloyd(int idVertice1, int idVertice2);
    void buscaProfundidade(int idVertice);
    void buscaEmLargura(int inicio);
    void menorCaminhoDijkstra(int v, int vN);
    bool verificaVisit(bool vet[], int n);
    void AGCMPrim();


private:
    // variavel para acompanhar o tamanho do grafo no momento que o no eh adicionado.
    int n;
    int ordem;
    No* primeiro;
    No* ultimo;
    bool orientado;
    bool ponderado_aresta;
    bool ponderado_vertice;

    No* buscaNoIndice(int indice);
    float getPesoArestaIndice(int indice1, int indice2);

    vector<Aresta *> GetAllArestas(No *_no, bool _isSorted);
    vector<No *> GetAllNos(No *_no, bool _isSorted);
    vector<No*> RemoveNoOutSolucao(vector<No *> _outSolucao, No *_target);
    static bool CompareArestas(Aresta *a, Aresta *b);
    static bool CompareNos(No *a, No *b);
};


#endif //TRABALHOGRAFOS_GRAFO_H