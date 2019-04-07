#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
typedef char TipoChave;
#define MAX_PALAVRA 43
// Definicao opaca do TAD: lista linear
struct Lista; // (pode ser encadeada ou nao)
typedef struct Lista Lista;
typedef struct ListaPalavras ListaPalavras;
typedef struct Noh Noh;
typedef struct Noh_palavra Noh_palavra;
// Operacoes (interface) do TAD
Lista* cria(void);
void destroi(Lista*);
bool underflow(const Lista*);
bool overflow(const Lista*);
void imprime(const Lista*);
Noh* busca(const Lista*, TipoChave*);
void ordena_listas(Lista*);
void busca_pchave(const Lista*, TipoChave*);
void salvar_listas(const Lista*);

enum Posicao { INICIO, // para inserir e remover
               FIM,    // idem
               ORDEM,  // para insercao em ordem
               VALOR   // para remocao de uma chave
};
typedef enum Posicao Posicao;

void inserir_primeirapalavra(Lista*, TipoChave*, Posicao, char*, int);
void inserir_palavrarepetida(Lista*, char*, char*, int);
void remover(Lista*, TipoChave, Posicao);

#endif // LISTA_H

