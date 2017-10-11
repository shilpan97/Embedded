#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include "RTL.h"
#include "LPC17xx.H"		/* LPC17xx definitions	*/
#include "GLCD.h"
#include "LED.h"
#include "joystick.h"
#include "timer.h"

#define __FI    1				/* Font index 16x24						*/
#define RD      Red
#define WT      White
#define YW      Yellow
#define GR      Green
#define BL		Black
#define WINCOND 20

OS_TID t_kbd;				/* assigned task id of task: keyread	*/
OS_TID t_jst;				/* assigned task id of task: joystick	*/
OS_TID t_lcd;				/* assigned task id of task: lcd		*/
OS_TID t_calc;
OS_TID t_score;

OS_MUT mut_GLCD;			/* Mutex to control GLCD access		*/

typedef struct node{
	int speed;
	int type; 
	int x;
	int y;
	int score;
	int orientation;
	struct node *next;
} node_t ;

int cursor_y = 302;
int cursor_x = 102;
int globalscore = 0;
int numfruits = 0;
int game_end=5;
int prev_score=0;
int game_on=0;
int delFlag = 0;

int prev_time = 0;
int responsecount = 0;

unsigned char lives[10];
node_t *head = NULL;
node_t *tail = NULL;

unsigned short apple_bitmap1[] = 			{BL,BL,RD,BL,BL,BL,BL,RD,BL,BL,
											BL,RD,RD,RD,BL,BL,RD,RD,RD,BL,
											BL,RD,RD,RD,RD,RD,RD,RD,RD,BL,
											RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,
											RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,
											RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,
											RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,
											BL,RD,RD,RD,RD,RD,RD,RD,RD,BL,
											BL,RD,RD,RD,BL,BL,RD,RD,RD,BL,
											BL,BL,RD,BL,BL,BL,BL,RD,BL,BL};
											
unsigned short apple_bitmap2[] = {BL,BL,BL,RD,RD,RD,RD,BL,BL,BL,
								BL,RD,RD,RD,RD,RD,RD,RD,RD,BL,
								RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,
								BL,RD,RD,RD,RD,RD,RD,RD,RD,BL,
								BL,BL,RD,RD,RD,RD,RD,RD,BL,BL,
								BL,BL,RD,RD,RD,RD,RD,RD,BL,BL,
								BL,RD,RD,RD,RD,RD,RD,RD,RD,BL,
								RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,
								BL,RD,RD,RD,RD,RD,RD,RD,RD,BL,
								BL,BL,BL,RD,RD,RD,RD,BL,BL,BL};

unsigned short lemon_bitmap1[] = {BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
								BL,BL,BL,YW,YW,YW,YW,BL,BL,BL,
								BL,BL,YW,YW,YW,YW,YW,YW,BL,BL,
								BL,YW,YW,YW,YW,YW,YW,YW,YW,BL,
								YW,YW,YW,YW,YW,YW,YW,YW,YW,YW,
								YW,YW,YW,YW,YW,YW,YW,YW,YW,YW,
								BL,YW,YW,YW,YW,YW,YW,YW,YW,BL,
								BL,BL,YW,YW,YW,YW,YW,YW,BL,BL,
								BL,BL,BL,YW,YW,YW,YW,BL,BL,BL,
								BL,BL,BL,BL,BL,BL,BL,BL,BL,BL};
								
unsigned short lemon_bitmap2[] = {BL,BL,BL,BL,YW,YW,BL,BL,BL,BL,
									BL,BL,BL,YW,YW,YW,YW,BL,BL,BL,
									BL,BL,YW,YW,YW,YW,YW,YW,BL,BL,
									BL,YW,YW,YW,YW,YW,YW,YW,YW,BL,
									BL,YW,YW,YW,YW,YW,YW,YW,YW,BL,
									BL,YW,YW,YW,YW,YW,YW,YW,YW,BL,
									BL,YW,YW,YW,YW,YW,YW,YW,YW,BL,
									BL,BL,YW,YW,YW,YW,YW,YW,BL,BL,
									BL,BL,BL,YW,YW,YW,YW,BL,BL,BL,
									BL,BL,BL,BL,YW,YW,BL,BL,BL,BL};

