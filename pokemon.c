#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMES 50
#define MAX_TIME 100

typedef struct{
    float vida;
    char *nome;
    int tipo;
    float ataque;
    unsigned int defesa;
}Pokemon;//estrutura de um pokemon

//verifica se os times estão no máximo correto
void* verify_times(int *times){
    if(times[0]>MAX_TIME || times[1]>MAX_TIME) {
        printf("Tamanho de um ou mais dos times inválido.");
        exit(1);
    }
    return 0;
}

//verifica se os numeros do pokemons estão com os dados corretos
void verify_pokemon(Pokemon pokemon){
    if(pokemon.defesa == 0 || pokemon.vida==0 || pokemon.ataque==0){
        printf("Atributo(s) do Pokemon %s inválido(s)",pokemon.nome);
        exit(1);
    }
}

/*
pega os tipos. A ideia dos tipos é utilizar a nomeclatura decimal para poder utilizar a diferença
absoluta deles para saber se é ou não alterado o golpe, representando um "circulo". Aqueles cuja
 diferença é de 1, é superefetivo, e -1, resistente. 
*/
int get_tipo(char *tipo){
    if(strcmp(tipo,"eletrico")==0) return 1;
    else if(strcmp(tipo,"agua")==0) return 2;
    else if(strcmp(tipo,"fogo")==0) return 3;
    else if(strcmp(tipo,"gelo")==0) return 4;
    else if(strcmp(tipo,"pedra")==0) return 5;
    else{
        printf("Tipo Inválido");
        exit(1);
    }
}
/*
retorna a efetividade. Para isso, usa a distancia, se 1 ou -1, tem efetividade, se não 0.
Logo, verifica estas distancias e retorna um aumento ou decrésimo de 20% dependendo dos 
casos que estão inseridos.
*/
float set_efetividade(Pokemon atacante, Pokemon defensor){
    if(atacante.tipo == 5 && defensor.tipo==1) return 1.2;
    else if(atacante.tipo == 1 && defensor.tipo==5) return 0.8;
    else{
        int diferenca = defensor.tipo - atacante.tipo;
        if(diferenca==1) return 1.2;
        else if(diferenca==-1) return 0.8;
        return 1;
    }
}

//pega o dano dado dois pokemons. utiliza do 1 e -1 como dito anteriormente
float get_dano(Pokemon atacante, Pokemon defensor){
    float dano_efetivo=0;
    float efetividade = set_efetividade(atacante,defensor);

    if(atacante.ataque*efetividade <= defensor.defesa) dano_efetivo = 1;
    else dano_efetivo = atacante.ataque*efetividade - defensor.defesa;

    return dano_efetivo;
}

void print_vencedor(Pokemon *player1, Pokemon *player2,int pk1, int pk2 , int * times, int ganhador){
    printf("Jogador %d venceu \n",ganhador);

   if(ganhador==1){
        printf("Pokemons sobreviventes:\n");

        for (int o = pk1; o < times[0]; o++){
            printf("%s\n",player1[o].nome);
        }


        printf("Pokemons derrotados:\n");
        for (int o = 0; o < pk2; o++){
            printf("%s\n",player2[o].nome);
        }

        for (int o = 0; o < pk1; o++){
            printf("%s\n",player1[o].nome);
        }
   }

   else{
        printf("Pokemons sobreviventes:\n");

        for (int o = pk2; o < times[1]; o++){
            printf("%s\n",player2[o].nome);
        }

        printf("Pokemons derrotados:\n");

        for (int o = 0; o < pk1; o++){
            printf("%s\n",player1[o].nome);
        }

        for (int o = 0; o < pk2; o++){
            printf("%s\n",player2[o].nome);
        }
   }
}


//faz os combates dos players
void combate(Pokemon *player1, Pokemon *player2, int * times){
    int i = times[0], j = times[1];
    //variáveis chave do combate. i e j = máximos de cada time, ou seja, a quantidade de pokemons vivos
    int turno = 0;
    //qual o jogador do momento. o 1 é par, o 2 é impar
    int pk1=0,pk2=0;
    //pokemons atuais
    while (i!=0 && j!=0){
    Pokemon * pokemon_player1 = &player1[pk1], * pokemon_player2 = &player2[pk2]; 
    //ponteiro para o pokemon atual dos jogadores
        if(turno%2==0){//player 1
            pokemon_player2->vida = pokemon_player2->vida - get_dano(*pokemon_player1,*pokemon_player2);
            if(pokemon_player2->vida<=0){
                printf("%s venceu %s\n", pokemon_player1->nome, pokemon_player2->nome);
                pk2++;
                j--;
            }
        }
        else{//player 2
            pokemon_player1->vida = pokemon_player1->vida - get_dano(*pokemon_player2,*pokemon_player1);
            if(pokemon_player1->vida<=0){
                printf("%s venceu %s\n", pokemon_player2->nome, pokemon_player1->nome);
                pk1++;
                i--;
            }
        }
        turno++;
    }

    if(i==0) {
        print_vencedor(player1,player2,pk1,pk2,times,2);
    }

    else if(j==0){
        print_vencedor(player1,player2,pk1,pk2,times,1);
    }

}

//pega os pokemons e inserir conforme os players
Pokemon  **get_players(int * times){
    FILE* file;
    file = fopen("input.txt","r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fscanf(file, " %d %d",&times[0],&times[1]);
    Pokemon **players = (Pokemon**)malloc( 2 * sizeof(Pokemon*));
    //matriz de 2 vetores, cada um sendo um player
    if (players == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    verify_times(times);

    for (int j = 0; j < 2; j++){
        players[j] = (Pokemon*)malloc( times[j] * sizeof(Pokemon));//players
        for (int i = 0; i < times[j]; i++){
            char temp[12];
            players[j][i].nome = (char *) malloc( MAX_NOMES * sizeof(char));
            fscanf(file, " %s %f %d %f %s", 
            players[j][i].nome, &players[j][i].ataque, 
            &players[j][i].defesa,&players[j][i].vida, temp);//leitura do pokemon
            players[j][i].tipo = get_tipo(temp);//pega o tipo
            verify_pokemon(players[j][i]);
        }
    }

    fclose(file);
    return players;
}


int main(){
    int times[2];

    Pokemon **players = get_players(times); 

    combate(players[0],players[1],times);

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