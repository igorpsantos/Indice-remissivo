#include <stdlib.h>
#include <stdio.h>
#include "lista.h"
#include <string.h>

struct Noh_palavra {
    TipoChave* diretorio;
    int linha;

    struct Noh_palavra* proximo;
};

struct Noh {
    TipoChave* palavra;
    int ocorrencias_total;

    struct ListaPalavras * ListaPalavras;

    struct Noh* proximo;
};
typedef struct Noh Noh;

struct ListaPalavras{
    Noh_palavra* cabeca;
};
typedef struct ListaPalavras ListaPalavras;

struct Lista {
    Noh* cabeca; // noh inicial (primeiro) da lista encadeada
};

Lista* cria(void) {
    Lista* l = malloc(sizeof(Lista));
    l->cabeca = NULL; // indicacao de primeiro noh inexistente (lista vazia)
    return l;
}

ListaPalavras* cria_p(void) {
    ListaPalavras* l = malloc(sizeof(ListaPalavras));
    l->cabeca = NULL; // indicacao de primeiro noh inexistente (lista vazia)
    return l;
}

bool underflow(const Lista* l) {
    return l->cabeca == NULL;
}

bool overflow(const Lista* l) {
    // Faz uma alocacao "fake" de um noh para verificar
    // condicao da memoria
    Noh* f = malloc(sizeof(Noh));
    if (f == NULL) {
        return true; // "lista" (memoria) estah cheia!
    }
    free(f); // nao deixa lixo para tras
    return false;
}

void inserir_primeirapalavra(Lista* l, TipoChave* x, Posicao p, char * diretorio, int linha) {
    Noh* n = malloc(sizeof(Noh));

    n->palavra = malloc((strlen(x) + 1) * sizeof(char*));

    strcpy(n->palavra, x);

    n->ocorrencias_total = 1;

    switch (p) {
        case FIM:
            n->proximo = NULL;
            if (!underflow(l)) {
                Noh* u = l->cabeca;
                while (u->proximo != NULL) { // enquanto NAO for o ultimo
                    u = u->proximo;
                }
                // Na saida do laco, 'u' estah apontando para a cauda (ultimo)
                u->proximo = n;
                break; // o 'break' do 'case FIM' soh existe se !underflow
            }
            // ATENCAO: a falta de 'break' aqui eh INTENCIONAL, pois no caso
            // de lista vazia, a insercao no FIM eh igual a insercao no INICIO;
            // sem o 'break', a execucao continua pelo 'case INICIO' abaixo...

        case INICIO:
            // Eh NECESSARIO obedecer a ordem do proximo par de comandos,
            // do contrario - se elas forem invertidas - a referencia anterior
            // ao inicio da lista (e a tudo mais dela) serah perdida (substi-
            // tuida pelo endereco do novo noh)
            n->proximo = l->cabeca;
            l->cabeca = n;
            break;

        case ORDEM: // caso especial onde a posicao tem a ver com o valor
                    // da chave 'x' que estah sendo inserida; eh preciso
                    // percorrer a lista procurando o ponto certo onde o
                    // novo noh vai ser "enxertado"
            break;

        default:
            // Posicao VALOR nao eh valida para insercao
            puts("POSICAO DE INSERCAO INVALIDA!");
    }

    n->ListaPalavras = cria_p();

    Noh_palavra * np = malloc(sizeof(Noh_palavra));

    np->diretorio = malloc((strlen(diretorio)+1) * sizeof(char*));

    strcpy(np->diretorio, diretorio);

    np->linha = linha;

    np->proximo = n->ListaPalavras->cabeca;
    n->ListaPalavras->cabeca = np;

}

