#ifndef SELECT_FECHAS_H
#define SELECT_FECHAS_H

#include <QDialog>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class Select_fechas;
}

class Select_fechas : public QDialog
{
    Q_OBJECT

public:
    QTimer *timer;
    int H_horas;
    int H_minutos;
    int D_horas;
    int D_minutos;

    explicit Select_fechas(QWidget *parent = nullptr);
    ~Select_fechas();

    QString getFecha_desde() const;
    void setFecha_desde(const QString &value);

    QString getFecha_hasta() const;
    void setFecha_hasta(const QString &value);

    bool getDhmenos() const;
    void setDhmenos(bool value);

    bool getDhmas() const;
    void setDhmas(bool value);

    bool getDMmenos() const;
    void setDMmenos(bool value);

    bool getDMmas() const;
    void setDMmas(bool value);

    bool getHhmenos() const;
    void setHhmenos(bool value);

    bool getHhmas() const;
    void setHhmas(bool value);

    bool getHMmenos() const;
    void setHMmenos(bool value);

    bool getHMmas() const;
    void setHMmas(bool value);

private slots:
    void on_Regresar_clicked();

    void on_Btn_Guardar_clicked();

    void on_Btn_D_menosH_clicked();

    void on_Btn_D_masH_clicked();

    void on_Btn_D_menosH_pressed();

    void on_Btn_D_masH_pressed();

    void on_Btn_D_menosH_released();

    void ciclo_Dhmenos();
    void ciclo_Dhmas();
    void ciclo_DMmenos();
    void ciclo_DMmas();
    void ciclo_Hhmenos();
    void ciclo_Hhmas();
    void ciclo_HMmenos();
    void ciclo_HMmas();

    void on_Btn_D_masH_released();

    void on_Btn_D_menosM_clicked();

    void on_Btn_D_menosM_pressed();

    void on_Btn_D_menosM_released();

    void on_Btn_D_masM_clicked();

    void on_Btn_D_masM_pressed();

    void on_Btn_D_masM_released();

    void on_Btn_H_menosH_clicked();

    void on_Btn_H_menosH_pressed();

    void on_Btn_H_menosH_released();

    void on_Btn_H_masH_clicked();

    void on_Btn_H_masH_pressed();

    void on_Btn_H_masH_released();

    void on_Btn_H_menosM_clicked();

    void on_Btn_H_menosM_pressed();

    void on_Btn_H_menosM_released();

    void on_Btn_H_masM_clicked();

    void on_Btn_H_masM_pressed();

    void on_Btn_H_masM_released();

private:
    Ui::Select_fechas *ui;
    bool Dhmenos = false;
    bool Dhmas = false;
    bool DMmenos = false;
    bool DMmas = false;
    bool Hhmenos = false;
    bool Hhmas = false;
    bool HMmenos = false;
    bool HMmas = false;
    QString fecha_desde;
    QString fecha_hasta;
};

#endif // SELECT_FECHAS_H