unsigned short grape_bitmap[] = 	{BL,BL,BL,GR,GR,GR,GR,BL,BL,BL,
									BL,BL,GR,GR,GR,GR,GR,GR,BL,BL,
									BL,GR,GR,GR,GR,GR,GR,GR,GR,BL,
									GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,
									GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,
									GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,
									GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,
									BL,GR,GR,GR,GR,GR,GR,GR,GR,BL,
									BL,BL,GR,GR,GR,GR,GR,GR,BL,BL,
									BL,BL,BL,GR,GR,GR,GR,BL,BL,BL};


unsigned short black_bitmap[] = 			{									BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
																				BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL};

unsigned short black_bot[] = {BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL, BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL};
unsigned short cursor_bitmap[] = {WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT};
unsigned short cursor_cut[] = {BL,BL,BL,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,BL,BL,BL,
BL,BL,BL,BL,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,BL,BL,BL,BL,
BL,BL,BL,BL,BL,WT,WT,WT,WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,
WT,BL,BL,BL,BL,BL,WT,WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,WT,
WT,WT,BL,BL,BL,BL,BL,WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,WT,
WT,WT,WT,BL,BL,BL,BL,BL,WT,WT,WT,BL,BL,BL,BL,WT,WT,WT,
WT,WT,WT,WT,BL,BL,BL,BL,BL,WT,BL,BL,BL,BL,WT,WT,WT,WT,
WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,WT,BL,BL,BL,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,BL,BL,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,WT,WT,WT,WT,WT,WT,
WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,BL,BL,BL,BL,WT,WT,WT,WT,
WT,WT,WT,WT,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,WT,WT,WT,WT,
WT,WT,WT,BL,BL,BL,BL,BL,BL,WT,BL,BL,BL,BL,BL,WT,WT,WT,
WT,WT,BL,BL,BL,BL,BL,BL,WT,WT,WT,BL,BL,BL,BL,BL,WT,WT,
WT,BL,BL,BL,BL,BL,BL,WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,WT,
BL,BL,BL,BL,BL,WT,WT,WT,WT,WT,WT,WT,WT,BL,BL,BL,BL,BL,
BL,BL,BL,BL,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,BL,BL,BL,BL,
BL,BL,BL,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,WT,BL,BL,BL};

node_t* init_list(){
  node_t *curr = (node_t *) malloc(sizeof(node_t));
  curr->next = NULL;
  //tail = (node_t *) malloc(sizeof(node_t*));
  //tail = curr;
  return curr;
}

void display_list(node_t *head)
{
    int count;
    node_t *temp;
    temp=head;
    count = 0;
    while(temp->next!=NULL)
    {
        printf("Node #%d, y = %d",count, temp->y);
        
        if(temp->next != NULL)
            printf(", next->y = %d\n",temp->next->y);
        else
            printf("\n");
        
        count+=1;
        temp=temp->next;
    }
        printf("Node #%d, numfruits = %d, y = %d, tail->y = %d\n", count, numfruits, temp->y, tail->y);
}

//adding "fruit" nodes to the linked list
node_t* add(node_t* tail, int type, int x){
  node_t *curr = tail;

  if(numfruits == 0) {
    
  } else {
    curr->next = (node_t *) malloc(sizeof(node_t));
    curr = curr-> next;
  }

    curr->y = 0;
	curr->x = x;
	curr->type = type;
	curr->next = NULL;
	curr-> speed = 15;
	curr-> orientation = 0;
  
	if(type == 0){ // Apple type
		
		curr-> score = 1;
	} else if(type == 1) { // orange type
		
		curr-> score = 2;
	} else if(type == 2) { // grape type
		
		curr-> score = 3;
	}
  
  numfruits++;
  return curr;
}	

node_t* del( node_t *head){
	node_t *curr = head; 

	curr = curr->next;
  
    numfruits--;
    printf("removing head in function head y %d, curr y, %d\n", head->y, curr->y);

	free(head);
    display_list(curr);
    printf("after removed head in function curr y, %d\n", curr->y);
	return curr;
} 

