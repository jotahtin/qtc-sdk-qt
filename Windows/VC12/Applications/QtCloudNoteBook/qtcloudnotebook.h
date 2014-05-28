#ifndef QTCLOUDNOTEBOOK_H
#define QTCLOUDNOTEBOOK_H

#include <QtWidgets/QMainWindow>
#include "ui_qtcloudnotebook.h"

class QtCloudNoteBook : public QMainWindow
{
	Q_OBJECT

public:
	QtCloudNoteBook(QWidget *parent = 0);
	~QtCloudNoteBook();

private:
	Ui::QtCloudNoteBookClass ui;
};

#endif // QTCLOUDNOTEBOOK_H
