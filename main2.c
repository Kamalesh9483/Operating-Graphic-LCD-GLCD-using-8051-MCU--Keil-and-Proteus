#include <reg51.h>

// Data port
#define displayPort P0 		// Note while using P0 use a pull up resistor
													// while P2 has internal pull up and dont need any eternal pull ups
// Other port config
sbit RS = P3^5;
sbit RW = P3^6;
sbit EN = P3^1;
sbit CS1 = P3^3;
sbit CS2 = P3^2;
sbit RST = P3^4;

// Function prototype
void Delay(unsigned int time);
void glcd_init();
void glcd_command(unsigned char command);
void glcd_Display(unsigned int X, unsigned int Y);
void XsubData(unsigned char dot);

// Note: Refer data sheet for Display control instructions
// function for delay
void Delay(unsigned int time)
{
	int i, j;
	for (i=0; i<time; i++)
	{
		for (j=0; j<1275; j++){}	
	}
}
/********************************************************************************************************/
// Function to initialize function
void glcd_init()
{
	CS1 = 1;									// activating 1st half display
	CS2 = 1;
	RW = 0;										// RW is set ro low to select write operation									// activating 2nd half display
	//RST = 1;									// RST = 1 indicates system is being initialized

	glcd_command(0x3F);				// 0x3F means 00111111 - (DB7 to DB0) -- to turn on the display
	glcd_command(0x40);				// 0x40 means 01000000 - (DB7 to DB0) -- Y address (column = 0)
	glcd_command(0xB8);				// 0xB8 means 10111000 - (DB7 to DB0)	-- X address (page no = 0)
	glcd_command(0xC0);				// 0xC0 means 11000000 - (DB7 to DB0) -- X sub position address (line start = 0)
	
}
/********************************************************************************************************/
// Function to pass Initilization commands
void glcd_command(unsigned char command)
{
	RS = 0;										// RS is set ro low to select command register
	
	displayPort = command;
	EN = 1;										// Make High to low transistion with delay inbetween
	Delay(10);
	EN = 0;
}
/********************************************************************************************************/
// Function to write data in GLCD display
void glcd_Display(unsigned short X, unsigned short Y)
{
	
	unsigned char Xsubpos, dot;
	unsigned char Ypos;
	
	/***X position***/					
	unsigned int Xpos = X/8;
	glcd_command(0xB8 | Xpos);
	
	/***Y position***/
	if (Y <= 63)							// 1st half of GLCD - Ypos Data
	{
		CS1 = 1;
		CS2 = 0;
		Ypos = (0x40 | Y);
		glcd_command(Ypos);
	}
	else											// 2nd half of GLCD - Ypos Data
	{
		CS1 = 0;
		CS2 = 1;
		Ypos = (0x40 | (Y - 64));
		glcd_command(Ypos);	
	}
	/*** Xsub position***/
		Xsubpos = X%8;
		dot = (1 << Xsubpos);			// The modulus value is the remaining value. 1 is left shifted by Xsubpos.
		XsubData(dot);
}
/********************************************************************************************************/
// Function to activate X sub position Data registers	
void XsubData(unsigned char dot)
{
		RS = 1;										// RS is set to high to select Data register
		//RW = 0;										// RW is set to low to select write opeartion
		displayPort = dot;
		EN = 1;
		Delay(1);
		EN = 0;
}
/***********************************************************************************************************/
// Main function
void main()
{
	RST = 1;
	glcd_init();
	while(1)
		{
	glcd_Display(50, 50);
			while(1);
	//Delay(100);
		}
}