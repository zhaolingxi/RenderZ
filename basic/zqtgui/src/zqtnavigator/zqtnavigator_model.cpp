#include"zqtnavigator/zqtnavigator_model.h"
#include "zqtnavigator/treeitem.h"

ZQTGUI_NS_BEGIN



ZQtNavigatorModel::ZQtNavigatorModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Root"; // ���ڵ����ݣ�ͨ�������ص�
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
    // beginResetModel/endResetModel �����Ч�����ģ�͵��ź�
    beginResetModel();
    delete m_rootItem; // ɾ�����оɵ�TreeItem

    // ���´������ڵ�
    QList<QVariant> rootData;
    rootData << "Root";
    m_rootItem = new TreeItem(rootData);
    endResetModel();
}

QModelIndex ZQtNavigatorModel::addItem(const QModelIndex& parent, const QString& text, const QIcon& icon)
{
    TreeItem* parentItem = getItem(parent);
    if (!parentItem) {
        return QModelIndex(); // ������ڵ���Ч�������
    }

    // beginInsertRows/endInsertRows ֪ͨ��ͼ�������ݲ��룬���Ǳ���ģ�
    // ���Ǳ���ɶԳ��֣��м���ʵ�ʵ������޸Ĳ�����
    int newRow = parentItem->childCount();
    beginInsertRows(parent, newRow, newRow);

    // �����µĽڵ�����
    QList<QVariant> itemData;
    itemData << text;
    TreeItem* newItem = new TreeItem(itemData, parentItem);

    // ����ṩ��ͼ�꣬��������
    if (!icon.isNull()) {
        newItem->setData(0, QVariant::fromValue(icon), IconRole);
    }

    parentItem->appendChild(newItem);

    endInsertRows();

    // �����´��������������������������
    return index(newRow, 0, parent);
}


// ʾ������: ��һ�����������ַ����б�����ʼ��ģ������
void ZQtNavigatorModel::setupModelData(const QStringList& lines)
{
    beginResetModel();
    // ���������
    delete m_rootItem;
    QList<QVariant> rootData;
    rootData << "Root";
    m_rootItem = new TreeItem(rootData);

    // ����������
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
            // �ҵ���ȷ�ĸ��ڵ�
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

            // ����½ڵ�
            QList<QVariant> columnData;
            columnData << lineData;
            TreeItem* parent = parents.last();
            parent->appendChild(new TreeItem(columnData, parent));
        }
    }

    endResetModel();
}

ZQTGUI_NS_END