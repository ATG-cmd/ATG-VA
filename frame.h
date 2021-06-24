#ifndef FRAME_H
#define FRAME_H

#include <QFrame>


namespace Ui {
class Frame;
}


class Frame : public QFrame
{
    Q_OBJECT



public:


    explicit Frame(QWidget *parent = nullptr);
    ~Frame();
      void   mensaje(QString Mensaje, QString Titulo,QString Incidiente);
       void   mensaje(QString Mensaje, QString Titulo,QString Incidiente,bool Botones);
      void mensaje(QString Mensaje, QString Titulo );
      QPixmap Pix;





private slots:
      void on_pushButton_clicked();

private:
    Ui::Frame *ui;
};


#endif // FRAME_H
