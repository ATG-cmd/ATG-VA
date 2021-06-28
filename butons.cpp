#include "butons.h"
#include "ui_butons.h"

Butons::Butons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Butons)
{
    ui->setupUi(this);
    Timer1 = new QTimer();
}

Butons::~Butons()
{
    delete ui;
}

void Butons::on_btn_masmin_clicked()
{
    ui->LineMin->clear();
       if(minutos == 59)minutos = 0;
       else minutos ++;
     ui->LineMin->setText(QString::number(minutos));
     LineMinText =QString::number(minutos);
}

void Butons::on_btn_menosmin_clicked()
{
    ui->LineMin->clear();
       if(minutos == 0)minutos = 59;
       else minutos --;
     ui->LineMin->setText(QString::number(minutos));
     LineMinText =QString::number(minutos);
}

void Butons::on_btn_masmin_pressed()
{ connect(Timer1,&QTimer::timeout,this,&Butons::on_btn_masmin_clicked);Timer1->start(250);}

void Butons::on_btn_menosmin_released()
{ disconnect(Timer1,&QTimer::timeout,this,&Butons::on_btn_menosmin_clicked);Timer1->start(250);}

void Butons::on_btn_masmin_released()
{ disconnect(Timer1,&QTimer::timeout,this,&Butons::on_btn_masmin_clicked); Timer1->stop();}

void Butons::on_btn_menosmin_pressed()
{ connect(Timer1,&QTimer::timeout,this,&Butons::on_btn_menosmin_clicked);Timer1->start(250);   }

void Butons::on_btn_menosHor_clicked()
{
    ui->LineEditHor->clear();
       if(Horas == 0)Horas = 23;
       else Horas --;
     ui->LineEditHor->setText(QString::number(Horas));
     LineHorText =QString::number(Horas);
}

void Butons::on_btn_masHor_clicked()
{
    ui->LineEditHor->clear();
       if(Horas == 23)Horas = 0;
       else Horas ++;
     ui->LineEditHor->setText(QString::number(Horas));
     LineHorText =QString::number(Horas);
}

void Butons::on_btn_menosHor_pressed()
{ connect(Timer1,&QTimer::timeout,this,&Butons::on_btn_menosHor_clicked);Timer1->start(250);}

void Butons::on_btn_menosHor_released()
{ disconnect(Timer1,&QTimer::timeout,this,&Butons::on_btn_menosHor_clicked); Timer1->stop(); }

void Butons::on_btn_masHor_pressed()
{ connect(Timer1,&QTimer::timeout,this,&Butons::on_btn_masHor_clicked);Timer1->start(250);  }

void Butons::on_btn_masHor_released()
{ disconnect(Timer1,&QTimer::timeout,this,&Butons::on_btn_masHor_clicked); Timer1->stop(); }

QString Butons::getLineHorText() const
{
   if(LineHorText.toInt() <10)
    return "0" +LineHorText;
   else
        return LineHorText;

}

void Butons::setLineHorText(const QString &value)
{
    LineHorText = value;
}

QString Butons::getLineMinText() const
{
    if(LineMinText.toInt() <10)
     return "0" +LineMinText;
    else
         return LineMinText;
}

void Butons::setLineMinText(const QString &value)
{
    LineMinText = value;
}
bool Butons::getIsHabilitado() const
{
    return isHabilitado;
}

void Butons::setIsHabilitado(bool value)
{
    isHabilitado = value;
}

void Butons::on_checkBox_stateChanged(int arg1)
{
    if (arg1== 0)
       {
           btn_Habilitado(ui->btn_menosmin,false);
           ui->LineMin->setEnabled(false);
           btn_Habilitado(ui->btn_masmin,false);
           btn_Habilitado(ui->btn_menosHor,false);
           ui->LineEditHor->setEnabled(false);
            btn_Habilitado(ui->btn_masHor,false);
            isHabilitado = false;
       }
       else {

        btn_Habilitado(ui->btn_menosmin,true);
        ui->LineMin->setEnabled(true);
        btn_Habilitado(ui->btn_masmin,true);
        btn_Habilitado(ui->btn_menosHor,true);
        ui->LineEditHor->setEnabled(true);
         btn_Habilitado(ui->btn_masHor,true);
         isHabilitado= true;
       }

}

bool Butons::getIsSelect() const
{
    return isSelect;
}

void Butons::setIsSelect(bool value)
{
    isSelect = value;
    if (value)
       {
           btn_Habilitado(ui->btn_menosmin,false);
           ui->LineMin->setEnabled(false);
           btn_Habilitado(ui->btn_masmin,false);
           btn_Habilitado(ui->btn_menosHor,false);
           ui->LineEditHor->setEnabled(false);
            btn_Habilitado(ui->btn_masHor,false);
            isHabilitado = false;
             ui->checkBox->hide();
       }
       else {

        btn_Habilitado(ui->btn_menosmin,true);
        ui->LineMin->setEnabled(true);
        btn_Habilitado(ui->btn_masmin,true);
        btn_Habilitado(ui->btn_menosHor,true);
        ui->LineEditHor->setEnabled(true);
         btn_Habilitado(ui->btn_masHor,true);
         isHabilitado= true;

         ui->checkBox->hide();
       }

}

bool Butons::getChekBoxishabilitado() const
{
    return ChekBoxishabilitado;
}

void Butons::setChekBoxishabilitado(bool value)
{
    ChekBoxishabilitado = value;
    ui->checkBox->setEnabled(value);
}

void Butons::btn_Habilitado(QPushButton *Boton,bool hab )
{
    Boton->setStyleSheet(hab ? BotonHabilitado :BotonDeshabilitado);
    Boton->setEnabled(hab);
}

