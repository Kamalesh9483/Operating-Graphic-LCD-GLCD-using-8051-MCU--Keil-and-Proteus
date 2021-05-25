#include "reg51.h"
#include "math.h"
#include <stdio.h>

// Data port
#define displayPort P0

// Other port config
sbit RS = P2^7;
sbit RW = P2^6;
sbit EN = P2^5;
sbit CS1 = P2^4;
sbit CS2 = P2^3;
sbit RST = P2^2;
/*****************************************************************************************/
// function for delay
void Delay(unsigned int time)
{
	int i, j;
	for (i=0; i<time; i++)
	{
		for (j=0; j<1275; j++)
		{
		}
	}
}
/********************************************************************************************/
// Function for command initialisation
void init()
{
		CS1 = 1;							// Activating display of first half of LCD 
		CS2 = 1;							// Activating display of second half of LCD 
		
		RS = 0;								// Setting RS and RW to '0' to activate command mode  
		RW = 0;

		EN = 1;								// Enable line set to 1, then delay is set, then set to 0 to load data into input register
		Delay(10);
		EN = 0;
}
/*****************************************************************************************/
/****Note refer Datasheet Display Control Instruction****/
// Function for GLCD command
void glcd_Cmd()
{
	// Display ON
	displayPort = 0x3F;		// Command for Display ON --- 0000111111 is equivalent to 0x3F
	init();
}
/****************************************************************************************/
// Function for Page selection - X
void glcd_pgSel(unsigned char Xpage)
{
	displayPort = Xpage;	// Address of X 
	init();
}
/*********************************************************************************************/
// Function for Column Selection - Y
void glcd_clmSel(unsigned char Ycolumn)
{
		displayPort = Ycolumn + 0x40;	// 0x40 denotes 
		RS = 0;												// Setting RS and RW to '0' to activate command mode 
		RW = 0;
		
		EN = 1;												// Enable line set to 1, then delay is set, then set to 0 to load data into input register
		Delay(10);
		EN = 0;	
}
/**************************************************************************************************/
// function to find the remainder ie the data of X sub position 
unsigned int glcd_subXpos(unsigned char Xsubpos)
{
	unsigned int Xdata, power;
	Xdata = Xsubpos%4;
	power = pow(2, Xdata );			// base^power = pow(base, power)
	return power;
}
/************************************************************************************************/
// function to Display data in LCD
void glcd_display(Y, unsigned char dataSub)
{
	displayPort = dataSub;
	if (Y <= 63)
	{
		CS1 = 0;						
		CS2 = 1;
	}
	else 
	{
		CS1 = 1;
		CS2 = 0;
	}
		RS = 1;
		RW = 0;
		
		EN = 1;
		Delay(10);
		EN = 0;
	}
/**********************************************************************************************/
// Function for GLCD coordinates display
void glcd_disp(unsigned int X, unsigned int Y)
{
	unsigned int Xpos;
	unsigned int dataSub;
	glcd_Cmd();
	
	// Page selection - X address
	Xpos = X/8;
	glcd_pgSel(0xB8 | Xpos); 	// 0xB8 is the page 1 along X direction. it is or operated with the required page number
	
	// Column Selection - Y address
	glcd_clmSel(Y);
	
	// Calculating the Data value to put in Data register
	dataSub = glcd_subXpos(X);
	
	// Data Display in LCD
	glcd_display(Y, dataSub);
}
/**************************************************************************************************/
void main()
{
	Delay(10);
	RST = 1;
	Delay(10);
	RST = 0;
	// Enter the X and Y coordinates in range of (0 to 127) for Y coordinate and (0 to 63) for X coordinate
	while (1)
	{
	glcd_disp(0,0);
	}
}
