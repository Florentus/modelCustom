/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
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

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
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

// Index pour les branches du treeVeiw , saute le header
QModelIndex TreeModel::indexHelper(int row, int column, const QModelIndex &parent) const
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

    if (parentItem == rootItem) {
        success = parentItem->insertChildren(position,rows,rootItem->columnCount());
    } else {
        if (parentItem->childCount() == 0) {
            parentItem->insertItemForAnHeader(rootItem->columnCount(),headerItem, UidHeaderRole);
        }
        success = parentItem->insertChildren(position+1,rows,rootItem->columnCount());
    }

    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
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
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    int count = parentItem ? parentItem->childCount() : 0;
    return count;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value, role);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}