void inserir_palavrarepetida(Lista * m, char * temp, char * diretorio, int linha_palavra) {

    Noh * n = busca(m, temp);

    n->ocorrencias_total++;

    Noh_palavra* u = malloc(sizeof(Noh_palavra));

    u->diretorio = malloc((strlen(diretorio)+1) * sizeof(char*));

    strcpy(u->diretorio, diretorio);

    u->linha = linha_palavra;

    u->proximo = NULL;

           if (!underflow(n->ListaPalavras)) {
            Noh_palavra* x = n->ListaPalavras->cabeca;
                while (x->proximo != NULL) { // enquanto NAO for o ultimo
                    x = x->proximo;
                }
                // Na saida do laco, 'u' estah apontando para a cauda (ultimo)
                x->proximo = u;

            }
            // ATENCAO: a falta de 'break' aqui eh INTENCIONAL, pois no caso
            // de lista vazia, a insercao no FIM eh igual a insercao no INICIO;
            // sem o 'break', a execucao continua pelo 'case INICIO' abaixo...
}

void remover(Lista* l, TipoChave x, Posicao p) {
    // Presume-se que houve uma chamada anterior a underflow() para
    // verificar se a lista estava vazia ANTES da chamada a remover()


    Noh* n = l->cabeca; // faz acesso ao primeiro noh da lista
    switch (p) {
        case FIM:
            if (n->proximo != NULL) { // cabeca da lista NAO estah sozinha
                Noh* anterior = NULL; // memoria do endereco do noh anterior a 'n'
                while (n->proximo != NULL) {
                    anterior = n; // lembra quem era o anterior de 'n'
                    n = n->proximo;
                }

                anterior->proximo = NULL; // atualiza o anterior como nova cauda
                                          // (ultimo noh) da lista
                free(n);
                break; // break do 'case FIM' se noh nao era UNICO
            }
            // ATENCAO: a falta de 'break' aqui eh INTENCIONAL, pois no caso
            // de lista com um UNICO noh, a remocao do FIM eh igual a do INICIO;
            // sem o 'break', a execucao continua pelo 'case INICIO' abaixo...

        case INICIO:
            l->cabeca = n->proximo; // indica como nova cabeca aquele que
                                    // era ateh agora o segundo noh da lista

            free(n);
            break;

        case VALOR:
            break;

        default:
            // Posicao ORDEM nao eh valida para remocao
            puts("POSICAO/VALOR DE REMOCAO INVALIDA!");
    }
    //return r;
}

void destroi(Lista* l) {
    while (!underflow(l)) {
        Noh* n = l->cabeca;
        l->cabeca = n->proximo;
        free(n);
    }
    free(l);
}


void imprime(const Lista* l) {
    int qtd_palavras = 1;
    if (underflow(l)) {
        printf("(VAZIA)");
        return;
    }
    printf("(CABECA) \n");
    Noh* n = l->cabeca;
    Noh_palavra * np = n->ListaPalavras->cabeca; // cria uma lista de palavras
    Noh_palavra * aux = np->proximo;
    while (n != NULL) {
        printf("--------------------------------------\n");
        printf("%s : \trepeticoes : %d\n\n", n->palavra, n->ocorrencias_total); // imprime a chave

        while(np != NULL)
        {
            if(np == n->ListaPalavras->cabeca)
                printf("DIRETORIO: %s\nLINHA: %d \n\n", np->diretorio, np->linha);
            else {
                if(aux != NULL){
                    if(strcmp(np->diretorio, aux->diretorio) == 0)
                        printf("%d ", np->linha);
                    else printf("DIRETORIO: %s\nLINHA: %d \n\n", np->diretorio, np->linha);
                } else
                    printf("DIRETORIO: %s\nLINHA: %d \n\n", np->diretorio, np->linha);

            }

            np = np->proximo;
            aux = aux->proximo;
        }
        printf("---------------------------------------\n\n\n");
        //printf("-->");
        ++qtd_palavras;
        n = n->proximo;
    }
    printf("NULL (CAUDA)\n");
}
// funcao que busca uma palavra se existem palavras repetidas, retorno o noh da palavra ou returna nullo
Noh* busca(const Lista* l, TipoChave* x) {
    Noh* n = l->cabeca;

    while (n != NULL) { // serve tambem para verificar lista nula
        if (strcmp(n->palavra, x) == 0) {
            return n;
        }
        n = n->proximo; // continua busca pelo proximo noh
    }
    return NULL; // chegou ao fim sem encontrar a chave
}

