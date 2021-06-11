#ifndef CALE_H
#define CALE_H

#include <QDialog>
#include <QDate>

namespace Ui {
class Cale;
}

class Cale : public QDialog
{
    Q_OBJECT

public:
    explicit Cale(QWidget *parent = nullptr);
    ~Cale();
    QDate Dato;
    QDate Fecha;



private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::Cale *ui;


};

#endif // CALE_H
