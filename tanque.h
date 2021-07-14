#ifndef TANQUE_H
#define TANQUE_H
#include<QString>
#include<QObject>
#include<QMainWindow>
#include <QLabel>
#include <QGroupBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>

class Tanque :public QWidget
{
    Q_OBJECT

    QString NomTank;
    QString Cad;
    QString ID;
    int IdTanque;
    bool TMaximizado = true;

    int contador=0;
    int niv;
    bool Metricas = false;
    QString SistemaUnidades;

    /**************Limites de Tanque****************/
    double VolMax;
    double ProductoAlto;
    double DesbordamientoP;
    double NecesitaProducto;
    double ProductoBajo;
    double Alarma_de_Agua;
    double Advertencia_de_Agua;
    double VolumenA;
    double VolumenCon;
    double AlturaTank;
    double LimSup;

    /*****************************************************/


    /***************Configuracion de Tanque ******************/
    double TankDiametro = 200;
    double Tankaltura;
    double Tanklargo;
    double Tankancho;
    double Capacidad ;
    int CodigoProducto;
    int CodigoCombustible;
    double CoeficienteTermico;
    double AjusteAltura;
    QString Producto;
    double TankTiempoEntrega;
    double TankMinimoEntrega;
    bool Config;
    double angulo = 0;
    double distancia =500;
    double angle= 50;
    double frombase=500;
    double NivelAgua;
    double VolumeTc;
     double Temperatura;
    int puntosCubicacion;
    /***************************************************************/


    /*****Conversion EEUU ******/
    double VolumenEEUU;
    double CapacidadEEUU;
    double AlturaEEUU;
    double TemperaturaEEUU;
    double VolumeAguaEEUU;
    double VolumenTCEEUU;
    /**************************/

/****** Configuracion Entregas ******/
    QString InicioEntrega;
    int    deliveryTimeOut = 0;
    double deliveryMinimunVolume = 0;
    double deliveryVolumeRead = 0;
    double deliveryMaxVolumeRead = 0;
    int    deliveryInProcess = 0;
    double deliveryLastInventoryRead = 0;
    double deliveryCountIncrement = 0;
    double deliveryCountDecrement = 0;
    double deliveryInventoryStart = 0;
    double deliveryInventoryStartfuelheight =0 ;
    double deliveryInventoryStartwaterheight = 0;
    double deliveryInventoryStartTemp = 0;
    int Config_MinimoEntrega;
    int Config_TiempoEntrega;
    double Volumen_inicial;
    double Temperatura_inicial;
    double Volumen_Final;
    double Temperatura_Final;
    bool isConnected = true;
    bool Entregando = false ;
  /************************************************/

  /**********************Construccion de Tanque****************************/
    double  v;
    int nivA;
    int PosX;
    int PosY;
    int PosBX;
    int posBY;
    int Tipo;
    QString C = "0.60";
    QString D="0.611";
    QString E="0.80";
    QString F="0.81";
    QString Color1 = "rgba(255, 255, 255)";
    QString Color2 = "gray";
    QString Color3=  "rgb(25, 158, 218)";
    QString Color4= "rgba(255, 0, 0, 255)";
    QString Color5= "purple";
    QString t="0.25";
    QString U ="0.254";
    QString R="100";
    QString RT="200";
    QString FontTank= "18";
    QString BorderTank="2";
    bool ishabilitado= false;
    bool Desconectado= false;
    int posTank;
    int Vacio100;
    int Vacio90;

