#include "mainwindow.h"
#include "treemodel.h"
#include "treeitem.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TreeModel *model = new TreeModel({"toto","Alain","ludo"});
    ui->tableView->setModel(model);
    model->insertRows(0,12); // pour test

    QModelIndex index = model->index(1,0);
    QModelIndex index2 = model->index(1,2);

    model->setData(index,23);
    model->setData(index,Qt::Unchecked,Qt::CheckStateRole);
    model->getItem(index)->setCheckable(index.column(),true);
    model->setFlags(index, TreeItem::getDefaultsFlags() | Qt::ItemIsUserCheckable);

    model->setData(index2,Qt::Checked,Qt::CheckStateRole);
    model->getItem(index2)->setCheckable(index2.column(),true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

