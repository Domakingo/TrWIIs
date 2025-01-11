#include "headers/update.h"
#include "headers/globals.h"
#include "headers/audio.h"
#include "headers/engine.h"
#include "headers/debug.h"
#include "headers/draw.h"

void PlaceMark(int row, int col) {
    Player* current = currentPlayer();
    Player* waiting = waitingPlayer();

    if (board[row][col] == ' ') {
        board[row][col] = current->mark;

        PlayAudioAsync(&placeSound, 100, 1.0f);

        bool gameEnded = false;

        if (CheckWinCondition(current, winningPositions)) {
            gameEnded = true;
            current->myTurn = false;
            waiting->myTurn = false;
            ActivateRumbleAsync(current, 2000);
            ActivateRumbleAsync(waiting, 2000);
            StartWinningAnimation(winningPositions);
        } else {
            bool isDraw = true;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') {
                        isDraw = false;
                        break;
                    }
                }
            }

            if (isDraw) {
                gameEnded = true;
                current->myTurn = false;
                waiting->myTurn = false;
                ActivateRumbleAsync(current, 2000);
                ActivateRumbleAsync(waiting, 2000);
                StartDrawAnimation();
            }
        }

        if (!gameEnded) {
            current->myTurn = false;
            waiting->myTurn = true;
        }
    }
}

bool CheckWinCondition(Player* player, Position winningPositions[3]) {
    // Row check
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player->mark && board[i][1] == player->mark && board[i][2] == player->mark) {
            winningPositions[0] = (Position){i, 0};
            winningPositions[1] = (Position){i, 1};
            winningPositions[2] = (Position){i, 2};
            return true;
        }
    }

    // Column check
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == player->mark && board[1][i] == player->mark && board[2][i] == player->mark) {
            winningPositions[0] = (Position){0, i};
            winningPositions[1] = (Position){1, i};
            winningPositions[2] = (Position){2, i};
            return true;
        }
    }

    // Diagonals check
    if (board[0][0] == player->mark && board[1][1] == player->mark && board[2][2] == player->mark) {
        winningPositions[0] = (Position){0, 0};
        winningPositions[1] = (Position){1, 1};
        winningPositions[2] = (Position){2, 2};
        return true;
    }

    if (board[0][2] == player->mark && board[1][1] == player->mark && board[2][0] == player->mark) {
        winningPositions[0] = (Position){0, 2};
        winningPositions[1] = (Position){1, 1};
        winningPositions[2] = (Position){2, 0};
        return true;
    }

    return false;
}