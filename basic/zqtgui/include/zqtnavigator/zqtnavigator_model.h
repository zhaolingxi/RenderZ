#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include"zqtgui.h"

ZQTGUI_NS_BEGIN

class TreeItem;

class ZQTGUI_API ZQtNavigatorModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ZQtNavigatorModel(QObject* parent = nullptr);
    ~ZQtNavigatorModel();

    // --- 必须实现的虚函数 ---
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QModelIndex index(int row, int column,
        const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    // --- 自定义公共方法 ---
    void clear();
    QModelIndex addItem(const QModelIndex& parent, const QString& text, const QIcon& icon = QIcon());
    TreeItem* getItem(const QModelIndex& index) const;
    QString getPath(const QModelIndex& index) const;
private:
    void setupTree(const QStringList& lines, TreeItem* parent);
    void setupModelData(const QStringList& lines);

    TreeItem* m_rootItem;
};
ZQTGUI_NS_END