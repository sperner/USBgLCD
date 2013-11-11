/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 to USB Adapter Commandline-Tool
		 partly based on Christian Starkjohanns PowerSwitch
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

#define USBDEV_SHARED_VENDOR    0x16C0  /* VOTI */
#define USBDEV_SHARED_PRODUCT   0x05DC  /* Obdev's free shared PID */

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
#define LCD_LINE  	18

/* These are the vendor specific SETUP commands implemented by our USB device */

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
    fprintf(stderr, "  %s pixel <x> <y>\n", name);
    fprintf(stderr, "  %s line <x1> <y1> <x2> <y2>\n", name);
    fprintf(stderr, "  %s contrast <+/->\n", name);
    fprintf(stderr, "  %s backlight <+/->\n\n", name);

    fprintf(stderr, "Ports are single digits in the range 1...1 ;-)\n");
    fprintf(stderr, "The pulse duration for switching temporarily is given in seconds.\n");
}


static int  usbGetStringAscii(usb_dev_handle *dev, int index, int langid, char *buf, int buflen)
{
char    buffer[256];
int     rval, i;

    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, langid, buffer, sizeof(buffer), 1000)) < 0)
        return rval;
    if(buffer[1] != USB_DT_STRING)
        return 0;
    if((unsigned char)buffer[0] < rval)
        rval = (unsigned char)buffer[0];
    rval /= 2;
    /* lossy conversion to ISO Latin1 */
    for(i=1;i<rval;i++){
        if(i > buflen)  /* destination buffer overflow */
            break;
        buf[i-1] = buffer[2 * i];
        if(buffer[2 * i + 1] != 0)  /* outside of ISO Latin1 range */
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}


#define USB_ERROR_NOTFOUND  1
#define USB_ERROR_ACCESS    2
#define USB_ERROR_IO        3

