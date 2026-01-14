#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include <QObject>
#include <atomic>
class SearchWorker : public QObject
{
    Q_OBJECT
public:
    explicit SearchWorker(QObject *parent = nullptr);
    std::atomic<bool> stopFlag;

public slots:
    void doSearch(QString path, QString keyword);

signals:
    void foundFile(QString filePath);
    void searchFinished();
};

#endif // SEARCHWORKER_H
