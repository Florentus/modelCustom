#include "treeitem.h"
#include <QDebug>

// items implementation
//***************************************************************************
void Items::operator=(const Items &p)
{
    for (int i=0;i<p.items.size();i++) {
        items << p.items[i];
    }
}

Items::Items (const Items &p)
{
  *this = p;
}

void Items::setData(const QVariant &value, int role )
{
    QVector<item_t>::iterator it;
    for (it = items.begin(); it != items.end(); ++it) {
        if ((*it).role == role) {
            if (value.isValid()) {
                if ((*it).value == value)
                    return;
                (*it).value = value;
            } else {
                items.erase(it);
            }
        }
    }
    items.append({role, value});
}

QVariant Items::data(int role) const
{
    QVector<item_t>::const_iterator it;
    for (it = items.begin(); it != items.end(); ++it) {
        if ((*it).role == role)
            return (*it).value;
    }
    return QVariant();
}

// End items implementation
//***************************************************************************

TreeItem::TreeItem(const QVector<Items> &data, TreeItem *parent)
    :  parentItem(parent), itemData(data)
{}

TreeItem::TreeItem(const QStringList &qstrList)
{
    parentItem = nullptr;

    for (auto const &_st : qstrList) {
        Items tmp;
        tmp.items.append({Qt::DisplayRole,_st});
        itemData.append(tmp);
    }

}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
    qDeleteAll(deletedChildItems);
}

void TreeItem::backupItemData()
{
    itemDataSaved.clear();
    // A la première modification d'un élément, detach est appelé (ex : itemData[n] provoque un detach)
    itemDataSaved = itemData;
}

TreeItem *TreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int TreeItem::rowCount() const
{
    return childItems.count();
}

int TreeItem::rowNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column, int role) const
{
    if (column < 0 || column >= itemData.size())
        return QVariant();

    return itemData.at(column).data(role);
}

bool TreeItem::setData(int column, const QVariant &value,Global::rowStatus mode, int role)
{
    if (column < 0 || column >= itemData.size())
        return false;

    // Si ce n'est pas une nouvelle ligne (on est pas en mode init) alors on sauvegarde si pas encore fait
    if (mode == Global::edit && rowStatus == Global::init) {

        rowStatus = Global::modified;

        qDebug() << "role : " << role << " " << value << itemDataSaved.isEmpty();

        if (itemDataSaved.isEmpty()) {
            qDebug() << "Backup";
            backupItemData();
        }
    }

    itemData[column].setData(value, role);
    return true;
}

Qt::ItemFlags TreeItem::flags(int column) const
{
    QVariant v = data(column,Qt::UserRole - 1);

    // Si pas de flags, on retourne les flags par défault
    if (!v.isValid())
        return Qt::ItemFlags(defaultsFlags);

    return Qt::ItemFlags(v.toInt());
}

int TreeItem::find(QVariant data, int role)
{
    for (int i=0;i<itemData.size();i++) {
        if (itemData.at(i).data(role) == data) return i;
    }
    return -1;
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

bool TreeItem::insertRows(int position, int count, int columns, Global::rowStatus mode)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<Items> data(columns);
        TreeItem *item = new TreeItem(data, this);
        item->rowStatus = mode;
        childItems.insert(position, item);
    }

    return true;
}

bool TreeItem::removeRows(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        TreeItem* lTreeItem = childItems.takeAt(position);
        if (lTreeItem->rowStatus != Global::inserted)
            deletedChildItems.push_back(lTreeItem);
        else delete lTreeItem;
    }

    return true;
}

bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column) {
        itemData.insert(position,{});
    }

    for (TreeItem *child : qAsConst(childItems))
        child->insertColumns(position, columns);

    return true;
}

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    for (TreeItem *child : qAsConst(childItems))
        child->removeColumns(position, columns);

    return true;
}

// insere une ligne qui sert de header dans un treeView
void TreeItem::insertItemForAnHeader(int column, QStringList &QStrList, int role)
{

    if (column > QStrList.size()) column = QStrList.size();
    insertRows(0,1,column,Global::init);

    const QString *tmp;

    for (int i=0; i<column; i++) {
        tmp = &QStrList.at(i);
        childItems[0]->itemData[i].setData(*tmp, role);
    }
}

bool TreeItem::setFlags(int column, Qt::ItemFlags flags)
{
    if (column < 0 || column >= itemData.size())
        return false;

    QVariant v = data(column,Qt::UserRole - 1);
    if (!v.isValid()) flags = flags | Qt::ItemFlags(defaultsFlags);

    itemData[column].setData((int)flags, Qt::UserRole - 1);
    return true;
}

void TreeItem::setEditable(int column,bool editable)
{
    if (column < 0 || column >= itemData.size())
        return;

    changeFlags(column,editable, Qt::ItemIsEditable);
}

void TreeItem::setCheckable(int column, bool checkable)
{
    if (column < 0 || column >= itemData.size())
        return;

    if (checkable) {
        if (!data(column,Qt::CheckStateRole).isValid())
            setData(column, Qt::Unchecked, Global::init, Qt::CheckStateRole);
    }
    changeFlags(column, checkable, Qt::ItemIsUserCheckable);
}

void TreeItem::setToolTip(int column,const QString &atoolTip)
{
    if (column < 0 || column >= itemData.size())
        return;
    setData(column, atoolTip, Global::init, Qt::ToolTipRole);
}

void TreeItem::changeFlags(int column, bool enable, Qt::ItemFlags f)
{
    Qt::ItemFlags _flags = flags(column);
      if (enable)
          _flags |= f;
      else
          _flags &= ~f;
      itemData[column].setData((int)_flags, Qt::UserRole - 1);
}