void display_fruit(node_t *node){
    GLCD_Bitmap(node->x, node->y - node->speed, 10, 10, (unsigned char*) black_bitmap);
	switch(node->type){
        case 0:
			if(node->orientation < 7) {
				GLCD_Bitmap(node->x, node->y, 10, 10, (unsigned char*) apple_bitmap1);
				node->orientation++;
			} else if (node->orientation < 14){
				GLCD_Bitmap(node->x, node->y, 10, 10, (unsigned char*) apple_bitmap2);
				node->orientation++;
			} else node->orientation = 0;
            break;
        case 1:
			if(node->orientation < 6) {
				GLCD_Bitmap(node->x, node->y, 10, 10, (unsigned char*) lemon_bitmap1);
				node->orientation++;
			} else if(node->orientation < 12) {
				GLCD_Bitmap(node->x, node->y, 10, 10, (unsigned char*) lemon_bitmap2);
				node->orientation++;
			} else node->orientation = 0;
            break;
        case 2:
			GLCD_Bitmap(node->x, node->y, 10, 10, (unsigned char*) grape_bitmap);
            break;
    }
}

/*----------------------------------------------------------------------------
	Task 4 'joystick': process an input from the joystick
 *---------------------------------------------------------------------------*/
__task void joystick (void) {
	for (;;) {
		if (joystick_read() == JOY_LEFT) {
            //LED_set(3);
            //printf("%d\n", rand()%3);
//printf("lTimertoggle: %d\n", timer_read());
            cursor_x--;
		} else if (joystick_read() == JOY_RIGHT) {
            //LED_set(4) ;
            cursor_x++;
        }
    cursor_x = cursor_x % 240;
		os_dly_wait (1);				/* wait for timeout: 5 ticks		*/
	}
}

//This is required for calculating next position.
__task void calculate(void) {
    node_t *temp;
	int curr_time;
	
    while(1){
      os_mut_wait(mut_GLCD, 0xffff);
       
	   curr_time = timer_read();
	  
	  if(delFlag == 1){
		
		GLCD_DisplayString(0,0, __FI, (unsigned char*) "GOT'EM!        ");
		GLCD_Bitmap(0, cursor_y-15, 240, 15, (unsigned char*) black_bot);
		globalscore += head -> score;
		head = del(head);
		delFlag = 0;
	  }
	  
      //printf("timer pre: %d\n", timer_read()%5000);
      if(game_on == 1 && curr_time - prev_time > 4000000){
        printf("head_x %d, head_y %d, head_type %d, head_score %d, cursor_x %d, cursor_y %d\n", head->x, head->y, head->type, head->score, cursor_x, cursor_y );

        tail = add(tail, rand()%3, (rand()%180 + 20)); /// Add to the screen every 5 seconds. 
          
        prev_time = curr_time;        

        printf("timer 1: %d\n", timer_read());
          
        printf("head_x %d, head_y %d, head_type %d, head_score %d, cursor_x %d, cursor_y %d\n", head->x, head->y, head->type, head->score, cursor_x, cursor_y );
      }
        
      temp = head;
      while(temp -> next != NULL){
        temp->y += temp->speed;
        
          if(temp->y > 320){ // if the fruit is below the cursor, delete that node( aka the head)
                head = del(head);
				printf("delete head\n");
				game_end--;
                break;
            }
          temp = temp->next;
          
        
      }
      tail->y += tail->speed;  // the y direction will move with speed. 
      
           
      
      os_dly_wait(50);
      os_mut_release(mut_GLCD);
    }
}

