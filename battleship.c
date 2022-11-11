//librerias a usar

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//estructuras
//se crearon las estructuras bajo las instrucciones del profesor, se hizo el tableron con ese tamaño para evitar hacer multiples funciones
//asi el resize quedaba mucho mas sencillo
//la estructura celda1 es dedicada para el usuario
//cuando se utilizaba unicamente la estructura celda se provocaban errores bastante curiosos
//no supe arreglar esos bugs como tal, pero al hacer la division de estructuras resulto mas sencillo trabajarlos
struct CELDA{
    int estado_celda;
    int ID;
    int impact;
}tablero[100][100];
 struct NAVE{
    char tipo_nave;
    char orientacion;
    int hundida;
    int espacio;
};
struct CELDA1{
    int estado_celda;
    int ID;
    int impact;
}tablero1[100][100];
//aqui tenemos 2 punteros, uno al tablero de la pc el que vendria siendo tablero
//y el puntero que dice p_tablero1 es el puntero a lo del usuario
struct CELDA *p_tablero = &tablero[0][0];
struct CELDA1 *p_tablero1 = &tablero1[0][0];

//funcion para elegir una nave aleatoria con id y size 
//esta funcion fue creada de esta forma para que fuera mas facil identificar que el tamaño de la nave era igual a su longitud
//de esta forma es mas sencillo trabajarlo, el return ship_size se utilizara de forma repetida en las siguientes funciones
//por cierto algo que aprendi es que la funcion random al hacer el % tienes que hacer un numero mayor al que quieres
//en lugar de hacer un aleatorio de 1-5 como se esperaria hace de 0-4 como si de un array se tratase
int choose_ship(){
    int ship_size;
    int ship_ID;
    int random = rand() % 5;
    switch (random){
        case 0:
            ship_size = 5;
            ship_ID = 5;
            break;
        case 1:
            ship_size = 4;
            ship_ID = 4;
            break;
        case 2:
            ship_size = 3;
            ship_ID = 3;
            break;
        case 3:
            ship_size = 2;
            ship_ID = 2;
            break;
        case 4:
            ship_size = 1;
            ship_ID = 1;
            break;
    }
    return ship_size;
}




//function para llenar de forma aleatoria un 25-35% del tablero con naves usando apuntadores
//al principio quisimos manejar esta funcion con las demas que recibiera el struct celda *pointer
//sin embargo por razones que desconozco nos resulto demasiado complicado aplicarlo sin []
//pero el profe me dijo en tutoria que me basara en el examen
//y eso hice,
//el int max y min, se basan en multiplicar el valor por si mismo y sacarle su 0.25 y 0.35
//sin embargo cuando lo dejabamos asi se llenaba el tablero por completo entonces recurrimos a mochar un poco el int size que recibe
//lo multiplicamos por su 70% haciendo que funcione de forma adecuada
//aqui volvemos a utilizar random para que sea la colocacion de naves aleatoria
//se utiliza para elegir su tamaño y orientacion
//dependiendo el tamaño de la nave y su orientacion se aplica de forma distintas
//para todo se usa un contador que cuando se equivale al tamaño de la nave se activan las celdas
//y se prosigue
void fill_board_ships(int size){
     struct CELDA *pointer = &tablero[0][0];
    int random;
    int x, y;
    int ship_size;
    char orientation;
    int i;
    int j;
    int k;
    int l;
    int count = 0;
    int max = (size * size) * 0.25;
    int min = (size * size) * 0.35;
    int random_2 = rand() % (max - min + 1) + min;
    for (i = 0; i < random_2; i++){
        do{
            x = rand() % size;
            y = rand() % size;
            random = rand() % 2;
            switch (random){
                case 0:
                    orientation = 'h';
                    break;
                case 1:
                    orientation = 'v';
                    break;
            }
            ship_size = choose_ship();
            if (orientation == 'h'){
                if (ship_size <= (size - x)){
                    for (j = 0; j < ship_size; j++){
                        if ((*(pointer + (x + j) + (y * size))).estado_celda == 0){
                            count++;
                        }
                    }
                    if (count == ship_size){
                        for (k = 0; k < ship_size; k++){
                            (*(pointer + (x + k) + (y * size))).estado_celda = 1;
                            (*(pointer + (x + k) + (y * size))).ID = ship_size;
                        }
                    }
                    else{
                        count = 0;
                    }
                }
            }
            else if (orientation == 'v'){
                if (ship_size <= (size - y)){
                    for (l = 0; l < ship_size; l++){
                        if ((*(pointer + (x) + ((y + l) * size))).estado_celda == 0){
                            count++;
                        }
                    }
                    if (count == ship_size){
                        for (k = 0; k < ship_size; k++){
                            (*(pointer + (x) + ((y + k) * size))).estado_celda = 1;
                            (*(pointer + (x) + ((y + k) * size))).ID = ship_size;
                        }
                    }
                    else{
                        count = 0;
                    }
                }
            }
        }while (count != ship_size);
    }
}




