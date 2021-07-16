#include "tanque.h"
#include <QTimer>
#include <QObject>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtDebug>
#include <QtMath>
#include <QToolButton>
#include <QTabWidget>
#include <QDateTime>

#define TablaDeCubicacion 0
#define CilindroHorizontal 1
#define CilindroVerical 2


QString Tanque::getID() const { return ID; }
void Tanque::setID(const QString &value) { ID = value; }
double Tanque::getTankTiempoEntrega() const { return TankTiempoEntrega; }
void Tanque::setTankTiempoEntrega(double value) { TankTiempoEntrega = value; }
double Tanque::getTankMinimoEntrega() const { return TankMinimoEntrega; }
void Tanque::setTankMinimoEntrega(double value) { TankMinimoEntrega = value; }
int Tanque::getTipo() const { return Tipo; }
void Tanque::setTipo(int value) { Tipo = value; }
int Tanque::getProtocolo() const { return Protocolo; }
void Tanque::setProtocolo(int value) { Protocolo = value; }
bool Tanque::getTMaximizado() const { return TMaximizado; }
void Tanque::setTMaximizado(bool value) { TMaximizado = value; }
int Tanque::Niv() { return niv; }
int Tanque::NivA() { return nivA; }
double Tanque::GetVolumen() {if(Metricas) {  return VolumenCon;} else { return VolumenEEUU; } }
double Tanque::GetAltura() { return AlturaTank; }
void Tanque::SetLimSup(double L) { LimSup= L; LsVal->setText(QString::number(LimSup));}
double Tanque::GetLimSup() { return LimSup; }
double Tanque::getAngle() const { return angle; }
void Tanque::setAngle(double value) { angle = value; }
double Tanque::getFrombase() const { return frombase; }
void Tanque::setFrombase(double value) {frombase = value; }
bool Tanque::getIshabilitado() const {return ishabilitado;}
void Tanque::setIshabilitado(bool value) {ishabilitado = value;}
double Tanque::getVolumenCon() const{if (Metricas) {return VolumenCon;} else{ return VolumenEEUU;}}
void Tanque::setVolumenCon(double value){VolumenCon = value;}
double Tanque::getVolumenA() const {if (Metricas){ return VolumenA;} else{return VolumeAguaEEUU;}  }
void Tanque::setVolumenA(double value){VolumenA = value;}
double Tanque::getNivelAgua() const{return NivelAgua;}
void Tanque::setNivelAgua(double value){NivelAgua = value;}
double Tanque::getCapacidad() const{ return Capacidad;}
void Tanque::setCapacidad(double value){ Capacidad = value;}
int Tanque::getCodigoProducto() const { return CodigoProducto; }
void Tanque::setCodigoProducto(int value) { CodigoProducto = value; }
int Tanque::getCodigoCombustible() const { return CodigoCombustible; }
void Tanque::setCodigoCombustible(int value) { CodigoCombustible = value; }
double Tanque::getAjusteAltura() const { return AjusteAltura; }
void Tanque::setAjusteAltura(double value) { AjusteAltura = value; }
double Tanque::getCoeficienteTermico() const { return CoeficienteTermico; }
void Tanque::setCoeficienteTermico(double value) {CoeficienteTermico = value; }
QString Tanque::getProducto() const { return Producto; }
void Tanque::setProducto(const QString &value) {Producto = value; }
int Tanque::getIdTanque() const { return IdTanque;}
void Tanque::setIdTanque(int value){IdTanque = value;}
int Tanque::getDeliveryTimeOut() const { return deliveryTimeOut; }
void Tanque::setDeliveryTimeOut(int value) { deliveryTimeOut = value; }
double Tanque::getDeliveryMinimunVolume() const { return deliveryMinimunVolume; }
void Tanque::setDeliveryMinimunVolume(double value) { deliveryMinimunVolume = value; }
double Tanque::getDeliveryMaxVolumeRead() const { return deliveryMaxVolumeRead; }
void Tanque::setDeliveryMaxVolumeRead(double value) { deliveryMaxVolumeRead = value; }
int Tanque::getDeliveryInProcess() const { return deliveryInProcess; }
void Tanque::setDeliveryInProcess(int value) {deliveryInProcess = value; }
double Tanque::getDeliveryLastInventoryRead() const { return deliveryLastInventoryRead;}
void Tanque::setDeliveryLastInventoryRead(double value) { deliveryLastInventoryRead = value; }
double Tanque::getDeliveryCountIncrement() const { return deliveryCountIncrement; }
void Tanque::setDeliveryCountIncrement(double value) { deliveryCountIncrement = value; }
double Tanque::getDeliveryCountDecrement() const {  return deliveryCountDecrement; }
void Tanque::setDeliveryCountDecrement(double value) { deliveryCountDecrement = value; }
double Tanque::getDeliveryInventoryStart() const { return deliveryInventoryStart; }
void Tanque::setDeliveryInventoryStart(double value) { deliveryInventoryStart = value;}
bool Tanque::getIsConnected() const { return isConnected;}
void Tanque::setIsConnected(bool value) { isConnected = value; }
bool Tanque::getDesconectado() const {return Desconectado; }
void Tanque::setDesconectado(bool value){Desconectado = value; }
int Tanque::getPosTank() const { return posTank; }
void Tanque::setPosTank(int value) { posTank = value; }
int Tanque::getVacio100() const {return Vacio100; }
void Tanque::setVacio100(int value) { Vacio100 = value;}
int Tanque::getVacio90() const { return Vacio90;}
void Tanque::setVacio90(int value) { Vacio90 = value;}
double Tanque::getVolumeTc() const {if (Metricas) {return VolumeTc;} else {return VolumenTCEEUU;}}
void Tanque::setVolumeTc(double value) {VolumeTc = value;}
QString Tanque::getSistemaUnidades() const {return SistemaUnidades;}
void Tanque::setSistemaUnidades(const QString &value)
{ SistemaUnidades = value; if(value == "Metrico") Metricas = true; else Metricas=false;}
void Tanque::clickbuton(){ setTMaximizado(false); emit Camino(posTank); }
void Tanque::SetVolMax(double vol) { VolMax = vol; }
double Tanque::GetVolMax() { return VolMax; }
void Tanque::SetProducto_Alto(double producto_alto) { ProductoAlto = producto_alto; }
double Tanque::GetProducto_Alto() { return ProductoAlto; }
void Tanque::SetDesbordamiento(double desboramiento) { DesbordamientoP = desboramiento;}
double Tanque::GetDesbordamiento() { return DesbordamientoP; }
void Tanque::SetNecesitaProducto(double necesita_producto) { NecesitaProducto = necesita_producto; }
double Tanque::GetNecesitaProducto() { return NecesitaProducto; }
void Tanque::SetProductoBajo(double producto_bajo) { ProductoBajo = producto_bajo; }
double Tanque::GetProductoBajo() { return ProductoBajo; }
void Tanque::SetAlarma_de_Agua(double alarma_agua) { Alarma_de_Agua = alarma_agua; }
double Tanque::GetAlarma_de_Agua() { return Alarma_de_Agua; }
void Tanque::SetAdvertencua_de_Agua(double advetencia_agua) {Advertencia_de_Agua = advetencia_agua; }
double Tanque::GetAdvertencia_de_Agua() { return Advertencia_de_Agua; }
void Tanque::SetTankDiametro(double Diametro) { TankDiametro = Diametro; }
double Tanque::GetTankDiametro() { return TankDiametro; }
void Tanque::SetTankAltura(double Altura) { Tankaltura = Altura; }
double Tanque::GetTanqueAltura() { if (Metricas) {return Tankaltura;}else { return  AlturaEEUU;} }
void Tanque::SetTankLargo(double Largo) { Tanklargo = Largo; }
double Tanque::GetTankLargo() { return Tanklargo; }
void Tanque::SetTankAncho(double Ancho){ Tankancho = Ancho; }
double Tanque::GetTankAncho(){ return Tankancho; }
double Tanque::GetTemperatura(){ if (Metricas){return Temperatura;} else {return TemperaturaEEUU;}}
QString Tanque::GetColor() { return Color2; }
void Tanque::SetnameTank(const QString &SNT) {T1->setTitle(SNT); NomTank = SNT; }
QString Tanque::GetNameTank(){ return NomTank; }

