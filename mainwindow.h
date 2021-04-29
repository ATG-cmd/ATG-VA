#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tanque.h>
#include <QComboBox>
#include <QDateTime>
#include <sonda.h>
#include <QTimer>
#include <QValidator>
#include <QLineEdit>
#include <QEvent>
#include "dialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPluginLoader>
#include <QSerialPort>
#include <QSerialPortInfo>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
  // bool event(QEvent * event) override;
   bool eventFilter(QObject *obj, QEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int ProGaugeCount= 0;
    Tanque *Tconf;
    Tanque *tanques[8];
    Tanque *Maximizado;
    Sonda  *Sondas[8];
    QTimer *Time1;
    QTimer *Time2;
    QTimer *Time3;
    QTimer *deliveryProGaugeTimer;
    QTimer *Gpio_timer;
    QLabel *Lab_Title;
    QLabel *Avisos;
    QLabel *Indicadores[2];
    QLabel *Reloj;
    QFrame *line1;
    int N =-1;
    int S=0;
    int indice =0;
    QString Series[8];
    int frame = 0;
    int frameant= 0;
     QSqlDatabase DB;
     QSerialPort *puertoserie;
     QString portname;
     int G;
     QString ProGaugeId[8];
     int IDSerie;
     bool Maxi=false;
     int ProGaugeCount1 = 0;
     int ProGaugeCountCMD = 0;
     int indiceM = 0;
     bool RX= false;
     bool SondasOnline[8];
     int intento = 0;
     double Volumen_inicial;
     double Temperatura_inicial;
     double Volumen_Final;
     double Temperatura_Final;
     bool S_input[16];

    void Buscar_Tanques();  // Es funcion busca los tanques dados de alta en BD
    void Rellenar_combo_taques(QString tanque_index); // Rellenaremos los combobox con los tanque escontrados
    void Rellenar_tabla_cubicacion(int Id_tanque); // Rellenaremos la tabla de cubicacion con datos de BD
    void Rellenar_campos_cubicacion(QString p, QString a, QString v);
    bool Validar_update_cubicacion(int punto, int tanque, double altura, double volumen);
    void setEnabledBoton(QPushButton *btn, bool b);
    void enableCubicTableFields(bool v);
    void enableCubicTableBtn(bool a, bool b, bool c);
    void clearCubicTableFields();
    void guardar_limites();
    void rellenar_limites();
    void evaluar_limites(Tanque *tanque);
    void insertar_incidente(QString tipo, QString Descripcion, QString usuario);
    int X(int Y);
    int calcY(int y );


private slots:

    void on_Btn_Home_clicked();

    void on_Btb_Sonda_clicked();

    void on_Btn_Tanque_clicked();

   void ConCombocol(QComboBox *combo);

   void Modificar_TextoTank();

   void on_Btn_Guardar_clicked();

   void Actualizar_Time();

   void on_Btn_Config_clicked();

   void on_pushButton_3_clicked();

   void on_Btn_user_clicked();

   void Leer_datos();

   void inicbuff1();

   void addcbuff1(char c);

   void Guardar_Sonda();

   void Estados();

   void Enviar_qry(QString qry);

   void Protocolo(QString Cad);
   
   void Descargar();

   void Geometrytank();

   void on_pushButton_2_clicked();

   void Tanque_Maximisado();

   void on_Regresar_clicked();

   void consultaBD();

   void offlineSonda(QString offsonda);

   void SendCMD();

   void on_Btn_Comunicacion_clicked();

   void on_Btn_Barra_Estados_clicked();

   void Guardar_Comunicacion();
   
   void on_Btn_tabla_cubicacion_clicked();

   void on_Btn_Alarmas_clicked();

   void on_tableWidget_cellClicked(int row, int column);

   void on_Combo_CubTanque_currentIndexChanged(int index);

   void on_Btn_Cub_Guardar_clicked();

   void on_Btn_Cub_Editar_clicked();

   void on_Btn_Cub_Cancelar_clicked();

   void on_Btn_CubGenerar_clicked();

   void on_Combo_cub_generar_currentIndexChanged(int index);

   void on_Btn_Entregas_clicked();

   void deliveryProGaugeCountIncrement();

   void on_Btn_SaveTank_clicked();

   void on_btn_menu_clicked();

   void on_Btn_Inventario_clicked();

   void on_Btn_Reports_clicked();

   void on_Btn_Entregas_or_clicked();

   void Botones();

   void on_pushButton_clicked();

   void on_Combo_IdTanque_activated(int index);


   void on_Regresar_Home_clicked();

   void Leer_GPIO();
   void on_Combo_tanque_limites_currentIndexChanged(const QString &arg1);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
