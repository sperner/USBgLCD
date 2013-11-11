/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 to USB Adapter Qt-Paint-Program
----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
using namespace std;

#include <QDateTime>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include <math.h>
#include <stdlib.h>

#include "paintfield.h"
#include "usbglcd.h"


PaintField::PaintField(QWidget *parent)
    : QWidget(parent)
{
     setAttribute(Qt::WA_StaticContents);
     setAttribute(Qt::WA_NoBackground);
     setPalette(QPalette(QColor(100, 150, 200)));
//    setAutoFillBackground(true);
     mousePressed = false;
     setGeometry(1,1,320,240);

     theImage = QImage(320, 240, QImage::Format_RGB32);
     theImage.fill(qRgb(255, 255, 255));
     color = Qt::blue;
     thickness = 1;
     update();
     updateGeometry();
     firstRun = true;
     lcdOpen = false;
}

//Display öffnen
 void PaintField::open()
 {
//      cout << "bin im open()" << endl;
      //ParallelPort über Gerätedatei öffnen
//      gLCD.SdCD = SDCONN_open("/dev/parport0");
      //Display Optionen festlegen
//      gLCD.SdD = serdisp_init(gLCD.SdCD,"SED1335","WIDTH=320;HEIGHT=240;WIRING=YASEDW");
//      serdisp_clear(gLCD.SdD);
//      if (gLCD.SdD != 0) 
	  lcdOpen = true;
 }
 
 //Display schliessen
 void PaintField::close()
 {
      //Display schliessen
//      serdisp_close(gLCD.SdD);
      //Port schliessen
//      SDCONN_close(gLCD.SdCD);
      
      lcdOpen = false;
 }

//Anzeigebereich löschen
 void PaintField::clear()
 {
      firstRun=true;
      if (lcdOpen)
	  gLCD.clear();
      update();
 }

//Event: MausButton betätigt
 void PaintField::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() != Qt::LeftButton)
         return;
     aktPos = event->pos();
     lastPos = aktPos;
     mousePressed = true;
//D     cout << mousePressed << " @ x: " << aktPos.x() << " y: " << aktPos.y() << endl;
     update();
 }

//Event: Mausbewegung
 void PaintField::mouseMoveEvent(QMouseEvent *event)
 {
     aktPos = event->pos();
//-     mousePressed = true;
//D     cout << mousePressed << " @ x: " << aktPos.x() << " y: " << aktPos.y() << endl;
     update();
 }

//Event: MausButton losgelassen
 void PaintField::mouseReleaseEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton)
	 mousePressed = false;
 }

//Eventhandler: Neuzeichnen des Schussfeldes
 void PaintField::paintEvent(QPaintEvent * /* event */)
 {
     QPainter painter(this);
//D     cout << "paintEvent()" << endl;
     if (firstRun) {
	 painter.drawImage(QPoint(0, 0), theImage);
	 firstRun=false;
	 firstPress=true;
     }
     if (mousePressed) {
//D         cout << "paintEvent:IF(mousePressed)" << endl;
	  //painter.setRenderHint(QPainter::Antialiasing, true);
	  painter.setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	 //painter.setPen(Qt::black);	
	 //painter.setBrush(Qt::white);	
	 if ( ((aktPos.x()-lastPos.x() > 1) || (aktPos.x()-lastPos.x() < -1) || 
	       (aktPos.y()-lastPos.y() > 1) || (aktPos.y()-lastPos.y() < -1))
	       && !firstPress ) {
	    painter.drawLine(lastPos,aktPos);
	    if (lcdOpen) {
		gLCD.draw_line(lastPos.x(),lastPos.y(),aktPos.x(),aktPos.y());
	    }
	 }
	 else {
	    painter.drawPoint(aktPos);
	    if (lcdOpen) {
		gLCD.set_pixel(aktPos.x(), aktPos.y());
	    }
	    if (firstPress)
	      firstPress=false;
	 }
	 lastPos=aktPos;
//-	 mousePressed = false;
     }
 }
