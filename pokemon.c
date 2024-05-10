#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NOMES 100

typedef struct{
    float vida;
    char *nome;
    int tipo;
    float ataque;
    int defesa;
}Pokemon;


int *get_times(){
    FILE* file;
    file = fopen("input.txt","r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int times[2];
    fscanf(file, " %d %d",&times[0],&times[1]);
    fclose(file);
    return times;
}


int get_tipo(char *tipo){
    if(strcmp(tipo,"elétrico")) return 1;
    else if(strcmp(tipo,"água")) return 2;
    else if(strcmp(tipo,"fogo")) return 3;
    else if(strcmp(tipo,"gelo")) return 4;
    else return 5;

}

int combate(Pokemon **pokemon){
    int *times = get_times();
    int i = times[0], j = times[1];
    while (i && j){
        
    }
    
}

Pokemon  **get_players(){
    FILE* file;
    file = fopen("input.txt","r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int times[2];
    fscanf(file, " %d %d",&times[0],&times[1]);
    Pokemon **players = (Pokemon**)malloc( 2 * sizeof(Pokemon*));
    if (players == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }


    for (int j = 0; j < 2; j++){
        players[j] = (Pokemon*)malloc( times[j] * sizeof(Pokemon));
        for (int i = 0; i < times[j]; i++){
            char temp[12];
            players[j][i].nome = (char *) malloc( MAX_NOMES * sizeof(char));
            fscanf(file, " %s %f %d %f %s", 
            players[j][i].nome, &players[j][i].ataque, 
            &players[j][i].defesa,&players[j][i].vida, temp);
            players[j][i].tipo = get_tipo(temp);
        }
    }

    fclose(file);
    return players;
}


void main(){
    Pokemon **players = get_players(); 

    int *times = get_times();

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < times[j]; i++)
            free(players[j][i].nome);
        
        free(players[j]);
        
    }
    free(players);

}