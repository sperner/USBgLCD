/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 to USB Adapter Firmware
		 partly based on Christian Starkjohanns PowerSwitch
----------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "oddebug.h"

#include "glcd/sed1335.h"
#include "glcd/graphic.h"

#define DEBUG 0


unsigned char usb_cmd_nr =0;
int x1, y1, x2, y2;


#define EEPROM_LOCATION (void *)37

static uchar    actionTimers[8];

static void outputByte(uchar b)
{
    PORTD = (PORTD & ~0x03) | (b & 0x03);  //->nur PD0-1
}

static uchar    computeTemporaryChanges(void)
{
uchar   i, status = 0, mask = 1;

    for(i=0;i<8;i++){
        if(actionTimers[i])
            status |= mask;
        mask <<= 1;
    }
    return status;
}

static void computeOutputStatus(void)
{
uchar   status = eeprom_read_byte(EEPROM_LOCATION) ^ computeTemporaryChanges();

    outputByte(status);
}


static void timerInterrupt(void)
{
static uchar    prescaler;
uchar           i;

    if(!prescaler--){
        prescaler = 8;  /* rate = 12M / 1024 * 256 * 9 */
        for(i=0;i<8;i++){
            if(actionTimers[i])
                actionTimers[i]--;
        }
        computeOutputStatus();
    }
}

USB_PUBLIC uchar usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;
uchar           status = eeprom_read_byte(EEPROM_LOCATION);
//static uchar    replyBuf[2];
static uchar    replyBuf[4];

    usbMsgPtr = replyBuf;
    if(rq->bRequest == 0){  /* PSCMD_ECHO */
        replyBuf[0] = rq->wValue.bytes[0];
        replyBuf[1] = rq->wValue.bytes[1];
        return 2;
    }
    if(rq->bRequest == 1){  /* PSCMD_GET / GET_STATUS -> result = 2 bytes */
        replyBuf[0] = status;
        replyBuf[1] = computeTemporaryChanges();
        return 2;
    }
    if(rq->bRequest == 2 || rq->bRequest == 3){ /* SWITCH_ON or SWITCH_OFF, index = bit number */
        uchar bit = rq->wIndex.bytes[0] & 7;
        uchar mask = 1 << bit;
        uchar needChange, isOn = status & mask;
        if(rq->bRequest == 2){  /* PSCMD_ON */
            status |= mask;
            needChange = !isOn;
        }else{              /* PSCMD_OFF = 3 */
            status &= ~mask;
            needChange = isOn;
        }
        if(rq->wValue.bytes[0] == 0){   /* duration == 0 -> permanent switch */
            actionTimers[bit] = 0;
/*==>>*/    status &= 0x03;	//nur PD0-1
            eeprom_write_byte(EEPROM_LOCATION, status);
        }else if(needChange){   /* temporary switch: value = duration in 200ms units */
            actionTimers[bit] = rq->wValue.bytes[0];
        }
    }
    if(rq->bRequest == 5){  /* USB_DEBUG */
        replyBuf[0] = 1;
        replyBuf[1] = 2;
        replyBuf[2] = 3;
        replyBuf[3] = 4;
        return 4;
    }
    if(rq->bRequest == 10){  /* LCD_INIT */
	GLCD_Initialize();
	OCR1A=24;
	OCR1B=100;
    }
    if(rq->bRequest == 11){  /* LCD_CLR_GFX */
	GLCD_ClearGraphic();
    }
    if(rq->bRequest == 12){  /* LCD_CLR_TXT */
	GLCD_ClearText();
    }
    if(rq->bRequest == 13){  /* LCD_PIXEL */
	unsigned char highbyte, lowbyte;
	unsigned int x ,y;
	lowbyte = rq->wValue.bytes[0];
	highbyte = rq->wValue.bytes[1];
	x = (highbyte << 8) | lowbyte;
	y = rq->wIndex.bytes[0];
	GLCD_SetPixel(x, y, 1);
    }
    if(rq->bRequest == 14){  /* LCD_GOTO */
	GLCD_TextGoTo((int)rq->wValue.bytes[0], (int)rq->wIndex.bytes[0]);
    }
    if(rq->bRequest == 15){  /* LCD_TEXT */
	char* puffer;
	sprintf(puffer,"%c",(int)((rq->wValue.bytes[1] << 8) | rq->wValue.bytes[0]));
	GLCD_WriteText(puffer);
    }
    if(rq->bRequest == 16){  /* LCD_CONTRAST */
	if(((int)(rq->wValue.bytes[1] << 8) | rq->wValue.bytes[0]) == +1)
	    OCR1A--;
	if(((int)(rq->wValue.bytes[1] << 8) | rq->wValue.bytes[0]) == -1)
	    OCR1A++;
    }
    if(rq->bRequest == 17){  /* LCD_BACKLIGHT */
	if(((int)(rq->wValue.bytes[1] << 8) | rq->wValue.bytes[0]) == +1)
	    OCR1B += 3;
	if(((int)(rq->wValue.bytes[1] << 8) | rq->wValue.bytes[0]) == -1)
	    OCR1B -= 3;
    }
    if(rq->bRequest == 18){  /* LCD_LINE */
	if(usb_cmd_nr == 0) {
	    x1 = ((int)(rq->wValue.bytes[1] << 8) | rq->wValue.bytes[0]);
	    y1 = ((int)(rq->wIndex.bytes[1] << 8) | rq->wIndex.bytes[0]);
	    usb_cmd_nr++;
	}else 
	    if(usb_cmd_nr == 1) {
	    x2 = ((int)(rq->wValue.bytes[1] << 8) | rq->wValue.bytes[0]);
	    y2 = ((int)(rq->wIndex.bytes[1] << 8) | rq->wIndex.bytes[0]);
	    usb_cmd_nr=0;
	    GLCD_Line( x1, y1, x2, y2 );
	}
    }
    computeOutputStatus();
    return 0;
}

