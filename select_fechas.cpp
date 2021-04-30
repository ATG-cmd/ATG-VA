#include "select_fechas.h"
#include "ui_select_fechas.h"

Select_fechas::Select_fechas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_fechas)
{
    ui->setupUi(this);
}

Select_fechas::~Select_fechas()
{
    delete ui;
}

QString Select_fechas::getFecha_desde() const
{
    return fecha_desde;
}

void Select_fechas::setFecha_desde(const QString &value)
{
    fecha_desde = value;
}

QString Select_fechas::getFecha_hasta() const
{
    return fecha_hasta;
}

void Select_fechas::setFecha_hasta(const QString &value)
{
    fecha_hasta = value;
}

void Select_fechas::on_Regresar_clicked()
{
    reject();
}

void Select_fechas::on_Btn_Guardar_clicked()
{   QString cadena1 = "";
    QString cadena2 = "";

    cadena1.append(ui->Line_H_Fecha->text());
    cadena1.append(" ");
    cadena1.append(ui->Line_H_horas->text());
    cadena1.append(":");
    cadena1.append(ui->Line_H_minutos->text());
    cadena1.append(":00");

    cadena2.append(ui->Line_D_Fecha->text());
    cadena2.append(" ");
    cadena2.append(ui->Line_D_horas->text());
    cadena2.append(":");
    cadena2.append(ui->Line_D_minutos->text());
    cadena2.append(":00");

    setFecha_hasta(cadena1);
    setFecha_desde(cadena2);
    accept();
}
