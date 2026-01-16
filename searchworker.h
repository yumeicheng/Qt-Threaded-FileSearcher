#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include <QObject>
#include <QThread>
#include <atomic>


//文件信息结构体
struct FileInfoData{
    QString fileName;                   //文件名
    QString filePath;                   //完整路径
    QString size;                       //大小
    QString modified;                   //修改时间
};

Q_DECLARE_METATYPE(FileInfoData);


class SearchWorker : public QObject
{
    Q_OBJECT
public:
    explicit SearchWorker(QObject *parent = nullptr);
    std::atomic<bool> stopFlag;

public slots:
    void doSearch(QString path, QString keyword);

signals:
    void foundFile(FileInfoData info);
    void searchFinished();
};

#endif // SEARCHWORKER_H
