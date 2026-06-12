#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>


struct Piece; 
struct Board; 

void clear(); 
void delay(int); 
void rotate_piece(struct Piece *);
void add_piece_board(struct Piece *); 
void display_board();
void init_piece(struct Piece *,int,int,int,int,char [][2]);
void clear_board();
void clear_lines(int);
void check_lines();
void controller(struct Piece *); 
void rotate_piece(struct Piece *);
int get_min(struct Piece *, int);
int get_max(struct Piece *, int); 
int check_collision(struct Piece *, int, int);
int check_if_no_lose();
struct Piece* select_active_piece(struct Piece *);

// Utility functions : 
void clear()
{
	printf("\e[1;1H\e[2J"); 
}

void delay(int milli) 
{
	usleep(milli * 1000); 
}

char piece_char = '#'; 
char board_char = '-';
char init_board[20][10];

char BAR_SHAPE[4][2] = { {0,0},
       			 {0,1},	
       			 {0,2},	
       			 {0,3}};	
char SQR_SHAPE[4][2] = { 
			 {0,0}, 
			 {1,1}, 
			 {0,1}, 
			 {1,0}}; 

char S_SHAPE[4][2] = {
		         {2,1},
		         {0,2},
		         {1,2},
		         {1,1}};
char Z_SHAPE[4][2] = {
			{0,0},
			{1,0},
			{1,1},
			{2,1}};

char J_SHAPE[4][2] = {
			{0,2},
			{1,0},
			{1,1},
			{1,2}};

char L_SHAPE[4][2] = {
			{0,0},
			{1,2},
			{0,2},
			{0,1}};

char T_SHAPE[4][2] = {
			{1,0},
			{0,1},
			{1,1},
			{2,1}};

int score = 0;
int lines = 0;
int SPEED = 1;

typedef struct Piece
{
	int x;
	int y;
	int width;
	int height; 
	char shape[4][2]; 		  
} Piece;  


typedef struct Board
{
	char board[20][10]; 
} Board;

Board new_board;
Board *Bptr = &new_board;

Piece bar; 
Piece square;
Piece s; 
Piece z; 
Piece l;
Piece j; 
Piece t; 

void add_piece_board(Piece *ptr) 
{
	int pieceX = ptr->x; 
	int pieceY = ptr->y; 
	int numRows = sizeof(ptr->shape[0]) / sizeof(ptr->shape[0][0]); 
	for (int i=0; i<4;i++)
	{
		int point_x = ptr->shape[i][0]+pieceX; 
		int point_y = ptr->shape[i][1]+pieceY; 
		Bptr->board[point_y][point_x] = piece_char; 	
	}
}

void display_board()
{

	int numRows = 20; 
	int numCols = 10; 
	for (int i=0; i<numRows; i++) 
	{
		for (int j=0; j<numCols; j++)
		{ 
			printf(" %c ",Bptr->board[i][j]); 
		}
		printf("\n");
	}
}

void init_piece(Piece *p, int x, int y,int width, int height,char shape[][2]) 
{
	p->x = x; 
	p->y = y; 
	p->width = width; 
	p->height = height; 
	memcpy(p->shape, shape, sizeof(p->shape)); 
}

void clear_board() 
{
	memcpy(Bptr->board, init_board, sizeof(Bptr->board));	
}

Piece* select_active_piece(Piece *ptr)
{
//	int piece = 0;
	int piece = rand() % 8;

	switch (piece) 
	{
		case 0:
			ptr = &bar; 
			init_piece(ptr,4,0,1,4,BAR_SHAPE);
			break; 
		case 1: 
			ptr = &square; 
			init_piece(ptr,4,0,2,2,SQR_SHAPE);
			break; 
		case 2: 
			ptr = &s; 
			init_piece(ptr,4,0,3,2,S_SHAPE);
			break; 
		case 3: 
			ptr = &z;
			init_piece(ptr,4,0,3,2,Z_SHAPE);
			break;
		case 4: 
			ptr = &t; 
			init_piece(ptr,4,0,3,2,T_SHAPE); 
			break; 
		case 5: 
			ptr = &j;
			init_piece(ptr,4,1,2,3,J_SHAPE);
		        break;	
		case 6: 
			ptr = &l;
			init_piece(ptr,4,0,2,3,L_SHAPE);
			break;

	}
	return ptr; 
}

int get_min(Piece *ptr, int index)
{ 
	int lowest = ptr->shape[0][0];
	int rows = sizeof(ptr->shape) / sizeof(ptr->shape[0]); 
	for (int i=0; i<rows; i++)
	{
		if (ptr->shape[i][index] < lowest)
		{
			lowest = ptr->shape[i][index];
		}
	}
	return lowest+1;
}

