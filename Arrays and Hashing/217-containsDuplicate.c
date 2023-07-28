#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*

Leetcode 217: Contains Duplicates
Given an integer array nums, return true if any value appears at least 
twice in the array, and return false if every element is distinct.

O QUE APRENDI RESOLVENDO ESTE PROBLEMA:
- BigO: Usar um for dentro de outro para pesquisar no array se há elementos
que se repetem faz com que o tempo de execução seja O(n^2), não sendo a resposta
ideal. Por isso, para que o tempo de execução seja O(n), é necessário utilizar
uma tabela Hash.
- Problemas de alocação de memória: em versões anteriores deste código estava 
ocorrendo um "Heap-Buffer-Overflow", que foi fixado utilizando a função
"liberaHash()" no lugar apropriado da função "containsDuplicate()"

*/

/*
Leetcode:
Input
nums = [3,1]

Output == true  ->  ?????
Expected == false 

FIX:
change ha->itens[newPos] != NULL to ha->itens[newPos]->n == chave

*/

struct hash {
    int qtd, TABLE_SIZE;
    struct numbers **itens;
};

struct numbers{
    int n;
}Numeros;

typedef struct hash Hash;

//Criando tabela Hash
Hash* criaHash(int TABLE_SIZE) {
    Hash* ha = (Hash*) malloc(sizeof(Hash));
    if (ha != NULL) {
        int i;
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->itens = (struct numbers**) malloc(TABLE_SIZE * sizeof(struct numbers*));
        if (ha->itens == NULL) {
            free(ha);
            return NULL;
        }
        ha->qtd = 0;
        for (i = 0; i < ha->TABLE_SIZE; i++)
            ha->itens[i] = NULL;
    }
    return ha;
}

/*Destruindo tabela Hash. OBS: dá pra usar apenas a função free() em 
containsDuplicate()*/
void liberaHash(Hash* ha) {
    if (ha != NULL) {
        int i;
        for (i = 0; i < ha->TABLE_SIZE; i++) {
            if (ha->itens[i] != NULL)
                free(ha->itens[i]);
        }
        free(ha->itens);
        free(ha);
    }
}

//Chave para tabela Hash
int chaveDivisao(int chave, int TABLE_SIZE) {
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

//Checa para colisoes
int sondagemLinear(int pos, int i, int TABLE_SIZE) {
    return ((pos + i) & 0x7FFFFFFF) % TABLE_SIZE;
}

//Insere na tabela checando para colisoes
int insereHash_EnderAberto(Hash* ha, struct numbers num) {
    int test = 0;
    if(ha == NULL || ha->qtd == ha->TABLE_SIZE)
        return 0;
    int chave = num.n;
    int i, pos, newPos;
    pos = chaveDivisao(chave, ha->TABLE_SIZE);
    for(i = 0; i < ha->TABLE_SIZE; i++) {
        newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
        if(ha->itens[newPos] == NULL) {
            struct numbers* novo;
            novo = (struct numbers*) malloc(sizeof(struct numbers));
            if (novo == NULL)
                return 0;
            *novo = num;
            ha->itens[newPos] = novo;
            ha->qtd++;
            return 0;
        } else if (ha->itens[newPos]->n == chave){
            return 1;
        }
    }
    return 0;
}

//Escreve a tabela Hash na tela
void printarTabela(Hash* ha) {
    printf("---   TABELA HASH   ---\n");
    printf("---   TABLE_SIZE: %d ---\n", ha->TABLE_SIZE);
    int i;
    for(i = 0; i < ha->TABLE_SIZE; i++) {
        if (ha->itens[i]) {
            printf("Indice: %d, Chave/Valor: %d\n", i, ha->itens[i]->n);
        }
    }
}

bool containsDuplicate(int* nums, int numsSize) {
    int i;
    bool collisions;
    Hash* ha;

    ha = criaHash(numsSize);

    for(i = 0; i < numsSize; i++) {
        Numeros.n = nums[i];
        collisions = insereHash_EnderAberto(ha, Numeros);
        //printarTabela(ha);
        if (collisions != 0)
            return collisions;
    }

    //printarTabela(ha);

    //Alternativa ao liberaHash(ha): free()
    liberaHash(ha);
    return collisions;
}

int main() {
    system("cls");
    int nums[256], numsSize;

    scanf("%d", &numsSize);

    for (int i = 0; i < numsSize; i++) {
       scanf("%d", &nums[i]); 
    }

    printf("%d\n", containsDuplicate(nums, numsSize));

    return 0;
}