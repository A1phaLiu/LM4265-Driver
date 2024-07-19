#include <reg52.h>
#ifndef __LM4265_H__
#define __LM4265_H__

#define uchar unsigned char
#define uint unsigned int

/**********************************************************
               {89C51}		----     {LCD4265}
        --------------------------------------------
                P0^0		----  		D0
                P0^1		----		D1
                P0^2		----		D2
                P0^3		----		D3
                P0^4		----		D4
                P0^5		----		D5
                P0^6		----		D6
                P0^7		----		D7
                P2^4        ----        FS1
                P2^3		----		CE
                P2^2		----		C\D
                P2^1		----		RD
                P2^0		----		WR
**********************************************************/
	
/***********Pin definition***********/
#define lcd_data P0;
sbit pin_wr = P2^0;
sbit pin_rd = P2^1;
sbit pin_cd = P2^2;
sbit pin_ce = P2^3;
sbit fs = P2^4;


void delay(uint t);
void select_mode(uchar mode);
void lcd_write_data(uchar dat);
void lcd_write_cmd(uchar cmd);
void lcd_write_cmd1(uchar cmd, uchar dat1);
void lcd_write_cmd2(uchar cmd, uchar dat1, uchar dat2);
void lcd_clear_screen(void);
void lcd_display_string(uchar x, y, uchar code *string, bit FS);
void lcd_display_point(uchar x, uchar y, uchar pointbit);
void lcd_display_line(uint x1, uint y1, uint x2, uint y2, uchar linebit);
void lcd_display_img(unsigned char code *img);
void lcd_display_grid_s();
void lcd_init();


/**********************************************************
function: void delay(uint t)
feature: delay
**********************************************************/
void delay(uint t)
{
	while(t--);
}


/**********************************************************
function: void select_mode(uchar mode)
feature: change Read/Write Command/Data mode
**********************************************************/
void select_mode(uchar mode)
{
	if(mode == 0x02){		// Data write
		pin_wr = 0;
		pin_rd = 1;
		pin_cd = 0;
	}
	else if(mode == 0x03){	// Command write
		pin_wr = 0;
		pin_rd = 1;
		pin_cd = 1;
	}
	else if(mode == 0x04){	// Data read
		pin_wr = 0;
		pin_rd = 1;
		pin_cd = 1;
	}
	else if(mode == 0x05){	// Status read 
		pin_wr = 0;
		pin_rd = 1;
		pin_cd = 1;
	}
	else{					// Other
		pin_wr = 1;
		pin_rd = 1;
		pin_cd = 1;
	}
}


/**********************************************************
function: void lcd_writeData(uchar dat)
feature: Write data to the LCD
**********************************************************/
void lcd_write_data(uchar dat){
	select_mode(0x02);
	pin_ce = 0;
	P0 = dat;
	pin_wr = 1;
	pin_ce = 1;
}


/**********************************************************
function: void lcd_writeCmd(uchar cmd)
feature: Write command to the LCD without parameters
**********************************************************/
void lcd_write_cmd(uchar cmd){
	select_mode(0x03);
	pin_ce = 0;
	P0 = cmd;
	pin_wr = 1;
	pin_ce = 1;	
}


/**********************************************************
function: void lcd_writeCmd1(uchar cmd, uchar dat1)
feature: Write command to the LCD with one parameter
**********************************************************/
void lcd_write_cmd1(uchar cmd, uchar dat1){

	lcd_write_data(dat1);
	lcd_write_cmd(cmd);
}


/**********************************************************
function: void lcd_writeCmd2(uchar cmd, uchar dat1, uchar dat2)
feature: Write command to the LCD with two parameters
**********************************************************/
void lcd_write_cmd2(uchar cmd, uchar dat1, uchar dat2){

	lcd_write_data(dat1);
	lcd_write_data(dat2);
	lcd_write_cmd(cmd);
}


//void LCD_moveCursor(unsigned char X, unsigned char Y){
//	LCD_writeCmd2(0x21, X, Y);
//}

/**********************************************************
function: void lcd_clear_screen(void)
feature: Clean LCD screen
**********************************************************/
void lcd_clear_screen(void)    
{
	uint i;
	lcd_write_cmd2(0x24, 0x00, 0x00);
	for (i=0; i<2048; i++) // 2048 = 128*128/8
	{
		lcd_write_cmd1(0xC0, 0x00); // Write data 0x00, address +1
	}
}


