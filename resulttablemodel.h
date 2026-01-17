#ifndef RESULTTABLEMODEL_H
#define RESULTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <vector>
#include "searchworker.h"

class ResultTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ResultTableModel(QObject *parent = nullptr);

    //1、告诉View有多少行
    int rowCount(const QModelIndex &parent = QModelIndex())const override;

    //2、告诉View有多少列
    int columnCount(const QModelIndex &parent = QModelIndex())const override;

    //3、View会循环调用这个函数，问要第row行，第col列数据
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const override;

    //4、设置表头
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)const override;

    //添加1条数据
    void appendData(const FileInfoData &info);

    //删除1调数据
    void removeRowData(int row);

    //清空数据
    void clear();

private:
    //使用std::vector来存数据，性能优于QListWidget
    std::vector<FileInfoData> m_dataList;

    //定义列的枚举，方便代码中维护
    enum Column{
        Col_Namr = 0,
        Col_Path,
        Col_Size,
        Col_Time,
        Col_Count                           //总列数
    };
};

#endif // RESULTTABLEMODEL_H
