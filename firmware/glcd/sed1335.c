/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 Routinen
----------------------------------------------------------------------------*/

#include <string.h>
#include "sed1335.h"

//extern void GLCD_InitializePorts(void);
extern void GLCD_InitPorts(void);

//-------------------------------------------------------------------------------------------------
// Initialisierung des Displays / Controllers
//-------------------------------------------------------------------------------------------------
void GLCD_Initialize(void)
{
	GLCD_InitPorts();

	GLCD_WriteCommand(SED1335_SYSTEM_SET); 
	GLCD_WriteData(SED1335_SYS_P1);	
	GLCD_WriteData(SED1335_SYS_P2);		
	GLCD_WriteData(SED1335_FY);		
	GLCD_WriteData(SED1335_CR);		
	GLCD_WriteData(SED1335_TCR);	
	GLCD_WriteData(SED1335_LF);		
	GLCD_WriteData(SED1335_APL);	
	GLCD_WriteData(SED1335_APH);	
	
	GLCD_WriteCommand(SED1335_SCROLL);   
	GLCD_WriteData(SED1335_SAD1L);		
	GLCD_WriteData(SED1335_SAD1H);		
	GLCD_WriteData(SED1335_SL1);		
	GLCD_WriteData(SED1335_SAD2L);		
	GLCD_WriteData(SED1335_SAD2H);		
	GLCD_WriteData(SED1335_SL2);		
	GLCD_WriteData(SED1335_SAD3L);		
	GLCD_WriteData(SED1335_SAD3H); 		
	GLCD_WriteData(SED1335_SAD4L);		
	GLCD_WriteData(SED1335_SAD4H);		
	
	GLCD_WriteCommand(SED1335_CSRFORM);
	GLCD_WriteData(SED1335_CRX);		
	GLCD_WriteData(SED1335_CSRF_P2);		
	
	GLCD_WriteCommand(SED1335_CGRAM_ADR);       
	GLCD_WriteData(SED1335_SAGL);			
	GLCD_WriteData(SED1335_SAGH);				
	
	GLCD_WriteCommand(SED1335_CSRDIR_R);      

	GLCD_WriteCommand(SED1335_HDOT_SCR);       
	GLCD_WriteData(SED1335_SCRD);			
	
	GLCD_WriteCommand(SED1335_OVLAY);            
	GLCD_WriteData(SED1335_OVLAY_P1);			
			
	GLCD_WriteCommand(SED1335_DISP_ON);
	GLCD_WriteData(SED1335_FLASH);
}
//-------------------------------------------------------------------------------------------------
// Pixel setzen
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(unsigned int x,unsigned int y, int color)
{
    unsigned char tmp = 0;
    unsigned int address = SED1335_GRAPHICSTART + (40 * y) + (x/8); 
	GLCD_SetCursorAddress(address);

	GLCD_WriteCommand(SED1335_MREAD);
	tmp = GLCD_ReadData();

	if(color)
		tmp |= (1 << (SED1335_FX - (x % 8)));
	else
		tmp &= ~(1 << (SED1335_FX - (x % 8)));

	GLCD_SetCursorAddress(address);
	GLCD_WriteCommand(SED1335_MWRITE);
	GLCD_WriteData(tmp);
}
//-------------------------------------------------------------------------------------------------
// Textausgabe
//-------------------------------------------------------------------------------------------------
void GLCD_WriteText(char * tekst)
{
	GLCD_WriteCommand(SED1335_MWRITE);
	while(*tekst)
		GLCD_WriteData(*tekst++);
}
//-------------------------------------------------------------------------------------------------
// Textausgabe einer Zahl
//-------------------------------------------------------------------------------------------------
void GLCD_WriteTextI(int number)
{
    char buffer[7];
	itoa( number, buffer, 7);
	GLCD_WriteText( buffer );
}
//-------------------------------------------------------------------------------------------------
// Textausgabe aus AVR(-ROM?)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteTextP(char * tekst)
{
	GLCD_WriteCommand(SED1335_MWRITE);
	while(GLCD_ReadByteFromROMMemory(tekst))
		GLCD_WriteData(GLCD_ReadByteFromROMMemory(tekst++));
}
//-------------------------------------------------------------------------------------------------
// Cursor-Adresse setzen
//-------------------------------------------------------------------------------------------------
void GLCD_SetCursorAddress(unsigned int address)
{
	GLCD_WriteCommand(SED1335_CSRW);
	GLCD_WriteData((unsigned char)(address & 0xFF));
	GLCD_WriteData((unsigned char)(address >> 8));
}
//-------------------------------------------------------------------------------------------------
// Cursor-Adresse auslesen (nur High-Byte, da der SED sonst zickt)
//-------------------------------------------------------------------------------------------------
unsigned int GLCD_GetCursorAddress(void)
{
    unsigned int csr;
    char csr_low, csr_high;
	GLCD_WriteCommand(SED1335_CSRR);
	csr_low = GLCD_ReadData();
	csr_high = GLCD_ReadData();
	csr = (unsigned int)(csr_low + ( csr_high * 256 ));
    return( csr );
}
//-------------------------------------------------------------------------------------------------
// Textcursor setzen
//-------------------------------------------------------------------------------------------------
void GLCD_TextGoTo(unsigned char x, unsigned char y)
{
	GLCD_SetCursorAddress((y * 40) + x);
}
//-------------------------------------------------------------------------------------------------
// Grafikcursor-Adresse setzen
//-------------------------------------------------------------------------------------------------
void GLCD_GraphicGoTo(unsigned int x, unsigned int y)
{
	GLCD_SetCursorAddress(SED1335_GRAPHICSTART + (y * 40) + x/8);
}
//-------------------------------------------------------------------------------------------------
// Text löschen
//-------------------------------------------------------------------------------------------------
void GLCD_ClearText(void)
{
    int i;
	GLCD_TextGoTo(0,0);
	GLCD_WriteCommand(SED1335_MWRITE);
	for(i = 0; i < 1200; i++)
		GLCD_WriteData(' ');
}
//-------------------------------------------------------------------------------------------------
// Grafik löschen
//-------------------------------------------------------------------------------------------------
void GLCD_ClearGraphic(void)
{
    unsigned int i;
	GLCD_SetCursorAddress(SED1335_GRAPHICSTART);
	GLCD_WriteCommand(SED1335_MWRITE);
	for(i = 0; i < (40 * 240); i++)
		GLCD_WriteData(0x00);
}
/*
//-------------------------------------------------------------------------------------------------
// Text scrollen
//-------------------------------------------------------------------------------------------------
void GLCD_Scroll(void)
{
    int i,lv;
    char buff[40];
	for( lv=0; lv<27; lv++ )
	{
	    GLCD_TextGoTo(0,lv+1);
	    GLCD_WriteCommand(SED1335_MREAD);
	    for(i = 0; i < 40; i++)
		    buff[i] = GLCD_ReadData();

	    GLCD_TextGoTo(0,lv);
	    GLCD_WriteCommand(SED1335_MWRITE);
	    for(i = 0; i < 40; i++)
		    GLCD_WriteData(buff[i]);
	}
	GLCD_TextGoTo(0,27);
	GLCD_WriteCommand(SED1335_MWRITE);
	for(i = 0; i < 80; i++)
		GLCD_WriteData(' ');
}
//-------------------------------------------------------------------------------------------------
// Textausgabe mit Scrolling
//-------------------------------------------------------------------------------------------------
void GLCD_WriteTextScroll(char * text)
{
    char no_scroll = 1;
    unsigned int len, z, i;
    unsigned char low, high;
	GLCD_WriteCommand(SED1335_CSRR);
	low = GLCD_ReadData();
	high = GLCD_ReadData();
	len = strlen( text );
	z = len / 40;
	if( high > 3 && low > 96 ) {	// ab x;Y : 1;29
		if( z < 3 ) {
			for( i=0 ; i<=z ; i++ )
				GLCD_Scroll();
			GLCD_TextGoTo(0,28-z);
			no_scroll = 0;
		}
	}
	if( z > 3 )
		GLCD_WriteText("ERROR: Text zu lang");
	else {
		GLCD_WriteText( text );
	}
}
//-------------------------------------------------------------------------------------------------
// NewLine / CarriageReturn
//-------------------------------------------------------------------------------------------------
void GLCD_NewLine()
{
    unsigned char csr_low, csr_high;
	if( csr_high > 3 && csr_low > 96 ) {	// ab x;Y : 1;29
		GLCD_Scroll();
		GLCD_TextGoTo(0,28);
	}
	else {
		GLCD_WriteCommand(SED1335_CSRR);
		csr_low = GLCD_ReadData();
		csr_high = GLCD_ReadData();
		csr_low /= 40;
		csr_low *= 40;
		csr_low += 40;
		GLCD_WriteCommand(SED1335_CSRW);
		GLCD_WriteData(csr_low);
		GLCD_WriteData(csr_high);
	}
}
*/
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


