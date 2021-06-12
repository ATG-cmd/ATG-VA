#include "calendario.h"
#include "ui_calendario.h"

Calendario::Calendario(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calendario)
{
    ui->setupUi(this);
}

Calendario::~Calendario()
{
    delete ui;
}

void Calendario::on_pushButton_clicked()
{
    close();
}

void Calendario::on_calendarWidget_clicked(const QDate &date)
{
   // ui->label->setText(date.toString("dd/MM/yyyy"));
}
