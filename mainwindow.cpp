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

    QString toFind{"col3"};
    int IndHeader = model->indexHeader(ligne0Col0_In_Ligne0Col0,toFind);

    QModelIndex indx = model->indexSkipHeader(5,0);

    int rowCount = model->rowCount(QModelIndex());
    for (int i = 0;i<rowCount;i++) {
        indx = model->index(i,0);
        model->setData(indx,i);
        QString tmp = QString("Mode : %1").arg(model->getRowMode(indx));
        model->setToolTip(indx,tmp);
        indx = model->index(i,IndHeader);
        model->setData(indx,toFind);        
    }

    //model->removeRows(0,1,indx.parent());

    model->setMode(Global::edit);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ButtonInsert_clicked()
{

    TreeModel *model = qobject_cast<TreeModel *>(ui->treeView->model());

    if (model) {
        QModelIndexList index;
        QModelIndex parent;
        index =  ui->treeView->selectionModel()->selectedRows();

        if (!index.isEmpty()) {
            parent = index[0].parent();
            model->insertRows(index[0].row(),1,parent);
        }
    }

}

void MainWindow::on_DeleteLine_clicked()
{
    TreeModel *model = qobject_cast<TreeModel *>(ui->treeView->model());

    if (model) {
        QModelIndexList index;
        QModelIndex parent;
        index =  ui->treeView->selectionModel()->selectedRows();

        if (!index.isEmpty()) {
            parent = index[0].parent();
            model->removeRows(index[0].row(),1,parent);
        }
    }
}

void MainWindow::on_DeleteModel_clicked()
{
    TreeModel *model = qobject_cast<TreeModel *>(ui->treeView->model());
    if (model)
        delete model;
}
