#ifndef BUTONS_H
#define BUTONS_H

#include <QWidget>
#include <QTimer>
#include <QPushButton>

namespace Ui {
class Butons;
}

class Butons : public QWidget
{
    Q_OBJECT

public:
    explicit Butons(QWidget *parent = nullptr);
    ~Butons();

    QString getLineMinText() const;
    void setLineMinText(const QString &value);

    QString getLineHorText() const;
    void setLineHorText(const QString &value);

    void btn_Habilitado(QPushButton *Boton,bool hab );

    bool getIsHabilitado() const;
    void setIsHabilitado(bool value);

    bool getChekBoxishabilitado() const;
    void setChekBoxishabilitado(bool value);

private slots:

    void on_btn_menosmin_clicked();
    void on_btn_masmin_clicked();

    void on_btn_masmin_pressed();

    void on_btn_menosmin_released();

    void on_btn_masmin_released();

    void on_btn_menosmin_pressed();

    void on_btn_menosHor_clicked();

    void on_btn_masHor_clicked();

    void on_btn_menosHor_pressed();

    void on_btn_menosHor_released();

    void on_btn_masHor_pressed();

    void on_btn_masHor_released();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::Butons *ui;
    QTimer * Timer1;
    int minutos = 0;
    int Horas =0;
    QString LineMinText;
    QString LineHorText;
    bool isHabilitado =false;
    bool ChekBoxishabilitado;
};

#endif // BUTONS_H
