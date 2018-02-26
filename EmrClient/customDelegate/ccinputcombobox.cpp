#include "ccinputcombobox.h"
 
#include <QLineEdit>
#include <QListWidget>
#include <QEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QListWidget>
#define DEFAULT_RATIO  0.25
#define MAX_VISIBLE_COUNT 50

/**********************************************************************************************************
*设置Widget的属性为Qt::Popup可以使当前的wiget成为优先级最高的获取事件的控件
*通过截取所有事件，再按照自己的需要继续处理和分发
* 参考QFontComboBox
**********************************************************************************************************/

CCInputComboBox::CCInputComboBox(QWidget *parent) : QWidget(parent)
{
    eatFocusOut = true;
    state = false;
    state1 = false;

    mLineEdit = new QLineEdit(this);
    mListWidget = new QListWidget();
    mListWidget->setParent(0, Qt::Popup);

    mLineEdit->installEventFilter(this);
    mListWidget->installEventFilter(this);

    connect(mLineEdit,SIGNAL(textChanged (const QString & )),this,SLOT(onTextChanged(const QString & )));
    connect(mListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onListItemSelectionChanged()));
    connect(mListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onListItemConfirmed(QListWidgetItem *)));
    connect(mListWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(onListCurrentItemChanged(QListWidgetItem*,QListWidgetItem*)));


    //获取可用桌面大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    mMaxListSize.setWidth(mLineEdit->width());
    mMaxListSize.setHeight(desktopWidget->availableGeometry().height() * DEFAULT_RATIO);
}

CCInputComboBox::~CCInputComboBox()
{
    delete mListWidget;
}

void CCInputComboBox::resize(int w, int h)
{
	QWidget::resize(w, h);
	mLineEdit->resize(w, h);
}

void CCInputComboBox::resize(const QSize & size)
{
	QWidget::resize(size);
	mLineEdit->resize(size);
}

bool CCInputComboBox::eventFilter(QObject *o, QEvent *e)
{

    if (o == mLineEdit && e->type() == QEvent::MouseButtonPress)
    {
        onTextChanged(mLineEdit->text());
        return true;
    }

    if (eatFocusOut && o == mLineEdit && e->type() == QEvent::FocusOut) {
        if (mListWidget && mListWidget->isVisible())
            return true;
    }

    if (o != mListWidget)
        return QObject::eventFilter(o, e);

    switch (e->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);

//        QModelIndex curIndex = mListWidget->currentIndex();
        const int key = ke->key();
        int iIndex = mListWidget->currentRow ();
        if ((key == Qt::Key_Up || key == Qt::Key_Down) && mListWidget->count() > 0)
        {
            if(key == Qt::Key_Up)
            {
                iIndex--;
                if(iIndex < 0) iIndex = 0;
                QListWidgetItem *pItem = mListWidget->item(iIndex);
                mListWidget->setCurrentItem(pItem);
            }
            else if (key == Qt::Key_Down)
            {
                iIndex++;
                if(iIndex >= mListWidget->count()) iIndex = mListWidget->count() - 1;
                QListWidgetItem *pItem = mListWidget->item(iIndex);
                mListWidget->setCurrentItem(pItem);
            }

            currentIndex = iIndex;

            return true;
        }

        // Handle popup navigation keys. These are hardcoded because up/down might make the
        // widget do something else (lineedit cursor moves to home/end on mac, for instance)
        switch (key) {
        case Qt::Key_End:
        case Qt::Key_Home:
            if (ke->modifiers() & Qt::ControlModifier)
                return false;
            break;
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            return false;
        }

        // Send the event to the widget. If the widget accepted the event, do nothing
        // If the widget did not accept the event, provide a default implementation
        eatFocusOut = false;
        (static_cast<QObject *>(mLineEdit))->event(ke);
        eatFocusOut = true;
        if (!mLineEdit || e->isAccepted() || !mListWidget->isVisible())
        {
            // widget lost focus, hide the popup
            if (mLineEdit && (!mLineEdit->hasFocus()
                              ))
                mListWidget->setVisible(false);
            if (e->isAccepted())
                return true;
        }

        // default implementation for keys not handled by the widget when popup is open
        switch (key)
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:
            mListWidget->setVisible(false);
//            onListItemConfirmed(mListWidget->currentItem());
            break;

        case Qt::Key_F4:
            if (ke->modifiers() & Qt::AltModifier)
                mListWidget->setVisible(false);
            break;

        case Qt::Key_Backtab:
        case Qt::Key_Escape:
            mListWidget->setVisible(false);
            break;

        default:
            break;
        }

        return true;
    }
    case QEvent::MouseButtonPress:
    {
        QMouseEvent * event = static_cast<QMouseEvent *>(e);

        if (event == NULL) return true;

        QPoint editPoint = mLineEdit->mapToGlobal(mLineEdit->pos());
        QPoint eventPoint = mListWidget->mapToGlobal(event->pos());
        QRect textRect=QRect(editPoint, mLineEdit->size());

        if (/*!mListWidget->underMouse() && */ textRect.contains(eventPoint)==false)
        {
            mListWidget->setVisible(false);
            return true;
        }
    }
        return false;

    case QEvent::InputMethod:
    case QEvent::ShortcutOverride:
        QApplication::sendEvent(mLineEdit, e);
        break;

    default:
        return false;
    }
    return false;
}

