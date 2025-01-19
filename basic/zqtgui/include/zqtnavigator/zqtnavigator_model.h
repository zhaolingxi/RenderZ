#pragma once

#include"zqtgui.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QMap>
#include<vector>
#include<variant>
#include<QVariant>
ZQTGUI_NS_BEGIN
class ZQTGUI_API TreeItem {
public:
    explicit TreeItem(const QVariant& data, TreeItem* parentItem = nullptr);

    ~TreeItem();

    void appendChild(TreeItem* child);

    TreeItem* child(int row);

    int childCount() const;

    int columnCount() const;

    QVariant data(int column) const;

    int row() const;

    TreeItem* parentItem() const;

    bool insertChildren(int position, int columns, const QVariant& data);

    bool removeChildren(int position, int count);

private:
    std::vector<TreeItem*> m_childItems;
    std::vector<QVariant> m_itemData;
    TreeItem* m_parentItem;
};

class ZQTGUI_API ZQtNavigatorModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit ZQtNavigatorModel(QObject* parent = nullptr);

    ~ZQtNavigatorModel();

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    TreeItem* getRootItem();

    TreeItem* ZQtNavigatorModel::getChildOfRoot(int row) const 
    { return m_rootItem->child(row); }

private:
    TreeItem* m_rootItem{ nullptr };
};
ZQTGUI_NS_END