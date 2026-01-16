#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
//#include <QListWidget>
#include <QTableView>
#include <QLineEdit>
#include <QWidget>
#include <QThread>
#include "searchworker.h"
#include "resulttablemodel.h"
#include <QDesktopServices>     //调用系统服务（打开文件、网页等）
#include <QUrl>                 //处理文件路径格式

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QWidget * centraWidget;
    QLineEdit * pathEdit;                       //路径输入框
    QLineEdit * keywordEdit;                    //类型选择
    //QListWidget * resultList;                   //文件列表
    QTableView * resultView;                    //表格视图（负责画图）
    ResultTableModel * model;                   //数据模型（负责存数据）
    QPushButton * startBtn;                     //开始按钮
    QPushButton * browseBtn;                    //浏览按钮
    QPushButton * stopBtn;                      //停止按钮



    QThread* workerThread;
    SearchWorker* worker;

public slots:
    void onStartSearch();
    void onWorkerFoundFile(FileInfoData info);
    void onWOrkerFinished();
    void onStopSearch();
    void onFileDoubleClicked(const QModelIndex &index);

signals:
    void startSearchReq(QString path, QString keyword);

};
#endif // MAINWINDOW_H
