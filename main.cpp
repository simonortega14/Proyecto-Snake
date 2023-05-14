#include <allegro.h>
#include "inicia.h"
#include <iostream>
#include <winalleg.h>
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#define ARRIBA 72 // números asociados a las flechas del teclado
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
#define ESC 27

using namespace std;

void gotoxy(int x,int y) //funcion que posiciona el cursor en la coordenada xy
{
    HANDLE hCon;
    COORD dwPos;
    dwPos.X=x;
    dwPos.Y=y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon,dwPos);
}

int cuerpo[200][2];
char tecla;
int n=1;
int tam = 10 ;
int x=10;
int y=12;
int dir=3;
int xc=30 , yc=15;
int velocidad = 100, h = 1;
int score = 0;

void pintar()
{
    // lineas horizontales
    for(int i=2; i<78; i++){
        gotoxy(i,3);printf("%c",205);
        gotoxy(i,23);printf("%c",205);
    }
    // lineas verticales
    for(int i=4; i<23; i++){
        gotoxy(2,i);printf("%c",186);
        gotoxy(77,i);printf("%c",186);
    }
    // esquinas
    gotoxy(2,3);printf("%c",201);
    gotoxy(2,23);printf("%c",200);
    gotoxy(77,3);printf("%c",187);
    gotoxy(77,23);printf("%c",188);
}

void guardar_posicion()
{
	cuerpo[n][0] = x;
	cuerpo[n][1] = y;
	n++;
	if(n==tam) n = 1;
}

void dibujar_cuerpo(){
	for(int i=1; i<tam ; i++){
		gotoxy(cuerpo[i][0],cuerpo[i][1]); printf("*");
	}
}

void borrar_cuerpo(){

		gotoxy(cuerpo[n][0],cuerpo[n][1]); printf(" ");
}

void teclear(){
	if(kbhit()){
			tecla=getch();
			switch(tecla){
				case ARRIBA :
					if(dir!= 2)
						dir=1;
						break;
				case ABAJO :
					if(dir!= 1)
						dir=2;
						break;
				case DERECHA :
					if(dir!=4)
						dir=3;
						break;
				case IZQUIERDA :
					if (dir !=3)
						dir=4;
						break;
			}
		}
}

void cambiar_velocidad(){
	if(score == h*20){
		velocidad -= 10;
		h++;
	}
}

void comida(){
	if(x == xc && y == yc){
        srand(time(0));
		xc = (rand()%73)+4;
		yc  = (rand()%19)+4;
		tam++;
		score += 10;
		gotoxy(xc,yc);printf("%c",4);

		cambiar_velocidad();
	}
}

bool game_over(){
    if (y == 3 || y == 23 || x == 2 || x == 77) {
        ofstream archivo;
        archivo.open("puntajes.txt", ios::app);
        archivo << "Puntuación: " << score << endl;
        archivo.close();
        return false;
    }
    for(int j = tam-1; j > 0; j-- ){
        if(cuerpo[j][0] == x && cuerpo[j][1] == y) {
            ofstream archivo;
            archivo.open("puntajes.txt", ios::app);
            archivo << "Puntuación: " << score << endl;
            archivo.close();
            return false;
        }
    }
    return true;
}

void puntos(){
	gotoxy(3,1); printf("score %d",score);
}

void jugar(){
    pintar();
    gotoxy(xc,yc);printf("%c",4);
    while(tecla != ESC && game_over()){
        borrar_cuerpo();
        guardar_posicion();
        dibujar_cuerpo();
        comida();
        puntos();
        teclear();
        teclear();
        if (dir == 1) y --;
        if(dir == 2) y++;
        if(dir == 3) x++;
        if(dir == 4) x--;
        Sleep(velocidad);
    }
    pintar();
    system("pause>null");
}

int main ()
{
    inicia_allegro(640,360);
    inicia_audio(70,70);
    install_mouse();

    BITMAP *buffer = create_bitmap(640,360);
    BITMAP *snake = load_bitmap("snake.bmp",NULL);
    BITMAP *snake2 = load_bitmap("snake_2.bmp",NULL);
    BITMAP *snake3 = load_bitmap("snake_3.bmp",NULL);
    BITMAP *cursor = load_bitmap("cursor.bmp",NULL);

    bool salida = false;

    if (snake == NULL) {
        allegro_message("Error al cargar la imagen snake");
        exit(EXIT_FAILURE);
    }

    while(!salida){
        if(mouse_x > 342 && mouse_x < 464 &&
           mouse_y > 141 && mouse_y < 174){

            blit(snake2,buffer,0,0,0,0,640,360);
            if(mouse_b & 1){
               jugar();
            }
            }else if(mouse_x > 360 && mouse_x < 448 &&
                     mouse_y > 238 && mouse_y < 271){

            blit(snake3,buffer,0,0,0,0,640,360);
            if(mouse_b & 1){
               salida = true;
            }

            }else blit(snake, buffer, 0, 0, 0, 0, snake->w, snake->h);

         masked_blit(cursor, buffer, 0, 0, mouse_x, mouse_y, 10, 20);
         blit(buffer, screen, 0, 0, 0, 0, 640, 360);
    }
    destroy_bitmap(buffer);

    return 0;

}
END_OF_MAIN ()










