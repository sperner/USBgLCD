/*----------------------------------------------------------------------------
 Author:         Sven Sperner	<cethss@gmail.com>
 Remarks:        
 known Problems: 
 Version:        05.10.2008
 Description:    SED1335 to USB Adapter Qt-Paint-Program
----------------------------------------------------------------------------*/
/****************************************************************************
 **
 ** Kompilierung:
 **			Projektdatei erzeugen mit: qmake -project
 **			Projektdatei erweitern um: LIBS = -lusb
						  [LIBS = -lserdisp]
 **			dann noch: qmake && make
 **
 ****************************************************************************/

#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <usbglcd.h>

#include "paintfield.h"

class MyWidget : public QWidget
{
  public:
	  MyWidget(QWidget *parent = 0);
  private slots:
	  void quit();
 };

MyWidget::MyWidget(QWidget *parent)
	: QWidget(parent)
{
      //Schlachfeld
      PaintField *paintField = new PaintField;

      //Open-Button instanzieren & verbinden
      QPushButton *openButton = new QPushButton(tr("Open"));
      openButton->setFont(QFont("Arial", 12, QFont::Bold));
      connect(openButton, SIGNAL(clicked()), paintField, SLOT(open()));
      //Close-Button instanzieren & verbinden
      QPushButton *closeButton = new QPushButton(tr("Close"));
      closeButton->setFont(QFont("Arial", 12, QFont::Bold));
      connect(closeButton, SIGNAL(clicked()), paintField, SLOT(close()));
      //Clear-Button instanzieren & verbinden
      QPushButton *clearButton = new QPushButton(tr("Clear"));
      clearButton->setFont(QFont("Arial", 12, QFont::Bold));
      connect(clearButton, SIGNAL(clicked()), paintField, SLOT(clear()));
      
      //SerDispLib: Display-/Controllername
      QLabel *nameLabel = new QLabel("Device-Name:");
      QLineEdit *nameEdit = new QLineEdit("SED1335");
      //SerDispLib: Displaymaﬂe in Pixel
      QLabel *dimensionsLabel = new QLabel("Dimensions:");
      QLineEdit *widthEdit = new QLineEdit("320");
      QLabel *xLabel = new QLabel("x");
      QLineEdit *heightEdit = new QLineEdit("240");
      //SerDispLib: Pfad des Ports
      QLabel *deviceLabel = new QLabel("Device-Path:");
      QLineEdit *deviceEdit = new QLineEdit("/dev/parport0");
      //SerDispLib: Anschlussvariante
      QLabel *wiringLabel = new QLabel("Wiring:");
      QLineEdit *wiringEdit = new QLineEdit("YASEDW");
      
      //Gitter-Layout
      QGridLayout *gridLayout = new QGridLayout;
/* void addWidget( QWidget * widget, int row, int column, Qt::Alignment alignment = 0 ) */
      gridLayout->addWidget(openButton, 0, 0);
      gridLayout->addWidget(nameLabel, 0, 1);
      gridLayout->addWidget(nameEdit, 0, 2, 1, 2);
      gridLayout->addWidget(closeButton, 1, 0);
      gridLayout->addWidget(dimensionsLabel, 1, 1);
      gridLayout->addWidget(widthEdit, 1, 2);
      gridLayout->addWidget(xLabel, 1, 3);
      gridLayout->addWidget(heightEdit, 1, 4);
      gridLayout->addWidget(clearButton, 2, 0);
      gridLayout->addWidget(deviceLabel, 2, 1);
      gridLayout->addWidget(deviceEdit, 2, 2, 1, 3);
      gridLayout->addWidget(wiringLabel, 3, 1);
      gridLayout->addWidget(wiringEdit, 3, 2, 1, 3);
/* void addWidget ( QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0 ) */
      gridLayout->addWidget(paintField, 4, 1, 2, 4);
      //gridLayout->setColumnStretch(2, 10);
      setLayout(gridLayout);
}


int main(int argc, char *argv[])
{
      //Qt-Anwendung erzeugen
      QApplication app(argc, argv);

      //Widget instanzieren
      MyWidget widget;
      //Gr√∂sse des Widgets festlegen
      //--widget.setGeometry(100, 100, 424, 293);
      widget.setGeometry(100, 100, 424, 393);
      //Widget anzeigen
      widget.show();

  return app.exec();
}
