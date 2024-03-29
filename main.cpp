#include <random>
#include "MenuPrincipal.h"
#include "Grafo.h"
#include "CaixeiroViajante.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <tuple>
#include <iomanip>
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
Grafo* leituraDados(char *arqEntrada, bool direcionado, bool ponderadoAresta, bool ponderadoNo, char* arqSaida){

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
    srand(time(NULL));
    int opcaoEscolhida;
    auto inicio = std::chrono::high_resolution_clock::now();

    if(argc == 4){
        ofstream file;
        char* arquivoEntrada = argv[1];
        char* arquivoSaida = argv[2];
        int opcao = atoi(argv[3]);

        if(opcao == 1){

            Grafo* x = CaixeiroViajante::BuildTSPGraphFromFile(arquivoEntrada);
            CaixeiroViajante* cv = new CaixeiroViajante();
            ExecutionParams execGuloso = cv->ExecGuloso(x);

            file.open(arquivoSaida, ofstream::ios_base::app);
            file << fixed;
            file.precision(5);
            file << "[Guloso] " << arquivoEntrada << ":";
            file << " - Peso: " << execGuloso.totalHeight;
            file << " - Tempo: " << execGuloso.timeInSeconds << endl;
            file.close();

            delete x;
            delete cv;
        } else if(opcao == 3){

            Grafo* x = CaixeiroViajante::BuildTSPGraphFromFile(arquivoEntrada);
            CaixeiroViajante* cv = new CaixeiroViajante();
            ExecutionParams execGulosoRandomizadoReativo = cv->ExecRandomizadoReativo(x);

            file.open(arquivoSaida, ofstream::ios_base::app);
            file << fixed;
            file.precision(5);
            file << "[Reativo] " << arquivoEntrada << ":";
            file << " - Peso: " << execGulosoRandomizadoReativo.totalHeight;
            file << " - Tempo: " << execGulosoRandomizadoReativo.timeInSeconds;
            file << " - Alfa: " << execGulosoRandomizadoReativo.alpha << endl;
            file.close();

            delete x;
            delete cv;
        } else {
            cout << "Opção inválida para execução por linha de comando";
            return 1;
        }
    } else if(argc == 5){
        ofstream file;
        char* arquivoEntrada = argv[1];
        char* arquivoSaida = argv[2];
        double alfa = atoff(argv[4]);

        Grafo* x = CaixeiroViajante::BuildTSPGraphFromFile(arquivoEntrada);
        CaixeiroViajante* cv = new CaixeiroViajante();
        ExecutionParams execGulosoRandomizado = cv->ExecRandomizado(x, alfa);

        file.open(arquivoSaida, ofstream::ios_base::app);
        file << fixed;
        file.precision(5);
        file << "[Randomizado] " << arquivoEntrada << ":";
        file << " - Peso: " << execGulosoRandomizado.totalHeight;
        file << " - Tempo: " << execGulosoRandomizado.timeInSeconds;
        file << " - Alfa: " << execGulosoRandomizado.alpha << endl;
        file.close();

        delete x;
        delete cv;
    } else if(argc != 3) {
        cout << "Argumentos insuficientes" << endl;
        return 1;
    } else {
        char* arquivoEntrada = argv[1];
        char* arquivoSaida = argv[2];
        bool isDirecionado;
        bool isPonderadoAresta;
        bool isPonderadoNo;

        int x;
        cout << "Qual o tipo de Grafo: " << endl;
        cout << "1) - Orientado e Ponderado nas Arestas " << endl;
        cout << "2) - Orientado e Nao podenrado nas Arestas" << endl;
        cout << "3) - Nao Orientado e Ponderado nas Arestas " << endl;
        cout << "4) - Nao Orientado e Nao ponderado nas Arestas" << endl;
        cin >> x;
        if(x == 1){
            isDirecionado = true;
            isPonderadoAresta = true;
        }else if(x == 2){
            isDirecionado = true;
            isPonderadoAresta = false;
        }else if(x == 3){
            isDirecionado = false;
            isPonderadoAresta = true;
        }else if(x == 4){
            isDirecionado = false;
            isPonderadoAresta = false;
        }
        cout << endl;

        ofstream file;
        file.open(arquivoSaida, ofstream::ios_base::app);

        string algoritmoEscolhido;

        do
        {

            opcaoEscolhida = MenuPrincipal::ReadOption();
            switch(opcaoEscolhida){
                case 1:{
                    algoritmoEscolhido = " busca em largura ";
                    Grafo* x;
                    x = leituraDados(arquivoEntrada, isDirecionado, isPonderadoAresta, isPonderadoNo, arquivoSaida);
                    cout << "No inicial busca em largura: ";
                    int idNo;
                    cin >> idNo;

                    auto inicioFuncoes = std::chrono::high_resolution_clock::now();
                    x->buscaEmLargura(idNo, arquivoSaida);
                    delete x;

                    auto resultadoFuncoes = std::chrono::high_resolution_clock::now() - inicioFuncoes;
                    long long resulFunc = std::chrono::duration_cast<std::chrono::milliseconds>(resultadoFuncoes).count();
                    file << " Tempo de execucao em ms" << algoritmoEscolhido << ": " << resulFunc << endl;
                }break;
                case 2:{
                    algoritmoEscolhido = " busca em profundidade ";
                    Grafo* x;
                    x = leituraDados(arquivoEntrada, isDirecionado, isPonderadoAresta, isPonderadoNo, arquivoSaida);
                    cout << "No inicial busca em profundidade: ";
                    int idNo;
                    cin >> idNo;

                    auto inicioFuncoes = std::chrono::high_resolution_clock::now();
                    x->buscaProfundidade(idNo, arquivoSaida);
                    delete x;

                    auto resultadoFuncoes = std::chrono::high_resolution_clock::now() - inicioFuncoes;
                    long long resulFunc = std::chrono::duration_cast<std::chrono::milliseconds>(resultadoFuncoes).count();
                    file << " Tempo de execucao em ms" << algoritmoEscolhido << ": " << resulFunc << endl;
                }break;
                case 3:{

                    algoritmoEscolhido = " floyd ";
                    Grafo* x;
                    x = leituraDados(arquivoEntrada, isDirecionado, isPonderadoAresta, isPonderadoNo, arquivoSaida);
                    int no1, no2;
                    cout << "No 1: ";
                    cin >> no1;
                    cout << "No 2: ";
                    cin >> no2;

                    auto inicioFuncoes = std::chrono::high_resolution_clock::now();
                    x->algFloyd(no1, no2, arquivoSaida);
                    delete x;

                    auto resultadoFuncoes = std::chrono::high_resolution_clock::now() - inicioFuncoes;
                    long long resulFunc = std::chrono::duration_cast<std::chrono::milliseconds>(resultadoFuncoes).count();
                    file << " Tempo de execucao em ms" << algoritmoEscolhido << ": " << resulFunc << endl;
                }break;
                case 4:{
                    algoritmoEscolhido = " dijsktra ";
                    Grafo* x;
                    x = leituraDados(arquivoEntrada, isDirecionado, isPonderadoAresta, isPonderadoNo, arquivoSaida);
                    int no1, no2;
                    cout << "No 1: ";
                    cin >> no1;
                    cout << "No 2: ";
                    cin >> no2;

                    auto inicioFuncoes = std::chrono::high_resolution_clock::now();
                    x->menorCaminhoDijkstra(no1, no2, arquivoSaida);
                    delete x;

                    auto resultadoFuncoes = std::chrono::high_resolution_clock::now() - inicioFuncoes;
                    long long resulFunc = std::chrono::duration_cast<std::chrono::milliseconds>(resultadoFuncoes).count();
                    file << " Tempo de execucao em ms" << algoritmoEscolhido << ": " << resulFunc << endl;
                    file << endl;
                }break;
                case 5:{
                    algoritmoEscolhido = " prim ";
                    Grafo* x;
                    x = leituraDados(arquivoEntrada, isDirecionado, isPonderadoAresta, isPonderadoNo, arquivoSaida);

                    auto inicioFuncoes = std::chrono::high_resolution_clock::now();
                    x->Prim(arquivoSaida);
                    delete x;

                    auto resultadoFuncoes = std::chrono::high_resolution_clock::now() - inicioFuncoes;
                    long long resulFunc = std::chrono::duration_cast<std::chrono::milliseconds>(resultadoFuncoes).count();
                    file << " Tempo de execucao em ms" << algoritmoEscolhido << ": " << resulFunc << endl;
                } break;
                case 6:{

                    algoritmoEscolhido = " kruskal ";
                    Grafo* y;
                    y = leituraDados(arquivoEntrada, isDirecionado, isPonderadoAresta, isPonderadoNo, arquivoSaida);

                    auto inicioFuncoes = std::chrono::high_resolution_clock::now();
                    y->kruskal(arquivoSaida);
                    delete y;

                    auto resultadoFuncoes = std::chrono::high_resolution_clock::now() - inicioFuncoes;
                    long long resulFunc = std::chrono::duration_cast<std::chrono::milliseconds>(resultadoFuncoes).count();
                    file << " Tempo de execucao em ms" << algoritmoEscolhido << ": " << resulFunc << endl;
                }break;
                case 7 :{
                    Grafo* x;
                    x = leituraDados(arquivoEntrada, isDirecionado, isPonderadoAresta, isPonderadoNo, arquivoSaida);
                    x->imprimirArestas();
                    delete x;
                }break;
                case 8:{

                    Grafo* x = CaixeiroViajante::BuildTSPGraphFromFile(arquivoEntrada);
                    CaixeiroViajante* cv = new CaixeiroViajante();

                    ExecutionParams execGuloso = cv->ExecGuloso(x);
                    file << "[Guloso] " << arquivoEntrada << ":";
                    file << " - Peso: " << execGuloso.totalHeight;
                    file << " - Tempo: " << execGuloso.timeInSeconds << endl;

                    ExecutionParams execGulosoRandomizado = cv->ExecRandomizado(x, 0.1);
                    file << "[Randomizado] " << arquivoEntrada << ":";
                    file << " - Peso: " << execGulosoRandomizado.totalHeight;
                    file << " - Tempo: " << execGulosoRandomizado.timeInSeconds;
                    file << " - Alfa: " << execGulosoRandomizado.alpha << endl;

                    ExecutionParams execGulosoRandomizadoReativo = cv->ExecRandomizadoReativo(x);
                    file << "[Reativo] " << arquivoEntrada << ":";
                    file << " - Peso: " << execGulosoRandomizadoReativo.totalHeight;
                    file << " - Tempo: " << execGulosoRandomizadoReativo.timeInSeconds;
                    file << " - Alfa: " << execGulosoRandomizadoReativo.alpha << endl;

                    delete x;
                    delete cv;

                }break;
            }
        }while(opcaoEscolhida != MenuPrincipal::FINAL_APLICACAO);
    }
}

