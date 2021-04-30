#ifndef SELECT_FECHAS_H
#define SELECT_FECHAS_H

#include <QDialog>

namespace Ui {
class Select_fechas;
}

class Select_fechas : public QDialog
{
    Q_OBJECT

public:
    explicit Select_fechas(QWidget *parent = nullptr);
    ~Select_fechas();

    QString getFecha_desde() const;
    void setFecha_desde(const QString &value);

    QString getFecha_hasta() const;
    void setFecha_hasta(const QString &value);

private slots:
    void on_Regresar_clicked();

    void on_Btn_Guardar_clicked();

private:
    Ui::Select_fechas *ui;
    QString fecha_desde;
    QString fecha_hasta;
};

#endif // SELECT_FECHAS_H