static int usbOpenDevice(usb_dev_handle **device, int vendor, char *vendorName, int product, char *productName)
{
struct usb_bus      *bus;
struct usb_device   *dev;
usb_dev_handle      *handle = NULL;
int                 errorCode = USB_ERROR_NOTFOUND;
static int          didUsbInit = 0;

    if(!didUsbInit){
        didUsbInit = 1;
        usb_init();
    }
    usb_find_busses();
    usb_find_devices();
    for(bus=usb_get_busses(); bus; bus=bus->next){
        for(dev=bus->devices; dev; dev=dev->next){
            if(dev->descriptor.idVendor == vendor && dev->descriptor.idProduct == product){
                char    string[256];
                int     len;
                handle = usb_open(dev); /* we need to open the device in order to query strings */
                if(!handle){
                    errorCode = USB_ERROR_ACCESS;
                    fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
                if(vendorName == NULL && productName == NULL){  /* name does not matter */
                    break;
                }
                /* now check whether the names match: */
                len = usbGetStringAscii(handle, dev->descriptor.iManufacturer, 0x0409, string, sizeof(string));
                if(len < 0){
                    errorCode = USB_ERROR_IO;
                    fprintf(stderr, "Warning: cannot query manufacturer for device: %s\n", usb_strerror());
                }else{
                    errorCode = USB_ERROR_NOTFOUND;
                    /* fprintf(stderr, "seen device from vendor ->%s<-\n", string); */
                    if(strcmp(string, vendorName) == 0){
                        len = usbGetStringAscii(handle, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
                        if(len < 0){
                            errorCode = USB_ERROR_IO;
                            fprintf(stderr, "Warning: cannot query product for device: %s\n", usb_strerror());
                        }else{
                            errorCode = USB_ERROR_NOTFOUND;
                            /* fprintf(stderr, "seen product ->%s<-\n", string); */
                            if(strcmp(string, productName) == 0)
                                break;
                        }
                    }
                }
                usb_close(handle);
                handle = NULL;
            }
        }
        if(handle)
            break;
    }
    if(handle != NULL){
        errorCode = 0;
        *device = handle;
    }
    return errorCode;
}


int main(int argc, char **argv)
{
usb_dev_handle      *handle = NULL;
unsigned char       buffer[8];
int                 nBytes=0;

    if(argc < 2){
        usage(argv[0]);
        exit(1);
    }
    usb_init();
    if(usbOpenDevice(&handle, USBDEV_SHARED_VENDOR, "S2Devel", USBDEV_SHARED_PRODUCT, "USBgLCD") != 0){
        fprintf(stderr, "Could not find USB device \"USBgLCD\" with vid=0x%x pid=0x%x\n", USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT);
        exit(1);
    }

    if(strcmp(argv[1], "test") == 0){
        int i, v, r;

//:int usb_control_msg(usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout);

        for(i=0;i<1000;i++){
            v = rand() & 0xffff;
            nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, PSCMD_ECHO, v, 0, (char *)buffer, sizeof(buffer), 5000);
            if(nBytes < 2){
                if(nBytes < 0)
                    fprintf(stderr, "USB error: %s\n", usb_strerror());
                fprintf(stderr, "only %d bytes received in iteration %d\n", nBytes, i);
                fprintf(stderr, "value sent = 0x%x\n", v);
                exit(1);
            }
            r = buffer[0] | (buffer[1] << 8);
            if(r != v){
                fprintf(stderr, "data error: received 0x%x instead of 0x%x in iteration %d\n", r, v, i);
                exit(1);
            }
        }
        printf("test succeeded\n");
    }else if(strcmp(argv[1], "debug") == 0){
        int x=0, y=0;
        if(argc < 4){
            usage(argv[0]);
            exit(1);
        }
        x = atoi(argv[2]);
        y = atoi(argv[3]);
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, USB_DEBUG, x, y, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 4){
            if(nBytes < 0)
                fprintf(stderr, "USB error: %s\n", usb_strerror());
            fprintf(stderr, "only %d bytes status received\n", nBytes);
            exit(1);
        }
	printf("Buffer: %d %d %d %d\n",buffer[0],buffer[1],buffer[2],buffer[3]);
    }else if(strcmp(argv[1], "status") == 0){
        int i;
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, PSCMD_GET, 0, 0, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 2){
            if(nBytes < 0)
                fprintf(stderr, "USB error: %s\n", usb_strerror());
            fprintf(stderr, "only %d bytes status received\n", nBytes);
            exit(1);
        }
//        for(i=0;i<8;i++){
        for(i=0;i<=1;i++){
            int isOn = buffer[0] & (1 << i);
            int isInv = buffer[1] & (1 << i);
            printf("port %d: %s%s\n", i, isOn ? "on" : "off", isInv ? (isOn ? " / pulse off" : " / pulse on") : "");
        }
    }else if(strcmp(argv[1], "init") == 0){
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_INIT, 0, 0, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else if(strcmp(argv[1], "clrgfx") == 0){
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_CLR_GFX, 0, 0, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else if(strcmp(argv[1], "clrtxt") == 0){
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_CLR_TXT, 0, 0, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else if(strcmp(argv[1], "pixel") == 0){
        int x=0, y=0;
        if(argc < 4){
            usage(argv[0]);
            exit(1);
        }
        x = atoi(argv[2]);
        y = atoi(argv[3]);
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_PIXEL, x, y, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else if(strcmp(argv[1], "line") == 0){
        int x1=0, y1=0, x2=0, y2=0;
        if(argc < 6){
            usage(argv[0]);
            exit(1);
        }
        x1 = atoi(argv[2]);
        y1 = atoi(argv[3]);
        x2 = atoi(argv[4]);
        y2 = atoi(argv[5]);
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_LINE, x1, y1, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0)
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	else
	    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_LINE, x2, y2, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else if(strcmp(argv[1], "goto") == 0){
        int x=0, y=0;
        if(argc < 4){
            usage(argv[0]);
            exit(1);
        }
        x = atoi(argv[2]);
        y = atoi(argv[3]);
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_GOTO, x, y, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else if(strcmp(argv[1], "text") == 0){
	char len=0;
	int i,j;
        if(argc < 3){
            usage(argv[0]);
            exit(1);
        }
	for( j=2 ; j<argc ; j++) {
	    len = strlen(argv[j]);
	    for( i=0 ; i<len ; i++) {
		nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_TEXT, argv[j][i], 0, (char *)buffer, sizeof(buffer), 5000);
		if(nBytes < 0)
		    fprintf(stderr, "USB error: %s\n", usb_strerror());
	    }
	    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_TEXT, ' ', 0, (char *)buffer, sizeof(buffer), 5000);
	}
    }else if(strcmp(argv[1], "contrast") == 0){
        if(argc < 3){
            usage(argv[0]);
            exit(1);
        }
        if(strcmp(argv[2],"+")==0)
	    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_CONTRAST, +1, 0, (char *)buffer, sizeof(buffer), 5000);
        else if(strcmp(argv[2],"-")==0)
	    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_CONTRAST, -1, 0, (char *)buffer, sizeof(buffer), 5000);
	else
	    usage(argv[0]);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else if(strcmp(argv[1], "backlight") == 0){
        if(argc < 3){
            usage(argv[0]);
            exit(1);
        }
        if(strcmp(argv[2],"+")==0)
	    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_BACKLIGHT, +1, 0, (char *)buffer, sizeof(buffer), 5000);
        else if(strcmp(argv[2],"-")==0)
	    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_BACKLIGHT, -1, 0, (char *)buffer, sizeof(buffer), 5000);
	else
	    usage(argv[0]);
        if(nBytes < 0) {
            fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
    }else{
        int port, duration = 0;
        if(argc < 3){
            usage(argv[0]);
            exit(1);
        }
        port = atoi(argv[2]);
        if(argc > 3){
            if((duration = (int)(atof(argv[3]) / 0.2 + 0.5)) > 255)
                duration = 255;
        }
        if(strcmp(argv[1], "on") == 0){
            nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, PSCMD_ON, duration, port, (char *)buffer, sizeof(buffer), 5000);
        }else if(strcmp(argv[1], "off") == 0){
            nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, PSCMD_OFF, duration, port, (char *)buffer, sizeof(buffer), 5000);
        }else{
            nBytes = 0;
            usage(argv[0]);
            exit(1);
        }
        if(nBytes < 0)
            fprintf(stderr, "USB error: %s\n", usb_strerror());
    }
    usb_close(handle);
    return 0;
}
