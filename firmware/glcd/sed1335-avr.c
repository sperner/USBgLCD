/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 AVR spezifisch
----------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


// Data-Port Pin-Definitions
#define SED1335_DATA_PORT0	PORTB
#define SED1335_DATA_DIR0	DDRB
#define SED1335_DATA_PIN0	PINB
#define SED1335_DATA0		(1 << PB0)

#define SED1335_DATA_PORT1	PORTB
#define SED1335_DATA_DIR1	DDRB
#define SED1335_DATA_PIN1	PINB
#define SED1335_DATA1		(1 << PB3)

#define SED1335_DATA_PORT2	PORTB
#define SED1335_DATA_DIR2	DDRB
#define SED1335_DATA_PIN2	PINB
#define SED1335_DATA2		(1 << PB4)

#define SED1335_DATA_PORT3	PORTB
#define SED1335_DATA_DIR3	DDRB
#define SED1335_DATA_PIN3	PINB
#define SED1335_DATA3		(1 << PB5)

#define SED1335_DATA_PORT4	PORTC
#define SED1335_DATA_DIR4	DDRC
#define SED1335_DATA_PIN4	PINC
#define SED1335_DATA4		(1 << PC0)

#define SED1335_DATA_PORT5	PORTC
#define SED1335_DATA_DIR5	DDRC
#define SED1335_DATA_PIN5	PINC
#define SED1335_DATA5		(1 << PC1)

#define SED1335_DATA_PORT6	PORTC
#define SED1335_DATA_DIR6	DDRC
#define SED1335_DATA_PIN6	PINC
#define SED1335_DATA6		(1 << PC2)

#define SED1335_DATA_PORT7	PORTC
#define SED1335_DATA_DIR7	DDRC
#define SED1335_DATA_PIN7	PINC
#define SED1335_DATA7		(1 << PC3)


// Control-Port Pin-Definitions
#define SED1335_CONTROL_PORT_RES PORTC
#define SED1335_CONTROL_DIR_RES	DDRC
#define	SED1335_CONTROL_PIN_RES	PINC
#define SED1335_RES		(1 << PC4)

#define SED1335_CONTROL_PORT_RD	PORTC
#define SED1335_CONTROL_DIR_RD	DDRC
#define	SED1335_CONTROL_PIN_RD	PINC
#define SED1335_RD		(1 << PC5)

#define SED1335_CONTROL_PORT_WR	PORTD
#define SED1335_CONTROL_DIR_WR	DDRD
#define	SED1335_CONTROL_PIN_WR	PIND
#define SED1335_WR		(1 << PD5)

#define SED1335_CONTROL_PORT_CS	PORTD
#define SED1335_CONTROL_DIR_CS	DDRD
#define	SED1335_CONTROL_PIN_CS	PIND
#define SED1335_CS		(1 << PD6)