/* en este metodo el tanque es desconectado este se pinta de un color solido
 y cambiando el taxto a Sin conexion.
Tambien en este metodo se elimina de la base de datos, de la tabla inventariomin
el inventario actual de ese tanque.*/

void Tanque::offline()
{
     latanq->setStyleSheet("background-color:lightgray;");
     latanq->setText("Sin Conexion");
    // ishabilitado = false;
     Desconectado= false;

     QSqlQuery qry;
           qry.exec("DELETE FROM `cistem`.`InventarioMin` WHERE ID ='"+QString::number(IdTanque)+"';");
}

/* En esta parte se construlle el tanque  se crea un GroupBox padre este almacena a nuesto label en forma
 * de tanque asi como los label que se utilizan para mostrar los valores de este*/

Tanque::Tanque(QWidget *parent,bool config) : QWidget(parent)
{
    Config = config;
/*En este if Se  configura si el tanque mostrara los datos a un lado de el o solamente sera el tanque solo*/
    if(config){
// Si Config es Verdadero se configura el tanque para mostrar labels indicadores de*/
        T1 = new QGroupBox(parent);
        Boton = new QPushButton (T1);
        // TituloTank = new QLabel(Boton);
        layoutWidget1 = new QWidget(Boton);
        Volumen = new QLabel(layoutWidget1);
        Altura = new QLabel(layoutWidget1);
        groupBox_4 = new QGroupBox(Boton);
        Ltemp = new QLabel(groupBox_4);
        latanq = new QLabel(groupBox_4);
        lbl_ProGaugeDeliveryInProccess = new QLabel(layoutWidget1);

        QFont font3;
        font3.setPointSize(30);
        QFont font2;
        font3.setPointSize(30);
        font2.setBold(true);
        QFont font1;
        font1.setBold(true);
        font3.setBold(true);
        font2.setPointSize(20);

        T1->setObjectName(QString::fromUtf8("T1"));
        T1->setEnabled(true);
        T1->setGeometry(QRect(0,20, 561, 300));
        T1->setFont(font3);
        T1->setTitle("Tanque");
        T1->setFlat(true);

        //T1->setStyleSheet("border: 2px solid blue;");
        Boton->setGeometry(0,50,560,299);
        Boton->setStyleSheet("QPushButton{background-color:rgb(240, 240, 240); border: 0px solid white; border-radius: 15px; padding: 5px;  }" );


        // esta conexion aprobecha la señal generada al dar click al boton
        // Esta señal se dirige al  slot clickbuton en el cual se emite una señal y cambie el estado de un boleano
        // Para asegurarnos que este fue el tanque que se clikio en la señal se manda la posicion del tanque clikeado

        connect(Boton, SIGNAL (clicked()),this, SLOT(clickbuton()));


        layoutWidget1->setGeometry(QRect(310, 20, 231, 253));


        vericalLayout_4 = new QVBoxLayout(layoutWidget1);
        horizontalLayout_3 = new QHBoxLayout();
        verticalLayout_2 = new QVBoxLayout();


        Temp = new QLabel(layoutWidget1);
        verticalLayout = new QVBoxLayout();
        Lim_Sup = new QLabel(layoutWidget1);
        VolVal = new QLabel(layoutWidget1);
        AltVal = new QLabel(layoutWidget1);
        TmpVal = new QLabel(layoutWidget1);
        NivAgua = new QLabel(layoutWidget1);
        LsVal = new QLabel(layoutWidget1);
        NivAVal = new QLabel(layoutWidget1);

        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);

        //  ↓  Este es el label de entrega en proceso Este solamente aparece cuando se esta realizando una entrega
         lbl_ProGaugeDeliveryInProccess->setObjectName(QString::fromUtf8("Entrega"));
      //   lbl_ProGaugeDeliveryInProccess->setGeometry(375,0,370,50);
         lbl_ProGaugeDeliveryInProccess->setFont(font2);
         lbl_ProGaugeDeliveryInProccess->setText("Entrega en Proceso");
         lbl_ProGaugeDeliveryInProccess->setStyleSheet("QLabel{ color: white;"
                                                       "background-color: red;"
                                                       "border: 3px solid red;"
                                                       "border-radius: 10px;"
                                                       "padding: 2px; }");
        // lbl_ProGaugeDeliveryInProccess->hide();

         vericalLayout_4->addWidget(lbl_ProGaugeDeliveryInProccess);
         // horizontalLayout_3->addLayout(vericalLayout_4);

        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        Volumen->setObjectName(QString::fromUtf8("Volumen"));
        Volumen->setEnabled(true);
        Volumen->setFont(font2);
        Volumen->setText("Volumen");
        Volumen->setStyleSheet("QLabel{ background-color: rgb(230, 230, 230); }");

        verticalLayout_2->addWidget(Volumen);

        Altura->setObjectName(QString::fromUtf8("Altura"));
        Altura->setFont(font2);
        Altura->setText("Altura");
        Altura->setStyleSheet("QLabel{ background-color: rgb(210, 210, 210); }");

        verticalLayout_2->addWidget(Altura);

        Lim_Sup->setObjectName(QString::fromUtf8("Lim_Sup"));
        Lim_Sup->setFont(font2);
        Lim_Sup->setText("Vacio");
        Lim_Sup->setStyleSheet("QLabel{ background-color: rgb(230, 230, 230); }");

        verticalLayout_2->addWidget(Lim_Sup);

        Temp->setObjectName(QString::fromUtf8("Temp"));
        Temp->setFont(font2);
        Temp->setText("Temperatura");
        verticalLayout_2->addWidget(Temp);
        Temp->setStyleSheet("QLabel{ background-color: rgb(210, 210, 210); }");

        NivAgua->setObjectName(QString::fromUtf8("NivAgua"));
        NivAgua->setFont(font2);
        NivAgua->setText("Nivel del Agua");
        NivAgua->setStyleSheet("QLabel{ background-color: rgb(230, 230, 230); }");

        verticalLayout_2->addWidget(NivAgua);

        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        VolVal->setObjectName(QString::fromUtf8("VolVal"));
        VolVal->setFont(font2);
        VolVal->setText("000");
        VolVal->setStyleSheet("QLabel{ background-color: rgb(230, 230, 230); }");

        verticalLayout->addWidget(VolVal);
        AltVal->setObjectName(QString::fromUtf8("AltVal"));
        AltVal->setFont(font2);
        AltVal->setText("0000");
        AltVal->setStyleSheet("QLabel{ background-color: rgb(210, 210, 210); }");

        verticalLayout->addWidget(AltVal);

        LsVal->setObjectName(QString::fromUtf8("LsVal"));
        LsVal->setFont(font2);
        LsVal->setText("000");
        LsVal->setStyleSheet("QLabel{ background-color: rgb(230, 230, 230); }");

        verticalLayout->addWidget(LsVal);
        TmpVal->setObjectName(QString::fromUtf8("TmpVal"));
        TmpVal->setFont(font2);
        TmpVal->setText("000");
        TmpVal->setStyleSheet("QLabel{ background-color: rgb(210, 210, 210); }");

        verticalLayout->addWidget(TmpVal);
        NivAVal->setObjectName(QString::fromUtf8("NivAVal"));
        NivAVal->setFont(font2);
        NivAVal->setText("000");
        NivAVal->setStyleSheet(" QLabel{background-color: rgb(230, 230, 230); }");
    //    NivAVal->setAlignment(Qt::AlignRight);
       verticalLayout->addWidget(NivAVal);


            // Inicialmente esta oculto  ↑

        horizontalLayout_3->addLayout(verticalLayout);

        vericalLayout_4->addLayout(horizontalLayout_3);

 }
 else
 {
// Si es falso se Mostrara el tanque solo
     T1 = new QGroupBox(parent);
    // Tab = new QTabWidget(Tab);
     layoutWidget1 = new QWidget(T1);
     Volumen = new QLabel(layoutWidget1);
     Altura = new QLabel(layoutWidget1);
     groupBox_4 = new QGroupBox(T1);

 //    ↓  latanq es el label del tanque
     latanq = new QLabel(groupBox_4);

      QFont font3;
      font3.setPointSize(18);
      QFont font2;
      font3.setPointSize(15);
      QFont font1;
      font2.setPointSize(15);

      T1->setObjectName(QString::fromUtf8("T1"));
      T1->setEnabled(true);
      T1->setGeometry(QRect(0,40, 561, 281));
      T1->setFont(font3);
      T1->setTitle("Tanque");

      T1->setFlat(true);
      T1->setChecked(false);

 }

     groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
     groupBox_4->setGeometry(QRect(10, 20, 200, 180));
     groupBox_4->setStyleSheet(QString::fromUtf8("border: 0px"));

     latanq->setObjectName(QString::fromUtf8("latanq"));
     latanq->setEnabled(true);
     latanq->setGeometry(QRect(40, 30, 201, 201));
     latanq->setAlignment(Qt::AlignCenter);
     latanq->setText("...INICIANDO...");
     latanq->raise();

 }


