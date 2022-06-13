/****************************************************************************
** Meta object code from reading C++ file 'multiwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../multiwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_multiwindow_t {
    QByteArrayData data[32];
    char stringdata0[386];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_multiwindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_multiwindow_t qt_meta_stringdata_multiwindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "multiwindow"
QT_MOC_LITERAL(1, 12, 10), // "sigInitDEM"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 10), // "sigInitCom"
QT_MOC_LITERAL(4, 35, 11), // "sigRunAlert"
QT_MOC_LITERAL(5, 47, 3), // "lat"
QT_MOC_LITERAL(6, 51, 3), // "lon"
QT_MOC_LITERAL(7, 55, 8), // "yaw_heli"
QT_MOC_LITERAL(8, 64, 8), // "heli_hgt"
QT_MOC_LITERAL(9, 73, 10), // "velocity_x"
QT_MOC_LITERAL(10, 84, 10), // "velocity_y"
QT_MOC_LITERAL(11, 95, 11), // "sigInitBars"
QT_MOC_LITERAL(12, 107, 10), // "sigEditYaw"
QT_MOC_LITERAL(13, 118, 24), // "pushButton_start_clicked"
QT_MOC_LITERAL(14, 143, 15), // "tableviewUpdate"
QT_MOC_LITERAL(15, 159, 6), // "actDet"
QT_MOC_LITERAL(16, 166, 4), // "recv"
QT_MOC_LITERAL(17, 171, 21), // "actDet_result_display"
QT_MOC_LITERAL(18, 193, 15), // "updatePlot_heli"
QT_MOC_LITERAL(19, 209, 11), // "SENSOR_DATA"
QT_MOC_LITERAL(20, 221, 10), // "tmp_sensor"
QT_MOC_LITERAL(21, 232, 15), // "updatePlot_obst"
QT_MOC_LITERAL(22, 248, 8), // "tmp_obst"
QT_MOC_LITERAL(23, 257, 6), // "replot"
QT_MOC_LITERAL(24, 264, 20), // "sltAppendConsole_DEM"
QT_MOC_LITERAL(25, 285, 11), // "msg_console"
QT_MOC_LITERAL(26, 297, 20), // "sltAppendConsole_COM"
QT_MOC_LITERAL(27, 318, 17), // "sltUpdateHeliData"
QT_MOC_LITERAL(28, 336, 14), // "sltChangeLight"
QT_MOC_LITERAL(29, 351, 1), // "i"
QT_MOC_LITERAL(30, 353, 22), // "sltUpdateVelandLADPlot"
QT_MOC_LITERAL(31, 376, 9) // "sltRecLAD"

    },
    "multiwindow\0sigInitDEM\0\0sigInitCom\0"
    "sigRunAlert\0lat\0lon\0yaw_heli\0heli_hgt\0"
    "velocity_x\0velocity_y\0sigInitBars\0"
    "sigEditYaw\0pushButton_start_clicked\0"
    "tableviewUpdate\0actDet\0recv\0"
    "actDet_result_display\0updatePlot_heli\0"
    "SENSOR_DATA\0tmp_sensor\0updatePlot_obst\0"
    "tmp_obst\0replot\0sltAppendConsole_DEM\0"
    "msg_console\0sltAppendConsole_COM\0"
    "sltUpdateHeliData\0sltChangeLight\0i\0"
    "sltUpdateVelandLADPlot\0sltRecLAD"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_multiwindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    6,  101,    2, 0x06 /* Public */,
      11,    0,  114,    2, 0x06 /* Public */,
      12,    1,  115,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,  118,    2, 0x0a /* Public */,
      14,    1,  119,    2, 0x0a /* Public */,
      17,    1,  122,    2, 0x0a /* Public */,
      18,    1,  125,    2, 0x0a /* Public */,
      21,    1,  128,    2, 0x0a /* Public */,
      23,    0,  131,    2, 0x0a /* Public */,
      24,    1,  132,    2, 0x0a /* Public */,
      26,    1,  135,    2, 0x0a /* Public */,
      27,    6,  138,    2, 0x0a /* Public */,
      28,    1,  151,    2, 0x0a /* Public */,
      30,    0,  154,    2, 0x0a /* Public */,
      31,    1,  155,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,    8,    9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 15,    2,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 15,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void multiwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<multiwindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigInitDEM(); break;
        case 1: _t->sigInitCom(); break;
        case 2: _t->sigRunAlert((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        case 3: _t->sigInitBars(); break;
        case 4: _t->sigEditYaw((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->pushButton_start_clicked(); break;
        case 6: _t->tableviewUpdate((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 7: _t->actDet_result_display((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 8: _t->updatePlot_heli((*reinterpret_cast< SENSOR_DATA(*)>(_a[1]))); break;
        case 9: _t->updatePlot_obst((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 10: _t->replot(); break;
        case 11: _t->sltAppendConsole_DEM((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->sltAppendConsole_COM((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->sltUpdateHeliData((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        case 14: _t->sltChangeLight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->sltUpdateVelandLADPlot(); break;
        case 16: _t->sltRecLAD((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (multiwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&multiwindow::sigInitDEM)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (multiwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&multiwindow::sigInitCom)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (multiwindow::*)(double , double , double , double , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&multiwindow::sigRunAlert)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (multiwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&multiwindow::sigInitBars)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (multiwindow::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&multiwindow::sigEditYaw)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject multiwindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_multiwindow.data,
    qt_meta_data_multiwindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *multiwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *multiwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_multiwindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int multiwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void multiwindow::sigInitDEM()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void multiwindow::sigInitCom()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void multiwindow::sigRunAlert(double _t1, double _t2, double _t3, double _t4, double _t5, double _t6)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void multiwindow::sigInitBars()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void multiwindow::sigEditYaw(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
