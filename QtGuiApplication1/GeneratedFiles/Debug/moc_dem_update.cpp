/****************************************************************************
** Meta object code from reading C++ file 'dem_update.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dem_update.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dem_update.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DEMUpdate_t {
    QByteArrayData data[17];
    char stringdata0[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DEMUpdate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DEMUpdate_t qt_meta_stringdata_DEMUpdate = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DEMUpdate"
QT_MOC_LITERAL(1, 10, 9), // "sigAdd2DB"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 6), // "actDet"
QT_MOC_LITERAL(4, 28, 16), // "sigAdd2DB_mahala"
QT_MOC_LITERAL(5, 45, 17), // "sigAdd2DB_lowpass"
QT_MOC_LITERAL(6, 63, 12), // "sigDEMUpdate"
QT_MOC_LITERAL(7, 76, 21), // "sigFinalResultDisplay"
QT_MOC_LITERAL(8, 98, 8), // "sig2plot"
QT_MOC_LITERAL(9, 107, 11), // "SENSOR_DATA"
QT_MOC_LITERAL(10, 119, 17), // "sigDEMConsoleInfo"
QT_MOC_LITERAL(11, 137, 13), // "initDEMupdate"
QT_MOC_LITERAL(12, 151, 10), // "calculator"
QT_MOC_LITERAL(13, 162, 14), // "vector<actDet>"
QT_MOC_LITERAL(14, 177, 15), // "database_update"
QT_MOC_LITERAL(15, 193, 4), // "recv"
QT_MOC_LITERAL(16, 198, 13) // "sltSendDBinfo"

    },
    "DEMUpdate\0sigAdd2DB\0\0actDet\0"
    "sigAdd2DB_mahala\0sigAdd2DB_lowpass\0"
    "sigDEMUpdate\0sigFinalResultDisplay\0"
    "sig2plot\0SENSOR_DATA\0sigDEMConsoleInfo\0"
    "initDEMupdate\0calculator\0vector<actDet>\0"
    "database_update\0recv\0sltSendDBinfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DEMUpdate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    2,   72,    2, 0x06 /* Public */,
       5,    1,   77,    2, 0x06 /* Public */,
       6,    1,   80,    2, 0x06 /* Public */,
       7,    1,   83,    2, 0x06 /* Public */,
       8,    1,   86,    2, 0x06 /* Public */,
      10,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   92,    2, 0x0a /* Public */,
      12,    1,   93,    2, 0x0a /* Public */,
      14,    1,   96,    2, 0x0a /* Public */,
      16,    1,   99,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Double,    2,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 3,   15,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void DEMUpdate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DEMUpdate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigAdd2DB((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 1: _t->sigAdd2DB_mahala((*reinterpret_cast< actDet(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->sigAdd2DB_lowpass((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 3: _t->sigDEMUpdate((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 4: _t->sigFinalResultDisplay((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 5: _t->sig2plot((*reinterpret_cast< SENSOR_DATA(*)>(_a[1]))); break;
        case 6: _t->sigDEMConsoleInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->initDEMupdate(); break;
        case 8: _t->calculator((*reinterpret_cast< vector<actDet>(*)>(_a[1]))); break;
        case 9: _t->database_update((*reinterpret_cast< actDet(*)>(_a[1]))); break;
        case 10: _t->sltSendDBinfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DEMUpdate::*)(actDet );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DEMUpdate::sigAdd2DB)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DEMUpdate::*)(actDet , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DEMUpdate::sigAdd2DB_mahala)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DEMUpdate::*)(actDet );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DEMUpdate::sigAdd2DB_lowpass)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DEMUpdate::*)(actDet );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DEMUpdate::sigDEMUpdate)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DEMUpdate::*)(actDet );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DEMUpdate::sigFinalResultDisplay)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DEMUpdate::*)(SENSOR_DATA );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DEMUpdate::sig2plot)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DEMUpdate::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DEMUpdate::sigDEMConsoleInfo)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DEMUpdate::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DEMUpdate.data,
    qt_meta_data_DEMUpdate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DEMUpdate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DEMUpdate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DEMUpdate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DEMUpdate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DEMUpdate::sigAdd2DB(actDet _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DEMUpdate::sigAdd2DB_mahala(actDet _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DEMUpdate::sigAdd2DB_lowpass(actDet _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DEMUpdate::sigDEMUpdate(actDet _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DEMUpdate::sigFinalResultDisplay(actDet _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DEMUpdate::sig2plot(SENSOR_DATA _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DEMUpdate::sigDEMConsoleInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
