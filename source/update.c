#include "headers/update.h"
#include "headers/players.h"
#include "headers/globals.h"
#include "headers/debug.h"

void UpdateObjects() {
    // Controlla le condizioni di vittoria o pareggio
    CheckWinCondition();
}

void PlaceMark(int row, int col) {
    if (board[row][col] == ' ') {
        board[row][col] = p1.myTurn ? 'X' : 'O';
        
        p1.myTurn = !p1.myTurn;
        p2.myTurn = !p2.myTurn;

        /*
        debug_send("New Board:\n%c %c %c\n%c %c %c\n%c %c %c\n",
            board[0][0], board[0][1], board[0][2],
            board[1][0], board[1][1], board[1][2],
            board[2][0], board[2][1], board[2][2]);
        */
    }
}


void CheckWinCondition() {
    // Controlla le condizioni di vittoria o pareggio
    // Questa è solo una logica di esempio
}