//funcion para rellenar el tablero con la estructura CELDA usando un puntero 
//simplemente lo que hicimos fue aprovechar que las direcciones de memorian estan colocadas de forma seguidita
//entonces podemos declarar todos los miembros del array 2d sin tener que hacer mucho esfuerzo


void fill_board(struct CELDA *p_tablero, int size){
    for (int i = 0; i <=size; i++){
        for (int j = 0; j < size; j++){
            p_tablero->estado_celda = 0;
            p_tablero->ID = 0;
            p_tablero->impact = 0;
            p_tablero++;
        }
    }
}

//funcion para disparar en el tablerito usando puntadoresss
//aqui implementamos una aritmetica de apuntadores
//la forma en la que llegamos a esa formula fue atraves de unos calculos
//debido a que cuando el usuario elegia la coordenada elegia la coordenada invertida
//entonces simplemente atraves de una simple formula se invierte y se logra que el jugador ataque donde guste
void shoot(struct CELDA *p_tablero, int size){
    int x, y;
    printf("\n ingresa la coordenada x:");
    scanf("%d", &x);
    printf("\n ingresa la coordenada y:");
    scanf("%d", &y);
    if (x > size || y > size){
        printf("coordenadas invalidas\n");
    }
    else{
        p_tablero += ((size - y-1) * size) + x;
        if (p_tablero->estado_celda == 0){
            p_tablero->impact = 1;
            printf("fallaste \n");
        }
        else{
            p_tablero->impact = 1;
            printf("le diste\n");
        }
    }
}



//esta funcion checa si la nave fue hundida
//es simple de usar simplemente se pregunta si ambas id son las mismas
//se accede a los impactos y si estos son positivos pues quiere decir que efectivamente fue hundida

int check_ship_sunk(struct CELDA *p_tablero,int size, int ship_ID){
    int i, j;
    int ship_sunk = 0;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            if (p_tablero->ID == ship_ID){
                if (p_tablero->impact == 1){
                    ship_sunk = 1;
                }
                else{
                    ship_sunk = 0;
                    break;
                }
            }
        }
    }
    return ship_sunk;
}



//function para imprimir el tablero y los impactos con pointersitos
//otra ves utilizando la teoria de que los arrays 2d todas sus memorias estan de forma seguida podemos acceder a ellas
//aunque esta vez no se modifican como tal solo hacemos un print
//aqui lo que decidimos hacer fue hacer el cycle for para poder imprimirlo mucho mas facil y las coordenadas
//lo hicimos de forma invertida para que 0,0 siempre fuera esquina izquierda inferior
//si no lo haciamos de esta forma se comportaba un poco raro, el i siempre tiene que ser igual al size
//y este tiene que llegar siempre a 0
//con k es un poco distinto se maneja como los demas for
//pero pusimos -1 para que el 0 fuera incluido en el print
//si no lo haciamos el 0 quedaba como fuera del print y se veia raro y era mas complicado interpretarlo
void print_board_and_impact(struct CELDA *p_tablero, int size){
    int i;
    int j;
    int k;
    printf("\nEste es el tablero de tu oponente\n");
    for (i = size-1; i >=0; i--){
      printf("\t %d", i);
        for (j = 0; j < size; j++)
        {
            if (p_tablero->impact == 0){
                if (p_tablero->estado_celda == 0){
                    printf("\t -");
                }
                else if (p_tablero->estado_celda == 1){
                    printf("\t O");
                }
            }
            else if (p_tablero->impact == 1){
                printf("\t X");
            }
            p_tablero++;
        }
        
        printf("\n");
    }
    for (k = -1; k <size; k++){
        if (k == -1){
            printf("\t  ");
        }
        else{
            printf("\t %d", k);
        }
    }
}


//funcion para checar si acabo el juego 
//aqui de nuevo utilizamos el concepto de las direcciones estando de forma consecutiva
//aqui lo que hacemos es que busca sobre todas y si alguna activa no tiene impacto pues no se ha acabado
//si ninguna esta activa entonces el juego ha finalizado
int check_game_over(struct CELDA *p_tablero, int size){
    int i, j;
    int game_over = 0;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            if (p_tablero->estado_celda == 1){
                if (p_tablero->impact == 0){
                    game_over = 0;
                    break;
                }
                else{
                    game_over = 1;
                }
            }
            p_tablero++;
        }
    }
    return game_over;
}


