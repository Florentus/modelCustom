#ifndef CENTEREDBOXSTYLE_H
#define CENTEREDBOXSTYLE_H

#include <QProxyStyle>
#include <QPainter>
#include <QStyleOption>
#include <QCommonStyle>
#include <QPixmap>

class CenteredBoxStyle : public QProxyStyle
{
public:

    enum {
      CheckAlignmentRole = Qt::UserRole + Qt::CheckStateRole + Qt::TextAlignmentRole
    };

    CenteredBoxStyle();

    void drawPrimitive(PrimitiveElement element,const QStyleOption *option,QPainter *painter,const QWidget *widget) const override;
    QRect subElementRect(QStyle::SubElement element,const QStyleOption *option,const QWidget *widget) const override;

private:
    void drawCheckBoxIndicator(const QStyleOption *option, QPainter *painter) const;
    void initPixMaps(const QStyleOption *option) const;

    mutable QPixmap lpixmapCheckBoxOff;
    mutable QPixmap lpixmapCheckBoxOn;
    mutable QPixmap lpixmapCheckBoxTriState;
};

#endif // CENTEREDBOXSTYLE_H