void CCInputComboBox::setData(std::vector<QString> & pData)
{
    dataVec.clear();
    dataVec = pData;
}

void CCInputComboBox::setMaxListHeight(int h)
{
	mMaxListSize.setHeight(h);
}

void CCInputComboBox::showPopup()
{
    if (mListWidget->count() == 0) return;

    QPoint pos;
    int w, h;

    QRect rectItem = mListWidget->visualItemRect(mListWidget->item(0));
    int tem = mListWidget->count() * rectItem.height();
    bool isLarge = (tem >= mMaxListSize.height()) ? true : false;
    h = (isLarge) ? mMaxListSize.height() : tem;

    mListWidget->setAutoScroll(isLarge);
    mListWidget->setVerticalScrollBarPolicy((isLarge) ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);

    pos = mLineEdit->mapToGlobal(QPoint(0, mLineEdit->height() - 2));
    w = mLineEdit->width();
    mListWidget->setGeometry(pos.x(), pos.y() + 1, w, h + 3);

    if (!mListWidget->isVisible())
    {
        mListWidget->setVisible(true);
        mLineEdit->setFocus();
    }
}

void CCInputComboBox::onTextChanged(const QString & strText)
{
    if (dataVec.size() == 0) return;

    if (strText == "")
    {
        mListWidget->setVisible(false);
        return;
    }

        if (state == true) return;

        state1 = true;
    mListWidget->clear();
        state1 = false;

    for(unsigned int i = 0;i < dataVec.size(); ++i)
    {

        if (dataVec[i].indexOf(strText) != -1)
        {
            mListWidget->addItem(dataVec[i]);
        }
    }

    if (mListWidget->count() > 0)
    {
        showPopup();
    }
    else
    {
        mListWidget->setVisible(false);
    }
}

void CCInputComboBox::onListItemSelectionChanged()
{
    state = true;
    if (state1 == false){
        mLineEdit->setText(mListWidget->currentItem()->text());
        mLineEdit->setFocus();
    }
    state = false;
}

void CCInputComboBox::onListItemConfirmed(QListWidgetItem * item)
{
    state = true;
    if (state1 == false){
        mLineEdit->setText(item->text());
        mLineEdit->setFocus();
    }
    state = false;

    mListWidget->setVisible(false);
}

void CCInputComboBox::onListCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous != NULL)
    {
        previous->setBackgroundColor(QColor(0, 0, 0, 0));
    }

    if (current != NULL)
    {
        current->setBackgroundColor(QColor(0, 0, 120, 100));
    }
}

