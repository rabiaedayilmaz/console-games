// necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include <unistd.h> // to sleep zzz


// in C, there is no built-in truth values so define them
// or it is possible to import boolean library
#define true 1
#define false 0

// global variables
int width, height;
int gameOver = 0;
int x, y, foodX, foodY, score;
int currdir;

int print;

// tail follow me config
// max tail length is 100
int tailX[100], tailY[100];
int nTail;

// trap for hard game mode :D
int trapX, trapY;
int gameMode;

int life;
int ateTrap;

// further info:
// https://riptutorial.com/c/example/6564/typedef-enum
// create an enumeration and declare `dir` as pseudoname to call it quickly
typedef enum eDirection {
    STOP=0, UP, LEFT, DOWN, RIGHT
    }dir;


void getBoard(){
    // this func, gets the size of the board from the user
    // then, returns it
    int m, n;

    printf("Enter the game board size, preciou-sssz: ");
    scanf("%d %d", &m, &n);
    width = m;
    height = n;
    printf("\nLovely! Next, select the difficulty level 0 or 1?: ");
    scanf("%d", &gameMode);
    if(!gameMode){printf("Okay! Good luck with normal mode!");}
    else if(gameMode){printf("Okay! Good luck with hard mode!\nBe aware of traps!");}
    else{printf("Unvalid command, try again."); gameOver=true;}
    sleep(1);
}

void setup(){
    int gameOver = false;
    dir currdir = STOP;
    
    // start in the center of the gameboard
    x = width / 2;
    y = height / 2;

    // randomized food location
    // modular arithmetic because we want a value between 0 and w/h - 1, not more or less
    foodX = rand() % width;
    foodY= rand() % height;

    if(gameMode){
        trapX = rand() % width;
        trapY = rand() % height;
    }

    score = 0;
    life = 3;
    ateTrap = 0;
}

void draw(){
    system("cls"); // clear previous drawing output
    
    for(int yeter=0; yeter<life; yeter++){printf("*");}
    printf("    LIFE: %d", life);
    printf("\n");

    // create top gameboard row
    for(int i=0; i<=width+1; i++){
        printf("#");
    }
    printf("\n");
    
    // create left and right columns
    for(int i=0; i<=height; i++){
        for(int j=0; j<=width; j++){
            if(j==0) {printf("#");}

            if(i==y && j==x) {printf("O");}
            else if(i==foodY && j==foodX) {printf("F");}
            else if(gameMode && (i==trapX && j==trapY)) {printf("X");}
            else{
                print = false;
                for(int k=0; k<nTail; k++){ 
                    if(tailX[k]==j && tailY[k]==i){
                        printf("o");
                        print = true;
                    }
                }
                if(!print) {printf(" ");}
            }

            if(j==width-1) {printf("#");}
        }
        printf("\n");
    }

    // create bottom gameboard row
    for(int i=0; i<=width+1; i++){
        printf("#");
    }
    printf("\nSCORE: %d", score);
    
    // if trap is hit
    // show a message
    // then restart the ateTrap flag
    if(ateTrap){
        printf("%*c", height/2, ' ');
        printf("Argh! Trap is not my fav food!");
        }
}

void input(){
    // function from conio library
    // checks whether if keyboard hit or not
    if(_kbhit()){
        // get the pressed character
        switch(_getch()){
            case 'a':
                currdir = LEFT;
                break;
            case 'd':
                currdir = RIGHT;
                break;
            case 'w':
                currdir = UP;
                break;
            case 's':
                currdir = DOWN;
                break;
            case 'q':
                // end the game
                system("cls");
                printf("EXIT");
                sleep(1);
                printf("...");
                sleep(1);
                printf("FROM");
                sleep(1);
                printf("...");
                sleep(1);
                printf("GAME");
                sleep(1);
                printf("...\n");
                sleep(2);
                printf("\nGOOD-BYTE ;)");
                sleep(1);
                printf(" ...");
                gameOver = true;
                break;
        }
    }
}

void logic(){

    // save the initial tail points
    int prevX = tailX[0];
    int prevY = tailY[0];

    tailX[0] = x;
    tailY[0] = y;

    // previous of previous
    int prev2X, prev2Y;

    // for each tail point
    // if you start at 0, then you create a fixed tail block
    for(int i=1; i<nTail; i++){
        // shift tail points to previous
        prev2X = tailX[i];
        prev2Y = tailY[i];

        tailX[i] = prevX;
        tailY[i] = prevY;

        prevX = prev2X;
        prevY = prev2Y;
    }

    // according to the current direction
    // update location of x and y of head
    switch(currdir){
        case LEFT:
            x--;
            break;
        
        case RIGHT:
            x++;
            break;
        
        case UP:
            y--;
            break;
        
        case DOWN:
            y++;
            break;
        
        default:
            break;
    }

    // end the game because hit the wall
    if(x > width-1 || x < 0 || y > height-1 || y < 0){
                system("cls");
                printf("you");
                sleep(1);
                printf("...");
                sleep(1);
                printf("HIT");
                sleep(1);
                printf("...");
                sleep(1);
                printf("the wall");
                sleep(1);
                printf("...\n");
                sleep(2);
                printf("\nGOOD-BYTE ;)");
                sleep(1);
                printf(" ...");
                gameOver = true;
    }

    for(int i=0; i<nTail; i++){
        // end the game because of hannibalism
        if(tailX[i] == x && tailY[i] == y){
                system("cls");
                printf("you");
                sleep(1);
                printf("...");
                sleep(1);
                printf("ATE");
                sleep(1);
                printf("...");
                sleep(1);
                printf("yourself");
                sleep(1);
                printf("...\n");
                sleep(2);
                printf("\nGOOD-BYTE ;)");
                sleep(1);
                printf(" ...");
                gameOver = true;
        }
    }

    // eat the food
    if(foodX == x && foodY == y){
        score += 10; // increase score by ten

        // relocate new food
        foodX = rand() % width;
        foodY = rand() % height;

        // ate food so grow up :)
        nTail++;
    }

    // caught in a trap
    // reversed locations, do not change
    // because yo need to change the variables in the draw func
    if(trapX == y && trapY == x){
        life = life - 1;
        ateTrap=1;

        // relocate new trap
        trapX = rand() % width;
        trapY = rand() % height;
    }

    // life ran out
    if(life==0){
            system("cls");
            printf("...");
            sleep(1);
            printf("i said that");
            sleep(1);
            printf("...");
            sleep(1);
            printf("TRAP IS NOT MY FAV FOOD");
            sleep(1);
            printf("...");
            sleep(1);
            printf("anyways");
            sleep(1);
            printf("...\n");
            sleep(2);
            printf("\nGOOD-BYTE ;)");
            sleep(1);
            printf(" ...");
            gameOver = true;
    }
}

int main(){

    getBoard();
    setup();

    while(!gameOver){
        draw();
        sleep(1);

        input();
        logic();
        
    }


    return 0;
}