    QString Radio="90";
   QGroupBox *T1;
   QWidget *layoutWidget1;
   QLabel *Volumen;
   QLabel *Altura;
   QLabel *latanq;
   QHBoxLayout *horizontalLayout_3;
   QVBoxLayout *verticalLayout_2;
   QVBoxLayout *vericalLayout_4;
   QLabel *Temp;
   QVBoxLayout *verticalLayout;
   QLabel *Lim_Sup;
   QLabel *VolVal;
   QLabel *AltVal;
   QLabel *TmpVal;
   QLabel *NivAgua;
   QLabel *LsVal;
   QLabel *NivAVal;
   QGroupBox *groupBox_4;
   QLabel *Ltemp;
   QLabel *label1;
   QPlainTextEdit *Volumen_Maximo;
   QPlainTextEdit *Producto_Alto;
   QPlainTextEdit *Desbordamiento;
   QPlainTextEdit *Necesita_producto;
   QLabel *LVolumen_Maximo;
   QLabel *LProducto_Alto;
   QLabel *LDesbordamiento;
   QLabel *LNecesita_producto;
   QPlainTextEdit *Producto_Bajo;
   QPlainTextEdit *Alarma_Alto_Nivel_Agua;
   QPlainTextEdit *Advertencia_Nivel_Agua;
   QLabel *LProducto_Bajo;
   QLabel *LAlarma_Alto_Nivel_Agua;
   QLabel *LAdvertencia_Nivel_Agua;
   QPushButton *Boton;
   QLabel *TituloTank;
   QTabWidget *Tab;
   QWidget * Tab1;
   QWidget * tab_2;
   QWidget *centralwidget;
       QTabWidget *tabWidget;
       QWidget *tab;
       QWidget *formLayoutWidget;
       QFormLayout *formLayout;
       QLabel *label;
       QLabel *label_7;
       QLabel *label_2;
       QLabel *label_8;
       QLabel *label_3;
       QLabel *label_9;
       QLabel *label_4;
       QLabel *label_10;
       QLabel *label_5;
       QLabel *label_11;
       QLabel *label_6;
       QLabel *label_12;
       QLabel *label_13;
       QLabel *label_14;
       QLabel *label_15;
       QLabel *label_16;
       QLabel *label_17;
       QLabel *label_18;
       QWidget *gridLayoutWidget;
       QGridLayout *gridLayout;
       QLabel *label_37;
       QLabel *label_26;
       QLabel *label_28;
       QLabel *label_35;
       QLabel *label_30;
       QLabel *label_40;
       QLabel *label_25;
       QLabel *label_22;
       QLabel *label_39;
       QLabel *label_32;
       QLabel *label_34;
       QLabel *label_31;
       QLabel *label_38;
       QLabel *label_23;
       QLabel *label_29;
       QLabel *label_27;
       QLabel *label_21;
       QLabel *label_36;
       QLabel *label_24;
       QLabel *label_20;
       QLabel *label_33;
       QLabel *lbl_ProGaugeDeliveryInProccess;



    QString  styleSheetTank =
            "font: 16pt \"Broadway\";"
            "color: green;"
            // " border-width: 5px;"
            "border-style: solid;"
            "border-color: black;"
            "border-radius: "+R+ "px;"
            "background-color:"
            "qlineargradient(spread:pad,"
            "x1:0, y1:0, x2:0, y2:1,"
            "stop:0 "+ Color1 +" ,"
            "stop:"+C +" "+ Color1 + " ,"
            "stop:"+D+" "+ Color2 + " ,"
            "stop: "+E+" "+ Color2 + " ,"
            "stop: "+F+" "+ Color3 + " ,"
            "stop:1 "+" "+ Color3 +" );";

/***********************************************************************/


private slots:

    void clickbuton();

signals:

    void Camino(int pos); // Esta Señal se emite al presionar un tanque
    void EntregaInicio(QString TankNom, int IDTank); // Esta Señal se emite al inicar un entrega
    void Entrega(QString SeRealizoEntrega);// esta señal se emite al finalizar una entrega


public:

    int Protocolo;
    double pointLevel[100];
    double pointVolume[100];

    QRect *Q;
    double Xp= 1366;
    double yp= 768;
    double pi = 3.1416;

    explicit Tanque(QWidget *parent,bool config);
    void funcionActivacionTimer();
    QString Value(double c ,double a);
    //  double volumen(double alturaAgua,double AlturaCombustible, int tipo);
    QString Temp1(double T);

    int Niv();
    int NivA();

    void SetVolumen(double c, double a);
    double GetVolumen();

    double SetAltura(double height, double water);
    double GetAltura();

    void SetLimSup(double L);
    double GetLimSup();

    void SetTemperatura(double T);
    double GetTemperatura();


    void Setgeometry(int x, int y,int XTG,int YTG);
    void color(QString Ct);

    QString GetColor();
    void SetnameTank(const QString &SNT);

