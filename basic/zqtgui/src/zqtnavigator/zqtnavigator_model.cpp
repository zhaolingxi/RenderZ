#include"zqtnavigator_model.h"

ZQTGUI_NS_BEGIN

TreeItem::TreeItem(const QVariant& data, TreeItem* parentItem ) {
    m_parentItem = parentItem;
    m_itemData.push_back(data);
}

TreeItem::~TreeItem() {
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem* child) {
    m_childItems.push_back(child);
}

TreeItem* TreeItem::child(int row) {
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems[row];
}

int TreeItem::childCount() const {
    return m_childItems.size();
}

int TreeItem::columnCount() const {
    return m_itemData.size();
}

QVariant TreeItem::data(int column) const {
    if (column < 0 || column >= m_itemData.size())
        return {};
    return m_itemData[column];
}

int TreeItem::row() const {
    if (m_parentItem) {
        auto it = std::find(m_parentItem->m_childItems.begin(), m_parentItem->m_childItems.end(), this);
        if (it != m_parentItem->m_childItems.end()) {
            return std::distance(m_parentItem->m_childItems.begin(), it);
        }
    }
    return 0;
}

TreeItem* TreeItem::parentItem() const {
    return m_parentItem;
}

bool TreeItem::insertChildren(int position, int columns, const QVariant& data) {
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int i = 0; i < columns; ++i) {
        TreeItem* newItem = new TreeItem(data, this);
        m_childItems.insert(m_childItems.begin() + position + i, newItem);
    }
    return true;
}

bool TreeItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int i = 0; i < count; ++i) {
        TreeItem* item = m_childItems.at(position + i);
        delete item;
    }
    m_childItems.erase(m_childItems.begin() + position, m_childItems.begin() + position + count);
    return true;
}


ZQtNavigatorModel::ZQtNavigatorModel(QObject* parent) : QAbstractItemModel(parent) {
    m_rootItem = new TreeItem(QVariant("RootNode"));
}

ZQtNavigatorModel::~ZQtNavigatorModel() {
    delete m_rootItem;
}

QModelIndex ZQtNavigatorModel::index(int row, int column, const QModelIndex& parent) const  {
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
    else
        return QModelIndex();
}

QModelIndex ZQtNavigatorModel::parent(const QModelIndex& index) const {
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ZQtNavigatorModel::rowCount(const QModelIndex& parent) const {
    TreeItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ZQtNavigatorModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return m_rootItem->columnCount();
}

QVariant ZQtNavigatorModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();
    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

bool ZQtNavigatorModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    // Implement data change logic here
    return true;
}

Qt::ItemFlags ZQtNavigatorModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

TreeItem* ZQtNavigatorModel::getRootItem() { return m_rootItem; };

ZQTGUI_NS_END