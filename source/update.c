#include "headers/update.h"

char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
char currentPlayer = 'X';

void UpdateObjects() {
    // Controlla le condizioni di vittoria o pareggio
    CheckWinCondition();
}

void PlaceMark(int row, int col) {
    // Verifica se la casella è vuota prima di posizionare il simbolo
    if (board[row][col] == ' ') {
        board[row][col] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

void CheckWinCondition() {
    // Controlla le condizioni di vittoria o pareggio
    // Questa è solo una logica di esempio
}
