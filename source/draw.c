#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#include "headers/draw.h"
#include "headers/player.h"
#include "headers/assets.h"
#include "headers/globals.h"
#include "headers/debug.h"

Position winningPositions[3];
bool highlightWinningCells = false;
bool showDrawScore = false;
bool inputThreadRunning = false;

void LoadAssets() {
    // Load the images
    backgroundTex = GRRLIB_LoadTexture(background_png);
    if (backgroundTex == NULL) {
        debug_send("Failed to load background.png\n");
    }
    
    gridTex = GRRLIB_LoadTexture(grid_png);
    if (gridTex == NULL) {
        debug_send("Failed to load grid.png\n");
    }

    XTex = GRRLIB_LoadTexture(X_png);
    if (XTex == NULL) {
        debug_send("Failed to load XTex.png\n");
    }

    OTex = GRRLIB_LoadTexture(O_png);
    if (OTex == NULL) {
        debug_send("Failed to load OTex.png\n");
    }

    // p1 textures
    p1CursorTex[0] = GRRLIB_LoadTexture(p1Move_png);
    if (p1CursorTex[0] == NULL) {
        debug_send("Failed to load p1 move.png\n");
    }

    p1CursorTex[1] = GRRLIB_LoadTexture(p1Select_png);
    if (p1CursorTex[1] == NULL) {
        debug_send("Failed to load p1 select.png\n");
    }

    p1CursorTex[2] = GRRLIB_LoadTexture(p1Win_png);
    if (p1CursorTex[2] == NULL) {
        debug_send("Failed to load p1 win.png\n");
    }

    p1CursorTex[3] = GRRLIB_LoadTexture(p1Draw_png);
    if (p1CursorTex[3] == NULL) {
        debug_send("Failed to load p1 draw.png\n");
    }

    // p2 textures
    p2CursorTex[0] = GRRLIB_LoadTexture(p2Move_png);
    if (p2CursorTex[0] == NULL) {
        debug_send("Failed to load p2 move.png\n");
    }

    p2CursorTex[1] = GRRLIB_LoadTexture(p2Select_png);
    if (p2CursorTex[1] == NULL) {
        debug_send("Failed to load p2 select.png\n");
    }

    p2CursorTex[2] = GRRLIB_LoadTexture(p2Win_png);
    if (p2CursorTex[2] == NULL) {
        debug_send("Failed to load p2 win.png\n");
    }

    p2CursorTex[3] = GRRLIB_LoadTexture(p2Draw_png);
    if (p2CursorTex[3] == NULL) {
        debug_send("Failed to load p2 draw.png\n");
    }

    // Load numbers images
    GRRLIB_texImg *numbers_0to4Tex = GRRLIB_LoadTexture(numbers_0to4_png);
    if (numbers_0to4Tex == NULL) {
        debug_send("Failed to load numbers_0to4Tex.png\n");
        return;
    }

    GRRLIB_texImg *numbers_5to9Tex = GRRLIB_LoadTexture(numbers_5to9_png);
    if (numbers_5to9Tex == NULL) {
        debug_send("Failed to load numbers_5to9Tex.png\n");
        return;
    }

    // Create sub-textures for numbers 0-4
    for (int i = 0; i < 5; i++) {
        numbers_0to4[i] = CreateSubTexture(numbers_0to4Tex, i);
        if (numbers_0to4[i] == NULL) {
            debug_send("Failed to create sub-texture for number %d\n", i);
        }
    }

    // Create sub-textures for numbers 5-9
    for (int i = 0; i < 5; i++) {
        numbers_5to9[i] = CreateSubTexture(numbers_5to9Tex, i);
        if (numbers_5to9[i] == NULL) {
            debug_send("Failed to create sub-texture for number %d\n", i + 5);
        }
    }

    debug_send("%d, %d", numbers_0to4Tex->w, numbers_0to4Tex->h);
    debug_send("%d, %d", numbers_0to4[0]->w, numbers_0to4[0]->h);
    debug_send("%d, %d", numbers_5to9[0]->w, numbers_5to9[0]->h);
}

GRRLIB_texImg* CreateSubTexture(GRRLIB_texImg* sourceTex, int index) {
    if (sourceTex == NULL) {
        debug_send("Source texture is NULL\n");
        return NULL;
    }

    // Each digit is 128x128
    int digitWidth = 128;
    int digitHeight = 128;

    // Calculate the x and y positions based on the index
    int x = (index % 5) * digitWidth;
    int y = (index / 5) * digitHeight;

    // Debugging: Print the calculated positions and dimensions
    debug_send("Creating sub-texture for index %d: x=%d, y=%d, width=%d, height=%d\n", index, x, y, digitWidth, digitHeight);

    // Create an empty texture with the desired dimensions
    GRRLIB_texImg* subTex = GRRLIB_CreateEmptyTexture(digitWidth, digitHeight);
    if (subTex == NULL) {
        debug_send("Failed to create sub-texture\n");
        return NULL;
    }

    // Cast data to appropriate type (assuming it's uint32_t for RGBA)
    uint32_t* subData = (uint32_t*)subTex->data;
    uint32_t* sourceData = (uint32_t*)sourceTex->data;

    // Copy the desired portion of the source texture to the sub-texture
    for (int i = 0; i < digitHeight; i++) {
        for (int j = 0; j < digitWidth; j++) {
            int sourceIndex = (y + i) * sourceTex->w + (x + j);
            int subIndex = i * digitWidth + j;
            subData[subIndex] = sourceData[sourceIndex];
        }
    }

    return subTex;
}

