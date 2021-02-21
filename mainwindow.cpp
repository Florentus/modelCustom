#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TreeModel *model = new TreeModel({"toto","Alain"});
    ui->tableView->setModel(model);
    model->insertRows(0,2);   
}

MainWindow::~MainWindow()
{
    delete ui;
}