#define SED1335_CONTROL_PORT_A0	PORTD
#define SED1335_CONTROL_DIR_A0	DDRD
#define	SED1335_CONTROL_PIN_A0	PIND
#define SED1335_A0		(1 << PD7)

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_InitPorts(void)
{
SED1335_DATA_DIR0 |= (SED1335_DATA0);
SED1335_DATA_DIR1 |= (SED1335_DATA1);
SED1335_DATA_DIR2 |= (SED1335_DATA2);
SED1335_DATA_DIR3 |= (SED1335_DATA3);
SED1335_DATA_DIR4 |= (SED1335_DATA4);
SED1335_DATA_DIR5 |= (SED1335_DATA5);
SED1335_DATA_DIR6 |= (SED1335_DATA6);
SED1335_DATA_DIR7 |= (SED1335_DATA7);
SED1335_CONTROL_PORT_A0 |= (SED1335_A0);
SED1335_CONTROL_PORT_WR |= (SED1335_WR);
SED1335_CONTROL_PORT_RD |= (SED1335_RD);
SED1335_CONTROL_PORT_CS |= (SED1335_CS);
SED1335_CONTROL_PORT_RES |= (SED1335_RES);
SED1335_CONTROL_DIR_A0 |= (SED1335_A0);
SED1335_CONTROL_DIR_WR |= (SED1335_WR);
SED1335_CONTROL_DIR_RD |= (SED1335_RD);
SED1335_CONTROL_DIR_CS |= (SED1335_CS);
SED1335_CONTROL_DIR_RES |= (SED1335_RES);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(unsigned char dataToWrite)
{
if(dataToWrite & 0x01)	SED1335_DATA_PORT0 |= (SED1335_DATA0);
else			SED1335_DATA_PORT0 &= ~(SED1335_DATA0);
if(dataToWrite & 0x02)	SED1335_DATA_PORT1 |= (SED1335_DATA1);
else			SED1335_DATA_PORT1 &= ~(SED1335_DATA1);
if(dataToWrite & 0x04)	SED1335_DATA_PORT2 |= (SED1335_DATA2);
else			SED1335_DATA_PORT2 &= ~(SED1335_DATA2);
if(dataToWrite & 0x08)	SED1335_DATA_PORT3 |= (SED1335_DATA3);
else			SED1335_DATA_PORT3 &= ~(SED1335_DATA3);
if(dataToWrite & 0x10)	SED1335_DATA_PORT4 |= (SED1335_DATA4);
else			SED1335_DATA_PORT4 &= ~(SED1335_DATA4);
if(dataToWrite & 0x20)	SED1335_DATA_PORT5 |= (SED1335_DATA5);
else			SED1335_DATA_PORT5 &= ~(SED1335_DATA5);
if(dataToWrite & 0x40)	SED1335_DATA_PORT6 |= (SED1335_DATA6);
else			SED1335_DATA_PORT6 &= ~(SED1335_DATA6);
if(dataToWrite & 0x80)	SED1335_DATA_PORT7 |= (SED1335_DATA7);
else			SED1335_DATA_PORT7 &= ~(SED1335_DATA7);
SED1335_DATA_DIR0 |= (SED1335_DATA0);
SED1335_DATA_DIR1 |= (SED1335_DATA1);
SED1335_DATA_DIR2 |= (SED1335_DATA2);
SED1335_DATA_DIR3 |= (SED1335_DATA3);
SED1335_DATA_DIR4 |= (SED1335_DATA4);
SED1335_DATA_DIR5 |= (SED1335_DATA5);
SED1335_DATA_DIR6 |= (SED1335_DATA6);
SED1335_DATA_DIR7 |= (SED1335_DATA7);
SED1335_CONTROL_PORT_CS &= ~(SED1335_CS);
SED1335_CONTROL_PORT_A0 &= ~(SED1335_A0);
SED1335_CONTROL_PORT_WR &= ~(SED1335_WR);
asm("nop");
SED1335_CONTROL_PORT_CS |= (SED1335_CS);
SED1335_CONTROL_PORT_A0 |= (SED1335_A0);
SED1335_CONTROL_PORT_WR |= (SED1335_WR);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(unsigned char commandToWrite)
{
if(commandToWrite & 0x01)	SED1335_DATA_PORT0 |= (SED1335_DATA0);
else				SED1335_DATA_PORT0 &= ~(SED1335_DATA0);
if(commandToWrite & 0x02)	SED1335_DATA_PORT1 |= (SED1335_DATA1);
else				SED1335_DATA_PORT1 &= ~(SED1335_DATA1);
if(commandToWrite & 0x04)	SED1335_DATA_PORT2 |= (SED1335_DATA2);
else				SED1335_DATA_PORT2 &= ~(SED1335_DATA2);
if(commandToWrite & 0x08)	SED1335_DATA_PORT3 |= (SED1335_DATA3);
else				SED1335_DATA_PORT3 &= ~(SED1335_DATA3);
if(commandToWrite & 0x10)	SED1335_DATA_PORT4 |= (SED1335_DATA4);
else				SED1335_DATA_PORT4 &= ~(SED1335_DATA4);
if(commandToWrite & 0x20)	SED1335_DATA_PORT5 |= (SED1335_DATA5);
else				SED1335_DATA_PORT5 &= ~(SED1335_DATA5);
if(commandToWrite & 0x40)	SED1335_DATA_PORT6 |= (SED1335_DATA6);
else				SED1335_DATA_PORT6 &= ~(SED1335_DATA6);
if(commandToWrite & 0x80)	SED1335_DATA_PORT7 |= (SED1335_DATA7);
else				SED1335_DATA_PORT7 &= ~(SED1335_DATA7);
SED1335_DATA_DIR0 |= (SED1335_DATA0);
SED1335_DATA_DIR1 |= (SED1335_DATA1);
SED1335_DATA_DIR2 |= (SED1335_DATA2);
SED1335_DATA_DIR3 |= (SED1335_DATA3);
SED1335_DATA_DIR4 |= (SED1335_DATA4);
SED1335_DATA_DIR5 |= (SED1335_DATA5);
SED1335_DATA_DIR6 |= (SED1335_DATA6);
SED1335_DATA_DIR7 |= (SED1335_DATA7);
SED1335_CONTROL_PORT_CS &= ~(SED1335_CS);
SED1335_CONTROL_PORT_WR &= ~(SED1335_WR);
asm("nop");
SED1335_CONTROL_PORT_CS |= (SED1335_CS);
SED1335_CONTROL_PORT_WR |= (SED1335_WR);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadData(void)
{
unsigned char tmp;
SED1335_CONTROL_PORT_CS &= ~(SED1335_CS);
SED1335_CONTROL_PORT_RD &= ~(SED1335_RD);
asm("nop");
SED1335_DATA_DIR0 &= ~(SED1335_DATA0);
SED1335_DATA_DIR1 &= ~(SED1335_DATA1);
SED1335_DATA_DIR2 &= ~(SED1335_DATA2);
SED1335_DATA_DIR3 &= ~(SED1335_DATA3);
SED1335_DATA_DIR4 &= ~(SED1335_DATA4);
SED1335_DATA_DIR5 &= ~(SED1335_DATA5);
SED1335_DATA_DIR6 &= ~(SED1335_DATA6);
SED1335_DATA_DIR7 &= ~(SED1335_DATA7);
asm("nop");
tmp = 0;
if(SED1335_DATA_PIN0 & SED1335_DATA0)	tmp |= 0x01;
if(SED1335_DATA_PIN1 & SED1335_DATA1)	tmp |= 0x02;
if(SED1335_DATA_PIN2 & SED1335_DATA2)	tmp |= 0x04;
if(SED1335_DATA_PIN3 & SED1335_DATA3)	tmp |= 0x08;
if(SED1335_DATA_PIN4 & SED1335_DATA4)	tmp |= 0x10;
if(SED1335_DATA_PIN5 & SED1335_DATA5)	tmp |= 0x20;
if(SED1335_DATA_PIN6 & SED1335_DATA6)	tmp |= 0x40;
if(SED1335_DATA_PIN7 & SED1335_DATA7)	tmp |= 0x80;
SED1335_CONTROL_PORT_CS |= (SED1335_CS);
SED1335_CONTROL_PORT_RD |= (SED1335_RD);
return tmp;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadByteFromROMMemory(unsigned char * ptr)
{
return pgm_read_byte(ptr);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

