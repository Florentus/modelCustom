#include "centeredboxstyle.h"
#include <QtDebug>

CenteredBoxStyle::CenteredBoxStyle()
{

}

void CenteredBoxStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (element) {
    //case PE_IndicatorCheckBox:
    case PE_IndicatorItemViewItemCheck:
        drawCheckBoxIndicator(option, painter);
        break;
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

QRect CenteredBoxStyle::subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    const QRect baseRes = QProxyStyle::subElementRect(element, option, widget);
    QRect retval = baseRes;

    const QStyleOptionViewItem* const itemOpt = qstyleoption_cast<const QStyleOptionViewItem*>(option);


    if (itemOpt) {

        QVariant alignData = itemOpt->index.data(CheckAlignmentRole);

        if (alignData.isNull()) alignData = Qt::AlignHCenter;

        if (element == SE_ItemViewItemCheckIndicator) {

            const int alignFlag = alignData.toInt();
            int x=0;

            if (alignFlag & Qt::AlignLeft) {
                x = option->rect.x();
            }
            else if (alignFlag & Qt::AlignRight){
                x= option->rect.x() + option->rect.width() - (baseRes.x()-option->rect.x())-baseRes.width();
            }
            else if (alignFlag & Qt::AlignHCenter){
                x = option->rect.x() + (option->rect.width()/2) - (baseRes.width()/2);
            }

            retval = QRect( QPoint(x, baseRes.y()), baseRes.size() + QSize(2,2));

        } else if (element == SE_ItemViewItemFocusRect) {
            retval = option->rect;
        }


    }

    return retval;
}

void CenteredBoxStyle::drawCheckBoxIndicator(const QStyleOption *option, QPainter *painter) const
{       

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (option->state & State_MouseOver) {
        painter->setBrush(option->palette.alternateBase());
    } else {
        painter->setBrush(option->palette.base());
    }

    if (lpixmapCheckBoxOff.isNull()) initPixMaps(option);

    painter->drawPixmap(lpixmapCheckBoxOff.rect().translated(option->rect.topLeft()), lpixmapCheckBoxOff);

    // si la checkbox est on ou en tri state
    if (option->state & (State_On | State_NoChange)) {

        if (!(option->state & State_Enabled)) {
            // la checkbox n'est pas enabled
            painter->drawPixmap(lpixmapCheckBoxOff.rect().translated(option->rect.topLeft()), lpixmapCheckBoxOff);
        } else if (option->state & State_NoChange) {
            // La checkbox est en tri state
            painter->drawPixmap(lpixmapCheckBoxTriState.rect().translated(option->rect.topLeft()), lpixmapCheckBoxTriState);
        } else {
            // Si on arrive ici alors la checkbox est on
            painter->drawPixmap(lpixmapCheckBoxOn.rect().translated(option->rect.topLeft()), lpixmapCheckBoxOn);
        }

    }
    painter->restore();
}

void CenteredBoxStyle::initPixMaps(const QStyleOption *option) const
{
    QPixmap pixmap;
    QSize sz = option->rect.size() + QSize(2,2);

    pixmap.load(":/icones/checkbox_empty.png");
    lpixmapCheckBoxOff = pixmap.scaled(sz,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmap.load(":/icones/checkbox_full.png");
    lpixmapCheckBoxOn = pixmap.scaled(sz,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmap.load(":/icones/checkbox_tri_state.png");
    lpixmapCheckBoxTriState = pixmap.scaled(sz,Qt::KeepAspectRatio,Qt::SmoothTransformation);
}