// for compatibility
#if !defined TCCR0 && defined TCCR0B
#define TCCR0   TCCR0B
#endif
#if !defined TIFR && defined TIFR0
#define TIFR    TIFR0
#endif

int main(void)
{
  uchar   i;

    /* initialisiere LCD */
    GLCD_Initialize();				//PortA.0-7 + PortC.0-4
    GLCD_ClearText();
    GLCD_ClearGraphic();
    DDRB |= (1<<PB1)|(1<<PB2);			//OC1A+OC1B -> Ausgänge
    TCCR1A = (1<<WGM10)|(1<<COM1A1)|(1<<COM1B1);//Standard-PWM...
    TCCR1B = (1<<CS10);
    OCR1A=24;					//Kontrast
    OCR1B=100;					//Hintergrundbeleuchtung
    
    wdt_enable(WDTO_1S);
    odDebugInit();
    DDRD = ~((1 << 2)|(1 << 4));
    PORTD = 0;

    computeOutputStatus();
    usbDeviceDisconnect();
    i = 0;
    while(--i){
        wdt_reset();
        _delay_ms(2);
    }
    usbDeviceConnect();
    TCCR0 = 5;
    usbInit();
    sei();

    #if DEBUG
	GLCD_TextGoTo( 1, 1 );
	GLCD_WriteText( "AtMega8 USB->gLCD is up and running!" );
	GLCD_TextGoTo( 1, 3 );
	GLCD_WriteText( "AtMega8 USB->gLCD is up and running!" );
	GLCD_TextGoTo( 1, 5 );
	GLCD_WriteText( "AtMega8 USB->gLCD is up and running!" );
    #endif
    
    for(;;){ // main loop
        wdt_reset();
        usbPoll();
        if(TIFR & (1 << TOV0)){
            TIFR |= 1 << TOV0;
            timerInterrupt();
	}
    }
    return 0;
}
