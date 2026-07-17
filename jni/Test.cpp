#include "Test.h"
#include <QtAndroid>
#include <QtAndroidExtras>

CTest::CTest(QObject *parent) : QObject(parent)
{   
}

int CTest::TestTimeout()
{
    QAndroidJniObject::callStaticMethod<void>(
            "io/github/KangLin/QtAndroidUtils/Test",
            "timeOut"
           );
    return 0;
}