/* En esta parte se calcula el volumen de el combustible y el volumen del agua dependiendo del tipo de
 * tanque incluyendo si este esta cubicado*/

 double Tanque::SetAltura(double height, double water)
 {
     double r=0;

     double h= height /1000;
     double res=0;
     double reswater=0;
     //double K= 0;
     double ho= 0;
     double a = 0;
     double tana = 0;
     double from= 0;
     double level = height;
     double x = 0;
     double h1;
     double fullL = 0;
     double d = 0;
     bool Sup =true;


     AlturaTank = height - AjusteAltura;

        double result = 0.0;

     switch(Tipo)
     {

     case TablaDeCubicacion:

         puntosCubicacion =  getPuntosCubicacion();
          readPuntosCubicacion();
          Capacidad= pointVolume[puntosCubicacion -1];

          if(Metricas){
              res =CalcularCubicacion(h);
              reswater = CalcularCubicacion(water/1000);

          }else{
              res= CalcularCubicacion(h *0.004323);
              reswater = CalcularCubicacion((water/1000) *0.004323);
          }
          SetVolumen(res,reswater);

          qDebug ( ) << "---------------------";
          qDebug() << "Volumen Cubicado : " << res;
          qDebug() << "-------------------";

          break;



     case CilindroVerical:
         qDebug() << "Cilindro vertical";
         r = TankDiametro / 2;


         res = (pi * (r * r) * h) / 1000;
         reswater = (pi * (r * r) *water) / 1000;

         break;
     case CilindroHorizontal:
         qDebug() << "Cilindro Horizontal ----------------------------------------------";
         r = TankDiametro / 2;
         // Tankaltura = TankDiametro;
         h = height;
         Tankaltura = height;

         d = TankDiametro;

         result = (Capacidad * 1000000) /(pi * qPow(r,2));
         //result = Capacidad / ((((r * r) * (qAcos((r - h) / r ))) - ((r - h) * (qSqrt((d * h) - (h * h))))) * 1000);
         qDebug() << "Resultado" << result;

         res = ((((r * r) * (qAcos((r - h) / r ))) - ((r - h) * (qSqrt((TankDiametro * h) - (h * h))))) * result) / 1000000;
         reswater = ((((r * r) * (qAcos((r - water) / r ))) - ((r - water) * (qSqrt((TankDiametro * water) - (water * water))))) * result) / 1000000;

         d = TankDiametro;

         a = angle / 180 * M_PI;
         tana = qTan(a);
         from =  Sup ? frombase : result - frombase;
         ho = level - tana * from;
         x = result;
         if (ho < 0) {
             x += ho / tana;
             ho = 0;
         }
         h1 = tana * x + ho;

         if (h1 > d) {
             fullL = (h1 - d) / tana;
             if (fullL == -0.00) {
                 fullL = 0;
             }
             x -= fullL;
             h1 = d;
         }

         if (a != 0.00) {
             double v1 = Volume(a, x, ho, r);
             double v2 = Volume(a, 0, ho, r);
             v =v1 - v2 + fullL * M_PI * r * r;
         }
         else {
             double theta = 2 * qAcos((r - ho) / r);
             v = r * r / 2 * (theta - qSin(theta)) * x;
         }

         // qDebug () <<  "Volumen con Formula de pagina" << v/ 1000000;
         // qDebug () << "Volumen con Formula del jefe" << res;
         // qDebug() << "Volumen del agua: " << reswater;


         break;

                  }

              SetVolumen(res,reswater);
               NivelAgua = water;

       return res;

 }

 double Tanque::Volume(double a, double x, double h0, double R) {

                  double u = (R - qTan(a) * x - h0) / R;

                  if (u > 1) u = 1; else if (u < -1) u = -1; //precision error

                  return -R * R * R * (u * qAcos(u) - qSqrt(1 - u * u) * (u * u + 2) / 3) / qTan(a);
              }


