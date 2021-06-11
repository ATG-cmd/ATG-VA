
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
#include "select_fechas.h"
#include <QTableWidget>
#include <cale.h>



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

   struct Settings {
          QString name;
          qint32 baudRate;
          QString stringBaudRate;
          QSerialPort::DataBits dataBits;
          QString stringDataBits;
          QSerialPort::Parity parity;
          QString stringParity;
          QSerialPort::StopBits stopBits;
          QString stringStopBits;
          QSerialPort::FlowControl flowControl;
          QString stringFlowControl;
          bool localEchoEnabled;
      };



    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

     Settings settings() const;

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
    QTimer *Inventory_Timer;
    QTimer *onesecond;

    QLabel *Lab_Title;
    QLabel *Avisos;
    QLabel *Indicadores[2];
    QLabel *Reloj;
    QFrame *line1;
    QPushButton *Btn_select_rango;
    int N =-1;
    int S=0;
    int indice =0;
    QString Series[8];
    int frame = 0;
    int frameant= 0;
     QSqlDatabase DB;
     QSerialPort *puertoserie;
     QSerialPort *Impresora;
     QString impresora_name = "CSN-A5";
     QString portname = "";
     int Alarmas = 0;
     int warnings = 0;
     QString ProGaugeId[8];
     int IDSerie;
     bool Maxi=false;
     int ProGaugeCount1 = 0;
     int ProGaugeCountCMD = 0;
     int indiceM = 0;
     bool RX[8]= {false,false,false,false,false,false,false,false};
     bool SondasOnline[8];
     int intento[8]={0,0,0,0,0,0,0,0};
     double Volumen_inicial;
     double Temperatura_inicial;
     double Volumen_Final;
     double Temperatura_Final;
     bool S_input[16];
      int TCon = 0;
     bool sensor_papel;



     int numerodetanques=0;
     int TanqueActual= 0;
     int TurnoActual =-1;
     int TurnoEncontrado;
     int MinutoSec =0;


     QMetaObject::Connection combo_connect1;
     QMetaObject::Connection combo_connect2;
     QMetaObject::Connection combo_connect3;
     QMetaObject::Connection combo_connect4;
     QMetaObject::Connection combo_connect5;
     QMetaObject::Connection combo_connect6;

     int SelecIntervalo= 0;
     int SelecHora =0;
     int SelecMinutos =0;
     int InvMin= 0;

     QString Bitso [13]= {"Deshabilitado","5 Minutos","10 Minutos","15 Minutos","20 Minutos","30 Minutos",
                    "1 Hora","2 Horas","4 Horas","6 Horas","8 Horas", "12 Horas", "24 Horas"};
     QStringList SelecionInventario = {"InventarioMin","inventario","InventarioCortes", "InventarioTurnos"};
     int minutos[8] = {0,0,0,0,0,0,0,0};
    // QTime Turnos[8];


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
    void insertar_incidente(QString tipo, QString Descripcion, QString usuario, QString Prioridad, QString Activo, bool filtro);
    void rellenar_incidentes(QString T_inicial, QString T_Final, int index);
    void limpiar_tabla(QTableWidget *tabla,int cont);
    void buscar_prioridad(QString priodidad);
    void rellenar_activos(QPushButton *btn);
    void limpiar_Activos();
    void buscar_alarmas();
    bool validar_activos(QString tipo, QString Descripcion, QString usuario, bool filtro);
    int X(int Y);
    int calcY(int y );
    void btn_Habilitado(QPushButton *Boton,bool hab);
    void GuardarConfigInv();
    void TimerConfigInventoryDB();
    void InVentoryHistory(int IDTank, int ComboInventory, bool Porfecha, QString QryFecha);
    void estado_sistema(QPushButton *btn,QString estado);
    void guardar_station();
    void guardar_Turnos();
    void Qry_Turnos(int Turno);
    void RellenarInventario(QString FechaInicio, QString FechaFin, int index);
    void buscar_impresora();
    void consultar_impresora();
    void guardar_impresora();
    bool papel_out();

    signals:

    void BotonMas(int Turno);


private slots:

    void on_Btn_Home_clicked();

    void on_Btb_Sonda_clicked();

    void on_Btn_Tanque_clicked();

   void ConCombocol(QComboBox *combo);

   QString ColorTank(QString Color);

  // void Modificar_TextoTank();

   void on_Btn_Guardar_clicked();

   void Actualizar_Time();

   void on_Btn_Config_clicked();

   void on_pushButton_3_clicked();

   void on_Btn_user_clicked();

   void Leer_datos();

   void leer_impresora();

   void inicbuff1();

   void addcbuff1(char c);

   void Guardar_Sonda();

   void Estados();

   void Enviar_qry(QString qry);

   void Protocolo(QString Cad);
   
   void Descargar();

   void Geometrytank();

   void on_pushButton_2_clicked();

   void Tanque_Maximisado(int index);

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

   void btn_clicked();

   void Leer_GPIO();

   void everysecond();

   void on_Combo_tanque_limites_currentIndexChanged(const QString &arg1);

   void Qry_Entrega(QString  Entrega_Realizada);

   void on_Btn_inventarioConfig_clicked();

   void on_BtnMasIntervalo_clicked();

   void on_Btn_IntervaloMenos_clicked();

   void on_BtnMasIntervalo_pressed();

   void on_BtnMasIntervalo_released();

   void on_Btn_IntervaloMenos_pressed();

   void on_Btn_IntervaloMenos_released();

   void on_Btn_D_masH_clicked();

   void on_Btn_D_menosH_clicked();

   void on_Btn_D_masH_pressed();

   void on_Btn_D_menosH_released();

   void on_Btn_D_masH_released();

   void on_Btn_D_menosH_pressed();

   void on_Btn_H_masM_clicked();

   void on_Btn_H_masM_pressed();

   void on_Btn_H_masM_released();

   void on_Btn_H_menosM_clicked();

   void on_Btn_H_menosM_released();

   void on_Btn_H_menosM_pressed();

   void on_Line_Intervalo_textChanged(const QString &arg1);

   void InventoryTank();

   void on_pushButton_5_clicked();

   void on_Btn_Impresora_clicked();

   void on_Btn_Station_clicked();

   void on_Btn_Turnos_clicked();

   void on_Combo_StationCode_activated(int index);

   void on_Combo_StationName_activated(int index);

   void on_Combo_User_activated(int index);

   void on_Combo_Memo_activated(int index);

   void on_Btn_SelecRange_clicked();

   void on_Btn_sensor_conf_clicked();

   void on_Btn_sensor_rep_clicked();

   void on_Combo_impresora_activated(int index);

   void handleError(QSerialPort::SerialPortError error);

   void on_Btn_impresion_prueba_clicked();

  void  fillPortsParameters();

  void updateSettings();

  void openSerialPort();

  void closeSerialPort();




  void on_Combo_MetodoCierre_currentIndexChanged(int index);


  void on_Btn_Fechayhora_clicked();

private:
    Ui::MainWindow *ui;
    Settings m_currentSettings;
};
#endif // MAINWINDOW_H

