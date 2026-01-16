#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>


int main(int argc, char *argv[])
{
    // // 打印根目录下的所有资源
    // QDir dir(":/");
    // qDebug() << "资源根目录下的东西: " << dir.entryList();

    // // 打印 new 下面的东西
    // QDir dir2(":/new");
    // qDebug() << "new 下面的东西: " << dir2.entryList();
    QApplication a(argc, argv);

    QFile file(":/style.qss");
    if(file.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    }
    else{
        qDebug() << "样表加载失败！" ;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
