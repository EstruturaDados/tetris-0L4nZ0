//BIBLIOTECAS PARA USO NO CÓDIGO
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>


//DEFINIÇÃO DE CONSTANTES
#define MAX_PECAS 4     //maximo de tipos de peças disponivel 
#define MAX_FILA 5      //maximo de unidades em uma fila
#define MAX_PILHA 3     //maximo de unidades em uma pilha
#define MAX_LIXEIRA 5   //maximo de unidades em uma lixeira
const char PECAS [MAX_PECAS] = {'L', 'I', 'O', 'T'};    //array constante para alocar os tipos de peças disponiveis


//CRIAÇÃO DE STRUCTS
typedef struct Peca     //struct que simboliza uma peça, com um tipo(referente ao array PEÇA) e um ID
{
    char tipo;
    int id;
}Peca;

typedef struct Fila     //struct que simboliza a fila principal, ela contem um array de peças(tipo e id), possui inicio para remoção, fim para adição e quantidade para calculos
{
    Peca pecas[MAX_FILA];
    int inicio, fim, quantidade;
    
}Fila;

typedef struct Pilha    //struct que simboliza a pilha de reservas, ela contem um array de peças(tipo e id), e possui um fim, tanto para adição e remoção e a quantidade para calculos
{
    Peca pecas[MAX_PILHA];
    int fim, quantidade;
}Pilha;

typedef struct Lixeira  //struct que simboliza a lixeira para reversão de jogadas, aloca a ultima peça jogada, e um estado(0 - vazia, 1 - possui uma peça para jogar)
{
    Peca ultima_jogada[MAX_LIXEIRA];
    int fim, quantidade;
}Lixeira;


//INICIALIZAÇÃO PRÉVIA DAS FUNÇÕES

//geração de peça
Peca gerar_peca();

//exibição de linha
void linha(char caractere, int quantidade)  ;

//exibição de estado das filas e pilhas
void exibir_estado(Fila *fila, Pilha *pilha, Lixeira *lixeira);

//exibição do menu
int exibir_menu();

//funções de verificação
int verificar_fila_cheia(Fila *fila_aux);
int verificar_pilha_cheia(Pilha *pilha_aux);
int verificar_lixeira_cheia(Lixeira *lixeira_aux);

//funções de inicialização
Fila *inicializar_fila();
Pilha *inicializar_pilha();
Lixeira *inicializar_lixeira();

//funções de manipulação da fila
void inserir_em_fila(Fila *fila_aux, Peca peca_aux);
void remover_da_fila(Fila *fila_aux, Lixeira *lixeira_aux);

//funções de manipulação da pilha
void inserir_em_pilha(Fila *fila_aux, Pilha *pilha_aux);
void remover_da_pilha(Pilha *pilha_aux, Lixeira *lixeira_aux);

//funções de manipulação da lixeira
void inserir_em_lixeira(Peca peca_aux, Lixeira *lixeira_aux);
Peca repassar_da_lixeira(Lixeira *lixeira_aux);
void remover_da_lixeira(Lixeira *lixeira_aux) ;

//funções de troca
void trocar_uma_peca(Fila *fila_aux, Pilha *pilha_aux);
void inverter_fila_e_pilha(Fila *fila_aux, Pilha *pilha_aux);
void desfazer_jogada(Fila *fila_aux, Lixeira *lixeira_aux);


//função main
int main(){

    setlocale(LC_ALL, "pt_br");
    srand(time(NULL));


    Fila *Fila_main = inicializar_fila();
    Pilha *Pilha_main = inicializar_pilha();
    Lixeira *Lixeira_main = inicializar_lixeira();

    int controlador_main = 1;
    int executor = 0;
    
    do
    {
        executor = 0;
        exibir_estado(Fila_main, Pilha_main, Lixeira_main);
        linha('=', 32);

        executor = exibir_menu();

        switch (executor)
        {
        case 1:
            remover_da_fila(Fila_main, Lixeira_main);
            break;

        case 2:
            inserir_em_pilha(Fila_main, Pilha_main);
            break;
        
        case 3:
            remover_da_pilha(Pilha_main, Lixeira_main);
            break;
        
        case 4:
            trocar_uma_peca(Fila_main, Pilha_main);
            break;
        
        case 5:
            desfazer_jogada(Fila_main, Lixeira_main);
            break;

        case 6:
            inverter_fila_e_pilha(Fila_main, Pilha_main);
            break;

        case 0:
            controlador_main--;
            break;

        default:
            printf("OPÇÃO INVÁLIDA!");
            break;
        }

    } while (controlador_main);
    

    free(Fila_main);
    free(Pilha_main);
    free(Lixeira_main);

    return 0;
}


