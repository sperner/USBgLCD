/****************************************************************************
** Meta object code from reading C++ file 'paintfield.h'
**
** Created: Fri Oct 2 16:16:09 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "paintfield.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paintfield.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PaintField[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      19,   11,   11,   11, 0x0a,
      27,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PaintField[] = {
    "PaintField\0\0open()\0close()\0clear()\0"
};

const QMetaObject PaintField::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PaintField,
      qt_meta_data_PaintField, 0 }
};

const QMetaObject *PaintField::metaObject() const
{
    return &staticMetaObject;
}

void *PaintField::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PaintField))
        return static_cast<void*>(const_cast< PaintField*>(this));
    return QWidget::qt_metacast(_clname);
}

int PaintField::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: open(); break;
        case 1: close(); break;
        case 2: clear(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
