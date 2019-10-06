#include <random>
#include "Classes/MenuPrincipal.h"
#include "Classes/Grafo.h"
#include "Classes/ListaArestas.h"
#include "Classes/No.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <cstdlib>



using namespace std;

/**
 * Metodo para leitura dos dados do arquivo e criacao do grafo de acordo com sua definicao.
 * @param arqEntrada caminho do arquivo de entrada.
 * @param direcionado 0 para grafo nao direcionado e 1 para grafo direcionado.
 * @param ponderadoAresta 0 para grafo nao ponderado nas arestas e 1 para grafo ponderado nas arestas.
 * @param ponderadoNo 0 para grafo nao ponderado nos vertices e 1 para grafo ponderado nos vertices.
 * @return retorna o grafo criado.
 */
Grafo* leituraDados(char *arqEntrada, int direcionado, int ponderadoAresta, int ponderadoNo, char* arqSaida){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNoOrigem;
    int idNoDestino;
    int ordem;
    ifstream file;

    file.open(arqEntrada);

    //Pegando a ordem do grafo
    file >> ordem;

    //Criando objeto grafo
    Grafo* graf = new Grafo(ordem, direcionado, ponderadoAresta, ponderadoNo);

    //Leitura de arquivo
    if(!graf->ehPonderadoAresta() && !graf->ehPonderadoVertice())
        while(file >> idNoOrigem >> idNoDestino)
            graf->addAresta(idNoOrigem, idNoDestino, 0);

    else if(graf->ehPonderadoAresta() && !graf->ehPonderadoVertice() ){
        float pesoAresta;
        while(file >> idNoOrigem >> idNoDestino >> pesoAresta)
            graf->addAresta(idNoOrigem, idNoDestino, pesoAresta);
    }

    else if(graf->ehPonderadoVertice() && !graf->ehPonderadoAresta()){
        float pesoNoOrigem, pesoNoDestino;
        while(file >> idNoOrigem >> pesoNoOrigem >> idNoDestino >> pesoNoDestino) {
            graf->addAresta(idNoOrigem, idNoDestino, 0);
            graf->buscaNo(idNoOrigem)->setPeso(pesoNoOrigem);
            graf->buscaNo(idNoDestino)->setPeso(pesoNoDestino);
        }
    }

    else if(graf->ehPonderadoVertice() && graf->ehPonderadoAresta()){
        float pesoNoOrigem, pesoNoDestino, pesoAresta;
        while(file >> idNoOrigem >> pesoNoOrigem >> idNoDestino >> pesoNoDestino >> pesoAresta) {
            graf->addAresta(idNoOrigem, idNoDestino, pesoAresta);
            graf->buscaNo(idNoOrigem)->setPeso(pesoNoOrigem);
            graf->buscaNo(idNoDestino)->setPeso(pesoNoDestino);
        }
    }

    file.close();
    return graf;
}

int main(int argc, char* argv[]) {
    int opcaoEscolhida;
    auto inicio = std::chrono::high_resolution_clock::now();

    if(argc != 6) {
        cout << "Argumentos insuficientes" << endl;
        return 1;
    }


    do
    {
        opcaoEscolhida = MenuPrincipal::ReadOption();
        switch(opcaoEscolhida){
            case 1:{
                Grafo* x;
                x = leituraDados(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
                cout << "No inicial busca em largura: ";
                int idNo;
                cin >> idNo;
                x->buscaEmLargura(idNo);
                delete x;
            }break;
            case 2:{
                Grafo* x;
                x = leituraDados(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
                cout << "No inicial busca em profundidade: ";
                int idNo;
                cin >> idNo;
                x->buscaProfundidade(idNo);
                delete x;
            }break;
            case 3:{
                Grafo* x;
                x = leituraDados(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
                int idNo1, idNo2;
                cout << "Vertice 1: ";
                cin >> idNo1;
                cout << "Vertice 2: ";
                cin >> idNo2;
                x->algFloyd(idNo1, idNo2);
                delete x;
            }break;
            case 4:{
                Grafo* x;
                x = leituraDados(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
                int idNo1, idNo2;
                cout << "Vertice 1: ";
                cin >> idNo1;
                cout << "Vertice 2: ";
                cin >> idNo2;
                x->menorCaminhoDijkstra(idNo1, idNo2);
                delete x;
            }break;
            case 5:{
                Grafo* x;
                x = leituraDados(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
                x->imprimirArestas();
                delete x;
            }break;
        }
    }while(opcaoEscolhida != MenuPrincipal::FINAL_APLICACAO);


    auto resultado = std::chrono::high_resolution_clock::now() - inicio;
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(resultado).count();
    cout << "Tempo em segundos: " << seconds << endl;
}