/**********************************************************
function: void lcd_display_string(uchar x, y, uchar code *string, bit FS)
feature: Writes n CGROM characters to the specified coordinates
Parameters:
	x: (0~15)
	y: (0~15)
**********************************************************/
void lcd_display_string(uchar x, y, uchar code *string, bit FS)
{
	uint i;
	if(FS)
		fs = 1;
	else
		fs = 0;
	
	lcd_write_cmd(0x94); // Text is displayed, graphics are closed
	lcd_write_cmd2(0x24, x, y);
	
	for(i=0; i<256; i++)
	{
		// string[i]-0x20 ---> ASCII code
		lcd_write_cmd1(0xC0, string[i]-0x20);
	}
}


/**********************************************************
function: void lcd_display_point(uchar x, uchar y, uchar pointbit)
feature: draw/clear a dot on (x,y)
Parameters:
	x: (0~127)
	y: (0~127)
	pointbit: 0(clear) / 1(draw)
**********************************************************/
void lcd_display_point(uchar x, uchar y, uchar pointbit)
{
	int addr;
	fs = 0;
	if((x>128) || (y>128))
		return;
	
	lcd_write_cmd(0x98); // graphics is displayed, text are closed
	addr = 0x00 + x/8 + y*16;
	lcd_write_cmd2(0x24, addr%256, addr/256);
	if(pointbit)
		lcd_write_cmd(0xf8 | (7-x%8)); // 0xf8 is the draw dot command
	else
		lcd_write_cmd(0xf0 | (7-x%8)); // 0xf0 is the clear dot command
}


/**********************************************************
function: void lcd_display_line(uint x1, uint y1, uint x2, uint y2, uchar linebit)
feature: draw/clear a line
**********************************************************/
void lcd_display_line(uint x1, uint y1, uint x2, uint y2, uchar linebit)
{
	int dy;
	int dx;
	int stepx, stepy, fraction;
	
	fs = 0;
	dy = y2 - y1;
	dx = x2 - x1;
	
	if(dy < 0){
		dy = -dy;
		stepy = -1;
	}
	else{
		stepy = 1;
	}
	
	if(dx < 0){
		dx = -dx;
		stepx = -1;
	}
	else{
		stepx = 1;
	}
	
	dy<<=1;
	dx<<=1;
	lcd_display_point(x1, y1, linebit);
	if(dx > dy)
	{
		fraction = dy - (dx>>1);
		while(x1 != x2)
		{
			if(fraction >= 0)
			{
				y1 += stepy;
				fraction -= dx;
			}
			x1 += stepx;
			fraction += dy;
			lcd_display_point(x1, y1, linebit);
		}
	}
	else
	{
		fraction = dx - (dy>>1);
		while(y1 != y2)
		{
			if(fraction >= 0)
			{
				x1 += stepx;
				fraction -= dy;
			}
			y1 += stepy;
			fraction += dx;
			lcd_display_point(x1, y1, linebit);
		}
	}
}


/**********************************************************
function: void lcd_display_img(void)
feature: Display a image(128x128) on full screen
**********************************************************/
void lcd_display_img(unsigned char code *img)
{
	unsigned int i;
	fs = 0;
	
	lcd_write_cmd(0x98);
	lcd_write_cmd2(0x24, 0x00, 0x00);
	lcd_write_cmd(0xB0); // Enable automatic data write
	for(i=0;i<2048;i++)
		lcd_write_data(img[i]);
	lcd_write_cmd(0xB2); // Disable automatic data write
}


/**********************************************************
function: void lcd_display_grid_s()
feature: Display small grid with border
**********************************************************/
void lcd_display_grid_s(){
    lcd_display_img(grid_s);
}

void lcd_welcome(){
    lcd_display_img(welcome);
}

/**********************************************************
function: void lcd_init(void)
feature: Initialize LCD
**********************************************************/
void lcd_init(void){
	delay(10);
	pin_ce = 0;
	pin_wr = 1;
	pin_rd = 1;
	delay(5);
	
	lcd_write_cmd2(0x40, 0x00, 0x00); // Set the start address of the text area
	lcd_write_cmd2(0x42, 0x00, 0x00); // Set the start address of the graphics area
	lcd_write_cmd2(0x41, 16, 0x00); // Set the width of the text area
	lcd_write_cmd2(0x43, 16, 0x00); // Set the width of the graph area
	lcd_write_cmd2(0x22, 0x1c, 0x00); // Set the offset CGRAM address
	lcd_write_cmd(0x80); // Set display mode is OR
}


#endif
