/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 to USB Adapter Qt-Paint-Program
----------------------------------------------------------------------------*/

#ifndef PAINTFIELD_H
#define PAINTFIELD_H

#include <QWidget>

#include "usbglcd.h"


//Klasse PaintField als Widget definieren
class PaintField : public QWidget
{
    //muss in jeder Klasse mit Signalen / Slots vorhanden sein
    Q_OBJECT

public:
    PaintField(QWidget *parent = 0);

    //SerialDisplay-ConnectionDescriptor
    USBgLCD gLCD;

public slots:
    void open();
    void close();
    void clear();
    
private slots:

signals:

protected:
    void paintEvent(QPaintEvent *event);	//Neuzeichnen/Updaten
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool firstRun;
    bool firstPress;
    bool mousePressed;
    QPoint lastPos;
    QPoint aktPos;
  
    QImage theImage;
    QColor color;
    int thickness;
    bool lcdOpen;
};

#endif
 