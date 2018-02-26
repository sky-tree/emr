#include "custompushbutton.h"
#include <QPushButton>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QTransform>
#include <QResizeEvent>
 
#define EXCUTEUPDATE(param)\
    if (param) update();

CustomPushButton::CustomPushButton(QSize &btnSize, QMargins & margins, QColor& borderColor, QColor &contentColor, int pointsNum, int contentSize) : QPushButton(NULL)
, margins(margins), borderColor(borderColor), contentColor(contentColor), pointsNum(pointsNum)
, contentSize(contentSize)
, pointsArray(NULL), path(NULL)
{
    this->resize(btnSize.width(), btnSize.height());
}

CustomPushButton::~CustomPushButton()
{
    delete path;
    delete pointsArray;
}


CustomPushButton *CustomPushButton::createSubButton(QWidget *parent, QSize &btnSize, QMargins &margins, QColor &borderColor, QColor &contentColor)
{
    CustomPushButton * subBtn = new CustomPushButton(btnSize, margins, borderColor, contentColor, 4, 0);
    subBtn->createSubButtonPath();
    subBtn->setParent(parent);
    return subBtn;
}

CustomPushButton* CustomPushButton::createPlusButton(QWidget *parent, QSize &btnSize, QMargins &margins, QColor & borderColor, QColor & contentColor, int contentSize)
{
    CustomPushButton * plusBtn = new CustomPushButton(btnSize, margins, borderColor, contentColor, 12, contentSize);
    plusBtn->createPlusButtonPoints();
    plusBtn->setParent(parent);
    return plusBtn;
}

CustomPushButton *CustomPushButton::createForkButton(QWidget *parent, QSize &btnSize, QMargins &margins, QColor &borderColor, QColor &contentColor, int contentSize)
{
    CustomPushButton * forkBtn = new CustomPushButton(btnSize, margins, borderColor, contentColor, 6, contentSize);
    forkBtn->createForkButtonPath();
    forkBtn->setParent(parent);
    return forkBtn;
}

void CustomPushButton::createPlusButtonPoints()
{
    if (pointsArray != NULL)
    {
        delete pointsArray;
        pointsArray = NULL;
    }

    pointsArray = new QPointF[12];
    int halfContentSize = this->contentSize / 2;
    int halfBtnWidth = this->width() / 2;
    int halfBtnHeight = this->height() / 2;
    pointsArray[0] = QPointF(margins.left(), halfBtnHeight - halfContentSize);
    pointsArray[1] = QPointF(halfBtnWidth - halfContentSize, halfBtnHeight - halfContentSize);
    pointsArray[2] = QPointF(halfBtnWidth - halfContentSize, margins.top());
    pointsArray[3] = QPointF(halfBtnWidth + halfContentSize, margins.top());
    pointsArray[4] = QPointF(halfBtnWidth + halfContentSize, halfBtnHeight - halfContentSize);
    pointsArray[5] = QPointF(this->width() - margins.right(), halfBtnHeight - halfContentSize);
    pointsArray[6] = QPointF(this->width() - margins.right(), halfBtnHeight + halfContentSize);
    pointsArray[7] = QPointF(halfBtnWidth + halfContentSize, halfBtnHeight + halfContentSize);
    pointsArray[8] = QPointF(halfBtnWidth + halfContentSize, this->height() - margins.bottom());
    pointsArray[9] = QPointF(halfBtnWidth - halfContentSize, this->height() - margins.bottom());
    pointsArray[10] = QPointF(halfBtnWidth - halfContentSize, halfBtnHeight + halfContentSize);
    pointsArray[11] = QPointF(margins.left(), halfBtnHeight + halfContentSize);
}

void CustomPushButton::createSubButtonPath()
{
    initPainterPath();
    QSize size = this->size();
    path->addRect(QRect(margins.left(), margins.top(), size.width() - margins.left() - margins.right(), size.height() - margins.top() - margins.bottom()));
}

void CustomPushButton::createForkButtonPath()
{
    initPainterPath();
    int height = this->size().height();
    int width = this->size().width();

    QPainterPath firstParallelogramPath;
    firstParallelogramPath.moveTo(margins.left(), margins.top());
    firstParallelogramPath.lineTo(margins.left() + contentSize, margins.top());
    firstParallelogramPath.lineTo(width - margins.right(), height - margins.bottom());
    firstParallelogramPath.lineTo(width - margins.right() - contentSize, height - margins.bottom());
    firstParallelogramPath.closeSubpath();
    QPainterPath secondParallelogramPath;
    secondParallelogramPath.moveTo(margins.left(), height - margins.bottom());
    secondParallelogramPath.lineTo(width - margins.right() - contentSize, margins.top());
    secondParallelogramPath.lineTo(width - margins.right(), margins.top());
    secondParallelogramPath.lineTo(margins.left() + contentSize, height - margins.bottom());
    secondParallelogramPath.closeSubpath();

    QPainterPath combinePath;
    combinePath.setFillRule(Qt::WindingFill);
    combinePath.addPath(firstParallelogramPath);
    combinePath.addPath(secondParallelogramPath);
    path->addPath(combinePath.simplified());
}

void CustomPushButton::setBorderColor(const QColor  &value, bool isUpdate)
{
    borderColor = value;
    EXCUTEUPDATE(isUpdate)
}
void CustomPushButton::setContentColor(const QColor &value, bool isUpdate)
{
    contentColor = value;
    EXCUTEUPDATE(isUpdate)
}

void CustomPushButton::initPainterPath()
{
    if (path != NULL)
    {
        delete path;
        path = NULL;
    }

    path = new QPainterPath();
}

//void CustomPushButton::resizeEvent(QResizeEvent * event)
//{
//    QWidget::resizeEvent(event);
//    //这个类只提供正方形比例的尺寸变换
//    float scale = 1.0f;
//    if (event->oldSize().width() != -1)
//    {
//        float oldWidth = event->oldSize().width();
//        float newWidth = event->size().width();
//        scale = oldWidth / newWidth;
//        margins = QMargins(margins.left() * scale, margins.top() * scale, margins.right() * scale, margins.bottom() * scale);
//        contentSize = contentSize * scale;
//    }
//
//    switch (pointsNum) {
//    case 4:
//    {
//        createSubButtonPath();
//    }
//    case 6:
//    {
//        createForkButtonPath();
//    }
//    case 12:
//    {
//        createPlusButtonPoints();
//    }
//        break;
//    default:
//        break;
//    }
//}

void CustomPushButton::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawControl(QStyle::CE_PushButton, option);

    painter.translate(0, 0);
    painter.setPen(QPen(borderColor,
                            1, // 边框宽 1 像素
                            Qt::SolidLine,
                            Qt::SquareCap,
                            Qt::MiterJoin));
    painter.setBrush(contentColor);

    switch (pointsNum) {
    case 12:
    {
        painter.drawPolygon(pointsArray, pointsNum);
    }
        break;
    case 4:
    case 6:
    {
        painter.drawPath(*path);
    }
    default:
        break;
    }
}






