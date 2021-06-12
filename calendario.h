#ifndef CALENDARIO_H
#define CALENDARIO_H

#include <QWidget>

namespace Ui {
class Calendario;
}

class Calendario : public QWidget
{
    Q_OBJECT

public:
    explicit Calendario(QWidget *parent = nullptr);
    ~Calendario();

private slots:
    void on_pushButton_clicked();

    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::Calendario *ui;
};

#endif // CALENDARIO_H