//funcion para que la pc ataque el tablero del jugador de forma aleatoria
//todas las funciones que tengan user en su nombre ya han sido explicadas con anterioridad
//son una copia de las anteriores solo cambiamos lo necesario para que se trabajara con el tablero del jugador
//y asi la pc pueda atacar 
void attack_user_board(struct CELDA1 *p_tablero1, int size){
    int x, y;
    x = rand() % size;
    y = rand() % size;
    p_tablero1 += ((size - y) * size) + x;
    if (p_tablero1->estado_celda == 0){
        p_tablero1->impact = 1;
        printf("\nfallo el tiro \n");
    }
    else{
        p_tablero1->impact = 1;
        printf("\nle atino\n");
    }
}

//function para imprimir el tablero del usuario y los impactos 

void print_user_board_and_impact(struct CELDA1 *p_tablero1, int size){
    int i;
    int j;
    int k;
    printf("\neste es tu tablero \n");
    for (i = size-1; i >=0; i--){
      printf("\t %d", i);
        for (j = 0; j < size; j++)
        {
            if (p_tablero1->impact == 0){
                if (p_tablero1->estado_celda == 0){
                    printf("\t -");
                }
                else if (p_tablero1->estado_celda == 1){
                    printf("\t O");
                }
            }
            else if (p_tablero1->impact == 1){
                printf("\t X");
            }
            p_tablero1++;
        }
        
        printf("\n");
    }
    for (k = -1; k <size; k++){
        if (k == -1){
            printf("\t  ");
        }
        else{
            printf("\t %d", k);
        }
    }
}

void fill_board_user(struct CELDA1 *p_tablero1, int size){
    for (int i = 0; i <=size; i++){
        for (int j = 0; j < size; j++){
            p_tablero1->estado_celda = 0;
            p_tablero1->ID = 0;
            p_tablero1->impact = 0;
            p_tablero1++;
        }
    }
}


void fill_board_ships_user(int size){

     struct CELDA1 *pointer1 = &tablero1[0][0];
    int random;
    int x, y;
    int ship_size;
    char orientation;
    int i;
    int j;
    int k;
    int l;
    int count = 0;
    int max = (size * size) * 0.25;
    int min = (size * size) * 0.35;
    int random_2 = rand() % (max - min + 1) + min;
    for (i = 0; i < random_2; i++){
        do{
            x = rand() % size;
            y = rand() % size;
            random = rand() % 2;
            switch (random){
                case 0:
                    orientation = 'h';
                    break;
                case 1:
                    orientation = 'v';
                    break;
            }
            ship_size = choose_ship();
            if (orientation == 'h'){
                if (ship_size <= (size - x)){
                    for (j = 0; j < ship_size; j++){
                        if ((*(pointer1 + (x + j) + (y * size))).estado_celda == 0){
                            count++;
                        }
                    }
                    if (count == ship_size){
                        for (k = 0; k < ship_size; k++){
                            (*(pointer1 + (x + k) + (y * size))).estado_celda = 1;
                            (*(pointer1 + (x + k) + (y * size))).ID = ship_size;
                        }
                    }
                    else{
                        count = 0;
                    }
                }
            }
            else if (orientation == 'v'){
                if (ship_size <= (size - y)){
                    for (l = 0; l < ship_size; l++){
                        if ((*(pointer1 + (x) + ((y + l) * size))).estado_celda == 0){
                            count++;
                        }
                    }
                    if (count == ship_size){
                        for (k = 0; k < ship_size; k++){
                            (*(pointer1 + (x) + ((y + k) * size))).estado_celda = 1;
                            (*(pointer1 + (x) + ((y + k) * size))).ID = ship_size;
                        }
                    }
                    else{
                        count = 0;
                    }
                }
            }
        }while (count != ship_size);
    }
}

int check_game_over_user(struct CELDA1 *p_tablero1, int size){
    int i, j;
    int game_over = 0;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            if (p_tablero1->estado_celda == 1){
                if (p_tablero1->impact == 0){
                    game_over = 0;
                    break;
                }
                else{
                    game_over = 1;
                }
            }
            p_tablero1++;
        }
    }
    return game_over;
}

//en esta funcion lo que hicimos simplemente fue unir todas las funciones y dandole forma al juego
//en este lo unico que considero que vale la pena mencionar es que el size lo tenemos que multiplicar por *0.7 para que se llene de 25--35%
//de lo contrario pasaban cosas raras
//de 10 pruebas que realizamos siempre se encontraban entre 25-35%
//el como se elige el turno es un simple rand
//aqui el usuario puede ver el tablero de la pc y el suyo

