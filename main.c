#include <snes.h>

extern char tilefont, palfont;
extern char character_tiles, character_tiles_end;
extern char character_pal, character_pal_end;

typedef struct
{
    u8 oamAddress;
    int _x;
    int _y;
} character;

character player1;
unsigned short pad0;
char moved, movedBefore, winner, turn;
int row=0, col=0, turns;
char buffer[10];
char board[3][3];

void updatePos(character *p, unsigned short pad)
{
    moved=0;
    if (pad & KEY_UP)
    {
        moved=1;
        if(!movedBefore) {
            row=row==0?2:row-1;
            p->_y=row*16+1;
        }
    }
    if (pad & KEY_DOWN)
    {
        moved=1;
        if(!movedBefore) {
            row=row==2?0:row+1;
            p->_y=row*16+1;
        }
    }
    if (pad & KEY_LEFT)
    {
        moved=1;
        if(!movedBefore) {
            col=col==0?2:col-1;
            p->_x=col*16+2;
        }
    }
    if (pad & KEY_RIGHT)
    {
        moved=1;
        if(!movedBefore) {
            col=col==2?0:col+1;
            p->_x=col*16+2;
        }
    }
    oamSetXY(p->oamAddress, p->_x, p->_y);
    movedBefore=moved;
}

void showBoard() {
    int r, c;
    for(r=0; r<3; r++) {
        for(c=0; c<3; c++) {
            sprintf(buffer, "%c", board[r][c]);
            consoleDrawText(c*2+1, r*2+1, buffer);
            if(c<2)
                consoleDrawText(c*2+2, r*2+1, "|");
        }
        if(r<2)
            consoleDrawText(1, r*2+2, "-----");
    }
}

void resetBoard() {
    movedBefore=0;
    winner=0;
    turn='X';
    turns=1;
    int r, c;
    for(r=0; r<3; r++)
        for(c=0; c<3; c++)
            board[r][c]=' ';
    showBoard();
}

int main(void)
{
    // Initialize text console with our font
    consoleSetTextVramBGAdr(0x6800);
    consoleSetTextVramAdr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(0, 32, &tilefont, &palfont);

    // Init background
    bgSetGfxPtr(0, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);

    player1._x = 2;
    player1._y = 1;
    player1.oamAddress = 0;
    oamSetVisible(player1.oamAddress, OBJ_SHOW);
    oamInitGfxSet(&character_tiles, (&character_tiles_end - &character_tiles), &character_pal, (&character_pal_end - &character_pal), 0, 0x6000, OBJ_SIZE16_L32);
    oamSet(player1.oamAddress, player1._x, player1._y, 2, 0, 0, 0, 0); // Put sprite in 100,100, with maximum priority 3 from tile entry 0, palette 0
    oamSetEx(player1.oamAddress, OBJ_SMALL, OBJ_SHOW);
    resetBoard();

    setScreenOn();

    // Wait for nothing :P
    while (1)
    {
        pad0 = padsCurrent(0);
        // update character pos
        updatePos(&player1, pad0);
        if(pad0 & KEY_A && winner==0 && board[row][col]==' ') {
            board[row][col]=turn;
            if((board[0][col]==turn&&board[1][col]==turn&&board[2][col]==turn)||(board[row][0]==turn&&board[row][1]==turn&&board[row][2]==turn)||(board[1][1]==turn&&
                ((board[0][0]==turn&&board[2][2]==turn)||(board[0][2]==turn&&board[2][0]==turn))))
            {
                winner=turn;
                sprintf(buffer, "%c wins!", turn);
                consoleDrawText(1, 7, buffer);
            }
            else if(turns++==9) {
                winner='T';
                consoleDrawText(1, 7, "Tie!");
            }
            else
                turn=turn=='X'?'O':'X';
            showBoard();
        }
        if(pad0 & KEY_START && winner) {
            resetBoard();
            consoleDrawText(1, 7, "       ");
        }
        WaitForVBlank();
    }
    return 0;
}