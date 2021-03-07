#include "mainwindow.h"
#include "treemodel.h"
#include "treeitem.h"
#include "ui_mainwindow.h"
#include "centeredboxstyle.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TreeModel *model = new TreeModel({"toto","Alain","ludo","Orlice"});
    model->setHeaderItem({"col1","col2","col3","col4"});

    ui->treeView->setModel(model);
    ui->tableView->setModel(model);
    model->insertRows(0,2); // pour test

    QModelIndex index = model->index(0,0);
    model->insertRows(0,1,index);
    QModelIndex index2 = model->index(1,2);

    model->setData(index,23);
    model->setData(index,Qt::Unchecked,Qt::CheckStateRole);
    model->setData(index,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
    model->setCheckable(index,true);
    //model->setFlags(index, TreeItem::getDefaultsFlags() | Qt::ItemIsUserCheckable);

    model->setData(model->index(0,1),"Hello");

    model->setData(index2,"ok",Qt::DisplayRole);
    model->setData(index2,Qt::Checked,Qt::CheckStateRole);
    model->setData(index2,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
    model->setCheckable(index2,false);

    QModelIndex indexInBranch = model->index(1,1,index);
    QModelIndex tmpIndex = model->indexHelper(0,1,index);

    model->setData(tmpIndex,"test toto");
    model->setData(tmpIndex,Qt::Unchecked,Qt::CheckStateRole);
    model->setData(tmpIndex,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
    model->setCheckable(tmpIndex,true);
    model->setEditable(tmpIndex,false);

    QVariant tmp = indexInBranch.data();
    QVariant htmp = tmpIndex.data();

    QString str = tmp.toString();

    index = model->index(1,0);
    model->insertRows(0,1,index);
    model->insertRows(0,1,index);

    tmpIndex = model->indexHelper(0,1,index);

    model->setData(tmpIndex,"test toto");
    model->setData(tmpIndex,Qt::Unchecked,Qt::CheckStateRole);
    model->setData(tmpIndex,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
    model->setCheckable(tmpIndex,true);
    model->insertRows(0,1,index);

}

MainWindow::~MainWindow()
{
    delete ui;
}