void FreeAssets() {
    GRRLIB_FreeTexture(backgroundTex);
    GRRLIB_FreeTexture(gridTex);
    GRRLIB_FreeTexture(XTex);
    GRRLIB_FreeTexture(OTex);

    for (int i = 0; i < 4; i++){
        GRRLIB_FreeTexture(p1CursorTex[i]);
        GRRLIB_FreeTexture(p2CursorTex[i]);
    }

    for (int i = 0; i < 5; i++) {
        GRRLIB_FreeTexture(numbers_0to4[i]);
        GRRLIB_FreeTexture(numbers_5to9[i]);
    }
}

void DrawObjects() {
    // Clear the screen with black
    GRRLIB_FillScreen(0x000000FF);

    // Draw the background image
    if (backgroundTex != NULL) {
        GRRLIB_DrawImg(0, 0, backgroundTex, 0, 1, 1, 0xFFFFFFFF);
    }

    // Draw the grid image
    if (gridTex != NULL) {
        GRRLIB_DrawImg(gridStartX, gridStartY, gridTex, 0, 1, 1, 0xFFFFFFFF);
    }

    //GRRLIB_DrawImg(0, 0, numbers_0to4[1], 0, 1, 1, 0xFFFFFFFF);

    // Draw the Tic-Tac-Toe board
    DrawBoard(board);

    // Draw the cursors
    DrawCursor(p1.ir.sx, p1.ir.sy, &p1, p1CursorTex[p1.cursorStatus]);
    DrawCursor(p2.ir.sx, p2.ir.sy, &p2, p2CursorTex[p2.cursorStatus]);

    // Render the frame buffer to the TV
    GRRLIB_Render();
}

void DrawBoard(char board[3][3]) {
    Player* current = currentPlayer();

    if (current != NULL) {
        // Check if the cursor is within the grid boundaries
        if (current->ir.sx >= gridStartX && current->ir.sx < gridStartX + gridSize &&
            current->ir.sy - 34 >= gridStartY && current->ir.sy - 34 < gridStartY + gridSize) {
            
            // Calculate the hovered column and row based on the cursor position
            int hoveredCol = (current->ir.sx - gridStartX) / cellSize;
            int hoveredRow = (current->ir.sy - 34 - gridStartY) / cellSize;

            // Check if the hovered cell is empty
            if (board[hoveredRow][hoveredCol] == ' ') {
                current->cursorStatus = 1;

                // Draw the hovered cell with the specified color
                GRRLIB_Rectangle(gridStartX + hoveredCol * cellSize, gridStartY + hoveredRow * cellSize, cellSize, cellSize, hoveredCellColor, true);
            } else {
                current->cursorStatus = 0;
            }
        } else {
            current->cursorStatus = 0;
        }
    }

    if (highlightWinningCells) {
        DrawWinningCells();
    }

    // Draw the marks on the board using textures
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int x = gridStartX + j * cellSize + (cellSize - 100) / 2; // Center the texture horizontally
            int y = gridStartY + i * cellSize + (cellSize - 100) / 2; // Center the texture vertically

            if (board[i][j] == 'X' && XTex != NULL) {
                GRRLIB_DrawImg(x, y, XTex, 0, 1, 1, 0xFFFFFFFF); // Draw the X texture
            } else if (board[i][j] == 'O' && OTex != NULL) {
                GRRLIB_DrawImg(x, y, OTex, 0, 1, 1, 0xFFFFFFFF); // Draw the O texture
            }
        }
    }

    if (showDrawScore) {
        DrawScore(drawABcounts.p1PressCount, drawABcounts.p2PressCount);
    }
}

void DrawCursor(int x, int y, Player* player, GRRLIB_texImg* cursorTex) {
    if (cursorTex == NULL) {
        return;
    }

    WPADData* data = WPAD_Data(player->id);
    float angle = data->orient.roll;

    // Calculate the position to draw the image such that its center is at (x, y)
    int drawX = x - cursorTex->w / 2;
    int drawY = y - cursorTex->h / 2;

    // Draw the cursor image with rotation around its center
    GRRLIB_DrawImg(drawX, drawY, cursorTex, angle, 1, 1, 0xFFFFFFFF);
}