//FUNÇÕES UTILITARIAS
Peca gerar_peca()   //geração de peça aleatória
{
    Peca peca_aux;  //cria uma peça que irá ser preenchida e retornada

    int i_peca = rand() % MAX_PECAS;    //forneçe um número aleatório de 0 a MAX_PECAS(4), que é usado como índice referente a constante de tipos de peça
    int i_id = 1 + rand() % 1000;   //cria um id random para a peça

    //atribui o valor as peças
    peca_aux.tipo = PECAS[i_peca];
    peca_aux.id = i_id;
                
    return peca_aux;    //retorna a peça criada

}

void linha(char caractere, int quantidade)      //função para imprimir uma sequencia de caracteres iguais, formando uma linha
{
    printf("\n");

    for(int i = 0; i < quantidade; i++)     //loop que irá exercer a função de criar a linha
    {
        printf("%c", caractere);
    }
}

void exibir_estado(Fila *fila, Pilha *pilha, Lixeira *lixeira)      //exibe o estado das filas e pilhas
{
    printf("\n======= ESTADO ATUAL =======\n");

    // ----- FILA -----
    printf("\nFILA (%d itens):\n", fila->quantidade);
    if (fila->quantidade == 0)
    {
        printf("  [vazia]\n");
    }
    else
    {
        for (int i = 0; i < fila->quantidade; i++)
        {
            int pos = (fila->inicio + i) % MAX_FILA;
            printf("  pos %d -> Peça: %c \t Id: %d\n",
                   i + 1,
                   fila->pecas[pos].tipo,
                   fila->pecas[pos].id);
        }
    }

    // ----- PILHA -----
    printf("\nPILHA (%d itens):\n", pilha->quantidade);
    if (pilha->quantidade == 0)
    {
        printf("  [vazia]\n");
    }
    else
    {
        for (int i = pilha->quantidade - 1; i >= 0; i--)
        {
            printf("  topo-%d -> Peça: %c \t Id: %d\n",
                   i + 1,
                   pilha->pecas[i].tipo,
                   pilha->pecas[i].id);
        }
    }

    // ----- LIXEIRA (AGORA TIPO PILHA) -----
    printf("\nLIXEIRA (%d itens):\n", lixeira->quantidade);
    if (lixeira->quantidade == 0)
    {
        printf("  [vazia]\n");
    }
    else
    {
        for (int i = lixeira->quantidade - 1; i >= 0; i--)
        {
            printf("  topo-%d -> Peça: %c \t Id: %d\n",
                   i + 1,
                   lixeira->ultima_jogada[i].tipo,
                   lixeira->ultima_jogada[i].id);
        }
    }

    linha('=', 32);
}

int exibir_menu()       //exibe o menu, e retorna a escolha pra ser usada na execução
{
    int controlador = 1;
    int entrada;

    do      //loop feito para evitar entradas inválidas
    {
        entrada = 0;
        printf("\n -----MENU PRINCIPAL-----");
        printf("\n1 - Jogar peça");
        printf("\n2 - Reservar peça");
        printf("\n3 - Usar peça reservada");
        printf("\n4 - Trocar peça do topo da pilha com a da frente da fila");
        printf("\n5 - Desfazer última jogada");
        printf("\n6 - Inverter fila com pilha");
        printf("\n0 - Sair");

        printf("\nSelecione sua opção: ");
        if (scanf(" %d", &entrada) != 1) {
            // limpar entrada inválida
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            entrada = -1;
        }

        if (entrada >= 0 && entrada <= 6)
        {
            controlador = 0;
        }
        else
        {
            printf("\nInsira uma entrada válida\n");
        }

    } while (controlador);

    return entrada;
}





//FUNÇÕES DE VERIFICAÇÃO
int verificar_fila_cheia(Fila *fila_aux)    //função para verificar o estado da fila
{
   return fila_aux->quantidade == MAX_FILA ?  1 :  0;  //retorna 1 se a fila estiver cheia e 0 se estiver livre
}

int verificar_pilha_cheia(Pilha *pilha_aux)     //função para verificar o estado da pilha
{
    return pilha_aux->quantidade == MAX_PILHA ? 1 : 0; //retorna 1 se a pilha estiver cheia e 0 se estiver livre
}

int verificar_lixeira_cheia(Lixeira *lixeira_aux)   //função para verificar o estado da lixeira
{
    return lixeira_aux->quantidade == MAX_LIXEIRA ? 1 : 0;    //retorna 1 se a lixeira estiver cheia e 0 se estiver vazia
}