void Tanque::SetVolumen(double c ,double a)
{
//    2.- ¿Cuál será el volumen final de 5 litros de Glicerina si sufre un calentamiento de 10°C a
//    30° C?
//    DATOS:
//    Petróleo
//    V0 = 5 litros
//    T0 = 10°C
//    TF = 30°C
//    INCÓGNITA:
//    VF = ?
//    FÓRMULA:
//    ΔV = 𝜷 𝑽𝟎 ( 𝑻𝑭− 𝑻𝒊 )
//    SUSTITUCIÓN:
//    ΔV = ( 𝟖𝟗𝟓 𝑿 𝟏𝟎 -6)( 𝟓 𝒍𝒊𝒕𝒓𝒐𝒔 )( 𝟑𝟎°𝑪− 𝟏𝟎°𝑪 )
//    ΔV = 0.089 litros es la dilatación cúbica o incremento en su volumen de tal forma que el volumen
//    final es de 5.089 litros.
    QSqlQuery qry;
    qry.exec("SELECT  Coeficiente FROM cistem.Productos WHERE Nombre='MAGNA';");

    while(qry.next())
    {
     CoeficienteTermico = qry.value(0).toDouble();
    }

    double CompensacionVol  = CoeficienteTermico * c * ( Temperatura -15);
    VolumeTc = c + CompensacionVol;
    qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    qDebug() << "Volumen Compensado:" << VolumeTc;
    qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    qDebug() << "Volumen Sin Compensacion: " << c;
    qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    VolumenA = a;
    VolumenCon=c;

    double d=0;
    double b=0;
    double CD;
    qDebug() << "Capacidad" << Capacidad ;

  // *************************** Conversion de Unidades **********************************
   VolumenEEUU = VolumenCon / 3.78541;
    VolumenTCEEUU = VolumeTc/ 3.78541;
    AlturaEEUU= (AlturaTank /10) * 2.74;
    TemperaturaEEUU = (Temperatura *1.8000) + 32;
    VolumeAguaEEUU = VolumenA /3.78541;


    if (TMaximizado)
    {

        if(!Metricas){
            AltVal->setText(QString::number(AlturaEEUU,'f',1)+ " In");
            VolVal->setText(QString::number(VolumenEEUU,'f',2)+" Gal");
            NivAVal->setText(QString::number(VolumeAguaEEUU, 'f', 2)+ " Gal");
            TmpVal->setText(QString::number(TemperaturaEEUU,'f',2)+ " ºF ");
            LsVal->setText(QString::number(Capacidad-VolumenEEUU)+ " Gal");
            Vacio100 = int(Capacidad- VolumenEEUU) ;
            Vacio90 = int((Capacidad* 0.9) - VolumenEEUU);

        }
        else {

            AltVal->setText(QString::number(AlturaTank,'f',1)+ " MM");
            VolVal->setText(QString::number(VolumenCon,'f',2)+" Lts");
            NivAVal->setText(QString::number(VolumenA, 'f', 2)+ " Lts");
            TmpVal->setText(QString::number(Temperatura,'f',2)+ " ºC ");
            LsVal->setText(QString::number(Capacidad-VolumenCon)+ " Lts");
            Vacio100 = int(Capacidad - VolumenCon) ;
            Vacio90 = int((Capacidad* 0.9) - VolumenCon);

        }


    }


    c=(Capacidad-c)/Capacidad;

    qDebug() << "C" << c;
    if (c<0)
        c=0;
    if (c>a)
        c=0.99-a;
    a=(Capacidad-a)/Capacidad;
    qDebug() << "a" << a;

    d= c+0.01;
    b=a+0.01;

     C= C.number(c);
     D= D.number(d);
     E = E.number(a);
     F= F.number(b);

    styleSheetTank =
            "font: "+FontTank+"pt \"Bold\";"
            "color: black;"
            // " border-width: 5px;"
            "border: "+BorderTank+"px solid;"
            "border-color: rgb(165, 165, 165);"
            "border-style: solid;"
            "border-radius: "+R+"px;"
            "background-color:"
            "qlineargradient(spread:pad,"
            "x1:0, y1:0, x2:0, y2:1,"
            "stop:0 rgba(255, 255, 255, 255),"
            "stop:"+C+" rgba(255, 255, 255, 255),"
            "stop:"+D+"  "+Color2+","
            "stop: "+E+" "+Color2+","
            "stop: "+F+" rgb(0, 0, 255),"
            "stop:1 rgb(0, 0, 255))";

        latanq->setStyleSheet(styleSheetTank);

    CD= double (c);
    niv = int((1.001-CD) *100);

    latanq->setText(QString::number(niv)+ "%");

}

