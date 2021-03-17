#include "treemodel.h"
#include "treeitem.h"

#include <QtWidgets>

TreeModel::TreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem(headers);
    linearGrad = QLinearGradient(QPointF(0,0), QPointF(100,20));
    linearGrad.setColorAt(0, Qt::white);
    linearGrad.setColorAt(1, Qt::gray);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->rowNumber(), 0, parentItem);
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    int count = parentItem ? parentItem->rowCount() : 0;
    return count;
}

int TreeModel::rowCountSkipHeader(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    int count = parentItem ? parentItem->rowCount() : 0;
    if (parentItem != rootItem) count--;
    return count;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) {
        if(parent.column() != 0)
            return QModelIndex();
    }

    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

// Index pour les branches du treeView , saute le header
QModelIndex TreeModel::indexSkipHeader(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) {
        if(parent.column() != 0)
            return QModelIndex();
    }

    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    if (parentItem != rootItem) row++;
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

int TreeModel::indexHeader(const QModelIndex &lindex,QString header)
{
    QModelIndex parent = lindex.parent();
    TreeItem *item = getItem(index(0,0,parent));
    int i = item->find(header,UidHeaderRole);
    return i;
}

void TreeModel::backupTreeItem(const QModelIndex &index)
{
    if (index.isValid()) {
        TreeItem *item = getItem(index);
        item->backupItemData();
    }
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;
    TreeItem *item = getItem(index);

    QVariant _tmp;

    switch (role)
    {
    case Qt::DisplayRole:
        _tmp = item->data(index.column(),UidHeaderRole);
        if (_tmp.isValid()) return _tmp;
        return item->data(index.column(),role);
        break;

    case Qt::BackgroundRole:
        if (item->data(index.column(),UidHeaderRole).isValid())
            return QBrush(linearGrad);
        else return QVariant();
        break;

    case Qt::ForegroundRole:
        if (item->data(index.column(),UidHeaderRole).isValid())
            return QColor(0,0,0);
        else return QVariant();
        break;

    case Qt::FontRole:
        if (item->data(index.column(),UidHeaderRole).isValid()) {
            QFont font;
            font.setItalic(true);
            return font;
        } else return QVariant();

    case Qt::SizeHintRole:
        if (item->data(index.column(),UidHeaderRole).isValid())
            return QSize(0,22);
        else return QSize(0,20);
        break;

    default:
        return item->data(index.column(),role);
    }
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value, mode, role);

    if (result)
        emit dataChanged(index, index);

    return result;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(section, value, Global::init);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;

    TreeItem *item = getItem(index);

    Qt::ItemFlags item_flags;

    if (item)
        item_flags = item->flags(index.column());
    else return Qt::ItemFlags(TreeItem::getDefaultsFlags());

    // header row is not enabled
    if (item->data(index.column(),UidHeaderRole).isValid())
        item_flags &= ~(Qt::ItemIsEnabled);

    return item_flags;
}

bool TreeModel::setFlags(const QModelIndex &index, Qt::ItemFlags flags)
{
    if (index.isValid()) {
        TreeItem *item = getItem(index);
        if (item)
            return item->setFlags(index.column(),flags);
    }
    return false;
}

void TreeModel::setEditable(const QModelIndex &index, bool editable)
{
    if (index.isValid())
        getItem(index)->setEditable(index.column(),editable);
}

void TreeModel::setCheckable(const QModelIndex &index, bool checkable)
{
    if (index.isValid())
        getItem(index)->setCheckable(index.column(),checkable);
}

void TreeModel::setToolTip(const QModelIndex &index, const QString &atoolTip)
{
    if (index.isValid())
        getItem(index)->setToolTip(index.column(),atoolTip);
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);

    bool success;

    // si on est en mode init alors on est en insertion d'une nouvelle ligne
    Global::rowStatus _mode = (mode == Global::init ? mode : Global::inserted);

    if (parentItem == rootItem) {
        success = parentItem->insertRows(position,rows,rootItem->columnCount(),_mode);
    } else {
        if (parentItem->rowCount() == 0) {
            parentItem->insertItemForAnHeader(rootItem->columnCount(),headerItem, UidHeaderRole);
        }
        success = parentItem->insertRows(position+1,rows,rootItem->columnCount(),_mode);
    }

    endInsertRows();

    return success;
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeRows(position, rows);
    endRemoveRows();

    return success;
}

Global::rowStatus TreeModel::getRowMode(const QModelIndex &index)
{
    if (!index.isValid())
        return Global::undef;

    TreeItem *item = getItem(index);
    if (item)
        return item->getRowStatus();

    return Global::undef;
}



