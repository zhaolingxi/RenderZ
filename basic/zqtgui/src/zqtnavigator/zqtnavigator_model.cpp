#include"zqtnavigator/zqtnavigator_model.h"
#include "zqtnavigator/treeitem.h"

ZQTGUI_NS_BEGIN



ZQtNavigatorModel::ZQtNavigatorModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Root"; // 根节点数据，通常是隐藏的
    m_rootItem = new TreeItem(rootData);
}

ZQtNavigatorModel::~ZQtNavigatorModel()
{
    delete m_rootItem;
}

int ZQtNavigatorModel::columnCount(const QModelIndex& parent) const
{
    return m_rootItem->columnCount();
}

QVariant ZQtNavigatorModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

Qt::ItemFlags ZQtNavigatorModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QModelIndex ZQtNavigatorModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem* parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ZQtNavigatorModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ZQtNavigatorModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}


TreeItem* ZQtNavigatorModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return m_rootItem;
}

void ZQtNavigatorModel::clear()
{
    // beginResetModel/endResetModel 是最高效的清空模型的信号
    beginResetModel();
    delete m_rootItem; // 删除所有旧的TreeItem

    // 重新创建根节点
    QList<QVariant> rootData;
    rootData << "Root";
    m_rootItem = new TreeItem(rootData);
    endResetModel();
}

QModelIndex ZQtNavigatorModel::addItem(const QModelIndex& parent, const QString& text, const QIcon& icon)
{
    TreeItem* parentItem = getItem(parent);
    if (!parentItem) {
        return QModelIndex(); // 如果父节点无效，则不添加
    }

    // beginInsertRows/endInsertRows 通知视图有新数据插入，这是必须的！
    // 它们必须成对出现，中间是实际的数据修改操作。
    int newRow = parentItem->childCount();
    beginInsertRows(parent, newRow, newRow);

    // 创建新的节点数据
    QList<QVariant> itemData;
    itemData << text;
    TreeItem* newItem = new TreeItem(itemData, parentItem);

    // 如果提供了图标，就设置它
    if (!icon.isNull()) {
        newItem->setData(0, QVariant::fromValue(icon), IconRole);
    }

    parentItem->appendChild(newItem);

    endInsertRows();

    // 返回新创建项的索引，方便继续添加子项
    return index(newRow, 0, parent);
}


// 示例函数: 用一个带缩进的字符串列表来初始化模型数据
void ZQtNavigatorModel::setupModelData(const QStringList& lines)
{
    beginResetModel();
    // 清理旧数据
    delete m_rootItem;
    QList<QVariant> rootData;
    rootData << "Root";
    m_rootItem = new TreeItem(rootData);

    // 解析新数据
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << m_rootItem;
    indentations << 0;

    for (const QString& line : lines) {
        int position = 0;
        while (position < line.length() && line.at(position).isSpace())
            position++;

        QString lineData = line.mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // 找到正确的父节点
            if (position > indentations.last()) {
                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount() - 1);
                    indentations << position;
                }
            }
            else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // 添加新节点
            QList<QVariant> columnData;
            columnData << lineData;
            TreeItem* parent = parents.last();
            parent->appendChild(new TreeItem(columnData, parent));
        }
    }

    endResetModel();
}

ZQTGUI_NS_END