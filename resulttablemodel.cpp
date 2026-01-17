#include "resulttablemodel.h"

ResultTableModel::ResultTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{}


int ResultTableModel::rowCount(const QModelIndex &parent) const
{
    //如果parent存在（用于树形结构），我们这里是平面表格，返回0
    if(parent.isValid())
    {
        return 0;
    }

    //返回我们数据列的长度
    return static_cast<int>(m_dataList.size());
}


int ResultTableModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    //返回数据总列数
    return Col_Count;
}

//View绘制每一个单元格时，都会调用这个函数
QVariant ResultTableModel::data(const QModelIndex &index, int role) const
{
    //1、基本校验
    if(!index.isValid() || index.row() >= static_cast<int>(m_dataList.size()))
    {
        return QVariant();
    }

    //2、拿到这一行的数据
    const FileInfoData &item = m_dataList[index.row()];

    //3、根据不同的“角色”返回不同的数据
    if(role == Qt::DisplayRole)
    {
        //qDebug() << "View 正在索要第" << index.row() << "行的数据";
        switch(index.column())
        {
            case Col_Namr: return item.fileName;
            case Col_Path: return item.filePath;
            case Col_Size: return item.size;
            case Col_Time: return item.modified;
        }
    }

    return QVariant();
}

//设置表头
QVariant ResultTableModel::headerData( int section, Qt::Orientation orientain, int role) const
{
    if(role == Qt::DisplayRole && orientain == Qt::Horizontal)
    {
        switch(section)
        {
            case Col_Namr: return "文件名";
            case Col_Path: return "完整路径";
            case Col_Size: return "大小";
            case Col_Time: return "修改时间";
        }
    }
    return QVariant();
}


//数据操作
void ResultTableModel::appendData(const FileInfoData &info)
{
    //beginInseverRows参数：父索引，开始行，结束行
    //告诉view：我要在末尾插入一行，准备好刷新
    beginInsertRows(QModelIndex(),m_dataList.size(),m_dataList.size());

    m_dataList.push_back(info);

    //插入完成，通知view更新
    endInsertRows();
}

//删除1行数据
void ResultTableModel::removeRowData(int row)
{
    //安全检查，防止行号越界
    if(row < 0 || row >= static_cast<int>(m_dataList.size()))
    {
        return;
    }

    //通知view，要开始删除了
    //参数：父索引，起始行，结束行
    beginRemoveRows(QModelIndex(),row,row);

    //真正的删除数据（操作vector）
    //m_dataList.begin()+row就是要找到的哪个位置的迭代器
    m_dataList.erase(m_dataList.begin()+row);

    //通知view删除完毕，view会自动刷新界面，把删除的那一行变成没
    endRemoveRows();
}

//清除
void ResultTableModel::clear()
{
    beginResetModel();
    m_dataList.clear();
    endResetModel();
}