void Tanque::SetTemperatura(double T)
{
    Temperatura = T;

    if (!Desconectado && TMaximizado){

        ishabilitado = true;
        Desconectado = true;
        QSqlQuery qry;
        qry.exec("SELECT * FROM `cistem`.`InventarioMin`  WHERE IDTank ="+QString::number(IdTanque)+"; ");
        while(qry.next())
        {
            qry.exec("INSERT INTO `cistem`.`InventarioCortes` (`IDTank`, `NombreTank`, `VolumenCo`, `Temperatura`, `AlturaC`, `AlturaA`, `Fecha`, `Estado`) "
                     "VALUES ('"+qry.value(1).toString()+"', '"+qry.value(2).toString()+"', '"+qry.value(3).toString()+"', '"+qry.value(4).toString()+"', '"+qry.value(5).toString()+"', '"+qry.value(6).toString()+"', '"+qry.value(9).toString()+"','Desconexion');");
        }

        qry.exec("INSERT INTO `cistem`.`InventarioCortes` (`IDTank`, `NombreTank`, `VolumenCo`, `Temperatura`, `AlturaC`, `AlturaA`, `Fecha`, `Estado`) "
                 "VALUES ('"+QString::number(IdTanque)+"', '"+NomTank+"', '"+QString::number(VolumenCon)+"', '"+QString::number(Temperatura)+"', '"+QString::number(AlturaTank)+"', '"+QString::number(NivelAgua)+"' , '"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"','Conexion');");
    }
    else {
        QSqlQuery qry;
        qry.exec("UPDATE `cistem`.`InventarioMin` SET `ID`='"+QString::number(IdTanque)+"', `NombreTank`='"+NomTank+"', `VolumenCo`='"+QString::number(VolumenCon)+"', `Temperatura`='"+QString::number(Temperatura)+"', `AlturaC`='"+QString::number(AlturaTank)+"', `AlturaA`='"+QString::number(NivelAgua)+"',`100%Vacio`='"+QString::number(Vacio100)+"', `90% Vacio`='"+QString::number(Vacio90)+"', `Fecha`='"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"' WHERE  `ID`="+QString::number(IdTanque)+";");

        if(TMaximizado){
            QSqlQuery qry;
            qry.exec("UPDATE `cistem`.`InventarioMin` SET `ID`='"+QString::number(IdTanque)+"', `NombreTank`='"+NomTank+"', `VolumenCo`='"+QString::number(VolumenCon)+"', `Temperatura`='"+QString::number(Temperatura)+"', `AlturaC`='"+QString::number(AlturaTank)+"', `AlturaA`='"+QString::number(NivelAgua)+"',`100%Vacio`='"+QString::number(Vacio100)+"', `90% Vacio`='"+QString::number(Vacio90)+"', `Fecha`='"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"',`ID`=  WHERE  `ID`="+QString::number(IdTanque)+";");
            qry.exec( "INSERT INTO `cistem`.`InventarioMin` (`ID`, `IDTank`, `NombreTank`, `VolumenCo`, `Temperatura`, `AlturaC`, `AlturaA`,`100%Vacio`, `90% Vacio`, `Fecha`) VALUES ('"+QString::number(IdTanque)+"', '"+QString::number(IdTanque)+"','"+NomTank+"','"+QString::number(VolumenCon)+"','"+QString::number(Temperatura)+"', '"+QString::number(AlturaTank)+"', '"+QString::number(NivelAgua)+"','"+QString::number(Vacio100)+"', '"+QString::number(Vacio90)+"', '"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"');");
        }

    }


}

