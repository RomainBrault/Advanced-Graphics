/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../part1/mainwindow.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x08,
      59,   11,   11,   11, 0x08,
      91,   12,   11,   11, 0x08,
     129,   11,   11,   11, 0x08,
     161,   11,   11,   11, 0x08,
     187,   11,   11,   11, 0x08,
     213,   11,   11,   11, 0x08,
     240,   11,   11,   11, 0x08,
     264,   11,   11,   11, 0x08,
     291,   11,   11,   11, 0x08,
     318,   11,   11,   11, 0x08,
     345,   11,   11,   11, 0x08,
     372,   11,   11,   11, 0x08,
     402,   11,   11,   11, 0x08,
     438,  432,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0position\0"
    "on_verticalSlider_1_valueChanged(int)\0"
    "on_lineEdit_1_editingFinished()\0"
    "on_verticalSlider_2_valueChanged(int)\0"
    "on_lineEdit_2_editingFinished()\0"
    "on_pushButton_1_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_action_Open_triggered()\0"
    "on_pushButton_clicked()\0"
    "on_radioButton_2_clicked()\0"
    "on_radioButton_3_clicked()\0"
    "on_radioButton_1_clicked()\0"
    "on_actionReset_triggered()\0"
    "on_actionSave_PPM_triggered()\0"
    "on_actionSave_PFM_triggered()\0index\0"
    "on_listView_doubleClicked(QModelIndex)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_verticalSlider_1_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_lineEdit_1_editingFinished(); break;
        case 2: _t->on_verticalSlider_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_lineEdit_2_editingFinished(); break;
        case 4: _t->on_pushButton_1_clicked(); break;
        case 5: _t->on_pushButton_2_clicked(); break;
        case 6: _t->on_action_Open_triggered(); break;
        case 7: _t->on_pushButton_clicked(); break;
        case 8: _t->on_radioButton_2_clicked(); break;
        case 9: _t->on_radioButton_3_clicked(); break;
        case 10: _t->on_radioButton_1_clicked(); break;
        case 11: _t->on_actionReset_triggered(); break;
        case 12: _t->on_actionSave_PPM_triggered(); break;
        case 13: _t->on_actionSave_PFM_triggered(); break;
        case 14: _t->on_listView_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