// Task for updating the lcd.  
__task void lcd (void) {
	node_t *temp;
	int cc;

    for (;;) {            
      os_mut_wait(mut_GLCD, 0xffff);
      if( game_end < 1){
	  game_on = 0;
	  GLCD_Clear(Black);
	  GLCD_SetTextColor(WT);
	  GLCD_SetBackColor(BL);
	  sprintf(lives, "Score: %d", globalscore);
	  
	  GLCD_DisplayString(5,0, __FI, "GAME OVER");
	  GLCD_DisplayString(6,0, __FI, lives);
      os_dly_wait (1000);
	  os_mut_release(mut_GLCD);
		}
	 if( globalscore > WINCOND){
	  game_on = 0;
	  GLCD_Clear(Black);
	  GLCD_SetTextColor(WT);
	  GLCD_SetBackColor(BL);
	  sprintf(lives, "Score: %d", globalscore);
	  
	  GLCD_DisplayString(5,0, __FI, "YOU WIN!!");
	  GLCD_DisplayString(6,0, __FI, lives);
	  cc = 0;
	  while(cc<300){
	  LED_display(255);
	  os_dly_wait(300);
	  LED_display(0);
	  cc++;
	  }
	 }
    
	  
	  if(game_on == 1){
		temp = head;
	
		GLCD_Bitmap(0, cursor_y, 240, 18, (unsigned char*) black_bot);
		GLCD_Bitmap(cursor_x, cursor_y, 18, 18, (unsigned char*)cursor_bitmap);

		// display all fruits in the linked list 
		// Fruits will not display when there's only one in the list
		display_fruit(head);
		while(temp -> next != NULL && temp->next!= head){ //printshead twice              
			display_fruit(temp);
			temp = temp->next;

		} 
		display_fruit(tail);// temp is tail at this point 

		GLCD_SetTextColor(WT);
		GLCD_SetBackColor(BL);
		sprintf(lives, "Lives: %d", game_end);
		GLCD_DisplayString(0,0, __FI, lives);
	  }
	  	  os_mut_release(mut_GLCD);
		  os_dly_wait (1);
	}
  }
// Score task is to update the score using the LEDs
__task void score (void) {
    while(1){
    os_mut_wait(mut_GLCD, 0xffff);
	  
		
      if(globalscore != prev_score){
	  GLCD_SetBackColor(White);
	  
      prev_score = globalscore;
      }
	  
	   LED_display(globalscore);
	  
      os_mut_release(mut_GLCD);
      os_dly_wait (1);
    } 
}

/*----------------------------------------------------------------------------
	Task 6 'init': Initialize
 *---------------------------------------------------------------------------*/
/* NOTE: Add additional initialization calls for your tasks here */
__task void init (void) {
    os_mut_init(mut_GLCD);

	while(game_on != 1){
	  GLCD_Clear(Black);
	  GLCD_SetBackColor(White);
	  GLCD_DisplayString(5,1, __FI, " FRUIT NINJA   ");
	  GLCD_DisplayString(6,1, __FI, "Tony & Shilpan");
	 
	  os_dly_wait (400);	
 	  GLCD_Clear(Black);	
	}

    //t_kbd = os_tsk_create (keyread, 0);		/* start the kbd task				*/
    t_jst = os_tsk_create (joystick, 0);	/* start the joystick task			*/
    t_calc = os_tsk_create(calculate,0);
    t_lcd = os_tsk_create (lcd, 0);			/* start task lcd					*/
    t_score = os_tsk_create(score,0);
    os_tsk_delete_self ();
}

void EINT3_IRQHandler(void){
    LPC_GPIOINT->IO2IntClr |= (1<<10);
    if(game_on == 0){
        game_on = 1;
        /* Initialize RTX and start init	*/
        
        timer_setup();
		tail = add(tail, rand()%3, rand()%180+20);
        
    }else {
		if(abs(head->x - cursor_x) <=18 && abs(head->y -cursor_y) <=14){
			delFlag = 1;
		}
		GLCD_Bitmap(cursor_x, cursor_y, 18, 18, (unsigned char*)cursor_cut);
	}
}

/*----------------------------------------------------------------------------`
	Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/


int main (void) {
    SystemInit();
    LED_setup();							/* Initialize the LEDs				*/
    GLCD_Init();							/* Initialize the GLCD				*/
    pushbutton_setup();
    joystick_setup();

    printf("init\n");
    head = init_list();
    tail = init_list();
	tail = head;
	
    GLCD_Clear(Black);						/* Clear the GLCD					*/
    os_sys_init(init);		
}
