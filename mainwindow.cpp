#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>          //弹出选择文件夹的窗口
#include <QMessageBox>          //用于弹窗提示
#include <QColor>
#include <QThread>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //1、设置窗口标题
    setWindowTitle("C++文件搜索器");
    resize(600,400);                            //初始化大小

    //2、实例化控件
    centraWidget = new QWidget(this);
    //告诉MAinWindow：这是中心区域
    this->setCentralWidget(centraWidget);

    pathEdit = new QLineEdit(centraWidget);
    keywordEdit = new QLineEdit(centraWidget);
    resultList = new QListWidget(centraWidget);
    startBtn = new QPushButton("开始搜索",centraWidget);
    browseBtn = new QPushButton("浏览",centraWidget);
    stopBtn = new QPushButton("停止",centraWidget);
    stopBtn->setEnabled(false);
    pathEdit->setPlaceholderText("请输入要寻找的地址");
    keywordEdit->setPlaceholderText("输入文件关键词（为空则搜索全部）");


    //开始布局
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(pathEdit,1);
    topLayout->addWidget(browseBtn);
    topLayout->addWidget(keywordEdit,1);
    topLayout->addWidget(startBtn);
    topLayout->addWidget(stopBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(resultList);

    centraWidget->setLayout(mainLayout);


    workerThread = new QThread(this);               //线程对象
    worker = new SearchWorker();                    //工作者对象
    worker->moveToThread(workerThread);             //移交worker实例线程归属权
    connect(this,&MainWindow::startSearchReq,worker,&SearchWorker::doSearch);
    connect(worker,&SearchWorker::foundFile,this,&MainWindow::onWorkerFoundFile);
    connect(worker,&SearchWorker::searchFinished,this,&MainWindow::onWOrkerFinished);
    workerThread->start();

    connect(browseBtn,&QPushButton::clicked,this,[=](){
        QString dir = QFileDialog::getExistingDirectory(
            this,
            "搜索目录",
            ".",
            QFileDialog::ShowDirsOnly
            );

        //如果用户选择了路径（不是空字符串）,就填入输入框
        if(!dir.isEmpty())pathEdit->setText(dir);
    });
    connect(startBtn,&QPushButton::clicked,this,&MainWindow::onStartSearch);
    connect(stopBtn,&QPushButton::clicked,this,&MainWindow::onStopSearch);
    connect(resultList,&QListWidget::itemDoubleClicked,this,&MainWindow::onFileDoubleClicked);
}

MainWindow::~MainWindow() {
    //1、请求线程打断
    workerThread->requestInterruption();
    //2、退出循环事件
    workerThread->quit();
    //3、等待子线程退出
    if(!workerThread->wait(3000)){
        qDebug() << "线程卡死，强制退出！";
        workerThread->terminate();
        workerThread->wait();
    }
}

void MainWindow::onStartSearch()
{
    //1、获取输入框的内容
    QString path = this->pathEdit->text();
    QString keyword = keywordEdit->text().trimmed();

    //2、校验
    if(path.isEmpty())
    {
        resultList->addItem("错误：请先选择目录");
        return;
    }

    //3、模拟搜索
    resultList->clear();
    resultList->addItem("开始搜索");
    startBtn->setEnabled(false);

    emit startSearchReq(path,keyword);
    stopBtn->setEnabled(true);
    // //强制刷新界面
    // QApplication::processEvents();

    // for(int i=0; i<5; ++i)
    // {
    //     QThread::sleep(1);

    //     resultList->addItem(QString("正在处理第%1个文件……").arg(i+1));
    // }

    // resultList->addItem(QString("搜索完成！"));
}

void MainWindow::onWOrkerFinished()
{
    resultList->addItem(QString("======搜索完毕========"));
    startBtn->setEnabled(true);
}

void MainWindow::onWorkerFoundFile(QString filePath)
{
    resultList->addItem(filePath);
    resultList->scrollToBottom();
}

void MainWindow::onStopSearch()
{
    if(workerThread->isRunning())
    {
        //workerThread->requestInterruption();
        worker->stopFlag = true;

        resultList->addItem(QString("正在停止搜索……"));
        stopBtn->setEnabled(false);
    }
}


void MainWindow::onFileDoubleClicked(QListWidgetItem* item)
{
    //1、拿取列表中的文件路径
    QString filePath = item->text();
    //2、转化为系统可识别的URL格式
    QUrl url = QUrl::fromLocalFile(filePath);
    //3、调用系统默认程序打开
    QDesktopServices::openUrl(url);
}
