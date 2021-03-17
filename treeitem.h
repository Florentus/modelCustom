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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QVector>
#include "global.h"

class Items {
public:

    struct item_t {
        int role;
        QVariant value;
    };

    QVector<item_t> items;

    Items(){};

    Items (const Items &p);

    // Surcharge de l'opérateur d'affectation
    void operator=(const Items &p);

    QVariant data(int role) const;
    void setData(const QVariant &value, int role);
};

class TreeItem
{
public:

    explicit TreeItem(const QVector<Items> &data = {}, TreeItem *parent = nullptr);
    explicit TreeItem(const QStringList &qstrList);
    ~TreeItem();

    TreeItem *child(int number);

    int rowCount() const;
    int columnCount() const;

    int rowNumber() const;

    TreeItem *parent();

    QVariant data(int column, int role=Qt::DisplayRole) const;
    bool setData(int column, const QVariant &value, Global::rowStatus mode, int role=Qt::EditRole);

    Qt::ItemFlags flags(int column) const;
    bool setFlags(int column, Qt::ItemFlags flags);
    static Qt::ItemFlags getDefaultsFlags() {return Qt::ItemFlags(defaultsFlags);}

    // insere une ligne qui sert de header dans un treeView
    void insertItemForAnHeader(int column, QStringList &QStrList,int role);

    bool insertRows(int position, int count, int columns, Global::rowStatus mode);
    bool insertColumns(int position, int columns);

    bool removeRows(int position, int count);
    bool removeColumns(int position, int columns);

    void setEditable(int column, bool editable);
    void setCheckable(int column, bool checkable);
    void setToolTip(int column, const QString &atoolTip);

    Global::rowStatus getRowStatus() {return rowStatus;}
    void backupItemData();

    int find(QVariant data, int role);

private:
    void changeFlags(int column, bool enable, Qt::ItemFlags f);

    static const int defaultsFlags =  Qt::ItemIsSelectable
                                     |Qt::ItemIsEnabled
                                     |Qt::ItemIsEditable
                                     |Qt::ItemIsDragEnabled
                                     |Qt::ItemIsDropEnabled;

    Global::rowStatus rowStatus;
    TreeItem *parentItem;
    QVector<TreeItem*> childItems;
    QVector<TreeItem*> deletedChildItems;
    QVector<Items> itemData;
    QVector<Items> itemDataSaved;   // conserve l'original si itemData est modifié la première fois.

};

#endif // TREEITEM_H
