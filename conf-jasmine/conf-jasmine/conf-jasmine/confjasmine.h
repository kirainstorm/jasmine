#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_confjasmine.h"

//! 建议用5000以上唯一的标识
const QEvent::Type custom_event_0 = (QEvent::Type)5001; 
const QEvent::Type custom_event_1 = (QEvent::Type)5002; 

class confjasmine : public QMainWindow
{
    Q_OBJECT

public:
    confjasmine(QWidget *parent = Q_NULLPTR);

private:
    Ui::confjasmineClass ui;


private:
	void customEvent(QEvent *e); //! 重载QObject的续虚函数



private slots:
	void pushButton_slots();


};
