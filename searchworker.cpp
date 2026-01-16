#include "searchworker.h"
#include <QThread>
#include <QDirIterator>    //遍历文件
#include <QDebug>
#include <QDateTime>

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
            //打包数据详情
            FileInfoData data;
            data.fileName = fileInfo.fileName();
            data.filePath = fileInfo.filePath();

            //计算大小(简单的除以1024显示kb)
            double sizeInKb = fileInfo.size() / 1024.0;
            data.size = QString::number(sizeInKb,'f',1)+"KB";

            //获取时间（格式为 yyyy-MM-dd HH:mm:ss）
            data.modified = fileInfo.lastModified().toString("yyyy-MM-dd HH:mm:ss");

            //发送结构体
            emit foundFile(data);

            QThread::msleep(10);
        }
    }
    emit searchFinished();
}
