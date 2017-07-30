#include <QApplication>
#include <QTextCodec>

#include <QObject>
#include "QWorkBase.h"

int main(int argc, char *argv[])
{
#ifndef HAVE_QT5
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
#endif
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    QApplication a(argc, argv);
    QWorkMainWindow w(QObject::tr("QxAdvice"), QObject::tr("QWorkIde"));
    w.show();

    return a.exec();
}
