#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QIcon>
#include "zqtnavigatordefs.h"
#include"zqtgui.h"
ZQTGUI_NS_BEGIN
class ZQTGUI_API  TreeItem
{
public:
    explicit TreeItem(const QList<QVariant>& data, TreeItem* parent = nullptr);
    ~TreeItem();

    void appendChild(TreeItem* child);

    TreeItem* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column, int role) const;
    void setData(int column, const QVariant& value, int role = Qt::EditRole);

    int row() const;
    TreeItem* parentItem();

private:
    TreeItem* m_parentItem;
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    QIcon m_icon;
};

#endif // TREEITEM_H
ZQTGUI_NS_END