void Tanque::Delate()
{
    T1->deleteLater();
    layoutWidget1->deleteLater();
    Volumen->deleteLater();
    Altura->deleteLater();
    groupBox_4->deleteLater();
    Ltemp->deleteLater();
    latanq->deleteLater();
    bool config = false;
    if (config)
    {
    horizontalLayout_3->deleteLater();
    verticalLayout_2->deleteLater();
    Temp->deleteLater();
    verticalLayout->deleteLater();
    Lim_Sup->deleteLater();
    VolVal->deleteLater();
    AltVal->deleteLater();
    TmpVal->deleteLater();
    NivAgua->deleteLater();
    LsVal->deleteLater();
    NivAVal->deleteLater();
    //BT1->deleteLater();
    }

}



void Tanque::Setgeometry(int x, int y,int XTG,int YTG)
{

    PosX= x;
    PosY= y;
    PosBX= x+560;
    posBY= y+45;
    int x1 = (YTG*380)/500;
    int r = (250*x1)/500;
    R = QString::number(r);
    int rt= (520*x1)/1000;
    RT = QString::number(rt);

    int x2= x1+21;
    T1->setGeometry(QRect(x,y,XTG, YTG));

    qDebug() << "geometrias del boton: x:" << 0 << " y:" << 50 << " w:" << XTG << " h:" << int(YTG*.8);
    qDebug() << "geometrias del circulo: x:" << 5 << " y:" << 5 << " w:" << x1 << " h:" << x1;
    qDebug() << "geometrias del labels: x:" << XTG/2 << " y:" << 20 << " w:" << int((XTG/2)*.95) << " h:" << int(YTG*.7);

    if(Config)
    {
        QFont font5;
        font5.setPointSize(int(XTG*.03));
        font5.setBold(true);
        Boton->setGeometry(QRect(0,50,XTG,int(YTG*.8)));
        latanq->setGeometry(QRect(5, 5, x1, x1));
        layoutWidget1->setGeometry(QRect(XTG/2 -40, 20,int((XTG/2)+30),int(YTG*.7)));
        layoutWidget1->setFont(font5);
        Volumen->setFont(font5);
        Temp->setFont(font5);
        Altura->setFont(font5);
        Lim_Sup->setFont(font5);
        VolVal->setFont(font5);
        AltVal->setFont(font5);
        TmpVal->setFont(font5);
        NivAgua->setFont(font5);
        LsVal->setFont(font5);
        NivAVal->setFont(font5);
        latanq->setFont(font5);

     FontTank =QString::number(int(XTG*.07));

     groupBox_4->setGeometry(QRect(10, 2, x2+x2,x2+x2));

    }
    else
    {
        latanq->setGeometry(QRect(5, 5, x1, x1));
        FontTank =QString::number(int(XTG*.1));
        groupBox_4->setGeometry(QRect(10, 20, x2+x2,x2+x2));

    }
    BorderTank = QString::number(int(XTG* .014));
}

void Tanque::color(QString Ct1)
{
    Color2 = Ct1;

    styleSheetTank =
            "font: 16pt \"Broadway\";"
            "color: green;"
            // " border-width: 5px;"
            "border-style: solid;"
            "border: 8px solid;"
            "border-color: rgb(165, 165, 165);"
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
        latanq->setStyleSheet(styleSheetTank);

}


