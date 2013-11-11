/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: GLCD_SetPixel fehlerhaft!
 Version:        05.10.2008
 Description:    SED1335 Grafik-Routinen
----------------------------------------------------------------------------*/

/*
void GLCD_Rectangle(unsigned int x, unsigned int y, unsigned int b, unsigned int a);
void GLCD_Circle(unsigned int cx, unsigned int cy , unsigned int radius);
void GLCD_SetPixel2(int X1, int Y1, int color);
*/
void GLCD_Line(int X1, int Y1, int X2, int Y2);
