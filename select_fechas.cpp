#include "select_fechas.h"
#include "ui_select_fechas.h"


Select_fechas::Select_fechas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_fechas)
{
    ui->setupUi(this);
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_Dhmenos()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_Dhmas()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_DMmenos()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_DMmas()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_Hhmenos()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_Hhmas()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_HMmenos()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ciclo_HMmas()));
    timer->start(250);

   ui->Line_D_Fecha->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
   ui->Line_D_horas->setText("00");
   ui->Line_D_minutos->setText("00");
   ui->Line_H_Fecha->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
   ui->Line_H_horas->setText(QDateTime::currentDateTime().toString("HH"));
   ui->Line_H_minutos->setText(QDateTime::currentDateTime().toString("mm"));
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
    cadena1.append(":");
    cadena1.append(QDateTime::currentDateTime().toString("ss"));

    cadena2.append(ui->Line_D_Fecha->text());
    cadena2.append(" ");
    cadena2.append(ui->Line_D_horas->text());
    cadena2.append(":");
    cadena2.append(ui->Line_D_minutos->text());
    cadena2.append(":");
    cadena2.append("00");

    setFecha_hasta(cadena1);
    setFecha_desde(cadena2);
    accept();
}

void Select_fechas::on_Btn_D_menosH_clicked()
{
    D_horas = ui->Line_D_horas->text().toInt();
    if(D_horas <= 0)
    {
     D_horas = 23;
    }else{
     D_horas--;
    }

    ui->Line_D_horas->setText(QString::number(D_horas));
}

void Select_fechas::on_Btn_D_masH_clicked()
{
    D_horas = ui->Line_D_horas->text().toInt();
    if(D_horas >= 23)
    {
     D_horas = 0;
    }else{
     D_horas++;
    }

    ui->Line_D_horas->setText(QString::number(D_horas));
}



void Select_fechas::ciclo_Dhmenos()
{   bool flag = getDhmenos();
    if(flag == true){
        D_horas = ui->Line_D_horas->text().toInt();
        if(D_horas <= 0)
        {
            D_horas = 23;
        }else{
            D_horas--;
        }
        ui->Line_D_horas->setText(QString::number(D_horas));
    }
}

void Select_fechas::ciclo_Dhmas()
{
    bool flag = getDhmas();
        if(flag == true){
            D_horas = ui->Line_D_horas->text().toInt();
            if(D_horas >= 23)
            {
                D_horas = 0;
            }else{
                D_horas++;
            }
            ui->Line_D_horas->setText(QString::number(D_horas));
        }
}

void Select_fechas::ciclo_DMmenos()
{
    bool flag = getDMmenos();
       if(flag == true){
           D_minutos = ui->Line_D_minutos->text().toInt();
           if(D_minutos <= 0)
           {
               D_minutos = 59;
           }else{
               D_minutos--;
           }
           ui->Line_D_minutos->setText(QString::number(D_minutos));
       }
}

void Select_fechas::ciclo_DMmas()
{
    bool flag = getDMmas();
        if(flag == true){
            D_minutos = ui->Line_D_minutos->text().toInt();
            if(D_minutos >= 59)
            {
                D_minutos = 0;
            }else{
                D_minutos++;
            }
            ui->Line_D_minutos->setText(QString::number(D_minutos));
        }
}

void Select_fechas::ciclo_Hhmenos()
{
    bool flag = getHhmenos();
       if(flag == true){
           H_horas = ui->Line_H_horas->text().toInt();
           if(H_horas <= 0)
           {
               H_horas = 23;
           }else{
               H_horas--;
           }
           ui->Line_H_horas->setText(QString::number(H_horas));
       }
}

void Select_fechas::ciclo_Hhmas()
{
    bool flag = getHhmas();
        if(flag == true){
            H_horas = ui->Line_H_horas->text().toInt();
            if(H_horas >= 23)
            {
                H_horas = 0;
            }else{
                H_horas++;
            }
            ui->Line_H_horas->setText(QString::number(H_horas));
        }
}

void Select_fechas::ciclo_HMmenos()
{
    bool flag = getHMmenos();
       if(flag == true){
           H_minutos = ui->Line_H_minutos->text().toInt();
           if(H_minutos <= 0)
           {
               H_minutos = 59;
           }else{
               H_minutos--;
           }
           ui->Line_H_minutos->setText(QString::number(H_minutos));
       }
}

void Select_fechas::ciclo_HMmas()
{
    bool flag = getHMmas();
        if(flag == true){
           H_minutos = ui->Line_H_minutos->text().toInt();
            if(H_minutos >= 59)
            {
                H_minutos = 0;
            }else{
                H_minutos++;
            }
            ui->Line_H_minutos->setText(QString::number(H_minutos));
        }
}