void Tanque::DeliveryinProcces()
{

    QSqlQuery qry;
    qry.exec("SELECT * FROM `cistem`.`Entregando` WHERE IDTank='"+QString::number(IdTanque)+"'  AND  STATUS='Entregando';");

    while(qry.next())
    {    deliveryCountIncrement = 31;
        qry.exec("INSERT INTO `cistem`.`entregas` (`IDTank`,`Tanque_Nombre`, `Volumen`, `Temperatrura`, `Volumen Entregado`, `Altura_de_Combustible`, `Altura_de_Agua`, `Fecha`,`Estado`) VALUES ('"+QString::number(IdTanque)+"','"+NomTank+"', '"+qry.value(3).toString()+"', '"+qry.value(4).toString()+"', '0', '"+qry.value(5).toString()+"', '"+qry.value(6).toString()+"', '"+qry.value(7).toString()+"', 'C.E.');");
        deliveryInProcess =1;
    }

}

void Tanque::SetConfigEntregas(int MinimoEntrega, int TiempoEntrega)
{
    Config_MinimoEntrega  = MinimoEntrega;
    Config_TiempoEntrega = TiempoEntrega;

}


void Tanque::deliveryProGaugeCountIncrement(){
 qDebug() << "Hola Desde Entregas";

    if(!Entregando){
        if(VolumenCon <= deliveryLastInventoryRead){
            deliveryCountDecrement++;
            if(deliveryCountDecrement >= 10)
            {
                deliveryCountDecrement = 0;
                deliveryCountIncrement = 0;
                deliveryInventoryStart = 0;
                deliveryMaxVolumeRead = deliveryLastInventoryRead;
            }
        }
            else {
                deliveryCountIncrement++;
                if(deliveryCountIncrement == 1.00){
                    qDebug () << "Hola El volumen :" << VolumenCon;
                    deliveryInventoryStart = VolumenCon;
                    deliveryInventoryStartfuelheight = AlturaTank;
                    deliveryInventoryStartwaterheight= NivelAgua;
                    Temperatura_inicial = Temperatura;
                }

                if(deliveryCountIncrement >= 30){
                    deliveryCountDecrement = 0;
                    deliveryCountIncrement = 0;
                    Entregando = true;

                QSqlQuery qry;
                     QString uo= "UPDATE `cistem`.`InicioEntrega` SET  `Tanque_Nombre`='"+NomTank+"', `Volumen`='"+QString::number(VolumenCon)+"', `Temperatura`='"+QString::number(Temperatura)+"', `Altura_de_Combustible`='"+QString::number(AlturaTank)+"', `Altura_de_Agua`='"+QString::number(NivelAgua)+"', `Fecha`='"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"', `Estado`='Inicio' WHERE `IDTank`='"+QString::number(IdTanque)+"';";
                    qDebug () << uo << "--------------Inico Entrega";
                     qry.exec(uo);
                     emit EntregaInicio(NomTank, IdTanque);

                    lbl_ProGaugeDeliveryInProccess->show();
                }
                deliveryCountDecrement = 0;

              }

        }
        else{
            if(deliveryMaxVolumeRead <= VolumenCon){
                deliveryMaxVolumeRead  = VolumenCon;
            }
            deliveryCountIncrement++;
            if(VolumenCon > deliveryLastInventoryRead){
                deliveryCountIncrement ++;
                if(deliveryCountDecrement >= 2 ){
                    deliveryCountIncrement=0;
                }
            }
            else {
                deliveryCountDecrement = 0;
            }
            if(deliveryCountIncrement >= (Config_TiempoEntrega * 60)){

                deliveryCountIncrement = 0;
                deliveryInProcess = 0;
                 QSqlQuery qry;

                double VolumenEntregado=  deliveryMaxVolumeRead -deliveryInventoryStart;
                if (VolumenEntregado >= Config_MinimoEntrega)
                {
                QString SeRealizoEntrega;
                qry.exec("SELECT * FROM `cistem`.`InicioEntrega` WHERE  IDTank='"+QString::number(IdTanque)+"';");
                while(qry.next()){
                    qry.exec("INSERT INTO `cistem`.`entregas` (`IDTank`,`Tanque_Nombre`, `Volumen`, `Temperatrura`, `Volumen Entregado`, `Altura_de_Combustible`, `Altura_de_Agua`, `Fecha`,`Estado`) VALUES ('"+qry.value(1).toString()+"','"+qry.value(2).toString()+"', '"+qry.value(3).toString()+"', '"+qry.value(4).toString()+"', '"+QString::number(VolumenEntregado)+"', '"+qry.value(5).toString()+"', '"+qry.value(6).toString()+"', '"+qry.value(7).toString()+"', 'Inicio');");
                }
                qry.exec("INSERT INTO `cistem`.`entregas` (`IDTank`,`Tanque_Nombre`, `Volumen`, `Temperatrura`, `Volumen Entregado`, `Altura_de_Combustible`, `Altura_de_Agua`, `Fecha`,`Estado`) VALUES ('"+QString::number(IdTanque)+"','"+NomTank+"', '"+QString::number(VolumenCon)+"', '"+QString::number(Temperatura)+"', '"+QString::number(VolumenEntregado)+"', '"+QString::number(AlturaTank)+"', '"+QString::number(NivelAgua)+"', '"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"', 'Fin');");

                qDebug()<< "  -------------------------------------";
                qDebug() << "---Hola ;P Se realizo una entrega---";
                qDebug()<< "  --------------------------------------";
                QString Cadena;
                Cadena.append("Tanque "
                              ""+NomTank+" "
                              "realizo entrega");
                emit Entrega(Cadena);

                }
                QString uo= "UPDATE `cistem`.`Entregando` SET  `Tanque_Nombre`='"+NomTank+"', `Volumen`='"+QString::number(VolumenCon)+"', `Temperatura`='"+QString::number(Temperatura)+"', `Altura_de_Combustible`='"+QString::number(AlturaTank)+"', `Altura_de_Agua`='"+QString::number(NivelAgua)+"', `Fecha`='"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"', `Status`='Entregado' WHERE `IDTank`='"+QString::number(IdTanque)+"';";

                qDebug() << uo;
                qry.exec(uo);

                Volumen_inicial =0;
                lbl_ProGaugeDeliveryInProccess->hide();
                deliveryInventoryStart = 0;
                Entregando = false;

            }
            else {
                QSqlQuery qry;
                QString CadenaTg = "UPDATE `cistem`.`Entregando` SET  `Tanque_Nombre`='"+NomTank+"', `Volumen`='"+QString::number(VolumenCon)+"', `Temperatura`='"+QString::number(Temperatura)+"', `Altura_de_Combustible`='"+QString::number(AlturaTank)+"', `Altura_de_Agua`='"+QString::number(NivelAgua)+"', `Fecha`='"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"', `Status`='Entregando' WHERE `IDTank`='"+QString::number(IdTanque)+"';";
           qDebug () << "Hola Desde la actualizacion" <<  CadenaTg ;
                qry.exec(CadenaTg);
            }
        }
    qDebug() << "----------------------";
    qDebug() <<"|"<< VolumenCon << deliveryLastInventoryRead << deliveryCountDecrement << deliveryCountIncrement << "|";
    qDebug() << "----------------------";
    deliveryLastInventoryRead = VolumenCon;

}
QString Tanque::ActualInventory(bool Inv)
{
    if(Desconectado)
    {
        if (Inv)
             return  "INSERT INTO `cistem`.`InventarioMin` (`IDTank`, `NombreTank`, `VolumenCo`, `Temperatura`, `AlturaC`, `AlturaA`, `Fecha`) VALUES ('"+QString::number(IdTanque)+"','"+NomTank+"','"+QString::number(VolumenCon)+"','"+QString::number(Temperatura)+"', '"+QString::number(AlturaTank)+"', '"+QString::number(NivelAgua)+"','"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"');";
            else {
             return  "INSERT INTO `cistem`.`inventario` (`IDTank`, `NombreTank`, `VolumenCo`, `Temperatura`, `AlturaC`, `AlturaA`, `Fecha`) VALUES ('"+QString::number(IdTanque)+"','"+NomTank+"','"+QString::number(VolumenCon)+"','"+QString::number(Temperatura)+"', '"+QString::number(AlturaTank)+"', '"+QString::number(NivelAgua)+"','"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"');";
        }
    }
}


