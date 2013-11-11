/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 Routinen
----------------------------------------------------------------------------*/

#define 	SED1335_SYSTEM_SET   	0x40
#define 	SED1335_SLEEP_IN   	0x53
#define 	SED1335_DISP_OFF   	0x58
#define 	SED1335_DISP_ON   	0x59
#define 	SED1335_SCROLL   	0x44
#define 	SED1335_CSRFORM   	0x5d
#define 	SED1335_CGRAM_ADR   	0x5c
#define 	SED1335_CSRDIR_U   	0x4e
#define 	SED1335_CSRDIR_D   	0x4f
#define 	SED1335_CSRDIR_L   	0x4d
#define 	SED1335_CSRDIR_R   	0x4c
#define 	SED1335_HDOT_SCR   	0x5a
#define 	SED1335_OVLAY   	0x5b
#define 	SED1335_CSRW   		0x46
#define 	SED1335_CSRR   		0x47
#define 	SED1335_MWRITE   	0x42
#define 	SED1335_MREAD   	0x43

#define 	SED1335_SCR_WIDTH   	319
#define 	SED1335_M0   		0
#define 	SED1335_M1   		0
#define 	SED1335_M2   		0
#define 	SED1335_WS   		0
#define 	SED1335_IV   		1
#define 	SED1335_FX   		7
#define 	SED1335_FY   		7
#define 	SED1335_WF   		1
#define 	SED1335_CR   		39
#define 	SED1335_TCR   		43
#define 	SED1335_LF   		239
#define 	SED1335_APL   		40
#define 	SED1335_APH   		0
#define 	SED1335_LINES   	30
#define 	SED1335_SAD1L   	0
#define 	SED1335_SAD1H   	0
#define 	SED1335_SL1   		0xEF
#define 	SED1335_SAD2L   	0xB0
#define 	SED1335_SAD2H   	0x04
#define 	SED1335_SL2   		0xEF
#define 	SED1335_SAD3L   	0
#define 	SED1335_SAD3H  		0
#define 	SED1335_SAD4L   	0
#define 	SED1335_SAD4H   	0
#define 	SED1335_CRX   		0x04
#define 	SED1335_CRY   		0x07
#define 	SED1335_CM   		0
#define 	SED1335_MX0   		1
#define 	SED1335_MX1   		0
#define 	SED1335_DM1   		0
#define 	SED1335_DM2   		0
#define 	SED1335_OV   		0
#define 	SED1335_SAGL   		0
#define 	SED1335_SAGH   		0x70
#define 	SED1335_SCRD   		0
#define 	SED1335_FLASH   	0x16
#define 	SED1335_TEXTSIZE   	((SED1335_SAD2H << 8) + SED1335_SAD2L)

#define		SED1335_GRAPHICSTART	((SED1335_SAD2H << 8) + SED1335_SAD2L)

#define 	SED1335_GRAPHICSIZE   	((SED1335_SL2+1) * (SED1335_SCR_WIDTH+1))>>3
#define 	SED1335_MEM_END   	10800
#define 	SED1335_SYS_P1   	0x10 | (SED1335_IV << 5) | (SED1335_WS << 3) | (SED1335_M2 << 2) | (SED1335_M1 << 1) | SED1335_M0
#define 	SED1335_SYS_P2   	0x00 | (SED1335_WF << 7) | SED1335_FX
#define 	SED1335_CSRF_P2   	0x00 | (SED1335_CM << 7) | SED1335_CRY
#define 	SED1335_OVLAY_P1   	0x00 | (SED1335_OV << 4) | (SED1335_DM2 << 3) | (SED1335_DM1 << 2) | (SED1335_MX1 << 1) | SED1335_MX0
//-----------------------------------------------------------------------------
void GLCD_Initialize(void);
void GLCD_WriteCommand(unsigned char);
void GLCD_WriteData(unsigned char);
unsigned char GLCD_ReadData(void);
char GLCD_ReadByteFromROMMemory(char*);

void GLCD_ClearText(void);
void GLCD_TextGoTo(unsigned char, unsigned char);
void GLCD_WriteText(char*);
void GLCD_WriteTextP(char*);
void GLCD_WriteTextI(int);

void GLCD_ClearGraphic(void);
void GLCD_GraphicGoTo(unsigned int, unsigned int);

void GLCD_SetPixel(unsigned int,unsigned int, int);
void GLCD_SetCursorAddress(unsigned int);
unsigned int GLCD_GetCursorAddress(void);
/*
void GLCD_Scroll(void);
void GLCD_WriteTextScroll(char*);
void GLCD_NewLine(void);
*/
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
