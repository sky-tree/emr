#pragma once
#include "qtreewidget.h"
#ifndef TREEWIDGET_H
#define TREEWIDGET_H
 
#include<QTreeWidget>
#include<QMouseEvent>
#include<QDragEnterEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include<QApplication>
#include<QMimeData>
#include<QDrag>
#include<QHBoxLayout>
#include<QPoint>
#include<QTreeWidgetItem>
#include<QTreeWidgetItemIterator>
class treeWidget :public QTreeWidget
{
	Q_OBJECT
public:
	treeWidget(QWidget *parent = 0);
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	// void dragEnterEvent(QDragEnterEvent *event);
	//void dragMoveEvent(QDragMoveEvent *event);
	//void dropEvent(QDropEvent *event);
private:
	QPoint m_startDragPoint;
	void performDrag();
};

#endif // TREEWIDGET_H