void DrawWinningCells() {
    for (int i = 0; i < 3; i++) {
        GRRLIB_Rectangle(gridStartX + winningPositions[i].col * cellSize, gridStartY + winningPositions[i].row * cellSize, cellSize, cellSize, 0xB8FCE2AA, true);
    }
}

// Function to handle input counting
void* InputCountingThread(void* arg) {
    PressCounts* drawABcounts = (PressCounts*)arg;
    Player* p1 = getPlayer(0);
    Player* p2 = getPlayer(1);

    uint32_t p1PrevButtons = 0;
    uint32_t p2PrevButtons = 0;

    while (inputThreadRunning) {
        // Get the current button states
        uint32_t p1CurrentButtons = WPAD_ButtonsDown(p1->id);
        uint32_t p2CurrentButtons = WPAD_ButtonsDown(p2->id);

        // Count button presses for Player 1
        if ((p1CurrentButtons & (WPAD_BUTTON_A | WPAD_BUTTON_B)) && !(p1PrevButtons & (WPAD_BUTTON_A | WPAD_BUTTON_B))) {
            drawABcounts->p1PressCount++;
        }

        // Count button presses for Player 2
        if ((p2CurrentButtons & (WPAD_BUTTON_A | WPAD_BUTTON_B)) && !(p2PrevButtons & (WPAD_BUTTON_A | WPAD_BUTTON_B))) {
            drawABcounts->p2PressCount++;
        }

        // Update previous button states
        p1PrevButtons = WPAD_ButtonsHeld(p1->id);
        p2PrevButtons = WPAD_ButtonsHeld(p2->id);

        usleep(100);
    }

    return NULL;
}

void* ButtonPressChallenge(void* arg) {
    inputThreadRunning = true;

    debug_send("starting in 3 sec...");
    usleep(3000000);

    // Create the input counting thread
    lwp_t inputThread;
    LWP_CreateThread(&inputThread, InputCountingThread, &drawABcounts, NULL, 8192, 80);

    // Wait for 10 seconds
    usleep(10000000);

    // Stop the input counting thread
    inputThreadRunning = false;

    debug_send("%d, %d", drawABcounts.p1PressCount, drawABcounts.p2PressCount);

    // Determine the winner
    if (drawABcounts.p1PressCount > drawABcounts.p2PressCount) {
        debug_send("Player 1 wins the button press challenge!\n");
    } else if (drawABcounts.p2PressCount > drawABcounts.p1PressCount) {
        debug_send("Player 2 wins the button press challenge!\n");
    } else {
        debug_send("It's a draw!\n");
    }

    showDrawScore = true;
    usleep(3000000);

    ResetBoard();
    
    return NULL;
}

void DrawScore(int score1, int score2) {
    char scoreStr1[10];
    char scoreStr2[10];
    snprintf(scoreStr1, sizeof(scoreStr1), "%d", score1);
    snprintf(scoreStr2, sizeof(scoreStr2), "%d", score2);

    // Calculate the vertical center of the screen
    int centerY = screenHeight / 2;

    // Calculate the positions for the scores
    int score1X = 50; // Fixed position from the left
    int score2X = screenWidth - 50 - (strlen(scoreStr2) * numbers_0to4[0]->w); // Fixed position from the right

    // Draw the first score
    for (int i = 0; scoreStr1[i] != '\0'; i++) {
        int digit = scoreStr1[i] - '0';
        GRRLIB_texImg* digitTex = (digit < 5) ? numbers_0to4[digit] : numbers_5to9[digit - 5];
        if (digitTex != NULL) {
            GRRLIB_DrawImg(score1X + i * (digitTex->w + 5), centerY - digitTex->h / 2, digitTex, 0, 1, 1, 0xFFFFFFFF);
        }
    }

    // Draw the second score
    for (int i = 0; scoreStr2[i] != '\0'; i++) {
        int digit = scoreStr2[i] - '0';
        GRRLIB_texImg* digitTex = (digit < 5) ? numbers_0to4[digit] : numbers_5to9[digit - 5];
        if (digitTex != NULL) {
            GRRLIB_DrawImg(score2X + i * (digitTex->w + 5), centerY - digitTex->h / 2, digitTex, 0, 1, 1, 0xFFFFFFFF);
        }
    }
}

void HandleDraw() {
    lwp_t challengeThread;
    LWP_CreateThread(&challengeThread, ButtonPressChallenge, NULL, NULL, 8192, 80);
}

void ResetBoard() {
    debug_send("Resetting board.\n");

    showDrawScore = false;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }

    playedGames++;

    if(playedGames % 2 == 0) {
        p1.myTurn = true;
    } else {
        p2.myTurn = true;
    }

    p1.cursorStatus = 0;
    p2.cursorStatus = 0;

    drawABcounts.p1PressCount = 0;
    drawABcounts.p2PressCount = 0;
}