#include <stdio.h>
#include <stdlib.h>

/*********************************
 * Created by
 * @author 'Ameer Hamza Naveed'
 * @date 10:13pm PST
 * @name 'Tic Tac Toe'
 ********************************/

// Program Constants
//#define clearScreen() system("cls")              // Windows
#define clearScreen() system("clear")              // Linux / Mac

#define BOARD_PATTERN "%s "

#define user_mark "X"
#define computer_mark "O"
#define unmarked_mark " "

// Ascii Graphics
#define TOP_RIGHT "╗"
#define TOP_LEFT "╔"
#define BOTTOM_LEFT "╚"
#define BOTTOM_RIGHT "╝"
#define RIGHT_MID "╣"
#define LEFT_MID "╠"
#define CROSS "╬"
#define VERTICAL "║"
#define HORIZONTAL "═"
#define TOP_MID "╦"
#define BOTTOM_MID "╩"

// enumerations
typedef enum {undefined = 0, user = 1, computer = 2} Player;
typedef enum {false = 0, true = 1} Boolean;

// structures
typedef struct {
    int x;
    int y;
} Position;
typedef struct {
    Player turn;
    Player *board;
    int boardSize;
} State;

// Functions
Player isWon(State *state){
    int x, y;
    Player FirstPlayer, WinningPlayer = undefined;

    //checking Rows
    for (x = 0; x < state->boardSize; x++)
    {
        FirstPlayer = state->board[x * state->boardSize];
        for (y = 0; y < state->boardSize; y++)
            if (state->board[x + (y*state->boardSize)] == FirstPlayer)
                WinningPlayer = FirstPlayer;
            else {
                WinningPlayer = undefined;
                break;
            }
        if (WinningPlayer != undefined)
            break;
    }

    if (WinningPlayer != undefined)
        return WinningPlayer;

    // checking Columns
    for (y = 0; y < state->boardSize; y++)
    {
        FirstPlayer = state->board[y*state->boardSize];
        for (x = 0; x < state->boardSize; x++)
        {
            if (state->board[x + (y*state->boardSize)] == FirstPlayer)
                WinningPlayer = FirstPlayer;
            else
            {
                WinningPlayer = undefined;
                break;
            }
        }
        if (WinningPlayer != undefined)
            break;
    }

    if (WinningPlayer != undefined)
        return WinningPlayer;

    // checking Diagonals
    FirstPlayer = state->board[0];
    for (x = 0; x < state->boardSize; x++)
    {
        if (state->board[x + x*state->boardSize] == FirstPlayer)
            WinningPlayer = FirstPlayer;
        else
        {
            WinningPlayer = undefined;
            break;
        }
    }
    if (WinningPlayer != undefined)
        return WinningPlayer;

    FirstPlayer = state->board[state->boardSize-1];
    for (x = state->boardSize - 1; x >= 0; x--)
    {
        if (state->board[x + (state->boardSize-x-1)*state->boardSize] == FirstPlayer)
            WinningPlayer = FirstPlayer;
        else
        {
            WinningPlayer = undefined;
            break;
        }
    }
    return WinningPlayer;
}
Boolean isDrawn(State *state){
    int i;
    for(i = 0; i < state->boardSize*state->boardSize; i++){
        if(state->board[i] == undefined)
            return false;
    }
    return true;
}
char *getMarkAt(State *state, Position pos){
    switch (state->board[pos.x + (pos.y * state->boardSize)]){
        case undefined:
            return unmarked_mark;
        case user:
            return user_mark;
        case computer:
            return computer_mark;
    }
}
void renderBoard(State *state){
    clearScreen();
    int i, j;
    for(i = 0; i <= state->boardSize * 2; i++){
        for(j = 0; j <= state->boardSize * 2; j++){
            char *character;
            if(i == 0){
                if(j == 0)
                    character = TOP_LEFT;
                else if (j == state->boardSize * 2)
                    character =  TOP_RIGHT;
                else
                    character = j%2?HORIZONTAL:TOP_MID;
            } else if (i == state->boardSize * 2){
                if(j == 0)
                    character =  BOTTOM_LEFT;
                else if (j == state->boardSize * 2)
                    character = BOTTOM_RIGHT;
                else
                    character = j%2?HORIZONTAL:BOTTOM_MID;
            } else if(i%2){
                if(j%2)
                    character = getMarkAt(state, (Position){i/2,j/2});
                else
                    character = VERTICAL;
            } else {
                if(j == 0)
                    character = LEFT_MID;
                else if (j == state->boardSize * 2)
                    character = RIGHT_MID;
                else
                    character = j%2?HORIZONTAL:CROSS;
            }
            printf(BOARD_PATTERN, character);
        }
        printf("\n");
    }
}
Boolean markOption(State *state, Position pos){
    int loc = (pos.x - 1) + ((pos.y - 1) * state->boardSize);
    if(loc >= 0 && loc < state->boardSize * state->boardSize && state->board[loc] == undefined){
        state->board[loc] = state->turn;
        return true;
    }
    return false;
}
void shiftTurn(State *state){
    state->turn = state->turn == user ? computer : user;
}
Position getSelectionFromUser(){
    Position pos;
    printf("\nEnter Row: ");
    scanf("%d", &pos.x);
    printf("Enter Column: ");
    scanf("%d", &pos.y);
    return pos;
}
void markComputerOption(State *state){
    if(rand()%2)
        for(int i = 0; i < state->boardSize; i++){
            for (int j = 0; j < state->boardSize ; ++j) {
                if(state->board[i + (j*state->boardSize)] == undefined){
                    markOption(state, (Position){i+1, j+1});
                    return;
                }
            }
        }
    else
        for(int i = state->boardSize-1; i >= 0; i--){
            for (int j = state->boardSize-1; j >= 0 ; j--) {
                if(state->board[i + j*state->boardSize] == undefined){
                    markOption(state, (Position){i+1, j+1});
                    return;
                }
            }
        }
}

int main() {
    int size;
    printf("Enter Game Size: ");
    scanf("%d", &size);

    Player game[size][size];
    for (int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            game[i][j] = undefined;
    State state = {user, (Player *) game, size};

    do{
        renderBoard(&state);
        if(state.turn == computer){
            markComputerOption(&state);
            shiftTurn(&state);
        } else {
            if (markOption(&state, getSelectionFromUser()))
                shiftTurn(&state);
        }
    } while(isWon(&state) == undefined && !isDrawn(&state));

    renderBoard(&state);
    shiftTurn(&state);

    if(isDrawn(&state))
        printf("Draw!");
    else
        printf("%s", state.turn == user? "You Won" : "You Lose");
    return 0;
}
