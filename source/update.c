#include "headers/update.h"
#include "headers/globals.h"
#include "headers/assets.h"
#include "headers/audio.h"
#include "headers/debug.h"
#include "headers/draw.h"

#include <ogc/lwp.h>
#include <unistd.h>

// Function to clear the winningPositions array after a delay
void* ClearWinningPositions(void* arg) {
    usleep(2000000); // Sleep for 2 seconds

    highlightWinningCells = false;
    ResetBoard();

    return NULL;
}

void PlaceMark(int row, int col) {
    Player* current = currentPlayer();
    Player* waiting = waitingPlayer();

    if (board[row][col] == ' ') {
        board[row][col] = current->mark;

        if (current->id == 0) {
            PlayAudioAsync(&placeMarkSoundP1, 100, 1.0f);
        } else {
            PlayAudioAsync(&placeMarkSoundP2, 100, 1.0f);
        }

        if (!highlightWinningCells) {
            current->myTurn = false;
            waiting->myTurn = true;

            current->cursorStatus = 0;
        }

        if (CheckWinCondition(current, winningPositions)) {
            current->cursorStatus = 2;
            waiting->cursorStatus = 0;

            getPlayer(0)->myTurn = false;
            getPlayer(1)->myTurn = false;

            highlightWinningCells = true;

            ActivateRumbleAsync(current, 2000);

            // Create a thread to clear the winningPositions after a delay
            lwp_t clearThread;
            LWP_CreateThread(&clearThread, ClearWinningPositions, winningPositions, NULL, 0, 80);
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
                getPlayer(0)->cursorStatus = 3;
                getPlayer(1)->cursorStatus = 3;

                getPlayer(0)->myTurn = false;
                getPlayer(1)->myTurn = false;
                ActivateRumbleAsync(current, 2000);
                ActivateRumbleAsync(waiting, 2000);

                lwp_t clearThread;
                LWP_CreateThread(&clearThread, ClearWinningPositions, winningPositions, NULL, 0, 80);
            }
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

