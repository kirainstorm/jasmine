#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_confjasmine.h"

//! ������5000����Ψһ�ı�ʶ
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
	void customEvent(QEvent *e); //! ����QObject�����麯��



private slots:
	void pushButton_slots();


};