int get_max(Piece *ptr, int index)
{ 	
	int max = ptr->shape[0][0];
	int rows = sizeof(ptr->shape) / sizeof(ptr->shape[0]); 
	for (int i=0; i<4; i++)
	{
		if (ptr->shape[i][index] > max)
		{
			max = ptr->shape[i][index];
		}
	}

	return max+1;
}


int check_collision(Piece *ptr, int newx, int yoffset) 
{
	int pieceX = newx; 
	int pieceY = ptr->y+yoffset;
	
	
	for (int i=0; i<4; i++)
	{
		int point_x = ptr->shape[i][0]+pieceX;
		int point_y = ptr->shape[i][1]+pieceY; 
		if (init_board[point_y][point_x] == piece_char) return 1;
	}	

	return 0;
}

int check_if_no_lose()
{
	int topRow = 0;
	for (int j=0;j<10;j++)
	{
		if (Bptr->board[1][j] == piece_char) return 0;
	}
	return 1; 
}

void clear_lines(int start_line_clr) 
{
	for (int i=start_line_clr; i>=0; i--)
	{
		for (int j=0;j<10;j++)
		{
			if (i==start_line_clr) Bptr->board[i][j] = board_char; 
			if (Bptr->board[i][j] != piece_char) continue; 
		        Bptr->board[i][j] = board_char;	
		        Bptr->board[i+1][j] = piece_char;
		}
	}
}	
				
void check_lines()
{
	int numRows = 20; 
	int numCols = 10; 
	int line_clr = 0; 

	for (int i=0; i<numRows;i++)
	{
		for (int j=0; j<numCols;j++)
		{
			if (Bptr->board[i][j] == board_char){ line_clr=0;  break;}
			else line_clr = 1; 
		}
		if (line_clr) 
		{
			score += SPEED * 80; 
			lines++;
			if (lines % 100 == 0) SPEED++;
			clear_lines(i);
			printf("CLEAR LINES"); 
		}
	     	line_clr = 0;	
	}
}

void controller(Piece *ptr) 
{
	if (_kbhit()) { 
		int ch = _getch();
		int newx = 0; 
		switch (ch) 
		{ 
			case 97:
				newx = ptr->x-1; 
			       	if (get_min(ptr,0)+newx<=0) break;	
				if (check_collision(ptr,newx,0)) break; 
				ptr->x=newx; 	
				break; 
			case 100:
			        newx = ptr->x+1; 	
				if (newx+get_max(ptr,0)>10) break;
				if (check_collision(ptr,newx,0)) break; 
				ptr->x=newx; 
				break;
			case 113: 
				rotate_piece(ptr); 
				break; 
			case 27:
				printf("Exit");
				exit(0);
				break; 	
			
		}
	}
}

void rotate_piece(Piece *ptr)
{
	int numRows = sizeof(ptr->shape) / sizeof(ptr->shape[0]); 
	int width = ptr->width; 
	int height = ptr->height;
	int x_pivot_point = width/2; 
	int y_pivot_point = height/2; 

	if (width == height) return;  	

	for (int i=0; i<numRows; i++)
	{
		
		int rot_x = -1*(ptr->shape[i][1]-x_pivot_point); 
		int rot_y = (ptr->shape[i][0]-y_pivot_point);
	       	
		ptr->shape[i][0] = rot_x+x_pivot_point; 
		ptr->shape[i][1] = rot_y+y_pivot_point;	

	}
	ptr->width = height; 
	ptr->height = width; 
}


int main() 
{
	srand(time(NULL));
	memset(init_board,board_char,sizeof(init_board));
	clear_board(); 

	Piece *active_ptr; 

	int move_active_piece = 1;
       	int game_active = 1; 
	int bottom_collision = 0; 
	while (game_active) 
	{
		active_ptr = select_active_piece(active_ptr); 
		while (move_active_piece)  
		{
			clear(); 
			printf("Score: %d	Lines: %d\n",score,lines);
		//	controller(active_ptr);
			clear_board();
			for (int i=0;i<SPEED;i++)
			{
				controller(active_ptr);
				if(check_collision(active_ptr,active_ptr->x,1)) 
				{
					move_active_piece = 0; break;
				}
				//clear_board(board_ptr);
				active_ptr->y += 1;
			//	add_piece_board(active_ptr,board_ptr);
			//	display_board(board_ptr);
				if (get_max(active_ptr,1)+active_ptr->y >= 20)
				{
					controller(active_ptr);
					move_active_piece = 0; break; 
				}
			}

			add_piece_board(active_ptr);
			display_board(); 
			
			delay(200); 
		}
		game_active = check_if_no_lose(); 
		check_lines();
		move_active_piece = 1; 
		memcpy(init_board, Bptr->board, sizeof(Bptr->board));	
		
	}
	printf("You lost!");
	return 0;
}
