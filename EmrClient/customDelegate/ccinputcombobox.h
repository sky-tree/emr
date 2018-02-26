/*******************************************************************
Copyright:

Created by: zhangrong

Description:封装实现带提示的单行文本输入框
******************************************************************/
 
#ifndef CCINPUTCOMBOBOX_H
#define CCINPUTCOMBOBOX_H

#include <QWidget>

#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class QLineEdit;
class QListWidget;
class QListWidgetItem;

class CCInputComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit CCInputComboBox(QWidget *parent);
    ~CCInputComboBox();

    void resize(int w, int h);
    void resize(const QSize &);

    bool eventFilter(QObject * o, QEvent * e);

    void setMaxListHeight(int);
    void setData(std::vector<QString> &);
protected:
    void showPopup();

signals:

public slots:
    void onTextChanged(const QString & );
    void onListItemSelectionChanged();
    void onListItemConfirmed(QListWidgetItem *);
    void onListCurrentItemChanged(QListWidgetItem*,QListWidgetItem*);
private:
    QListWidget * mListWidget;
    QLineEdit     * mLineEdit;

    QSize           mMaxListSize;

    std::vector<QString>  dataVec;
    bool eatFocusOut;
    int currentIndex;

    bool state;
    bool state1;
};

#endif // CCINPUTCOMBOBOX_H
