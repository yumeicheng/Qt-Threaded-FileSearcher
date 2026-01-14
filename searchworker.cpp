#include "searchworker.h"
#include <QThread>
#include <QDirIterator>    //遍历文件
#include <QDebug>

SearchWorker::SearchWorker(QObject *parent)
    : QObject{parent}
{
    stopFlag = false;
}

void SearchWorker::doSearch(QString path, QString keyword)
{
    stopFlag = false;
    //构造迭代器
    //path：起始目录
    //QDir：：files|QDir：：NoDotAndDotDot：只查看文件，忽略“.”和“..”文件夹
    //QDirIterator：：Subdirectories：自动递归遍历所有文件夹
    QDirIterator it(path,
                    QDir::Files | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);

    while(it.hasNext()){
        //QThread::currentThread()获取当前线程指针
        //isInterruptionRequested()检查是否有quit/terminate请求
        // if(QThread::currentThread()->isInterruptionRequested()){
        //     qDebug() << "检测到停止请求，提前结束";
        //     break;
        // }
        if(stopFlag)
        {
            qDebug() << "检测到停止请求" ;
            break;
        }

        QString filePath = it.next();

        QFileInfo fileInfo(filePath);
        if(fileInfo.fileName().contains(keyword,Qt::CaseInsensitive)){
            QThread::msleep(100);
            emit foundFile(filePath);
        }
    }
    emit searchFinished();
}
