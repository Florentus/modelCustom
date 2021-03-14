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
    model->setHeaderItems({"col1","col2","col3","col4"});

    ui->treeView->setModel(model);
    ui->tableView->setModel(model);
    model->insertRows(0,10);                                 // ligne 0 , 10 lignes

    QModelIndex indexRow0Col0 = model->index(0,0);
    model->insertRows(0,1,indexRow0Col0);                     // insert childItems

//    model->setData(indexRow0Col0,23);
//    model->setData(indexRow0Col0,Qt::Unchecked,Qt::CheckStateRole);
//    model->setData(indexRow0Col0,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
//    model->setCheckable(indexRow0Col0,true);

    QModelIndex indexRow0Col1 = model->index(0,1);
    model->setData(indexRow0Col1,"Hello");

    QModelIndex indexRow1Col2 = model->index(1,2);
    model->setData(indexRow1Col2,"ok",Qt::DisplayRole);
    model->setData(indexRow1Col2,Qt::Checked,Qt::CheckStateRole);
    model->setData(indexRow1Col2,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
    model->setCheckable(indexRow1Col2,false);

    QModelIndex row0Col1_In_Row0Col0 = model->indexSkipHeader(0,1,indexRow0Col0);

    model->setData(row0Col1_In_Row0Col0,"test toto");
    model->setData(row0Col1_In_Row0Col0,Qt::Unchecked,Qt::CheckStateRole);
    model->setData(row0Col1_In_Row0Col0,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
    model->setCheckable(row0Col1_In_Row0Col0,true);
    model->setEditable(row0Col1_In_Row0Col0,false);

    QVariant htmp = row0Col1_In_Row0Col0.data();

    QModelIndex indexRow1Col0 = model->index(1,0);
    model->insertRows(0,1,indexRow1Col0);
    model->insertRows(0,1,indexRow1Col0);

    QModelIndex row0Col0_In_Row1Col0 = model->indexSkipHeader(0,0,indexRow1Col0);
    model->setData(row0Col0_In_Row1Col0,"test toto");
    model->setData(row0Col0_In_Row1Col0,Qt::Unchecked,Qt::CheckStateRole);
    model->setData(row0Col0_In_Row1Col0,Qt::AlignLeft, CenteredBoxStyle::CheckAlignmentRole);
    model->setCheckable(row0Col0_In_Row1Col0,true);

    model->insertRows(0,1,indexRow0Col0);


    QModelIndex ligne0Col0_In_Ligne0Col0 = model->indexSkipHeader(0,0,model->index(0,0));
    int j = model->indexHeader(ligne0Col0_In_Ligne0Col0,"col3");

    j = model->rowCountSkipHeader(QModelIndex());

    QModelIndex indx = model->indexSkipHeader(5,0);

    int rowCount = model->rowCount(QModelIndex());
    for (int i = 0;i<rowCount;i++) {
        indx = model->index(i,0);
        model->setData(indx,i);
    }

    indx = model->index(0,0);
    model->setToolTip(indx,"test");
    indx = model->index(5,0);
    model->removeRows(indx.row(),1,indx.parent());

}

MainWindow::~MainWindow()
{
    delete ui;
}

