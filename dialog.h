#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QValidator>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

//#define btnX 50
//#define btnY 35
//#define btnXesp 5
//#define btnYesp 5

#define btnX 150
#define btnY 105
#define btnXesp 15
#define btnYesp 15

#define btnX1 180
#define btnY1 105
#define btnXesp1 15
#define btnYesp1 15

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);

    ~Dialog();
    int getTypekeyboar() const;
    void setTypekeyboar(int value);

    int getShift() const;
    void setShift(int value);

    QString getDatos() const;
    void setDatos(const QString &value);
    void validador(int index, int bottom, int top,int dec);
    void maskarator(int index,QString mask);
    void set_etiqueta(QString lb);

    int nivel = 0;

    bool getIsvalidator() const;
    void setIsvalidator(bool value);

    int getVal_index() const;
    void setVal_index(int value);

    int getNivel() const;
    void setNivel(int value);

    void conexiones_alfa();
    void conexiones_num();
    void desconexiones_alfa();
    void desconexiones_num();
    void teclado_alfanum();
    void teclado_num();
    void mostrar_tipo_teclado(int tipo);

public slots:
    void btn_setText();
    void btn_setNum();
    void btn_char();
    void btn_SHIFT();
    void btn_ENTER();
    void btn_num();
    void btn_exit();
    void btn_SPACE();
    void btn_bks();
    void btn_left();
    void btn_right();
    void data_out();
    void dobleclick();
private slots:

private:
    Ui::Dialog *ui;
    QPushButton *btn[38];
    QGroupBox *grupo;
    QLineEdit *line;
    int typekeyboar = 0;
    int shift = 0;
    QString datos;
    QString rango;
    QFont Fontstyle;
    QString  mask;
    QLabel *label;
    QLabel *debug;
    bool isvalidator = true;
    int val_index;
    QValidator *v_int;
    QValidator *v_dou;
};

#endif // DIALOG_H
