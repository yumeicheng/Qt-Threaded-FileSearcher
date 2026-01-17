#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>          //弹出选择文件夹的窗口
#include <QMessageBox>          //用于弹窗提示
#include <QColor>
#include <QThread>
#include <QApplication>
#include <QHeaderView>          //调整表头列宽
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QProcess>             //调用explorer
#include <QFile>


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
    //resultList = new QListWidget(centraWidget);
    resultView = new QTableView(this);
    model = new ResultTableModel(this);
    startBtn = new QPushButton("开始搜索",centraWidget);
    browseBtn = new QPushButton("浏览",centraWidget);
    stopBtn = new QPushButton("停止",centraWidget);
    stopBtn->setEnabled(false);
    pathEdit->setPlaceholderText("请输入要寻找的地址");
    keywordEdit->setPlaceholderText("输入文件关键词（为空则搜索全部）");

    resultView->setModel(model);
    resultView->setContextMenuPolicy(Qt::CustomContextMenu);
    //开始布局
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(pathEdit,1);
    topLayout->addWidget(browseBtn);
    topLayout->addWidget(keywordEdit,1);
    topLayout->addWidget(startBtn);
    topLayout->addWidget(stopBtn);

    //单选模式（一次只能选一行）
    resultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //最后一列自动拉伸
    //resultView->horizontalHeader()->setStretchLastSection(true);
    //获取表头对象
    QHeaderView *header = resultView->horizontalHeader();
    //根据内容自动调整宽度（ResizeToContents）
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    //第1列（路径）：自动拉伸，占据剩余空间（Stretch）
    header->setSectionResizeMode(1,QHeaderView::Stretch);

    //隐藏原本左边的行号
    resultView->verticalHeader()->setVisible(false);
    //启用交替背景行
    resultView->setAlternatingRowColors(true);
    //不允许编辑内容
    resultView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    //mainLayout->addWidget(resultList);
    mainLayout->addWidget(resultView);

    centraWidget->setLayout(mainLayout);


    workerThread = new QThread(this);               //线程对象
    worker = new SearchWorker();                    //工作者对象
    worker->moveToThread(workerThread);             //移交worker实例线程归属权
    connect(this,&MainWindow::startSearchReq,worker,&SearchWorker::doSearch);
    connect(worker,&SearchWorker::foundFile,this,&MainWindow::onWorkerFoundFile);
    connect(worker,&SearchWorker::searchFinished,this,&MainWindow::onWOrkerFinished);
    connect(workerThread,&QThread::finished,worker,&QObject::deleteLater);
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
    //connect(resultList,&QListWidget::itemDoubleClicked,this,&MainWindow::onFileDoubleClicked);
    connect(resultView,&QTableView::doubleClicked,this,&MainWindow::onFileDoubleClicked);
    connect(resultView,&QTableView::customContextMenuRequested,this,&MainWindow::onShowContextMenu);
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
        //resultList->addItem("错误：请先选择目录");
        return;
    }

    //3、模拟搜索
    //resultList->clear();
    //resultList->addItem("开始搜索");
    model->clear();
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
    //resultList->addItem(QString("======搜索完毕========"));
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
}

void MainWindow::onWorkerFoundFile(FileInfoData info)
{
    //resultList->addItem(filePath);
    //resultList->scrollToBottom();
    model->appendData(info);

    //自动滚到底部
    resultView->scrollToBottom();
}

void MainWindow::onStopSearch()
{
    if(workerThread->isRunning())
    {
        //workerThread->requestInterruption();
        worker->stopFlag = true;

        //resultList->addItem(QString("正在停止搜索……"));
        stopBtn->setEnabled(false);
    }
}


void MainWindow::onFileDoubleClicked(const QModelIndex &index)
{
    //1、拿取列表中的文件路径
    //QString filePath = item->text();
    //2、转化为系统可识别的URL格式
    //QUrl url = QUrl::fromLocalFile(filePath);
    //3、调用系统默认程序打开
    //QDesktopServices::openUrl(url);

    //获取被点击的那一行
    int row = index.row();
    //拿到完整路径
    QModelIndex pathIndex = index.sibling(row,1);
    //拿到数据
    QString filePath = pathIndex.data().toString();
    //打开文件
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}


void MainWindow::onShowContextMenu(const QPoint &pos)
{
    //以此点为根据，判断鼠标点在了哪一行
    //indexAt是QTableView的方法，通过坐标找到数据索引
    QModelIndex index = resultView->indexAt(pos);

    //如果点在空白处（没点中任何行），直接返回，不弹菜单
    if(!index.isValid())return;

    //创建菜单对象
    //parent设置为resultView，这样表格销毁时菜单也会销毁
    QMenu *menu = new QMenu(resultView);

    //创建“复制路径”动作
    QAction *copyAction = new QAction("复制完整路径",menu);

    //使用Lambda表达式处理点击逻辑
    connect(copyAction,&QAction::triggered,this,[=](){
        //记录当前行，第一列（Col_Path）的数据
        //记住ResultTableMdel里定义的Col_Path是1
        QString filePath = model->index(index.row(),1).data().toString();

        //写入系统剪贴板
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(filePath);
    });

    //创建“打开所在目录”动作
    QAction *openDirAction = new QAction("打开所在文件夹",menu);
    connect(openDirAction,&QAction::triggered,this,[=](){
        QString filePath = model->index(index.row(),1).data().toString();
        //使用explorer.exe/select，filename可以在打开文件夹的同时，高亮选中这个文件
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(filePath);
        QProcess::startDetached("explorer.exe",args);
    });

    //创建“删除”动作
    QAction *delAction = new QAction("删除文件",menu);
    connect(delAction,&QAction::triggered,this,[=](){
        int row = index.row();
        QString filePath = model->index(index.row(),1).data().toString();

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"确认删除",
                                      "确认永久删除这个文件吗？\n" + filePath,
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            //物理删除（删除硬盘上的文件）
            QFile file(filePath);
            if(file.remove())
            {
                //如果物理删除成功，再删除界面上的（删vector里的）
                model->removeRowData(row);
            }
            else{
                QMessageBox::warning(this,"失败","删除失败，可能文件正在被占用或没有权限");
            }
        }
    });

    //创建“打开”动作
    QAction *openAction = new QAction("打开",menu);
    connect(openAction,&QAction::triggered,this,[=](){
        this->onFileDoubleClicked(index);
    });

    //将动作添加到菜单
    menu->addAction(openAction);
    menu->addAction(copyAction);
    menu->addAction(openDirAction);
    menu->addSeparator();
    menu->addAction(delAction);

    //在在鼠标位置弹出菜单
    //mapToClobal：把把表格内的相对坐标（pos）转换为屏幕的绝对坐标
    //菜单必须在屏幕坐标系下弹出
    menu->exec(resultView->viewport()->mapToGlobal(pos));

    //内存清理：菜单关闭后自动delete
    //因为menu是new出来的，如果不delete会内存泄漏
    //setAttribute（Qt：：WA_DeleteOnClose）会在菜单关闭后自动销毁对象
    menu->setAttribute(Qt::WA_DeleteOnClose);
}
