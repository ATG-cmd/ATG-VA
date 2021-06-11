#include "cale.h"
#include "ui_cale.h"

Cale::Cale(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cale)
{
    ui->setupUi(this);
}

Cale::~Cale()
{
    delete ui;
}



void Cale::on_pushButton_clicked()
{
    close();
}

void Cale::on_pushButton_2_clicked()
{
   Fecha = Dato;
   close();

}



void Cale::on_calendarWidget_clicked(const QDate &date)
{
         Dato = date;
         ui->label->setText(date.toString("dd/MM/yyyy"));

}


