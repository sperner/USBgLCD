/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: GLCD_SetPixel fehlerhaft!
 Version:        05.10.2008
 Description:    SED1335 Grafik-Routinen
----------------------------------------------------------------------------*/

extern void GLCD_SetPixel(int x, int y, int color);

const unsigned char color = 1;
/*
//#################################### Rechteck ####################################
void GLCD_Rectangle(unsigned int x, unsigned int y, unsigned int b, unsigned int a)
{
  unsigned int j; // Zähler
  // vertikale Linien
  for (j = 0; j < a; j++) {
		GLCD_SetPixel(x, y + j, color);
		GLCD_SetPixel(x + b - 1, y + j, color);
	}
  // horizontale Linien
  for (j = 0; j < b; j++)	{
		GLCD_SetPixel(x + j, y, color);
		GLCD_SetPixel(x + j, y + a - 1, color);
	}
}

//###################################### Kreis #####################################
void GLCD_Circle(unsigned int cx, unsigned int cy ,unsigned int radius)
{
int x, y, xchange, ychange, radiusError;
x = radius;
y = 0;
xchange = 1 - 2 * radius;
ychange = 1;
radiusError = 0;
while(x >= y)
  {
  GLCD_SetPixel(cx+x, cy+y, color); 
  GLCD_SetPixel(cx-x, cy+y, color); 
  GLCD_SetPixel(cx-x, cy-y, color);
  GLCD_SetPixel(cx+x, cy-y, color); 
  GLCD_SetPixel(cx+y, cy+x, color); 
  GLCD_SetPixel(cx-y, cy+x, color); 
  GLCD_SetPixel(cx-y, cy-x, color); 
  GLCD_SetPixel(cx+y, cy-x, color); 
  y++;
  radiusError += ychange;
  ychange += 2;
  if ( 2*radiusError + xchange > 0 )
    {
    x--;
	radiusError += xchange;
	xchange += 2;
	}
  }
}
*/
//###################################### Linie #####################################
void GLCD_Line(int X1, int Y1,int X2,int Y2)
{
int CurrentX, CurrentY, Xinc, Yinc, 
    Dx, Dy, TwoDx, TwoDy, 
	TwoDxAccumulatedError, TwoDyAccumulatedError;

Dx = (X2-X1); // horizontale Länge
Dy = (Y2-Y1); // vertikale Länge

TwoDx = Dx + Dx; // doppelte horizontale Länge
TwoDy = Dy + Dy; // doppelte vertikale Länge

CurrentX = X1; // aktuelle X-Koordinate
CurrentY = Y1; // aktuelle Y-Koordinate

Xinc = 1; // X-Inkrement
Yinc = 1; // Y-Inkrement

if(Dx < 0) // jesli skadowa pozioma jest ujemna 
  {
  Xinc = -1; // to b?dziemy si? "cofa?" (krok ujemny)
  Dx = -Dx;  // zmieniamy znak skadowej na dodatni
  TwoDx = -TwoDx; // jak równie¿ podwojonej skadowej
  }

if (Dy < 0) // je?li skadowa pionowa jest ujemna
  {
  Yinc = -1; // to b?dziemy si? "cofa?" (krok ujemny)
  Dy = -Dy; // zmieniamy znak skadowej na dodatki
  TwoDy = -TwoDy; // jak równiez podwojonej skadowej
  }

GLCD_SetPixel(X1,Y1, color); // stawiamy pierwszy krok (zapalamy pierwszy piksel)

if ((Dx != 0) || (Dy != 0)) // sprawdzamy czy linia sk³ada siê z wiêcej ni¿ jednego punktu ;)
  {
  // sprawdzamy czy sk³adowa pionowa jest mniejsza lub równa sk³adowej poziomej
  if (Dy <= Dx) // jeœli tak, to idziemy "po iksach"
    { 
    TwoDxAccumulatedError = 0; // zerujemy zmienn¹ 
    do // ruszamy w drogê
	  {
      CurrentX += Xinc; // do aktualnej pozycji dodajemy krok 
      TwoDxAccumulatedError += TwoDy; // a tu dodajemy podwojon¹ sk³adow¹ pionow¹
      if(TwoDxAccumulatedError > Dx)  // jeœli TwoDxAccumulatedError jest wiêkszy od Dx
        {
        CurrentY += Yinc; // zwiêkszamy aktualn¹ pozycjê w pionie
        TwoDxAccumulatedError -= TwoDx; // i odejmujemy TwoDx
        }
       GLCD_SetPixel(CurrentX,CurrentY, color);// stawiamy nastêpny krok (zapalamy piksel)
       }while (CurrentX != X2); // idziemy tak d³ugo, a¿ osi¹gniemy punkt docelowy
     }
   else // w przeciwnym razie idziemy "po igrekach" 
      {
      TwoDyAccumulatedError = 0; 
      do 
	    {
        CurrentY += Yinc; 
        TwoDyAccumulatedError += TwoDx;
        if(TwoDyAccumulatedError>Dy) 
          {
          CurrentX += Xinc;
          TwoDyAccumulatedError -= TwoDy;
          }
         GLCD_SetPixel(CurrentX,CurrentY, color); 
         }while (CurrentY != Y2);
    }
  }
}
