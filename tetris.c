#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
// Utility functions : 

void clear()
{
	printf("\e[1;1H\e[2J"); 
}

void delay(int milli) 
{
	usleep(milli * 1000); 
}

char BAR_SHAPE[4][4] = { {'1','0','0','0'},
		   	 {'1','0','0','0'},
		   	 {'1','0','0','0'},
		   	 {'1','0','0','0'}}; 

char SQR_SHAPE[4][4] = { 
		   	{'0','1','1','0'},
		   	{'0','1','1','0'},
		   	{'0','0','0','0'},
		   	{'0','0','0','0'}}; 
			

typedef struct 
{
	int x;
	int y;
	int width;
	int height; 
	int speed; 
	char shape[4][4]; 		  
} Piece;  

typedef struct
{
	char board[20][10]; 
} Board;

void add_piece_board(Piece *ptr, Board *Bptr) 
{
	int pieceX = ptr->x; 
	int pieceY = ptr->y; 
	int numRows = 4; 
	int numCols = 4; 
	for (int i=0; i<numRows;i++)
	{
		for (int j=0; j<numCols; j++)
		{
			if (ptr->shape[i][j] == '0') continue; 
			Bptr->board[pieceY+i][pieceX+j] = '1';
		}
	}
}

void display_board(Board *ptr)
{
	int numRows = 20; 
	int numCols = 10; 
	for (int i=0; i<numRows; i++) 
	{
		for (int j=0; j<numCols; j++)
		{ 
			printf("%c ",ptr->board[i][j]); 
		}
		printf("\n");
	}
}

void init_piece(Piece *p, int x, int y, int speed,int width, int height,char shape[][4]) 
{
	p->x = x; 
	p->y = y; 
	p->speed = speed;
	p->width = width; 
	p->height = height; 
	memcpy(p->shape, shape, sizeof(p->shape)); 
}

void init_board(Board *p) 
{
	memset(p->board, '0', sizeof(p->board));
}

int main() 
{
	Board new_board;
	Board *board_ptr = &new_board;
	init_board(board_ptr); 

	display_board(board_ptr);
	
	Piece bar;
	Piece *bar_ptr = &bar; 	
	init_piece(bar_ptr,0,0,1,1,4,BAR_SHAPE);  

	
	add_piece_board(bar_ptr,board_ptr); 
	int running = 1;

	while (running) 
	{
		clear(); 
		init_board(board_ptr);
		bar_ptr->y += bar_ptr->speed;
		add_piece_board(bar_ptr,board_ptr);
		display_board(board_ptr); 
		if (bar_ptr->y+bar_ptr->height >= 20) 
		{
			break; 
		}
		delay(500); 
	} 	
	
	return 0;
}
