/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 to USB Adapter Qt-Paint-Program
		 partly based on Christian Starkjohanns PowerSwitch
----------------------------------------------------------------------------*/

#ifndef USBGLCD_H
#define USBGLCD_H

#include <QWidget>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

/* PowerSwitch uses the free shared default VID/PID. If you want to see an
 * example device lookup where an individually reserved PID is used, see our
 * RemoteSensor reference implementation. */
#define USBDEV_SHARED_VENDOR    0x16C0  /* VOTI */
#define USBDEV_SHARED_PRODUCT   0x05DC  /* Obdev's free shared PID */
/* Use obdev's generic shared VID/PID pair and follow the rules outlined
 * in firmware/usbdrv/USBID-License.txt. */

#define USB_ERROR_NOTFOUND  1
#define USB_ERROR_ACCESS    2
#define USB_ERROR_IO        3


/* These are the vendor specific SETUP commands */
#define PSCMD_ECHO     	0
#define PSCMD_GET      	1
#define PSCMD_ON       	2
#define PSCMD_OFF      	3
#define USB_DEBUG      	5
#define LCD_INIT       	10
#define LCD_CLR_GFX    	11
#define LCD_CLR_TXT    	12
#define LCD_PIXEL      	13
#define LCD_GOTO       	14
#define LCD_TEXT       	15
#define LCD_CONTRAST   	16
#define LCD_BACKLIGHT  	17
#define LCD_LINE       	18
#define USART0_SETBAUD 	20
#define USART0_INIT    	21
#define USART0_WRITE   	22
#define USART0_OK      	23
#define USART1_SETBAUD 	25
#define USART1_INIT    	26
#define USART1_WRITE   	27
#define USART1_OK      	28
#define UART_LOOP      	30

//Klasse SerialDisplay definieren
class USBgLCD
{

public:
      //Konstruktor
      USBgLCD();
      //Desktruktor
      ~USBgLCD();

      int clear();
      int set_pixel(int x, int y);
      int draw_line(int x1, int y1, int x2, int y2);

protected:

private:
      //USB-Device-Handler
      usb_dev_handle *handle;

};

#endif

/*
static void usage(char *name)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s status\n", name);
    fprintf(stderr, "  %s on <port> [<duration>]\n", name);
    fprintf(stderr, "  %s off <port> [<duration>]\n", name);
    fprintf(stderr, "  %s test\n", name);
    fprintf(stderr, "  %s debug <...> <...>\n\n", name);

    fprintf(stderr, "  %s init\n", name);
    fprintf(stderr, "  %s clrgfx\n", name);
    fprintf(stderr, "  %s clrtxt\n", name);
    fprintf(stderr, "  %s goto <x> <y>\n", name);
    fprintf(stderr, "  %s text <string> <...>\n", name);
    fprintf(stderr, "  %s pixel <x> <y>\n\n", name);
    fprintf(stderr, "  %s contrast <+/->\n", name);
    fprintf(stderr, "  %s backlight <+/->\n\n", name);

    fprintf(stderr, "  %s setbaudX <baudrate>\n", name);
    fprintf(stderr, "  %s initX\n", name);
    fprintf(stderr, "  %s writeX <string> <...>\n", name);
    fprintf(stderr, "  %s returnX\n", name);
    fprintf(stderr, "  %s newlineX\n", name);
    fprintf(stderr, "  %s okX\n", name);
    fprintf(stderr, "  %s uartloop <+/->\n\n", name);
    
    fprintf(stderr, "Ports are single digits in the range 1...1 ;-)\n");
    fprintf(stderr, "The Letter X by UART-Cmds stands for 0/1 -> UART0/1\n");
    fprintf(stderr, "The pulse duration for switching temporarily is given in seconds.\n");
}
*/
