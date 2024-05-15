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
}Pokemon;//estrutura de um pokemon

//pega os times pre-determinados no inicio do txt
int * get_times(){
    FILE* file;
    file = fopen("input.txt","r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int *times = (int*)malloc(2*sizeof(int));//cria o vetor para retornar como pointer
    fscanf(file, " %d %d",&times[0],&times[1]);
    fclose(file);
    return times;
}

/*
pega os tipos. A ideia dos tipos é utilizar a nomeclatura decimal para poder utilizar a diferença
absoluta deles para saber se é ou não alterado o golpe, representando um "circulo". Aqueles cuja
 diferença é de 1, é superefetivo, e -1, fraco. 
*/
int get_tipo(char *tipo){
    if(strcmp(tipo,"elétrico")==0) return 1;
    else if(strcmp(tipo,"água")==0) return 2;
    else if(strcmp(tipo,"fogo")==0) return 3;
    else if(strcmp(tipo,"gelo")==0) return 4;
    else return 5;

}
/*
retorna a efetividade. Para isso, usa a distancia, se 1 ou -1, tem efetividade, se não 0. Utiliza
de -1 ou 1 para o retorno por que isso é multiplicado no ataque * 0.2, assim altera o ataque 
posteriormente. Se desvantagem, retira ataque, se vantagem, adiciona. Isso se da pelo posterior 
calculo de "atacante.ataque + atacante.ataque*set_efetividade(atacante,defensor)*0.2".
*/
int set_efetividade(Pokemon atacante, Pokemon defensor){
    if(atacante.tipo == 5 && defensor.tipo==1) return 1;
    else if(atacante.tipo == 1 && defensor.tipo==5) return -1;
    else{
        int diferenca = defensor.tipo - atacante.tipo;
        if(diferenca==1 || diferenca==-1) return diferenca;
        return 0;
    }
}

//pega o dano dado dois pokemons. utiliza do 1 e -1 como dito anteriormente
float get_dano(Pokemon atacante, Pokemon defensor){
    float dano_efetivo = ((atacante.ataque + (atacante.ataque*set_efetividade(atacante,defensor)*0.2)) 
    -  defensor.defesa);
    if(atacante.ataque <= defensor.defesa) dano_efetivo=1;
    return dano_efetivo;
}

//faz os combates dos players
void combate(Pokemon *player1, Pokemon *player2){
    int *times = get_times();
    int jogador=2, i = times[0], j = times[1];
    //variáveis chave do combate. jogador = vencedor, i e j = máximos 
    int pk1=0,pk2=0;
    //pokemons atuais
    int turno = 0;
    //qual o jogador do momento. o 1 é par, o 2 é impar
    while (i!=0 && j!=0){
        if(turno%2==0){//player 1
            player2[pk2].vida = player2[pk2].vida - get_dano(player1[pk1],player2[pk2]);
             if(player2[pk2].vida==0){
                printf("%s venceu %s\n", player1[pk1].nome, player2[pk2].nome);
                pk2++;
                j--;
            }
        }
        else{//player 2
            player1[pk1].vida = player1[pk1].vida - get_dano(player2[pk2],player1[pk1]);
            if(player1[pk1].vida==0){
                printf("%s venceu %s\n", player2[pk2].nome, player1[pk1].nome);
                pk1++;
                i--;
            }
        }
        turno++;
    }
    if(i==0) {
        printf("Jogador 2 venceu \n");
        printf("Pokemons sobreviventes:\n");
        for (int o = pk2; o < times[1]; o++){
            printf("%s\n",player2[o].nome);
        }
        printf("Pokemons derrotados:\n");
        for (int o = 0; o < pk2; o++){
            printf("%s ",player2[o].nome);
        }
        for (int o = 0; o < pk1; o++){
            printf("%s ",player1[o].nome);
        }
    }

    else if(j==0){
        printf("Jogador 1 venceu \n");
         printf("Pokemons sobreviventes:\n");
        for (int o = pk1; o < times[0]; o++){
            printf("%s\n",player1[o].nome);
        }
        printf("Pokemons derrotados:\n");
        for (int o = 0; o < pk1; o++){
            printf("%s ",player1[o].nome);
        }
        for (int o = 0; o < pk2; o++){
            printf("%s ",player2[o].nome);
        }

    }
    
}

//pega os pokemons e ordena conforme os players
Pokemon  **get_players(){
    FILE* file;
    file = fopen("input.txt","r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int times[2];
    fscanf(file, " %d %d",&times[0],&times[1]);
    Pokemon **players = (Pokemon**)malloc( 2 * sizeof(Pokemon*));//matriz de 2 vetores, cada um sendo um player
    if (players == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }


    for (int j = 0; j < 2; j++){
        players[j] = (Pokemon*)malloc( times[j] * sizeof(Pokemon));//players
        for (int i = 0; i < times[j]; i++){
            char temp[12];
            players[j][i].nome = (char *) malloc( MAX_NOMES * sizeof(char));
            fscanf(file, " %s %f %d %f %s", 
            players[j][i].nome, &players[j][i].ataque, 
            &players[j][i].defesa,&players[j][i].vida, temp);//leitura do pokemon
            players[j][i].tipo = get_tipo(temp);//pega o tipo
        }
    }

    fclose(file);
    return players;
}


int main(){
    Pokemon **players = get_players(); 

    int *times = get_times();

    combate(players[0],players[1]);

    printf("\n");

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < times[j]; i++){
            free(players[j][i].nome);
        }
        
        free(players[j]);
        
    }
    free(players);
    return 0;
}