void play_game_didactico(struct CELDA *p_tablero,struct CELDA1 *p_tablero1,int size){
    int game_over = 0;
    int ship_sunk = 0;
    int ship_ID;
    int i,j,k,x,y;
    int n_shots = 0;
    int turn;
    fill_board(p_tablero, size);
    fill_board_ships(size*0.7);
    fill_board_user(p_tablero1,size);
    fill_board_ships_user(size*0.7);
    while (game_over == 0){
        turn = rand() % 2;
        if (turn == 0){
            printf("\nEs el turno del jugador\n");
            print_board_and_impact(p_tablero, size);
            shoot(p_tablero, size);
            n_shots++;
            for (i = 0; i < size; i++){
                for (j = 0; j < size; j++){
                    if (p_tablero->impact == 1){
                        if (p_tablero->estado_celda == 1){
                            p_tablero->ID = ship_ID;
                        }
                    }
                }
            }
            ship_sunk = check_ship_sunk(p_tablero,size, ship_ID);
            if (ship_sunk == 1){
                printf("\nHundiste una nave!\n");
                ship_ID--;
            }
            game_over = check_game_over(p_tablero, size);
        }
        else if (turn == 1){
            printf("\nturno de la pc \n");
            attack_user_board(p_tablero1, size);
            print_user_board_and_impact(p_tablero1, size);
            game_over = check_game_over_user(p_tablero1, size);
        }

}
    printf("Ganaste el juego!\n");
    printf("SOLO TE TOMO %d GANAR EL JUEGO!\n", n_shots);
}

//aqui es basicamente lo mismo que el anterior
//simplemente el jugador no puede ver el tablero del contricante como un battleship convencional 
void play_game_facil(struct CELDA *p_tablero,struct CELDA1 *p_tablero1,int size){
    int game_over = 0;
    int ship_sunk = 0;
    int ship_ID;
    int i,j,k,x,y;
    int n_shots = 0;
    int turn;
    fill_board(p_tablero, size);
    fill_board_ships(size*0.7);
    fill_board_user(p_tablero1,size);
    fill_board_ships_user(size*0.7);
    while (game_over == 0){
        turn = rand() % 2;
        if (turn == 0){
            print_user_board_and_impact(p_tablero1, size);
             printf("\nEs el turno del jugador\n");
            shoot(p_tablero, size);
            n_shots++;
            for (i = 0; i < size; i++){
                for (j = 0; j < size; j++){
                    if (p_tablero->impact == 1){
                        if (p_tablero->estado_celda == 1){
                            p_tablero->ID = ship_ID;
                        }
                    }
                }
            }
            ship_sunk = check_ship_sunk(p_tablero,size, ship_ID);
            if (ship_sunk == 1){
                printf("\nHundiste una nave!\n");
                ship_ID--;
            }
            game_over = check_game_over(p_tablero, size);
        }
        else if (turn == 1){
            printf("\nturno de la pc \n");
            attack_user_board(p_tablero1, size);
            print_user_board_and_impact(p_tablero1, size);
            game_over = check_game_over_user(p_tablero1, size);
        }

}
    printf("Ganaste el juego!\n");
    printf("SOLO TE TOMO %d GANAR EL JUEGO!\n", n_shots);
}

//un simple menu nada interesante que comentar aqui
void menu()
{
  printf("Bienvenido a batalla naval\n");
  printf("A. Modificar dimension del tablero \n"); 
  printf("Seleccionar modo de juego\n\t"); 
  printf("a. Didactico\n\t");
  printf("b. facil\n");
  printf("C. Iniciar juego\n"); 
  printf("D. salir\n"); 
  printf("introduce tu eleccion:\n");  
}

//main y menu en cuestion
//el system "cls" lo usamos para evitar tanta cumulacion de menus 
//como utilizamos el do while se itera de forma infinita hasta que se inicie el juego o se salga
int main(){
    srand(time(NULL));
    int size=10;
    char modo;
    char opcion;
    do {
        system("cls");
        menu();
        scanf("%c", &opcion);
        switch (opcion)
        {
        case 'A':
            printf("introduce la dimension del tablero\n");
            scanf("%d", &size);
            break;
        case 'a':
            printf("Has seleccionado el modo didactico\n");
            printf("iniciando juego\n");
            play_game_didactico(p_tablero,p_tablero1,size);
            break;
        case 'b':
            printf("Has seleccionado el modo facil\n");
            printf("iniciando juego\n");
                play_game_facil(p_tablero,p_tablero1,size);
            break;
         case 'C':
            printf("iniciando juego\n");
            play_game_didactico(p_tablero,p_tablero1,size);
            break;    
        case 'D':
            printf("adios\n");
            break;
        default:
            printf("opcion invalida\n");
            break;
        }
    } while (opcion != 'D');
    return 0;
}