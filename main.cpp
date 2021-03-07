#include "centeredboxstyle.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new CenteredBoxStyle);
    /*
    a.setStyleSheet(""
        "QTreeView {outline:none;show-decoration-selected: 1;}"
        "QTreeView {outline:none;border:0px;}"
        "QTreeView::branch{background-color: transparent;  }"
        "QTreeView::item:hover, QTreeView::branch:hover { background-color: transparent;border-color: rgb(255, 0, 0);}"
        "QTreeView::item:selected, QTreeView::branch:selected { background-color: #C5E0F7;}"
        "QTreeView::branch:open:has-children{image: url(:/branch-expand.png);}"
        "QTreeView::branch:closed:has-children{image: url(:/branch-collapse.png);}"
        "QTreeView::branch:has-siblings:!adjoins-item{border-image:url(:/branch-line.png) 0;}"
        "QTreeView::branch:has-siblings:adjoins-item{border-image:url(:/branch-more.png) 0;}"
        "QTreeView::branch:!has-children:!has-siblings:adjoins-item{border-image:url(:/branch-end.png) 0;   }"
        "QTreeView::branch:has-children:!has-siblings:closed,QTreeView::branch:closed:has-children:has-siblings{border-image:none;image: url(:/branch-collapse.png);    }"
        "QTreeView::branch:open:has-children:!has-siblings,QTreeView::branch:open:has-children:has-siblings{border-image:none;image: url(:/branch-expand.png);  }"
        );
*/
    MainWindow w;
    w.show();
    return a.exec();
}