QString Tanque::InventoryTurnos(int Turno)
{
    return  "INSERT INTO `cistem`.`InventarioTurnos` (`Turno`,`IDTank`, `NombreTank`, `VolumenCo`, `Temperatura`, `AlturaC`, `AlturaA`, `Fecha`) VALUES ('"+QString::number(Turno)+"','"+QString::number(IdTanque)+"','"+NomTank+"','"+QString::number(VolumenCon)+"','"+QString::number(Temperatura)+"', '"+QString::number(AlturaTank)+"', '"+QString::number(NivelAgua)+"','"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"');";
}

void Tanque::readPuntosCubicacion(){
    QSqlQuery qry;
    qry.exec("SELECT Punto, Altura, Volumen FROM cistem.tablacubicacion WHERE TanqueId ='"+QString::number(IdTanque)+"' ORDER BY id ASC");
    while(qry.next()){
        pointLevel[qry.value(0).toInt() - 1] = qry.value(1).toDouble() /100;
        pointVolume[qry.value(0).toInt() - 1] = qry.value(2).toDouble();
        qDebug() << pointLevel[qry.value(0).toInt() - 1] << pointVolume[qry.value(0).toInt() - 1];

    }
}

double Tanque::CalcularCubicacion(double height)
{
    // puntosCubicacion = 100;
    // height *= 100;
    qDebug() << "------------------------------------------------------------";
    qDebug() << puntosCubicacion;

    double res = 0;
    for(int i = 0; i < puntosCubicacion; i++){
        qDebug() << pointLevel[i] << height << "-----------------------";
        if(pointLevel[i] > height){
            if(i == 0){
                res = (height / (pointLevel[i] - 0)) * (pointVolume[i] - 0);

                qDebug() << "===================================";
                qDebug() << "res Cubico0:" << res << i << height;
                qDebug() << "====================================";
                break;

            }
            res = ((height - pointLevel[i - 1]) / (pointLevel[i] - pointLevel[i - 1])) * (pointVolume[i] - pointVolume[i - 1]) + pointVolume[i - 1];

            qDebug() << "i" << i;
            break;
        }
        if(i == (puntosCubicacion - 1)){
            res = pointVolume[i]; //qDebug() << "res Cubico SOBREPASA:" << res << i;
        }
    }
    return res;
}

int  Tanque::getPuntosCubicacion(){
    QSqlQuery qry;
    qry.exec("SELECT COUNT(IF(p_enable='1',1,null)) FROM cistem.tablacubicacion  WHERE TanqueId= '"+QString::number(IdTanque)+"';");
    if(qry.next()){
        return qry.value(0).toInt();
    }
    return 4;
}