//FUNÇÕES DE INICIALIZAÇÃO
Fila *inicializar_fila()     //função para inicializar fila
{
    Fila *fila_aux = (Fila*) calloc(1,sizeof(Fila));
    
    if(!fila_aux)
    {
        printf("Erro ao alocar memória!");      //caso não consiga alocar a memória, exibe mensagem de erro, pula o ELSE e não retorna a fila
        
        return NULL;
    }
    
    else    //caso a fila seja alocada
    {
        for(int i = 0; i < MAX_FILA; i++)   //loop para andar pelos indices da fila
        {
            fila_aux->pecas[fila_aux->fim] = gerar_peca();      //adiciona uma peça alatória
            fila_aux->fim = (fila_aux->fim + 1) % MAX_FILA;     //aumenta o indice da fila usando lógica circular
            fila_aux->quantidade++;     //aumenta a quantidade de itens na fila
        }

        return fila_aux;       //caso tudo ocorra bem, retorna o ponteiro da fila
    }
}

Pilha *inicializar_pilha()   //funçao para incializar uma pilha
{
    Pilha *pilha_aux = (Pilha*) calloc (1, sizeof(Pilha));      //cria um ponteiro para a pilha

    if(!pilha_aux)  //condicional identica ao iniciar a fila, se der erro ao alocar, libera a pilha e nao retorna nada
    {
        printf("Erro ao alocar memória!");
        return NULL;
    }

    else
    {
        return pilha_aux;      //caso tudo ocorra bem, retorna o ponteiro da pilha
    }
}

Lixeira *inicializar_lixeira()       //funçao para incializar uma lixeira
{
    Lixeira *lixeira_aux = (Lixeira*) calloc (1, sizeof(Lixeira));      //cria um ponteiro para a lixeira

    if(!lixeira_aux)        //condicional identica, se der erro ao alocar, libera a lixeira e nao retorna nada
    {
        printf("Erro ao alocar memória!");
        return NULL;
    }
    
    else
    {
        return lixeira_aux;        //caso tudo ocorra bem, retorna o ponteiro da lixeira
    }
}


//FUNÇÃO DE INSERÇAO E REMOÇÃO DA FILA
void inserir_em_fila(Fila *fila_aux, Peca peca_aux)    //função para inserir peças na fila
{
    if(!fila_aux) return;

    if(!verificar_fila_cheia(fila_aux))     //verifica se a fila não está cheia
    {
        fila_aux->pecas[fila_aux->fim] = peca_aux;      //adiciona a peça no final da fila
        fila_aux->fim = (fila_aux->fim + 1) % MAX_FILA;     //atualiza o final da fila
        fila_aux->quantidade++;     //aumenta a quantidade
    }
    else
    {
        printf("Fila cheia, impossível inserir!");      //caso a fila estiver cheia, exibe mensagem de erro
        return;
    }

}

void remover_da_fila(Fila *fila_aux, Lixeira *lixeira_aux)  //função para jogar a peça da fila
{
    Peca temporaria; 
    
    if (fila_aux->quantidade)       //condicional para verificar se a fila possui itens
    {
        temporaria = fila_aux->pecas[fila_aux->inicio];    //cria uma peça temporaria para passar para a lixeira

        fila_aux->inicio = (fila_aux->inicio + 1) % MAX_FILA;       //atualiza o inicio da fila
        fila_aux->quantidade--;     //atualiza a quantidade da fila


        inserir_em_lixeira(temporaria, lixeira_aux);    //inseri a peça temporaria na lixeira

        inserir_em_fila(fila_aux, gerar_peca());  //mantém a fila cheia

    }

    else    //caso a lixeira esteja cheia, imprime o erro
    {
        printf("Erro ao jogar, fila vazia!");
    }
    
}


//FUNÇÃO DE INSERÇAO E REMOÇÃO DA PILHA
void inserir_em_pilha(Fila *fila_aux, Pilha *pilha_aux)     //função para inserir peça em pilha
{
    if(fila_aux->quantidade != 0 && !verificar_pilha_cheia(pilha_aux))     //verifica se a fila não está vazia e se a pilha não está cheia
    {
        Peca temporaria = fila_aux->pecas[fila_aux->inicio];        //cria uma peça temporaria, apenas para armazenar o valor da peça a sair da fila

        pilha_aux->pecas[pilha_aux->fim] = temporaria;       //repassa esse valor para a pilha
        pilha_aux->fim++;
        pilha_aux->quantidade++;


        fila_aux->inicio = (fila_aux->inicio + 1) % MAX_FILA;       //atualiza o inicio da fila
        fila_aux->quantidade--;     //atualiza a quantidade da fila

        inserir_em_fila(fila_aux, gerar_peca());      //já inseri mais um item, para manter a fila cheia
    }
}

