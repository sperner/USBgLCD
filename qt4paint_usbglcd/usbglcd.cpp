/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 to USB Adapter Qt-Paint-Program
		 partly based on Christian Starkjohanns PowerSwitch
----------------------------------------------------------------------------*/

#include <usb.h>
#include "usbglcd.h"


static int usbGetStringAscii(usb_dev_handle *dev, int index, int langid, char *buf, int buflen)
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


static int usbOpenDevice(usb_dev_handle **device, int vendor, char *vendorName, int product, char *productName)
{
struct usb_bus      *bus;
struct usb_device   *dev;
usb_dev_handle      *tmp_handle = NULL;
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
                tmp_handle = usb_open(dev); /* we need to open the device in order to query strings */
                if(!tmp_handle){
                    errorCode = USB_ERROR_ACCESS;
                    fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
                if(vendorName == NULL && productName == NULL){  /* name does not matter */
                    break;
                }
                /* now check whether the names match: */
                len = usbGetStringAscii(tmp_handle, dev->descriptor.iManufacturer, 0x0409, string, sizeof(string));
                if(len < 0){
                    errorCode = USB_ERROR_IO;
                    fprintf(stderr, "Warning: cannot query manufacturer for device: %s\n", usb_strerror());
                }else{
                    errorCode = USB_ERROR_NOTFOUND;
                    /* fprintf(stderr, "seen device from vendor ->%s<-\n", string); */
                    if(strcmp(string, vendorName) == 0){
                        len = usbGetStringAscii(tmp_handle, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
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
                usb_close(tmp_handle);
                tmp_handle = NULL;
            }
        }
        if(tmp_handle)
            break;
    }
    if(tmp_handle != NULL){
        errorCode = 0;
        *device = tmp_handle;
    }
    return errorCode;
}


USBgLCD::USBgLCD()
{
    usb_init();
    if(usbOpenDevice(&handle, USBDEV_SHARED_VENDOR, "S2Devel", USBDEV_SHARED_PRODUCT, "USBgLCD") != 0){
        fprintf(stderr, "Could not find USB device \"USBgLCD\" with vid=0x%x pid=0x%x\n", USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT);
        exit(1);
    }
}

USBgLCD::~USBgLCD()
{
    usb_close(handle);
}

int USBgLCD::clear()
{
unsigned char       buffer[8];
int                 nBytes=0;

    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_CLR_GFX, 0, 0, (char *)buffer, sizeof(buffer), 5000);
    if(nBytes < 0)
	fprintf(stderr, "USB error: %s\n", usb_strerror());
    else
	nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_CLR_TXT, 0, 0, (char *)buffer, sizeof(buffer), 5000);
    if(nBytes < 0)
	fprintf(stderr, "USB error: %s\n", usb_strerror());
    return 0;
}

int USBgLCD::set_pixel(int x, int y)
{
unsigned char       buffer[8];
int                 nBytes=0;

    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_PIXEL, x, y, (char *)buffer, sizeof(buffer), 5000);
    if(nBytes < 0) 
	fprintf(stderr, "USB error: %s\n", usb_strerror());
    return 0;
}

int USBgLCD::draw_line(int x1, int y1, int x2, int y2)
{
unsigned char       buffer[8];
int                 nBytes=0;

    nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_LINE, x1, y1, (char *)buffer, sizeof(buffer), 5000);
    if(nBytes < 0)
	fprintf(stderr, "USB error: %s\n", usb_strerror());
    else
	nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, LCD_LINE, x2, y2, (char *)buffer, sizeof(buffer), 5000);
    if(nBytes < 0)
	fprintf(stderr, "USB error: %s\n", usb_strerror());
    return 0;
}

/*
int glcd_pixel(int argc, char **argv)
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
    if(strcmp(argv[1], "init") == 0){
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
    }
    usb_close(handle);
    return 0;
}
*/