    QString GetNameTank();
    void inicbuff1();
    double volumen(int Tipo,double height);

    void Delate();

    void SetVolMax(double vol);
    double GetVolMax();

    void SetProducto_Alto(double producto_alto);
    double GetProducto_Alto();

    void SetDesbordamiento(double desboramiento);
    double GetDesbordamiento();


    void SetNecesitaProducto(double necesita_producto);
    double GetNecesitaProducto();

    void SetProductoBajo(double producto_bajo);
    double GetProductoBajo();


    void SetAlarma_de_Agua(double alarma_agua);
    double GetAlarma_de_Agua();

    void SetAdvertencua_de_Agua(double advetencia_agua);
    double GetAdvertencia_de_Agua();

    void SetTankDiametro(double Diametro);
    double GetTankDiametro();

    void SetTankAltura(double Altura);
    double GetTanqueAltura();

    void SetTankLargo(double Largo);
    double GetTankLargo();

   void SetTankAncho(double Ancho);
   double GetTankAncho();


   QString getID() const;
   void setID(const QString &value);

   double getTankTiempoEntrega() const;
   void setTankTiempoEntrega(double value);

   double getTankMinimoEntrega() const;
   void setTankMinimoEntrega(double value);

   int getTipo() const;
   void setTipo(int value);

   int getProtocolo() const;
   void setProtocolo(int value);

   QString getA() const;
   void setA(const QString &value);

   bool getTMaximizado() const;
   void setTMaximizado(bool value);

   double  Volume(double a, double x, double h0, double R);

   void offline();

   double getAngle() const;
   void setAngle(double value);

   double getFrombase() const;
   void setFrombase(double value);

   bool getIshabilitado() const;
   void setIshabilitado(bool value);

   double getVolumenCon() const;
   void setVolumenCon(double value);

   double getVolumenA() const;
   void setVolumenA(double value);

   double getNivelAgua() const;
   void setNivelAgua(double value);

   double getCapacidad() const;
   void setCapacidad(double value);

   int getCodigoProducto() const;
   void setCodigoProducto(int value);

   int getCodigoCombustible() const;
   void setCodigoCombustible(int value);

   double getAjusteAltura() const;
   void setAjusteAltura(double value);

   double getCoeficienteTermico() const;
   void setCoeficienteTermico(double value);

   QString getProducto() const;
   void setProducto(const QString &value);

   int getIdTanque() const;
   void setIdTanque(int value);

   int getDeliveryTimeOut() const;
   void setDeliveryTimeOut(int value);

   double getDeliveryMinimunVolume() const;
   void setDeliveryMinimunVolume(double value);

   double getDeliveryMaxVolumeRead() const;
   void setDeliveryMaxVolumeRead(double value);

   int getDeliveryInProcess() const;
   void setDeliveryInProcess(int value);

   double getDeliveryLastInventoryRead() const;
   void setDeliveryLastInventoryRead(double value);

   double getDeliveryCountIncrement() const;
   void setDeliveryCountIncrement(double value);

   double getDeliveryCountDecrement() const;
   void setDeliveryCountDecrement(double value);

   double getDeliveryInventoryStart() const;
   void setDeliveryInventoryStart(double value);

   double getDeliverySensivilityVolume() const;
   void setDeliverySensivilityVolume(double value);

   void deliveryProGaugeCountIncrement();

   QString ActualInventory(bool Inv);

   bool getIsConnected() const;
   void setIsConnected(bool value);
   bool getDesconectado() const;
   void setDesconectado(bool value);
   QString InventoryTurnos(int Turno);

   int getPosTank() const;
   void setPosTank(int value);

   int getVacio100() const;
   void setVacio100(int value);

   int getVacio90() const;
   void setVacio90(int value);

   void DeliveryinProcces();

   void SetConfigEntregas(int MinimoEntrega, int TiempoEntrega);

   void readPuntosCubicacion();

   double CalcularCubicacion(double height);

   int getPuntosCubicacion();

   double getVolumeTc() const;
   void setVolumeTc(double value);

   QString getSistemaUnidades() const;
   void setSistemaUnidades(const QString &value);
};

#endif // TANQUE_H
