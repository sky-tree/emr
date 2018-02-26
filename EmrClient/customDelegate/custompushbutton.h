#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>
#include <QColor>
 
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

class QPainterPath;

class CustomPushButton : public QPushButton
{
    Q_OBJECT
public:
    CustomPushButton(QSize & btnSize, QMargins &margins,  QColor &  borderColor, QColor &contentColor, int pointsNum, int contentSize);

    ~CustomPushButton();

    void paintEvent(QPaintEvent *);
    void setMargins(int);
    int getMargins() const;

    static CustomPushButton *createSubButton(QWidget *parent, QSize &btnSize, QMargins &margins, QColor &borderColor, QColor &contentColor);
    static CustomPushButton *createPlusButton(QWidget *parent, QSize &btnSize, QMargins &margins, QColor &borderColor, QColor &contentColor, int contentSize);
     static CustomPushButton *createForkButton(QWidget *parent, QSize &btnSize, QMargins &margins, QColor &borderColor, QColor &contentColor, int contentSize);
    void createPlusButtonPoints();
    void createSubButtonPath();
    void createForkButtonPath();


    void setBorderColor(const QColor  &value, bool isUpdate = true);
    void setContentColor(const QColor &value, bool isUpdate = true);

//    void resize(int x, int y);
private:
    CustomPushButton& operator=(const CustomPushButton &);
    CustomPushButton(const CustomPushButton &);

    void initPainterPath();

protected:
    //virtual void resizeEvent(QResizeEvent *);
signals:

public slots:

private:
    QMargins margins; //+传的是以+为正方形的距离，×传的是最左上角的点和最右下角的点的距离
    QColor borderColor;
    QColor contentColor;

    int pointsNum;
    int contentSize;                         //绘制图形的粗细程度
    QPointF * pointsArray;
    QPainterPath *path;
};

#endif // CUSTOMPUSHBUTTON_H