void remover_da_pilha(Pilha *pilha_aux, Lixeira *lixeira_aux)
{
    if(pilha_aux->quantidade)
    {
        Peca temporaria = pilha_aux->pecas[pilha_aux->fim - 1];

        pilha_aux->fim--;
        pilha_aux->quantidade--;

        inserir_em_lixeira(temporaria, lixeira_aux);    //inseri a peça temporaria na lixeira

    }
}


//FUNÇÃO DE INSERÇAO E REMOÇÃO DA LIXEIRA
void inserir_em_lixeira(Peca peca_aux, Lixeira *lixeira_aux)  //função para inserir as peças na lixeira
{
    if(lixeira_aux == NULL) return;


    if(!verificar_lixeira_cheia(lixeira_aux))
    {
        // push normal
        lixeira_aux->ultima_jogada[lixeira_aux->fim] = peca_aux;
        lixeira_aux->fim++;
        lixeira_aux->quantidade++;
    }

    else
    {
        for(int i = 1; i < MAX_LIXEIRA; i ++)
        {
            lixeira_aux->ultima_jogada[i - 1] = lixeira_aux->ultima_jogada[i];        
        }

        // coloca o novo elemento no topo (última posição do array)
        lixeira_aux->fim = MAX_LIXEIRA;
        lixeira_aux->ultima_jogada[lixeira_aux->fim - 1] = peca_aux;


    }


}

Peca repassar_da_lixeira(Lixeira *lixeira_aux)     //função para retornar uma peça da lixeira(não remove a mesma de lá)
{
    if (lixeira_aux->quantidade == 0 || lixeira_aux == NULL)
    {
        printf("Lixeira vazia, impossível repassar.\n");
        
        // Retorna uma peça "nula" para evitar lixo de memória.
        Peca aux = {'?', -1};
        return aux;
    }
     
    return lixeira_aux->ultima_jogada[lixeira_aux->fim - 1];
}

void remover_da_lixeira(Lixeira *lixeira_aux)      //função para eliminar um item da lixeira
{

    if (!lixeira_aux || lixeira_aux->quantidade == 0) return;

    lixeira_aux->fim--;
    lixeira_aux->quantidade--;
}


//FUNÇÕES DE TROCAS
void trocar_uma_peca(Fila *fila_aux, Pilha *pilha_aux)      //função que troca apenas uma peça
{
    Peca peca_temp_fila, peca_temp_pilha;   //cria as peças temporárias

    peca_temp_fila = fila_aux->pecas[fila_aux->inicio];     //atribui o valor do primeiro da fila
    peca_temp_pilha = pilha_aux->pecas[pilha_aux->fim - 1];     //atribui o valor do último da pilha


    fila_aux->pecas[fila_aux->inicio] = peca_temp_pilha;    //inseri no inicio da fila a última da pilha

    pilha_aux->pecas[pilha_aux->fim - 1] = peca_temp_fila;      //inseri no fim da pilha, o primeiro da fila
    
}

void inverter_fila_e_pilha(Fila *fila_aux, Pilha *pilha_aux)        //função que inverte 3 valores da pilha e fila
{
    if(pilha_aux->quantidade < MAX_PILHA || fila_aux->quantidade < MAX_PILHA)
    {
        printf("Impossível inverter: fila e pilha precisam ter pelo menos %d itens.\n", MAX_PILHA);
        return;
    }

    //trocamos 3 posições (0,1,2)
    for (int i = 0; i < MAX_PILHA; i++)
    {
        int pos_fila = (fila_aux->inicio + i) % MAX_FILA;   // posição real na fila
        int pos_pilha = pilha_aux->fim - 1 - i;             // posição real na pilha

        Peca temp = fila_aux->pecas[pos_fila];
        
        fila_aux->pecas[pos_fila] = pilha_aux->pecas[pos_pilha];
        pilha_aux->pecas[pos_pilha] = temp;


    }

}

void desfazer_jogada(Fila *fila_aux, Lixeira *lixeira_aux)      //refaz a jogada enviando as peças da lixeira
{
    if(lixeira_aux->quantidade)
    {        

    Peca peca_restaurada = repassar_da_lixeira(lixeira_aux);

    fila_aux->inicio = (fila_aux->inicio - 1 + MAX_FILA) % MAX_FILA;
    fila_aux->pecas[fila_aux->inicio] = peca_restaurada;

    remover_da_lixeira(lixeira_aux);
    



    }
    
    else
    {
        printf("Impossível reverter, lixeira vazia!");
    }
}