// funcao que ordena as listas usando bubble_sort crescente
void ordena_listas(Lista* l) {

    Noh* n = l->cabeca;

    //char* aux = malloc((strlen(n->palavra) + 1) * sizeof(char*)); // malloc do mesmo tamanho da variavel noh palavra

    while (n != NULL) { // serve tambem para verificar lista nula

        Noh* i = l->cabeca;
        Noh* j = i->proximo;

        while(j != NULL){
        if (strcmp(i->palavra, j->palavra) > 0) { // se for menor que 0 é porque tem que ser inserido depois
                //troca palavra
                char* aux = i->palavra;
                i->palavra = j->palavra;
                j->palavra = aux;
                // troca ocorrencias
                int oc = i->ocorrencias_total;
                i->ocorrencias_total = j->ocorrencias_total;
                j->ocorrencias_total = oc;
                //trocas as listas, trocando as linhas tbm
                ListaPalavras * op = i->ListaPalavras;
                i->ListaPalavras = j->ListaPalavras;
                j->ListaPalavras = op;

            }
            j = j->proximo;
            i = i->proximo;
        }
        n = n->proximo; // continua busca pelo proximo noh
    }
}

void busca_pchave(const Lista* l, TipoChave* x) {
    Noh* n = l->cabeca;


    bool cond = false;

    while(n != NULL)
    {
         if (strcmp(x, n->palavra) == 0)
         {
            cond = true;
            break;
        }
        else n = n->proximo;
    }

    if(cond == true)
    {
        Noh_palavra* u = n->ListaPalavras->cabeca;// noh palavra
        Noh_palavra* ant = n->ListaPalavras->cabeca;// noh palavra

        while (u != NULL) {// serve tambem para verificar lista nula
                if(u == n->ListaPalavras->cabeca)
                printf("\nDIRETORIO: %s\nLINHA: %d ", u->diretorio, u->linha);
            else {
                if(u->proximo != NULL){
                    if(strcmp(u->diretorio, u->proximo->diretorio) == 0)
                        printf("%d ", u->linha);
                    else printf("\n\nDIRETORIO: %s\nLINHA: %d ", u->diretorio, u->linha);
                } else {

                    if(strcmp(ant->diretorio, u->diretorio) == 0){
                        printf("%d ", u->linha);
                    }else printf("\n\nDIRETORIO: %s\nLINHA: %d \n\n", u->diretorio, u->linha);
                }
            }
            ant = u;
            u = u->proximo; // continua busca pelo noh_palavra junto com o noh

        }
        printf("\n\nTotal de ocorrencias = %d\n", n->ocorrencias_total);

    } else return printf("A palavra chave nao existe!!!\n"); // chegou ao fim sem encontrar a chave
}

void salvar_listas(const Lista* l){
    Noh* n = l->cabeca; // noh para percorrer a lista inteira
    Noh_palavra* np = n->ListaPalavras->cabeca; // cabeca da listas palavra

    FILE* lista1 = fopen("c:/users/igor1/desktop/lista1.txt", "w");
    FILE* lista2 = fopen("c:/users/igor1/desktop/lista2.txt", "w");
    if(lista1 == NULL){
        perror("NAO FOI POSSIVEL SALVAR A LISTA....\n");
    }

    while(n != NULL){
        fprintf(lista1,"%s;%d\n", n->palavra, n->ocorrencias_total);
        n = n->proximo;
    }
    while(np != NULL){
        fprintf(lista2,"%s\n%d\n\n", np->diretorio, np->linha);
        np = np->proximo;
    }
    printf("LISTA SALVA COM SUCESSO NA SUA AREA DE TRABALHO!\n");
    system("PAUSE");
    fclose(lista1);
    fclose(lista2);
}
