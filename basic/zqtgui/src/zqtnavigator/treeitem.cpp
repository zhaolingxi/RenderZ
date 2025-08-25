#include "zqtnavigator/treeitem.h"
ZQTGUI_NS_BEGIN
TreeItem::TreeItem(const QList<QVariant>& data, TreeItem* parent)
    : m_parentItem(parent), m_itemData(data)
{}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem* item)
{
    m_childItems.append(item);
}

TreeItem* TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant TreeItem::data(int column, int role) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return m_itemData.at(column);
    }
    else if (role == IconRole) {
        return QVariant::fromValue(m_icon);
    }
    return QVariant();
}

void TreeItem::setData(int column, const QVariant& value, int role)
{
    if (column < 0 || column >= m_itemData.size())
        return;

    if (role == Qt::EditRole) {
        m_itemData[column] = value;
    }
    else if (role == IconRole) {
        m_icon = value.value<QIcon>();
    }
}


TreeItem* TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
ZQTGUI_NS_END