bool Select_fechas::getHMmas() const
{
    return HMmas;
}

void Select_fechas::setHMmas(bool value)
{
    HMmas = value;
}

bool Select_fechas::getHMmenos() const
{
    return HMmenos;
}

void Select_fechas::setHMmenos(bool value)
{
    HMmenos = value;
}

bool Select_fechas::getHhmas() const
{
    return Hhmas;
}

void Select_fechas::setHhmas(bool value)
{
    Hhmas = value;
}

bool Select_fechas::getHhmenos() const
{
    return Hhmenos;
}

void Select_fechas::setHhmenos(bool value)
{
    Hhmenos = value;
}

bool Select_fechas::getDMmas() const
{
    return DMmas;
}

void Select_fechas::setDMmas(bool value)
{
    DMmas = value;
}

bool Select_fechas::getDMmenos() const
{
    return DMmenos;
}

void Select_fechas::setDMmenos(bool value)
{
    DMmenos = value;
}

bool Select_fechas::getDhmas() const
{
    return Dhmas;
}

void Select_fechas::setDhmas(bool value)
{
    Dhmas = value;
}
bool Select_fechas::getDhmenos() const
{
    return Dhmenos;
}

void Select_fechas::setDhmenos(bool value)
{
    Dhmenos = value;
}

void Select_fechas::on_Btn_D_menosH_pressed()
{
    setDhmenos(true);

}
void Select_fechas::on_Btn_D_masH_pressed()
{
    setDhmas(true);
}

void Select_fechas::on_Btn_D_menosH_released()
{
    setDhmenos(false);
}

void Select_fechas::on_Btn_D_masH_released()
{
    setDhmas(false);
}

void Select_fechas::on_Btn_D_menosM_clicked()
{
    D_minutos = ui->Line_D_minutos->text().toInt();
    if(D_minutos <= 0)
    {
     D_minutos = 59;
    }else{
     D_minutos--;
    }

    ui->Line_D_minutos->setText(QString::number(D_minutos));
}

void Select_fechas::on_Btn_D_menosM_pressed()
{
    setDMmenos(true);
}

void Select_fechas::on_Btn_D_menosM_released()
{
    setDMmenos(false);
}

void Select_fechas::on_Btn_D_masM_clicked()
{
    D_minutos = ui->Line_D_minutos->text().toInt();
    if(D_minutos >= 59)
    {
     D_minutos = 0;
    }else{
     D_minutos++;
    }

    ui->Line_D_minutos->setText(QString::number(D_minutos));
}

void Select_fechas::on_Btn_D_masM_pressed()
{
    setDMmas(true);
}

void Select_fechas::on_Btn_D_masM_released()
{
    setDMmas(false);
}

void Select_fechas::on_Btn_H_menosH_clicked()
{
    H_horas = ui->Line_H_horas->text().toInt();
    if(H_horas <= 0)
    {
     H_horas = 23;
    }else{
     H_horas--;
    }

    ui->Line_H_horas->setText(QString::number(H_horas));
}

void Select_fechas::on_Btn_H_menosH_pressed()
{
    setHhmenos(true);
}

void Select_fechas::on_Btn_H_menosH_released()
{
    setHhmenos(false);
}

void Select_fechas::on_Btn_H_masH_clicked()
{
    H_horas = ui->Line_H_horas->text().toInt();
    if(H_horas >= 23)
    {
     H_horas = 0;
    }else{
     H_horas++;
    }

    ui->Line_H_horas->setText(QString::number(H_horas));
}

void Select_fechas::on_Btn_H_masH_pressed()
{
    setHhmas(true);
}

void Select_fechas::on_Btn_H_masH_released()
{
    setHhmas(false);
}

void Select_fechas::on_Btn_H_menosM_clicked()
{
    H_minutos = ui->Line_H_minutos->text().toInt();
    if(H_minutos <= 0)
    {
     H_minutos = 59;
    }else{
     H_minutos--;
    }

    ui->Line_H_minutos->setText(QString::number(H_minutos));
}

void Select_fechas::on_Btn_H_menosM_pressed()
{
    setHMmenos(true);
}

void Select_fechas::on_Btn_H_menosM_released()
{
    setHMmenos(false);
}

void Select_fechas::on_Btn_H_masM_clicked()
{
    H_minutos = ui->Line_H_minutos->text().toInt();
    if(H_minutos >= 59)
    {
     H_minutos = 0;
    }else{
     H_minutos++;
    }

    ui->Line_H_minutos->setText(QString::number(H_minutos));
}

void Select_fechas::on_Btn_H_masM_pressed()
{
    setHMmas(true);
}

void Select_fechas::on_Btn_H_masM_released()
{
    setHMmas(false);
}
