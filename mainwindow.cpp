#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <tanque.h>
#include <QLineEdit>
#include <sonda.h>
#include <dialog.h>
#include <QFrame>
#include <qmath.h>
#include "wiringPi.h"
#include <QProcess>
#include <calendario.h>
#include <QListView>
#include <QAbstractItemView>
#include <QListWidget>
#include <QScroller>
#include <frame.h>


#define SMenu 0
#define SHome 1
#define SSonda 2
#define STanque 3
#define STablaCub 4
#define SLogin 5
#define SHome2 6
#define STMaxi 7
#define SComunicacion 8
#define SVialarmas 9
#define Slimites 10
#define SEntregas 11
#define SComunicador 12
#define SMenuPub  13
#define sInventario 14
#define SReportes 15
#define SInventoryConfig 16
#define SPrinter 17
#define SStation 18
#define STurnos 19
#define SSensor_confi 20
#define SSensor_rep 21
#define SFecha_Hora 22
#define SFormato_FechaHora 23
#define SConfig_Entregas 24
#define SAlarmas 25
#define SProductos 26

#define CTRL 12
#define Buzzer 29
#define RX4 13
#define TX4 8
#define TX1 15
#define RX1 16
#define LD 7
#define SDA1 8
#define SCL1 9

#define IActual 0
#define IHistorico 1
#define ICortesEnergia 2
#define ITurnos 3

#define EActual 0
#define EHistorial 1
#define Eultima 2
#define SOH 0x01

 const int lenbuff1 = 1024;              // Longitud de buffer, Ajustar
 int xbuff1 = 0x00;                      // Índice: siguiente CHAR en cbuff
char cbuff1[lenbuff1];                  // Buffer
 char rcvchar1 = 0x00;                   // último carácter recibido
 double ProGaugeCapacidad = 40001.0;

//typedef  struct Turnos Turnos;

static struct Turnos
{
   int Turno;
   int Horas;
   int minutos;
}turnos[8];


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Time2 = new QTimer();
    Time3 = new QTimer();
    Gpio_timer = new QTimer();
    Inventory_Timer  = new QTimer();
    onesecond = new QTimer();

    wiringPiSetup();
    pinMode(CTRL,OUTPUT);
    pinMode(Buzzer,OUTPUT);
    pinMode(LD,OUTPUT);
    pinMode(Input[0],INPUT);
    pinMode(Input[1],INPUT);
    pinMode(Input[2],INPUT);
    pinMode(Input[3],INPUT);
    pinMode(Input[4],INPUT);
    pinMode(Input[5],INPUT);
    pinMode(Input[6],INPUT);
    pinMode(Input[7],INPUT);
    pinMode(Input[8],INPUT);
    pinMode(Input[9],INPUT);
    pinMode(Input[10],INPUT);
    pinMode(Input[11],INPUT);
    pinMode(Input[12],INPUT);
    pinMode(Input[13],INPUT);
    pinMode(Input[14],INPUT);
    pinMode(Input[15],INPUT);



    QFont Fonttitle;
    Fonttitle.setPointSize(48);
    Fonttitle.setBold(true);
    Lab_Title = new QLabel(ui->Btn_Barra_Estados);
    Lab_Title->setGeometry(200,20,500,50);
    Lab_Title->setStyleSheet("background-color: transparent;");
    Lab_Title->setText("Estado Sistema ");
    Lab_Title->setFont(Fonttitle);
    ui->lbl_ProGaugeDeliveryInProccess->hide();

    QFont FontReloj;
    FontReloj.setPointSize(35);
    FontReloj.setBold(true);
    Reloj= new QLabel(ui->Btn_Barra_Estados);
    Reloj->setGeometry(QRect(1350,12,800,50));
    Reloj->setStyleSheet("background-color: transparent;");
    Reloj->setFont(FontReloj);
    Reloj->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss ap"));
    estado_sistema(ui->Btn_Barra_Estados,"normal");

    QFont Font_btn;
    Font_btn.setPointSize(25);
    Font_btn.setBold(true);

    Btn_select_rango = new QPushButton(ui->Lab_Rango_Fecha);
    Btn_select_rango->setText("Select Range");
    Btn_select_rango->setStyleSheet("background-color: transparent;");
    Btn_select_rango->setGeometry(30,5,500,50);
    Btn_select_rango->setFont(Font_btn);

    Indicadores[0] = new QLabel(ui->Btn_Barra_Estados); // Alarma
    Indicadores[0]->setStyleSheet("background-color: transparent;");
    Indicadores[1] = new QLabel(ui->Btn_Barra_Estados); // warnings
    Indicadores[1]->setStyleSheet("background-color: transparent;");

    QFont fontAlarmas;
    fontAlarmas.setPointSize(22);
    fontAlarmas.setBold(true);

    Indicadores[0]->setGeometry(1000,10,300,25);
    Indicadores[0]->setFont(fontAlarmas);
    Indicadores[0]->setText("Alarmas:   "+QString::number(Alarmas)+"");

    Indicadores[1]->setGeometry(1000,40,300,35);
    Indicadores[1]->setFont(fontAlarmas);
    Indicadores[1]->setText("Warnings: "+QString::number(warnings)+"");

    connect(ui->Regresar_Home,&QPushButton::clicked,this,&MainWindow::on_Btn_Home_clicked);
    connect(ui->stackedWidget,&QStackedWidget::currentChanged,this,&MainWindow::Botones);
    connect(Btn_select_rango,&QPushButton::clicked,this,&MainWindow::btn_clicked);
    Maximizado = new Tanque(ui->Tanque_Maximizado,false);


    ConCombocol(ui->Combo_Color);

    deliveryProGaugeTimer = new QTimer(this);
    connect(Gpio_timer,SIGNAL(timeout()),this,SLOT(Leer_GPIO()));
    connect(onesecond,SIGNAL(timeout()),this,SLOT(everysecond()));
    onesecond->start(1000);
    Gpio_timer->start(2000);


    MainWindow::setFocus();
    ui->Line_AjusteAltura->installEventFilter(this);
    ui->Line_Licencia->installEventFilter(this);
    ui->line_Serie->installEventFilter(this);
    ui->Line_Usuario->installEventFilter(this);
    ui->Line_Contra->installEventFilter(this);
    ui->Line_Diametro->installEventFilter(this);
    ui->Line_Capacidad->installEventFilter(this);
    ui->Line_Nombre->installEventFilter(this);
    ui->Line_CodigoCombustible->installEventFilter(this);
   ui->dateEdit->installEventFilter(this);

    //ui->lineEdit->installEventFilter(this);
    //ui->stackedWidget->setCurrentIndex(1);
    ui->Lab_Titulo->setText("Inicio");
   // ocultar();

    puertoserie = new QSerialPort();
    Impresora = new QSerialPort();
    Time1 = new QTimer();

    Impresora->setBaudRate(QSerialPort::Baud9600);
    Impresora->setDataBits(QSerialPort::Data8);
    Impresora->setFlowControl(QSerialPort::NoFlowControl);
    Impresora->setParity(QSerialPort::NoParity);
    Impresora->setStopBits(QSerialPort::OneStop);
    connect(Impresora, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    connect(Impresora, &QSerialPort::readyRead, this, &MainWindow::leer_impresora);

    ui->stackedWidget->setCurrentIndex(3);
    fillPortsParameters();

    DB = QSqlDatabase::addDatabase("QMYSQL");

    // DB.setHostName("192.168.10.104");
    DB.setHostName("192.168.100.10");
   // DB.setHostName("192.168.100.216");
   // DB.setHostName("localhost");
    DB.setDatabaseName("cistem");
    DB.setPort(3306);
    DB.setUserName("abdiel");
    DB.setPassword("123456");

    if(!DB.open()){
        QMessageBox::critical(this, "Error", DB.lastError().text());
        return;
    }

    updateSettings();
    openSerialPort();


    consultaBD();
    Buscar_Tanques();
    Descargar();
    buscar_alarmas();
    conectar_signals();
    TimerConfigInventoryDB();
    DescargarFormatoFecha();
    consultar_sensores();

   qDebug () << "Sali de descargar";
   ui->Tab_entregas->horizontalHeader()->setVisible(true);
   ui->tableWidget->horizontalHeader()->setVisible(true);
   ui->tabla_incidentes->horizontalHeader()->setVisible(true);
   ui->Tabla_Inventario->verticalHeader()->setVisible(false);
  //ui->Tabla_Inventario->setColumnWidth(0,100);
  // ui->Tabla_Inventario->horizontalHeaderItem(0)->setText("Turno");
   ui->Tabla_Inventario->setColumnWidth(0,380);
   ui->stackedWidget->setCurrentIndex(SHome);
   ui->Btn_Guardar->setVisible(false);
   ui->Regresar->setVisible(false);
   ui->Btn_user->setVisible(true);
   ui->btn_menu->setVisible(true);
   ui->Btn_CambioTurno->setVisible(true);
   ui->SelecTank->setVisible(false);

   qDebug () << "Aqui ando";

   QListView * view = new QListView(ui->SelecTank);

   view->setWindowTitle(QString::fromLocal8Bit("demo"));
       view->resize(QSize(50, 350));

       view->setViewMode(QListView::ListMode);
       view->setFlow(QListView::TopToBottom);
       view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       view->setVerticalScrollMode(QListWidget::ScrollPerPixel);
       QScroller::grabGesture(view,QScroller::LeftMouseButtonGesture);

       view->setStyleSheet(
                               "QListView::item { "
                                "text-align:center;"
                               "font-size:46px; "
                                "width:400px;"
                               "height:200px;"
                               " border-radius: 100px;"
                               "border: 6px solid  gray ;"
                               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 white, stop:0.33 white, stop:0.34 white, stop:0.66 white, stop:0.67 rgba(255, 255, 0, 255), stop:1 rgba(255, 255, 0, 255));}"
                                );
  view->setFont(Fonttitle);
  ui->SelecTank->setView(view);
  //ui->SelecTank->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  for (int i = 0 ; i < ui->SelecTank->count() ; ++i) {
  ui->SelecTank->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
  };


qDebug() << "Iniciando --------------------------------";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Btn_Home_clicked()
{
    frame =SHome;
    ui->stackedWidget->setCurrentIndex(SHome);
    QObject::disconnect( combo_connect1 );
    QObject::disconnect( combo_connect2 );
    ui->Lab_Titulo->setText("Inicio");

    if (Maxi == true)
    { Maxi = false;

        qDebug() << "ando en el if de home";
        for (int i=0;i <= IDSerie-1; i++ )
        { tanques[i]->setTMaximizado(true);
            qDebug() << "entre al for";
            qDebug() << "Tanque" + QString::number(i)+ "true";
        }

    }
    frame = SHome;
}

void MainWindow::on_Btb_Sonda_clicked()
{   frame = SSonda;
    MainWindow::setFocus();
    ui->stackedWidget->setCurrentIndex(SSonda);
    ui->Lab_Titulo->setText("Sonda");

}

/*--------------------------------------------------------------------------------------------------------
 * En esta parte del codigo se maneja el stalcked del tanque, primeramente se le da el valor 2 a frame para
 * saber que estamos en el stalcked 2 a la hora de guardar, se le da un focus al mainwindos para que el teclado
 * no salte al entrar al stalk luego se limpia el combo para que no se vallan acumulando, se muestran los botones
 * que estaran disponibles en este stalk el guardar y regresar poniendo en true el setvisible, posteriomente
 * entramos al stalcked con un setCurrentIndex(3), se coloca el titulo tanque para saber en que stalck estamos
 * se agrega un item al combo vacio( por estetica a la hora de pulsa), luego se consulta la base de datos la tabla
 * sonda y con un while y el qry next nos traemos todas las sondas que an sido agregadas en el stalck sonda, en
 * en la parte inferior de esta funcion tenemos 2 slots el primero es para modificar el texto del tanque cuando
 * cuando modificamos el textedit, este aprobecha la señal generada por el tex edit llamada textChanged, dirigiendonos
 * a un slot en el cual se introduce el texto del textedit a nuestra clase tanque, en el segundo slot genera una
 * señal emitida por el combobox la cual nos dice que indice fue presionado, esta señal arroja un entero el cual
 * es aprobechado para introducir en un  switch  y mostrar lo indicado en cada uno de los casos.
 * -------------------------------------------------------------------------------------------------------*/

//Inicio del StackedTanque

void MainWindow::on_Btn_Tanque_clicked()
{
     frame = STanque;
     MainWindow::setFocus();
     ui->Combo_Sonda->clear();
     ui->ComboSeleccion->clear();
     ui->Combo_Producto->clear();
     ui->ComboSeleccion->setVisible(true);
     ui->stackedWidget->setCurrentIndex(STanque);
     ui->ComboSeleccion->addItem("General");
     ui->ComboSeleccion->addItem("Limites");

     combo_connect3 = QObject::connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
             [=](int index){

         switch (index)
          {
          case 0:ui->stackedWidget->setCurrentIndex(STanque); break;
          case 1: ui->stackedWidget->setCurrentIndex(Slimites); frame = Slimites;break;
         } });

     ui->Lab_Titulo->setText("Tanque");
     ui->Combo_Sonda->addItem(" ");

     QSqlQuery qry;
        qry.exec("SELECT Serie FROM `cistem`.`sonda` LIMIT 100 ;");
        while(qry.next())
        {
            ui->Combo_Sonda->addItem(qry.value(0).toString());
            qDebug() << qry.value(0);
        }
      qry.exec("SELECT Nombre  FROM cistem.Productos;");
      while (qry.next()) {
          ui->Combo_Producto->addItem(qry.value(0).toString());
      }


}
//Fin del StackedTanque

/*------------------------------------------------------------------------------------------------------------
 * Color Del ComboBox  en esta parte se selecciona el color utilizando ccs
 * y algunas propiedades del combo, el slot local se activa cada vez que abres el combo y agarra el indice
 * este indice es aprobechado y se introduce a un switch para saber que color fue presionado
 * dentro de cada case se le pasa el color corespondiente al indice mediante el metodo color.
 * tambien se cambia el color de la letra al combobox al color seleccionado (PD: Esto no se aprecia en la ras)
 *  *  * --------------------------------------------------------------------------------------------------------*/

// Inicio Del Combo Color

void MainWindow::ConCombocol(QComboBox *combo)
{
    QFont font3;
    font3.setPointSize(30);

    combo->setFont(font3);
    combo->addItem("");
    combo->addItem("");
    combo->addItem("");
    combo->addItem("");
    combo->addItem("");
    combo->setItemData(1, QBrush(Qt::lightGray),Qt::BackgroundColorRole);
    combo->setItemData(1, QBrush(Qt::lightGray),Qt::TextColorRole);
    combo->setItemData(2, QBrush(Qt::green),Qt::BackgroundColorRole);
    combo->setItemData(2, QBrush(Qt::green),Qt::TextColorRole);
    combo->setItemData(3, QBrush(Qt::yellow), Qt::BackgroundColorRole);
    combo->setItemData(3, QBrush(Qt::yellow), Qt::TextColorRole);
    combo->setItemData(4, QBrush(Qt::cyan), Qt::BackgroundColorRole);
    combo->setItemData(4, QBrush(Qt::cyan), Qt::TextColorRole);

    connect(combo, QOverload<int>::of(&QComboBox::activated), [=](int index){
        switch(index){
        case 1: combo->setStyleSheet(ColorTank("gray")); break;
        case 2: combo->setStyleSheet(ColorTank("green")); break;
        case 3: combo->setStyleSheet(ColorTank("yellow"));break;
        case 4: combo->setStyleSheet(ColorTank("cyan;")); break;
        default: break; }
    });

}
QString MainWindow::ColorTank(QString Color)
{
    return   "QComboBox{background-color: "+Color+";}"
             "QComboBox::drop-down {"
             "width: 40px;"
             "background-color: transparent;"
             "}"
             "QComboBox::down-arrow {"
             "margin: 5px 0px 0px 0px;"
             "width: 0;"
             "height: 0;"
             /*border: 2px solid #000;*/
             "border-top: 10px solid black;"
             "border-right: 10px solid "+Color+";"
             "border-bottom: 5px solid "+Color+";"
             " border-left: 10px  solid "+Color+";"
                                                 "}" ;

}// Final  Del Combo Color

/*-------------------------------------------------------------------------------------------------------------
 * Cada stalcked tiene su bonton de guardar y volver al momento de de entrar a un stalcked mediante el munu dentro del
 * slot del boton se asigna un  numero de frame para a la hora de guardar o regresar este entre al casecon el numero de frame
 * y gurade correctamente los datos del stalcked seleccionado
 * ------------------------------------------------------------------------------------------------------------------*/
//Inicio de boton de Guardar
void MainWindow::on_Btn_Guardar_clicked()
{
    qDebug() << frame <<"--------------------------------------";
    switch(frame){
    case SSonda : Guardar_Sonda(); break;
    case STanque: on_Btn_SaveTank_clicked(); break;
    case SComunicacion: Guardar_Comunicacion(); break;
    case Slimites : guardar_limites(); break;
    case SInventoryConfig: GuardarConfigInv(); break;
    case SStation: guardar_station(); break;
    case STurnos:  guardar_Turnos(); break;
    case SPrinter:  guardar_impresora(); break;
    case SSensor_confi: guardar_sensores(); break;
    case SFecha_Hora: Guardar_FechaHora();  break;
    case SFormato_FechaHora: guardar_FormatoFecha(); break;
    case SAlarmas : guardar_alarmas_config(); break;
    case SProductos:Guardar_ConfigProducto(); break;
    }
    insertar_incidente("Warning","System Setup Modified","user","0","1",false);
}
//Fin de boton de Guardar
/*--------------------------------------------------------------------------------------------------------------------
 * Este Timer es activado Cada Segundo para actualizar el reloj de la parte superior derecha de la pantalla
 * -----------------------------------------------------------------------------------------------------------------*/
//Inicio de timer que actualiza reloj
void MainWindow::Actualizar_Time()   //"dd-MM-yyyy HH:mm:ss AP"
{
    Reloj->setText(QDateTime::currentDateTime().toString(Formato_FH));
}//Fin del Timer que actualiza el reloj

/*--------------------------------------------------------------------------------------------------------------------
 * En este slot ira el login
 * -------------------------------------------------------------------------------------------------------------------*/
//Inicio del slot configuracion
void MainWindow::on_Btn_Config_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()  // boton log in
{
    frame =SMenu;
    QMessageBox msg;
    bool user = false, pas = false;
    QSqlQuery qry;
    QString consulta, usuario, pass;
    consulta = "SELECT * FROM cistem.usuarios";
    qry.exec(consulta);

    while (qry.next())
    {

        qDebug() << qry.value(0).toInt() << qry.value(1).toString() << qry.value(2).toString();
        usuario = qry.value(1).toString();
        pass = qry.value(2).toString();
        if(ui->Line_Usuario->text() == usuario)
        {
            user = true;
            qDebug() << "Usuario encontrado";
            if(ui->Line_Contra->text() == pass)
            {
                qDebug() << "Pasword encontrado";
                pas = true;
            }
            break;
        }
    }
    user = true; pas = true;
    if(user == true)
    {
        if(pas == true)
        {
            ui->stackedWidget->setCurrentIndex(SMenu);
            ui->Btn_Guardar->setVisible(false);
            //ui->Btn_Config->setVisible(false);
            ui->Btn_user->setVisible(false);
            ui->Lab_Titulo->setText("Configuracion");
            ui->Line_Usuario->clear();
            ui->Line_Contra->clear();

        }else{
            msg.setText("Contraña invalida");
            msg.exec();
        }

    }else {
        msg.setText(" Usuario y contraseña invalidos");
        msg.exec();
    }
    insertar_incidente("Warning","usuario a iniciado sesion","user","0","1",false);
}

void MainWindow::on_Btn_user_clicked()
{   frame = SLogin;
    MainWindow::setFocus();
    ui->stackedWidget->setCurrentIndex(SLogin);
    ui->Lab_Titulo->setText("Iniciar Secion");

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->Line_AjusteAltura && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde el filtro ajuste altura";
        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese un numero en el rango de 100 a 2000");
        //dlg->use_validator(val_1);
        //QValidator *val_1 = new QIntValidator(100,2000,this);
        dlg->validador(1,100,4000,0);
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_AjusteAltura->clear();
            ui->Line_AjusteAltura->setText(dlg->getDatos());
        }
        delete dlg;
        return true;
    }
    if(obj == ui->line_Serie && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde el filtro serie";
        QLabel *Label;
        Label = new QLabel(this );


        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese un numero en el numero de serie");
        //dlg->use_validator(val_1);
        //QValidator *val_1 = new QIntValidator(100,2000,this);
        dlg->maskarator(0,">99999;#");
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->line_Serie->clear();
            ui->line_Serie->setText(dlg->getDatos());
        }

        delete dlg;
        return true;
    }
    if(obj == ui->Line_Licencia && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde el filtro licencia";
        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese su licencia: AAAA-AAAA-AAAA-AAAA-AAAA");
        dlg->maskarator(0,">AAAA-AAAA-AAAA-AAAA;#");
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_Licencia->clear();
            ui->Line_Licencia->setText(dlg->getDatos());
        }
        delete dlg;
        return true;
    }
    if(obj ==ui->Line_Usuario && event->type() == QEvent::FocusIn)
    {
//        MainWindow::setFocus();
        qDebug() << "Hola desde el filtro usuario";
        Dialog *dlg = new Dialog(this);

        dlg->set_etiqueta("Ingrese su nombre de usuario");
        dlg->maskarator(0,"");
        dlg->setGeometry(30,315,1860,765);
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_Usuario->clear();
            ui->Line_Usuario->setText(dlg->getDatos());
        }
        delete dlg;
        MainWindow::setFocus();
        return true;
    }
    if(obj ==ui->Line_Contra && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde el filtro contrasena";
        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese su password");
        dlg->maskarator(3,"NNNNNNNNN;");
        ui->Line_Contra->setEchoMode(QLineEdit::Password);
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_Contra->clear();
            ui->Line_Contra->setText(dlg->getDatos());
        }
        delete dlg;
        return true;
    }
    if(obj ==ui->Line_Nombre && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde nombre de tanque";
        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese su nombre de usuario");
        dlg->maskarator(0,"");
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_Nombre->clear();
            ui->Line_Nombre->setText(dlg->getDatos());
        }
        delete dlg;
        return true;
    }
    if(obj == ui->Line_Diametro && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde medida 1";
        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese "+ ui->Lab_Medida1->text());
        //dlg->use_validator(val_1);
        //QValidator *val_1 = new QIntValidator(100,2000,this);
        dlg->validador(2,0,5000,2);
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_Diametro->clear();
            ui->Line_Diametro->setText(dlg->getDatos());
        }

        delete dlg;
        return true;
    }
    if(obj == ui->Line_Capacidad && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde medida 2";
        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese " + ui->Lab_Medida2->text());
        //dlg->use_validator(val_1);
        //QValidator *val_1 = new QIntValidator(100,2000,this);
        dlg->validador(2,0,50000,2);
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_Capacidad->clear();
            ui->Line_Capacidad->setText(dlg->getDatos());
        }

        delete dlg;
        return true;
    }
    if(obj == ui->Line_CodigoCombustible && event->type() == QEvent::FocusIn)
    {
        MainWindow::setFocus();
        qDebug() << "Hola desde medida 2";
        Dialog *dlg = new Dialog(this);
        dlg->setGeometry(30,315,1860,765);
        dlg->set_etiqueta("Ingrese Codigo de Combuestible");
        //dlg->use_validator(val_1);
        //QValidator *val_1 = new QIntValidator(100,2000,this);
        dlg->validador(2,0,50000,2);
        int res;
        res = dlg->exec();
        if(res == QDialog::Accepted)
        {
            ui->Line_CodigoCombustible->clear();
            ui->Line_CodigoCombustible->setText(dlg->getDatos());
        }

        delete dlg;
        return true;
    }

    //    if(obj == ui->lineEdit && event->type() == QEvent::FocusIn)
    //    {
    //        MainWindow::setFocus();
    //        qDebug() << "Hola desde medida 3";
    //        Dialog *dlg = new Dialog(this);
    //        dlg->set_etiqueta("Ingrese " + ui->Lab_Medida3->text());
    //        //dlg->use_validator(val_1);
    //        //QValidator *val_1 = new QIntValidator(100,2000,this);
    //        dlg->validador(2,0,2500,2);
    //        int res;
    //        res = dlg->exec();
    //        if(res == QDialog::Accepted)
    //        {
    //            ui->lineEdit->clear();
    //            ui->lineEdit->setText(dlg->getDatos());
    //        }

    //        delete dlg;
    //        return true;
    //    }
    return false;
}

void MainWindow::Leer_datos()
{
    qDebug() << "Hola Me llego un dato";
    RX[ProGaugeCount1]=true;
    QByteArray data;
    char dato;
    while( puertoserie->bytesAvailable())
    {
        data =  puertoserie->read(1);
        //qDebug() << data;
        dato = data.at(0);
        //qDebug()<< dato;
        addcbuff1(dato);
    }
}

void MainWindow::inicbuff1(){
    for (int i = 0; i < lenbuff1; i++){ cbuff1[i] = 0x00; } xbuff1 = 0x00;
}

void MainWindow::addcbuff1(char c){
    switch (c){
    case 0x0D: cbuff1[xbuff1++] = c; Protocolo(cbuff1); //qDebug() << cbuff1;
        break;
    case SOH: inicbuff1(); cbuff1[xbuff1++] = c; // qDebug() << c;
        break;
    default: cbuff1[xbuff1++] = c; break;
    }
}

void MainWindow::Guardar_Sonda()
{
    frame = SSonda;
    ui->Combo_Sonda->addItem(" ");
    Sondas[N] = new Sonda(this);
    Sondas[N]->setSerie(ui->line_Serie->text().toInt());
    Sondas[N]->setProtocolo(ui->Combo_protocolo->currentText().toInt());
    Sondas[N]->setFlotadores(ui->ComboFlotadores->currentText().toInt());
    Sondas[N]->setLicencia(ui->Line_Licencia->text());
    Sondas[N]->setAjuste_Altura(ui->Line_AjusteAltura->text().toInt());

    QSqlQuery qry;

    qry.exec("INSERT INTO `cistem`.`sonda` "
             "(`Serie`, `Protocolo`, `Flotadores`, `Licencia`, `fecha`) "
             "VALUES ('"+ui->line_Serie->text()+"', "
             "'"+ui->Combo_protocolo->currentText()+"', "
             "'"+ui->Combo_protocolo->currentText()+"', "
             "'"+ui->Line_Licencia->text()+"', "
             "'2020-01-14 00:03:00' );");

    ui->line_Serie->clear();
    ui->Combo_protocolo->setCurrentIndex(0);
    ui->ComboFlotadores->setCurrentIndex(0);
    ui->Line_Licencia->clear();
    ui->Line_AjusteAltura->clear();

    ui->stackedWidget->setCurrentIndex(SMenu);
    ui->Lab_Titulo->setText("Menu Principal");

    N++;
}

void MainWindow::Protocolo(QString cad)
{
    QSqlQuery qry;

    indice =0;

    qDebug() << "Cadena: " << cad;

    QString busqueda = cad.mid(0,5);

    for (int i = 0; i <= IDSerie; i++) {
        QString IDactual = ProGaugeId[i];
        qDebug() << "ProGaugeID" << ProGaugeId[i];
        if (IDactual == busqueda)
        {
            indice = i;
            break;
        }
    }


   disconnect(tanques[indice],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);
    connect(tanques[indice],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);

        if (cad[7]== "=" && cad[12]== "=" && cad[18] =="=")
        {
            tanques[indice]->setFocusPolicy(Qt::NoFocus);
            tanques[indice]->SetAltura(cad.mid(13,5).toDouble(), cad.mid(19,4).toDouble());
            tanques[indice]->SetTemperatura(cad.mid(9,3).toDouble());
        }


        if (cad[7]== "=" && cad[12]== "=" && cad[21] =="=" && cad.length() == 36)
        {

            tanques[indice]->SetAltura(cad.mid(13,8).toDouble(),cad.mid(22,8).toDouble());
            tanques[indice]->SetTemperatura(cad.mid(9,3).toDouble()/10);
            if(tanques[indice]->getIsConnected() == false){
                tanques[indice]->setIsConnected(true);
                insertar_incidente("Warning" ,tanques[indice]->GetNameTank() + " Conectado","user","1","1",false);
            }
            evaluar_limites(tanques[indice]);

            qDebug() << "Tamano de cadena:"<< cad.length();
        }
        inicbuff1();



    if(Maxi)
        Tanque_Maximisado(indice);
}

void MainWindow::Descargar()
{

    connect(deliveryProGaugeTimer, SIGNAL(timeout()), this, SLOT(deliveryProGaugeCountIncrement()));

    qDebug() << "Descargado Tanques .....";
    S=0;
    QSqlQuery qry;

    qry.exec("SELECT SistemaUnidades FROM cistem.Interfaz");
    while(qry.next()){
       SistemaUnidades = qry.value(0).toString();
       qDebug() << "////////////////////////////////////////";
       qDebug () << "Sistema De Unidades" << SistemaUnidades;
       qDebug() << "//////////////////////////////////////////";
    }

    qry.exec("SELECT COUNT(1) FROM `cistem`.`tanques` WHERE Configurado = 1;");
  while(qry.next())   {
   qDebug() << "Que esta pasando";
   if (qry.value(0).toInt() > 0)
   {  numerodetanques = qry.value(0).toInt();
       deliveryProGaugeTimer->start(1000/numerodetanques);
  qDebug () << "En este momento S:" << S;
    if(qry.exec("SELECT * FROM `cistem`.`tanques` where configurado = 1 ;"))
    {
        while (qry.next())
        {
            Geometrytank();
           // connect(tanques[S],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);
            connect(tanques[S],&Tanque::Entrega,this,&MainWindow::Qry_Entrega);
            ui->stackedWidget->setCurrentIndex(3);
           // ProGaugeId[S]=qry.value(1).toString();
            qDebug() <<   "ID del tanque dw:" <<  qry.value(1).toInt();
            tanques[S]->setFocusPolicy(Qt::NoFocus);
            tanques[S]->setIdTanque(qry.value(1).toInt());
            tanques[S]->setIshabilitado(qry.value(2).toBool());
            tanques[S]->SetnameTank(qry.value(3).toString());
            tanques[S]->setCodigoProducto(qry.value(4).toInt());
            tanques[S]->setPosTank(S);

            switch (qry.value(5).toInt())
            {
            case 1: tanques[S]->color("gray");   break;
            case 2: tanques[S]->color("green");  break;
            case 3: tanques[S]->color("yellow"); break;
            case 4: tanques[S]->color("cyan");   break;
            }

            tanques[S]->setCodigoCombustible(qry.value(6).toInt());
            tanques[S]->setID(qry.value(7).toString());
            tanques[S]->setAjusteAltura(qry.value(8).toDouble());
            tanques[S]->SetTankDiametro(qry.value(9).toDouble());
            tanques[S]->setCapacidad(qry.value(10).toDouble());
            tanques[S]->setTipo(qry.value(11).toInt());
            tanques[S]->setAngle(qry.value(12).toInt());
            tanques[S]->setFrombase(qry.value(13).toDouble());
            tanques[S]->setCoeficienteTermico(qry.value(14).toDouble());
            tanques[S]->setProducto(qry.value(15).toString());
            tanques[S]->SetConfigEntregas(qry.value(16).toInt(),qry.value(17).toInt());
            tanques[S]->setSistemaUnidades(SistemaUnidades);

            S++;
            ui->stackedWidget->setCurrentIndex(SHome);
        }
    }
    else
    {
        QMessageBox::critical(this, "Error",tr(qry.lastError().text().toUtf8()));
    }


    for (int i=0;i < S;i++) {
        if(qry.exec("SElECT * FROM cistem.limites WHERE Id_Taque = " +  QString::number(tanques[i]->getIdTanque())))
        {
           while(qry.next())
           {
               qDebug () << i;
               tanques[i]->SetVolMax(qry.value(2).toDouble());
               tanques[i]->SetProducto_Alto(qry.value(3).toDouble());
               tanques[i]->SetDesbordamiento(qry.value(4).toDouble());
               tanques[i]->SetNecesitaProducto(qry.value(5).toDouble());
               tanques[i]->SetProductoBajo(qry.value(6).toDouble());
               tanques[i]->SetAlarma_de_Agua(qry.value(7).toDouble());
               tanques[i]->SetAdvertencua_de_Agua(qry.value(8).toDouble());

               tanques[i]->DeliveryinProcces();


               qDebug() <<"Hola Desde Limites ;p"
                        <<qry.value(0).toString()
                        <<qry.value(1).toString()
                        <<qry.value(2).toString()
                        <<qry.value(3).toString()
                        <<qry.value(4).toString()
                        <<qry.value(5).toString()
                        <<qry.value(6).toString();
           }
        }
        else {
            qDebug() << "Valio Cabeza de Puerco";
        }
      }
    }
  }

         TCon = 0;

          if(qry.exec(" SELECT * FROM cistem.Turnos WHERE Habilitado = 1;"))
          {
              if  ( qry.next())
              {
                  IProg = true;
              while (qry.next())
              {

                 turnos[TCon].Turno = qry.value(1).toInt();
                 turnos[TCon].Horas = qry.value(2).toInt();
                 turnos[TCon].minutos = qry.value(3).toInt();

                    qDebug() << "TURNO"<<  qry.value(2).toInt() << "Horas" << qry.value(3).toInt() << "Munitos" << qry.value(4).toInt();
                    qDebug() << "ID" << TCon;
                    TCon ++;

              }

              }
              else {
                  IProg= false;
              }
          }



 }

void MainWindow::Geometrytank()
{

    ui->stackedWidget->setCurrentIndex(0);
    if(S<4)
        tanques[S]= new Tanque(ui->Home,true);
    else
        tanques[S] = new Tanque(ui->Home2,true);
     ui->stackedWidget->setCurrentIndex(3);
    switch(S){
    case 0:case 4: tanques[S]->Setgeometry(120,3,750,410);   break;
    case 1:case 5: tanques[S]->Setgeometry(960,3,750,410);  break;
    case 2:case 6: tanques[S]->Setgeometry(120,413,750,410); break;
    case 3:case 7: tanques[S]->Setgeometry(960,413,750,410);break;
    }
}

void MainWindow::Estados()
{
    switch(ProGaugeCount){
    case 0:
        connect(Time3,SIGNAL(timeout()),this,SLOT(SendCMD()));
        Time3->start(300); Time1->stop();
        disconnect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));
        break;
    case 1:  if (RX[ProGaugeCount1] == true ) { ProGaugeCount ++ ; RX[ProGaugeCount1]=false; }
        else if (intento[ProGaugeCount1] >= 5)
        {
            ProGaugeCount ++ ; RX[ProGaugeCount1]=false; intento[ProGaugeCount1] = 0;
            qDebug () << "Sonda Fuera de Linea: " << ProGaugeId[ProGaugeCount1];
            offlineSonda(ProGaugeId[ProGaugeCount1]);
            SondasOnline[ProGaugeCount1] = false;
        }
        else { intento[ProGaugeCount1] ++; ProGaugeCount++; qDebug () << intento; } break;
    case 2:  if(ProGaugeCount1 >= IDSerie-1) ProGaugeCount1= 0;
        else  { ProGaugeCount1++;} ProGaugeCount= 0;  break;
    }

}

void MainWindow::Enviar_qry(QString qry)
{
    QSqlQuery Qqry ;

    if (!Qqry.exec(qry))
    {
        QMessageBox::critical(this, "Error",tr(Qqry.lastError().text().toUtf8()));

        return;
    }
    else {
        Dialogo1 = new Frame(this);
        Dialogo1->setGeometry(0,0,1920,1080);
        Dialogo1->mensaje("Los Datos se guardaron \ncorrectamente", "Exito ","Informacion");
        Dialogo1->show();

        return;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    frame = SHome2;
    ui->stackedWidget->setCurrentIndex(SHome2);
}

void MainWindow::Tanque_Maximisado(int index)
{

    QString UnidadesVolumen;
    QString UnidadesDistancia;
    QString UnidadesTemperatura;

     if(tanques[indiceM]->getSistemaUnidades() == "Metrico")
     {
         UnidadesVolumen =  " Lts ";
         UnidadesDistancia= " MM ";
         UnidadesTemperatura = " ºC ";
     }
     else {
         UnidadesVolumen = " Gal ";
         UnidadesDistancia = " In ";
         UnidadesTemperatura = " ºF ";
     }

    if(!Maxi)
    {
        indiceM =index;
        ui->stackedWidget->setCurrentIndex(0);

          Maxi = true;
        qDebug()<< "indiceM"<< indiceM;
        // Maximizado->setPosTank(tanques[indiceM]->getPosTank());
        Maximizado->Setgeometry(15,5,1000,1000);

        Maximizado->setID(tanques[indiceM]->getID());
        ui->Lab_Titulo->setText(tanques[indiceM]->GetNameTank());
        Maximizado->SetnameTank("");
        Maximizado->color(tanques[indiceM]->GetColor());
        Maximizado->SetVolMax(tanques[indiceM]->GetVolMax());
        Maximizado->SetProducto_Alto(tanques[indiceM]->GetProducto_Alto());
        Maximizado->SetDesbordamiento(tanques[indiceM]->GetDesbordamiento());
        Maximizado->SetNecesitaProducto(tanques[indiceM]->GetNecesitaProducto());
        Maximizado->SetProductoBajo(tanques[indiceM]->GetProductoBajo());
        Maximizado->SetAlarma_de_Agua(tanques[indiceM]->GetAlarma_de_Agua());
        Maximizado->SetAdvertencua_de_Agua(tanques[indiceM]->GetAdvertencia_de_Agua());
        Maximizado->setTankTiempoEntrega(tanques[indiceM]->getTankTiempoEntrega());
        Maximizado->setTankMinimoEntrega(tanques[indiceM]->getTankMinimoEntrega());
        Maximizado->SetTankAltura(tanques[indiceM]->GetTanqueAltura());
        Maximizado->setIdTanque(tanques[indiceM]->getIdTanque());
     //   Maximizado->setAjusteAltura(tanques[indiceM]->getAjusteAltura());

        qDebug() << "ID" << Maximizado->getID();

        Maximizado->SetTankDiametro(tanques[indiceM]->GetTankDiametro());
        Maximizado->setTipo(tanques[indiceM]->getTipo());
        Maximizado->setCapacidad(tanques[indiceM]->getCapacidad());
        qDebug() << "*******************************************";
        qDebug() << "Capacidad " << tanques[indiceM]->getCapacidad();
        qDebug() << "*****************************************";
        qDebug()  << "Get Tipo " << tanques[indiceM]->getTipo();
        qDebug() << "******************************************";


    }

    Maximizado->setTMaximizado(false);
    tanques[indiceM]->setTMaximizado(false);
    Maximizado->SetAltura(tanques[indiceM]->GetAltura(),tanques[indiceM]->getNivelAgua());
    Maximizado->SetTemperatura(tanques[indiceM]->GetTemperatura());
    ui->Lab_VolMaxi->setText(QString::number(tanques[indiceM]->getVolumenCon()) + UnidadesVolumen);
    ui->Lab_AlturaMaxi->setText(QString::number(tanques[indiceM]->GetAltura())+ UnidadesDistancia);
    ui->Lab_VolAgua->setText(QString::number(tanques[indiceM]->getVolumenA()) + UnidadesVolumen);
    ui->Lab_AlturaAgua->setText(QString::number(tanques[indiceM]->getNivelAgua())+ UnidadesDistancia);
    ui->Lab_Maxitemp->setText(QString::number(tanques[indiceM]->GetTemperatura())+ UnidadesTemperatura);
    ui->Lab_Diametro->setText(QString::number(tanques[indiceM]->GetTankDiametro())+ UnidadesDistancia);
    ui->Lab_VolMaximo->setText(QString::number(tanques[indiceM]->GetVolMax()) + UnidadesDistancia);
    ui->Lab_DesbordamientomMaxi->setText(QString::number(tanques[indiceM]->GetDesbordamiento()));
    ui->Lab_ProductoAlto->setText(QString::number(tanques[indiceM]->GetProducto_Alto()));
    ui->Lab_NecesitaEntrega->setText(QString::number(tanques[indiceM]->GetNecesitaProducto()));
    ui->Lab_ProductoBajo->setText(QString::number(tanques[indiceM]->GetProductoBajo()));
    ui->Lab_AlarmaAguaAlta->setText(QString::number(tanques[indiceM]->GetAlarma_de_Agua()));
    ui->Lab_AdvertenciaAlarmaAlta->setText(QString::number(tanques[indiceM]->GetAdvertencia_de_Agua()));
    ui->Lab_TcVolumen->setText(QString::number(tanques[indiceM]->getVolumeTc()) + UnidadesVolumen);
    ui->Lab_Vacio->setText(QString::number(tanques[indiceM]->getVacio100())+ UnidadesVolumen);
    ui->Lab_Capacidad->setText(QString::number(tanques[indiceM]->getCapacidad())+ UnidadesVolumen);

    inicbuff1();
    frame = STMaxi;
    qDebug() << "Hola desde Tanque maximizado ;p";
    ui->stackedWidget->setCurrentIndex(STMaxi);

}

void MainWindow::on_Regresar_clicked()
{
    qDebug() << frame <<  "------------Estoy en Regresar ----------";
    MainWindow::setFocus();
    switch (frame) {
    // MENU DE CONFIGURACION
case SMenu : case SHome : case SSonda : case STanque : case STablaCub: case SLogin :
case SHome2 : case STMaxi : case SComunicacion: case SVialarmas : case Slimites : case SFecha_Hora:
case SComunicador :case SInventoryConfig: case SPrinter: case SStation: case SFormato_FechaHora:
case SProductos: case SConfig_Entregas: case STurnos: case SAlarmas: case SSensor_confi: frame= SMenu;break;

        //MENU PUBLICO
case  sInventario: case SReportes: case SEntregas : case SSensor_rep: frame= SMenuPub;  break;

  }
ui->stackedWidget->setCurrentIndex(frame);
}
void MainWindow::consultaBD()
{
    int i= 0;

    QSqlQuery qry;
    qry.exec("SELECT Serie_Sonda FROM `cistem`.`tanques` where configurado = 1;");
    while(qry.next())
    {
        ProGaugeId[i] = qry.value(0).toString();
        qDebug() << qry.value(0);
        i++;
         IDSerie = i;
        if (IDSerie > 0)
        {
            qDebug()<<"entre No se por que  pero id serie vale" << IDSerie;
            connect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));
            Time1->start(1500);
        }
        qDebug()<< IDSerie;
    }
}

void MainWindow::offlineSonda(QString offsonda)
{
    int indice= 0;
    QString busqueda = offsonda;
   qDebug () << "offlineSonda";
    for (int i = 0; i <= IDSerie; i++) {
        QString IDactual = ProGaugeId[i];
        qDebug() << "ProGaugeID" << ProGaugeId[i];
        qDebug() << "ProGaugeID" << "--------------------------------------------------";
        if (IDactual == busqueda) {  indice = i; break;  }
    }
    if(tanques[indice]->getIsConnected() == true){
        tanques[indice]->setIsConnected(false);
        insertar_incidente("Alarma",tanques[indice]->GetNameTank() + " Desconectado","user","1","1",false);
    }
    tanques[indice]->offline();
    qDebug ()  << "Id_Tanque:" << tanques[indice]->getIdTanque();
    disconnect(tanques[indice],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);
  }

void MainWindow::SendCMD()
{
    switch(ProGaugeCountCMD){
    case 0:
       // puertoserie->setDataTerminalReady(false);
       digitalWrite(CTRL,HIGH);
        ProGaugeCountCMD++;
        break;
    case 1:
        puertoserie->write(("M"+ProGaugeId[ProGaugeCount1]).toUtf8()+char(13)+ char(10));
        qDebug() << ("M"+ProGaugeId[ProGaugeCount1]).toUtf8();
        ProGaugeCountCMD++;
        break;

    case 2:
       // puertoserie->setDataTerminalReady(true);
        digitalWrite(CTRL,LOW);
        ProGaugeCountCMD = 0;
        ProGaugeCount++;
        Time3->stop();
        disconnect(Time3,SIGNAL(timeout()),this,SLOT(SendCMD()));
        connect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));

        QSqlQuery qry;
        qry.exec("SELECT COUNT(1) FROM cistem.tanques WHERE Configurado = 1;");
        while(qry.next())
        {
           Time1->start(1000 /(qry.value(0).toInt()));
        }
          break;
    }

}

void MainWindow::on_Btn_Comunicacion_clicked()
{
    frame = SComunicacion;
    ui->stackedWidget->setCurrentIndex(SComunicacion);
    ui->Lab_Titulo->setText("Comunicación");
    ui->Combo_Sonda->clear();
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->setVisible(true);
    ui->ComboSeleccion->addItem("Sonda");
    ui->ComboSeleccion->addItem("Comunicador");

    combo_connect4 = QObject::connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated), [=](int index){
        switch (index)
         {
         case 0:ui->stackedWidget->setCurrentIndex(SComunicacion); break;
         case 1: ui->stackedWidget->setCurrentIndex(SComunicador); frame = SComunicador;break;
        } });

}

void MainWindow::on_Btn_Barra_Estados_clicked()
{
    ui->Lab_Rango_Fecha->setText("Desde: " + QDateTime::currentDateTime().toString("yyyy-MM-dd") + " 00:00:00"+ "  Hasta: " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"      ");
    frame = SReportes;
    if (Maxi == true)
    { Maxi = false;
        qDebug() << "ando en el if de home";
        for (int i=0;i <= IDSerie-1; i++ )
        { tanques[i]->setTMaximizado(true);
            qDebug() << "entre al for";
            qDebug() << "Tanque" + QString::number(i)+ "true";
        }
    }

    ui->stackedWidget->setCurrentIndex(SReportes);
    //Btn_select_rango->setGeometry(0,0,0,0);
    Btn_select_rango->setText("Limpiar Activos");
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->addItem("Activos");
    ui->ComboSeleccion->addItem("Prioritarios");
    ui->ComboSeleccion->addItem("No Prioritarios");
    ui->ComboSeleccion->addItem("Historico");
    limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());
    QObject::disconnect( combo_connect1 );
    QObject::disconnect( combo_connect2 );

    combo_connect2 = QObject::connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
            [=](int index){

        switch (index)
         {
         case 0:rellenar_activos(Btn_select_rango); break;
         case 1:buscar_prioridad("1"); break;
         case 2:buscar_prioridad("0"); break;
         case 3:rellenar_incidentes(QDateTime::currentDateTime().toString("yyyy-MM-dd")+" 00:00:00",  // modificar aqui
                                    QDateTime::currentDateTime().toString("yyyy-MM-dd")+" "+
                                    QDateTime::currentDateTime().toString("HH")+ ":" +
                                    QDateTime::currentDateTime().toString("mm")+":00",3); break;
        } });

    ui->ComboSeleccion->setCurrentIndex(0);
    ui->ComboSeleccion->activated(0);
}

void MainWindow::Guardar_Comunicacion()
{
    closeSerialPort();

    QSqlQuery qry;
   QString A = "UPDATE `cistem`.`config_sonda` SET "
               " `Baudios`='"+ui->baudRateBox->currentText()+"',"
               " `Datos`='"+ui->dataBitsBox->currentText()+"',"
               " `Stop`='"+ui->parityBox->currentText()+"',"
               "  `Paridad`='"+ui->parityBox->currentText()+"',"
               " `FlowControl`='"+ui->flowControlBox->currentText()+"' "
               "WHERE  `ID`=0;";
   qDebug() << "Hola desde  Guardar Comunicacion" << A;
   qry.exec(A);

   updateSettings();
   openSerialPort();

}
void MainWindow::on_Btn_tabla_cubicacion_clicked()
{
    frame = STablaCub;
    ui->stackedWidget->setCurrentIndex(STablaCub);
    ui->Lab_Titulo->setText("Tabla De Cubicacion");
    enableCubicTableFields(false);
    enableCubicTableBtn(true,false,false);
    clearCubicTableFields();
    ui->Combo_CubTanque->activated(0);


}

void MainWindow::on_Btn_Alarmas_clicked()
{
    frame = SAlarmas;
    ui->stackedWidget->setCurrentIndex(SAlarmas);
}
/* A partir de esta seccion se configura el funcionamiento de la tabla de cubicacion
*/
void MainWindow::Buscar_Tanques()
{
    QString cadena;
    cadena = "SELECT Id_Taque FROM cistem.tanques;";
    QSqlQuery qry;
    qDebug() << qry.exec(cadena);
    while (qry.next())
    {
        Rellenar_combo_taques(qry.value(0).toString());
    }
}

void MainWindow::Rellenar_combo_taques(QString tanque_index)
{
    ui->Combo_CubTanque->addItem(tanque_index);
    ui->Combo_cub_generar->addItem(tanque_index);
    ui->Combo_tanque_limites->addItem(tanque_index);
}


void MainWindow::Rellenar_tabla_cubicacion(int Id_tanque)
{
    limpiar_tabla(ui->tableWidget,ui->tableWidget->rowCount());
    QString cadena;
    cadena.append("SELECT Punto, Altura, Volumen FROM cistem.tablacubicacion WHERE "
                  "TanqueId = '" + QString::number(Id_tanque) +"' AND p_enable = 1;");
    QSqlQuery qry;
    qDebug() << qry.exec(cadena);
    while (qry.next())
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(qry.value(0).toInt())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(qry.value(1).toDouble(), 'f', 3)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(qry.value(2).toDouble(), 'f', 3)));
        configurar_tablas(ui->tableWidget,2,1);
        //qDebug() << qry.value(0).toInt() << qry.value(1).toInt() << qry.value(2).toInt();
    }

}

void MainWindow::Rellenar_campos_cubicacion(QString p, QString a, QString v, bool b)
{
    if(b){
        ui->Line_Punto->setText(p);
        ui->Line_Altura->setText(a);
        ui->Line_Volumen->setText(v);
    }else {
        ui->Line_Puntos->setText(p);
        ui->Line_Altura_max->setText(a);
        ui->Line_Capacidad_2->setText(v);
    }
}
bool MainWindow::Validar_update_cubicacion(int punto, int tanque, double altura, double volumen)
{
    bool alt = false, vol = false;
    int p = 0;
    int i = 0;
    double altura_ant = 0, altura_pos = 0, volumen_ant = 0, volumen_pos = 0;
    if(punto == 1) p = 0;
    else p = punto - 2;
    QMessageBox msj;
    QSqlQuery qry;
    QString cadena;
    cadena.append("SELECT Altura, Volumen FROM cistem.tablacubicacion WHERE TanqueId "
                  "= '" + QString::number(tanque) + "' LIMIT " + QString::number(p) + " , 3;");
    qDebug() << qry.exec(cadena);
    while (qry.next())
    {
        switch (i)
        {
        case 0:
            if(punto == 1)
            {
                altura_ant = 0;
                volumen_ant = 0;
                qDebug() << altura_ant << volumen_ant << "ant";
            }else{
                altura_ant = qry.value(0).toDouble();
                volumen_ant = qry.value(1).toDouble();
                qDebug() << altura_ant << volumen_ant << "ant";
            }
            break;
        case 1: // presente
            break;
        case 2:
            altura_pos = qry.value(0).toDouble();
            volumen_pos = qry.value(1).toDouble();
            qDebug() << altura_pos << volumen_pos << "pos";
            break;
        default: return false;
        } qDebug() << qry.value(0).toDouble() << qry.value(1).toDouble() << "current cycle";
        i++;
    }
    if(altura > altura_ant && altura < altura_pos) alt = true;
    else{
        alt = false;
        msj.setText("altura fuera de rango");
        msj.exec();
    }
    if(volumen >volumen_ant && volumen < volumen_pos) vol = true;
    else{
        vol = false;
        msj.setText("volumen fuera de rango");
        msj.exec();
    }
    if(alt == true && vol == true) return true;
    else return false;
}

void MainWindow::on_Combo_CubTanque_activated(int index)
{
        Rellenar_tabla_cubicacion(index + 1);
}




void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    ui->tableWidget->selectRow(row);
    Rellenar_campos_cubicacion(
                ui->tableWidget->item(row,0)->text(),
                ui->tableWidget->item(row,1)->text(),
                ui->tableWidget->item(row,2)->text(),
                true
                );
}

void MainWindow::setEnabledBoton(QPushButton *btn, bool b)
{
    if(b){
        btn->setStyleSheet("QPushButton { color: white;	background-color: royalblue; border-width: 3px; border-radius: 10px; padding: 2px; }");
    } else{
        btn->setStyleSheet("QPushButton { color: white;	background-color: gray; border-width: 3px; border-radius: 10px; padding: 2px; }");
    }
    btn->setEnabled(b);
}

void MainWindow::enableCubicTableFields(bool v)
{
    ui->Line_Punto->setEnabled(false);
    ui->Line_Altura->setEnabled(v);
    ui->Line_Volumen->setEnabled(v);
}

void MainWindow::enableCubicTableBtn(bool a, bool b, bool c)
{
    setEnabledBoton(ui->Btn_Cub_Editar,a);
    setEnabledBoton(ui->Btn_Cub_Guardar,b);
    setEnabledBoton(ui->Btn_Cub_Cancelar,c);
}

void MainWindow::clearCubicTableFields()
{
    ui->Line_Punto->setText("");
    ui->Line_Altura->setText("");
    ui->Line_Volumen->setText("");
}

void MainWindow::guardar_limites()
{
    QSqlQuery qry;
    QString cadena;
    int tank_id =  0;
    cadena.append("UPDATE cistem.limites SET Volumen_maximo = '" + ui->Line_volumen_maximo->text() + "',"
                  " Producto_alto = '" + ui->Line_producto_alto->text() + "',"
                  " Desbordamiento = '" + ui->Line_desbordamiento->text() + "',"
                  " Entrega_necesaria = '" + ui->Line_limite_entrega->text() + "',"
                  " Producto_bajo = '" + ui->Line_producto_bajo->text() + "',"
                  " Alarma_agua_alta = '" + ui->Line_alarma_agua->text() + "',"
                  " Advertencia_agua_alta = '" + ui->Line_advertencia_agua->text() + "',"
                  " Fecha_modificacion = '" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+ "'"
                  " WHERE Id_Taque = '" + ui->Combo_tanque_limites->currentText() + "';");
    qDebug() << cadena;

    qDebug() << qry.exec(cadena);
    for (int i = 0; i <= IDSerie; i++) {
        if(tanques[i]->getIdTanque() == ui->Combo_tanque_limites->currentText().toInt())
        {
            tank_id = i;
              //  qDebug() << " posicion en el arreglo tanques[] de tanque ID :" << tank_id;
                break;
        }
    }

    tanques[tank_id]->SetVolMax(ui->Line_volumen_maximo->text().toDouble());
    tanques[tank_id]->SetProducto_Alto(ui->Line_producto_alto->text().toDouble());
    tanques[tank_id]->SetDesbordamiento(ui->Line_desbordamiento->text().toDouble());
    tanques[tank_id]->SetNecesitaProducto(ui->Line_limite_entrega->text().toDouble());
    tanques[tank_id]->SetProductoBajo(ui->Line_producto_bajo->text().toDouble());
    tanques[tank_id]->SetAlarma_de_Agua(ui->Line_alarma_agua->text().toDouble());
    tanques[tank_id]->SetAdvertencua_de_Agua(ui->Line_advertencia_agua->text().toDouble());

}

void MainWindow::rellenar_limites()
{
    QSqlQuery qry;
    QString cadena;
    cadena.append("SELECT Volumen_maximo, Producto_alto, Desbordamiento, "
                  "Entrega_necesaria, Producto_bajo, Alarma_agua_alta,"
                  "Advertencia_agua_alta FROM cistem.limites "
                  "WHERE Id_Taque = '" + ui->Combo_tanque_limites->currentText() + "';");
    qDebug() << cadena;
    qDebug() << qry.exec(cadena);
    while(qry.next())
    {
        ui->Line_volumen_maximo->setText(QString::number(qry.value(0).toDouble()));
        ui->Line_producto_alto->setText(QString::number(qry.value(1).toDouble()));
        ui->Line_desbordamiento->setText(QString::number(qry.value(2).toDouble()));
        ui->Line_limite_entrega->setText(QString::number(qry.value(3).toDouble()));
        ui->Line_producto_bajo->setText(QString::number(qry.value(4).toDouble()));
        ui->Line_alarma_agua->setText(QString::number(qry.value(5).toDouble()));
        ui->Line_advertencia_agua->setText(QString::number(qry.value(6).toDouble()));

    }
}

void MainWindow::evaluar_limites(Tanque *tanque)
{
    //  tanque->getVolumenCon()  // volumen tanque
    //  tanque->getVolumenA()    // volumen agua

    qDebug() << "Aqui se evalua el valumen: "  << tanque->getVolumenCon();
    int porcentaje = (static_cast<int>((tanque->getVolumenCon() * 100) / tanque->getCapacidad()));
    qDebug() << "porcentaje de volumen de tanque: " << porcentaje;

    if(porcentaje >= tanque->GetDesbordamiento()) insertar_incidente("Alarma",tanque->GetNameTank() + " Desbordado","user","1","1",true);
    if(porcentaje >= tanque->GetProducto_Alto()) insertar_incidente("Alarma",tanque->GetNameTank() + " Producto Alto","user","1","1",true);
    if(porcentaje <= tanque->GetNecesitaProducto()) insertar_incidente("Alarma",tanque->GetNameTank() + " Necesita producto","user","1","1",true);
    if(tanque->getVolumenCon() <= tanque->GetProductoBajo()) insertar_incidente("Alarma",tanque->GetNameTank() + " Producto Bajo","user","1","1",true);
    if(tanque->getVolumenA() >= tanque->GetAlarma_de_Agua()) insertar_incidente("Alarma",tanque->GetNameTank() + " Alarma Agua alta","user","1","1",true);
    if(tanque->getVolumenA() >= tanque->GetAdvertencia_de_Agua()) insertar_incidente("Warning",tanque->GetNameTank() + " Warning Agua alta","user","1","1",true);

    //if(porcentaje >= tanque->GetDesbordamiento()) insertar_incidente("Alarma",tanque->GetNameTank() + " Desbordado","user","1","1");
}

void MainWindow::insertar_incidente(QString tipo, QString Descripcion, QString usuario,QString Prioridad,QString Activo,bool filtro)
{   // ejemplo  insertar_incidente("Warning","System Setup Modified","user","0","1",false);
    QSqlQuery qry;
    QString cadena;
    cadena.append("INSERT INTO cistem.incidentes (Tipo_incidente, Descripcion, usuario, Fecha_incidente,Prioridad,Activo)"
                  " VALUES ('"+tipo+"', '"+Descripcion+"', '"+usuario+"', '"+ QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") +"','"+ Prioridad+"', '"+Activo+"');");

    qDebug() << cadena;
    if (validar_activos(tipo,Descripcion,usuario,filtro)){

        qry.exec(cadena);
        if(tipo == "Alarma")
        {
            Alarmas++;
            estado_sistema(ui->Btn_Barra_Estados,"alarma");
        }
        else if(tipo == "Warning")
        {
            warnings++;
            estado_sistema(ui->Btn_Barra_Estados,"warning");
        }
    }
    Indicadores[0]->setText("Alarmas:   "+QString::number(Alarmas)+"");
    Indicadores[1]->setText("Warnings: "+QString::number(warnings)+"");
    if(frame == SReportes && ui->ComboSeleccion->currentIndex() == 0)
    rellenar_activos(Btn_select_rango);
}

void MainWindow::rellenar_incidentes(QString T_inicial, QString T_Final,int index)
{
    QString cadena1;
    QSqlQuery qry;
    limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());
    //Btn_select_rango->setGeometry(30,5,200,40);
    Btn_select_rango->setText("Select Range");
    switch (index) {

    case 1:    cadena1 = ("SELECT * FROM cistem.incidentes WHERE "
                                  "Prioridad = '1' AND (Fecha_incidente "
                                  "BETWEEN ('"+T_inicial+"') AND ('"+T_Final+"'))"
                                  " ORDER BY Fecha_incidente DESC;"); break;

    case 2:    cadena1 = ("SELECT * FROM cistem.incidentes WHERE "
                                  "Prioridad = '0' AND (Fecha_incidente "
                                  "BETWEEN ('"+T_inicial+"') AND ('"+T_Final+"'))"
                                  " ORDER BY Fecha_incidente DESC;"); break;

    case 3:    cadena1 = ("SELECT * FROM cistem.incidentes WHERE Fecha_incidente "
                                  "BETWEEN ('"+T_inicial+"') AND ('"+T_Final+"')"
                                  " ORDER BY Fecha_incidente DESC;"); break;
    default:break;
    }
    qry.exec(cadena1);
   qDebug() << cadena1;
    while(qry.next())
    {
        ui->tabla_incidentes->insertRow(ui->tabla_incidentes->rowCount());
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 2, new QTableWidgetItem(qry.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 3, new QTableWidgetItem(qry.value(5).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 4, new QTableWidgetItem(qry.value(3).toString()));
        configurar_tablas(ui->tabla_incidentes,4,0);
    }

}

void MainWindow::limpiar_tabla(QTableWidget *tabla, int cont)
{
       tabla->clearContents();
        int i = cont; //ui->tabla_incidentes->rowCount() -1;
        for(int n = i; n >= 0;n--)
        {
            tabla->removeRow(n);
        }

}

void MainWindow::buscar_prioridad(QString priodidad)
{

    limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());
    QString cadena;
    QSqlQuery qry;
    Btn_select_rango->setText("Select Range");
    cadena.append ("SELECT * FROM cistem.incidentes WHERE Prioridad = '"+ priodidad +"' "
           "AND (Fecha_incidente BETWEEN ('"+QDateTime::currentDateTime().toString("yyyy-MM-dd") + " 00:00:00" +"') AND ('"+ QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+ "'))"
           " ORDER BY Fecha_incidente DESC;");
   qDebug() << cadena;
   qDebug() << qry.exec(cadena);

    while(qry.next())
    {
        ui->tabla_incidentes->insertRow(ui->tabla_incidentes->rowCount());
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 2, new QTableWidgetItem(qry.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 3, new QTableWidgetItem(qry.value(5).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 4, new QTableWidgetItem(qry.value(3).toString()));
        configurar_tablas(ui->tabla_incidentes,4,0);
    }
}

void MainWindow::rellenar_activos(QPushButton *btn)
{
    btn->setText("Limpiar Activos");
    limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());
    QString cadena;
    cadena.append("SELECT * FROM cistem.incidentes WHERE Activo = '1' ORDER BY Fecha_incidente DESC;");

    QSqlQuery qry;
    qry.exec(cadena);

    while (qry.next()) {
        ui->tabla_incidentes->insertRow(ui->tabla_incidentes->rowCount());
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 2, new QTableWidgetItem(qry.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 3, new QTableWidgetItem(qry.value(5).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 4, new QTableWidgetItem(qry.value(3).toString()));
        configurar_tablas(ui->tabla_incidentes,4,0);
    }


}

void MainWindow::limpiar_Activos()
{   qDebug() <<  "hola desde limpiar activos";
    QString cadena;
    QSqlQuery qry;
    qDebug() <<  ui->tabla_incidentes->rowCount();
//    for (int i = ui->tabla_incidentes->rowCount();i <= 0;i--) {
     for (int i = ui->tabla_incidentes->rowCount() - 1;i >= 0;i--) {
        cadena.append("UPDATE cistem.incidentes SET Activo = '0', "
                      "Clear_time = '"+ QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") +"' "
                      "WHERE "
                      "Tipo_incidente = '"+ ui->tabla_incidentes->item(i,0)->text() +"' AND "
                      "Descripcion = '" +ui->tabla_incidentes->item(i,1)->text() + "' AND "
                      "Fecha_incidente = '" +ui->tabla_incidentes->item(i,2)->text() + "' AND "
                      "usuario = '" +ui->tabla_incidentes->item(i,4)->text() + "';");
        qDebug() << cadena;
        qry.exec(cadena);
        cadena.clear();
    }
   limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());
    Alarmas = 0;
    warnings = 0;
    Indicadores[0]->setText("Alarmas:   "+QString::number(Alarmas)+"");
    Indicadores[1]->setText("Warnings: "+QString::number(warnings)+"");
    estado_sistema(ui->Btn_Barra_Estados,"normal");
}

void MainWindow::buscar_alarmas()
{
    QString cadena = "SELECT * FROM cistem.incidentes WHERE Activo = 1;";
    QSqlQuery qry;

    qry.exec(cadena);
    while(qry.next())
    {
      if(qry.value(1).toString() == "Alarma")
      {
          Alarmas++;
          estado_sistema(ui->Btn_Barra_Estados,"alarma");
      }
      else if(qry.value(1).toString() == "Warning")
      {
          warnings++;
          estado_sistema(ui->Btn_Barra_Estados,"warning");
      }

    }
        Indicadores[0]->setText("Alarmas:   "+QString::number(Alarmas)+"");
        Indicadores[1]->setText("Warnings: "+QString::number(warnings)+"");

}

bool MainWindow::validar_activos(QString tipo, QString Descripcion, QString usuario,bool filtro)
{
    bool valido = true;
    QString cadena = "SELECT * FROM cistem.incidentes WHERE Activo = 1;";
    QSqlQuery qry;

    if(filtro == true)
    {
        qry.exec(cadena);
        while(qry.next())
        {
          if(qry.value(1).toString() == tipo
             && qry.value(2).toString() == Descripcion
             && qry.value(3).toString() == usuario)
          {
               valido = false;
               break;
          }
          else valido = true;
        }
    }else valido = true;
    return valido;
}

/* Este Metodo es para colocar las bolitas de colores en su lugar
  Respecto al limite que le corresponde, la siguiente formula es la
  formula del semicirculo
 Referencia : https://www.youtube.com/watch?v=uDS4Q-QCU8M */

int MainWindow::X(int Y)
{
    return static_cast<int>(qSqrt(144400 - qPow(Y-380,2))+430);
}

int MainWindow::calcY(int y)
{
    return  800-(800 * y )/100;
}

void MainWindow::on_Btn_Cub_Editar_clicked()
{
    enableCubicTableFields(true);
    enableCubicTableBtn(false,true,true);
}

void MainWindow::on_Btn_Cub_Guardar_clicked()
{
    bool res = false;
    QSqlQuery qry;
    QString consulta;
    consulta.append("UPDATE cistem.tablacubicacion SET Altura = '" + ui->Line_Altura->text()+ "',"
                   " Volumen = '" + ui->Line_Volumen->text() + "' "
                   "WHERE Punto = '" + ui->Line_Punto->text() + "' "
                                                                                                                                                                                       "AND TanqueId = '" + QString::number(ui->Combo_CubTanque->currentIndex() + 1) + "';");
    res = Validar_update_cubicacion(ui->Line_Punto->text().toInt(),ui->Combo_CubTanque->currentIndex() + 1,ui->Line_Altura->text().toDouble(),ui->Line_Volumen->text().toDouble());
    if(res == true)
    {
        qDebug() << consulta;
        qDebug() << qry.exec(consulta);
        Rellenar_tabla_cubicacion(ui->Combo_CubTanque->currentIndex() + 1);
    }
    enableCubicTableFields(false);
    enableCubicTableBtn(true,false,false);
    clearCubicTableFields();
}

void MainWindow::on_Btn_Cub_Cancelar_clicked()
{
    enableCubicTableFields(false);
    enableCubicTableBtn(true,false,false);
    clearCubicTableFields();
}

void MainWindow::on_Combo_cub_generar_currentIndexChanged(int index)
{
    Rellenar_tabla_cubicacion(index +1);
    Rellenar_campos_cubicacion(ui->tableWidget->item(ui->tableWidget->rowCount()-1,0)->text(),
                               ui->tableWidget->item(ui->tableWidget->rowCount()-1,1)->text(),
                               ui->tableWidget->item(ui->tableWidget->rowCount()-1,2)->text(),
                               false);
}
void MainWindow::on_Btn_CubGenerar_clicked()
{
    QSqlQuery qry1,qry2;
    QString Cadena1, Cadena2;
    Cadena1.append("UPDATE cistem.tablacubicacion SET "
                   "Altura = 0, "
                   "Volumen = 0, "
                   "p_enable = 0 "
                   "WHERE TanqueId = " + QString::number(ui->Combo_cub_generar->currentIndex() +1) +";");
    qDebug() << Cadena1;
    qry1.exec(Cadena1);
    for (int i = 0;i <= 100; i++) {
        if(i <= ui->Line_Puntos->text().toInt()){
            Cadena2.append("UPDATE cistem.tablacubicacion SET "
                           "Altura = '" + QString::number(i * (ui->Line_Altura_max->text().toDouble()/ui->Line_Puntos->text().toDouble())) +"', "
                           "Volumen = '" + QString::number(i * (ui->Line_Capacidad_2->text().toDouble()/ui->Line_Puntos->text().toDouble())) +"', "
                           "p_enable = 1 "
                           "WHERE TanqueId = '" + QString::number(ui->Combo_cub_generar->currentIndex() +1) +"' "
                           "AND Punto = '" + QString::number(i) +"';");
            qDebug() << Cadena2;
            qry2.exec(Cadena2);
            Cadena2.clear();
        }
    }
    Rellenar_tabla_cubicacion(ui->Combo_cub_generar->currentIndex() + 1);
}

/* Termina la configuracion de la tabla de cubicacion*/

void MainWindow::on_Btn_Entregas_clicked()
{
    frame = SConfig_Entregas;
    ui->stackedWidget->setCurrentIndex(SConfig_Entregas);
    ui->Lab_Titulo->setText("Entregas");


}
void MainWindow::deliveryProGaugeCountIncrement(){
    if (TanqueActual == numerodetanques-1) { TanqueActual = 0;}
    else { TanqueActual++;
  }
      qDebug() << "Tanque Actual"<< TanqueActual;
    tanques[TanqueActual]->deliveryProGaugeCountIncrement();

}

void MainWindow::on_Btn_SaveTank_clicked()
{
    disconnect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));
//*** Configurar --- 0 Habilitado o Deshabilitado --- ***//
 int hab = 0;
 QSqlQuery qry;
 qry.exec("SELECT COUNT(1) FROM cistem.tanques WHERE Configurado = 1;");
 while(qry.next())  { S = qry.value(0).toInt();

     qDebug() << "La S en este moento es igual a" << S;

       for (int i = 0 ;i < S;i++) { tanques[i]->Delate(); qDebug()<< "Tanque Borrado:" <<i; }
 }

    if(ui->RHabilitado->isChecked()) {hab=1;}
    else { hab=0; }

      Enviar_qry( "UPDATE `cistem`.`tanques` SET "
                  " `Configurado`='"+QString::number(hab)+"',"
                  " `Nombre`='"+ui->Line_Nombre->text()+"',"
                  " `CodigoProducto`='"+ui->Line_Codigo_producto->text()+"',"
                  " `Color` ='"+QString::number(ui->Combo_Color->currentIndex())+"',"
                  " `CodigoCombustible`='"+ui->Line_CodigoCombustible->text()+"',"
                  " `Serie_Sonda`='"+ui->Combo_Sonda->currentText()+"', "
                  "`AjusteAltura`='"+ui->Line_AjusteAltura->text()+"', "
                  "`Diametro`='"+ui->Line_Diametro->text()+"', "
                  "`Capacidad`='"+ui->Line_Capacidad->text()+"', "
                  "`Tipo`='"+QString::number(ui->Combo_Tipo->currentIndex())+"', "
                  "`Angulo`='"+ui->Line_Angulo->text()+"', "
                  "`Frombase`='"+ui->Line_Distancia->text()+"', "
                  "`CoeficienteTermico`='"+ui->Line_coeficiente->text()+"', "
                  "`Producto`='"+ui->Combo_Producto->currentText()+"',"
                  "`MinimoEntrega`='"+ui->Line_MinimoEntrega_2->text()+"',"
                  "`TiempoEntrega`='"+ui->Line_TiempoEntrega_2->text()+"' WHERE  `Id_Taque`= '"+QString::number(ui->Combo_IdTanque->currentIndex())+"' ;");

      consultaBD();

      Descargar();

      frame = SMenu;
      ui->stackedWidget->setCurrentIndex(SMenu);
      ui->Lab_Titulo->setText("Menu Principal");

}

void MainWindow::on_btn_menu_clicked() {ui->Lab_Titulo->setText("Menu");frame = SMenuPub; ui->stackedWidget->setCurrentIndex(SMenuPub);}

void MainWindow::on_Btn_Inventario_clicked()
{
    ui->Lab_Titulo->setText("Inventario");
    ui->SelecTank->setVisible(true);
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->addItem("Actual");
    ui->ComboSeleccion->addItem("Historico");
    ui->ComboSeleccion->addItem("Cortes de Energía");
    ui->ComboSeleccion->addItem("Turnos");

    QObject::disconnect( combo_connect5 );

   combo_connect5 = QObject::connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
           [=](int index){

       if (frame == sInventario || frame == SEntregas ){
           if (index !=0 )
           {
               ui->SelecTank->setCurrentIndex(1);ui->SelecTank->activated(1);
               ui->SelecTank->setItemText(0, " ");
           }
           else{
               ui->SelecTank->setCurrentIndex(0); ui->SelecTank->activated(0);
               ui->SelecTank->setItemText(0, "ALL");
           }
       }}
);

    QObject::disconnect( combo_connect6 );
   combo_connect6 = QObject::connect(ui->SelecTank, QOverload<int>::of(&QComboBox::activated),
            [=](int index){
       if (index != 0 || ui->ComboSeleccion->currentIndex() == 0)
    InVentoryHistory(index,ui->ComboSeleccion->currentIndex(),false," ");

         });
    frame = sInventario; ui->stackedWidget->setCurrentIndex(sInventario);
    ui->ComboSeleccion->setCurrentIndex(0);
    ui->ComboSeleccion->activated(0);
}

void MainWindow::on_Btn_Reports_clicked()
{
    ui->Lab_Rango_Fecha->setText("Desde: " + QDateTime::currentDateTime().toString("yyyy-MM-dd") + " 00:00:00"+ "  Hasta: " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"      ");
    frame = SReportes;
    ui->stackedWidget->setCurrentIndex(SReportes);
    Btn_select_rango->setGeometry(30,5,500,50);
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->addItem("Activos");
    ui->ComboSeleccion->addItem("Prioritarios");
    ui->ComboSeleccion->addItem("No Prioritarios");
    ui->ComboSeleccion->addItem("Historico");
     QObject::disconnect(combo_connect5);
     QObject::disconnect( combo_connect1 );

    combo_connect1 = QObject::connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
            [=](int index){

        switch (index)
         {
         case 0:rellenar_activos(Btn_select_rango); break;
         case 1:buscar_prioridad("1"); break;
         case 2:buscar_prioridad("0"); break;
         case 3:rellenar_incidentes(QDateTime::currentDateTime().toString("yyyy-MM-dd")+" 00:00:00",  // modificar aqui
                                    QDateTime::currentDateTime().toString("yyyy-MM-dd")+" "+
                                    QDateTime::currentDateTime().toString("HH")+ ":" +
                                    QDateTime::currentDateTime().toString("mm")+":00",3); break;
        } });

    ui->ComboSeleccion->setCurrentIndex(3);
    ui->ComboSeleccion->activated(3);
}

void MainWindow::on_Btn_Entregas_or_clicked()
{

    frame = SEntregas;
    ui->Lab_Titulo->setText("Entregas");
    ui->stackedWidget->setCurrentIndex(SEntregas);

    ui->SelecTank->setVisible(true);
    ui->ComboSeleccion->setVisible(true);
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->addItem("Entrga Actual");
    ui->ComboSeleccion->addItem("Historial de Entragas");
    ui->ComboSeleccion->addItem("Ultima Entrega");
    ui->ComboSeleccion->activated(0);

    combo_connect8 = QObject::connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
                                      [=](int index){
        if ( index == 0)
        {
            ui->SelecTank->setCurrentIndex(1);ui->SelecTank->activated(1);
            ui->SelecTank->setItemText(0, " ");
        }
        else {
            ui->SelecTank->setCurrentIndex(1);ui->SelecTank->activated(1);
            ui->SelecTank->setItemText(0, " ");
        }

    });
    combo_connect9= connect(ui->SelecTank, QOverload<int>::of(&QComboBox::activated),
                            [=](int index){
        QString cadena;
        ui->Tab_entregas->clearContents();
        ui->Tab_entregas->setRowCount(0);
        switch (ui->ComboSeleccion->currentIndex()) {

        case 0: cadena.append("SELECT * FROM cistem.Entregando WHERE STATUS='Entregando';"); ComboSelect= EActual; break;
        case 1: cadena.append("SELECT * FROM `cistem`.`entregas` where IDTank = '"+ui->SelecTank->itemText(index)+"';"); ComboSelect= EHistorial; break;
        case 2: cadena.append("SELECT * FROM cistem.entregas  WHERE IDTank='"+ui->SelecTank->itemText(index)+"' ORDER BY Fecha DESC LIMIT 2;"); ComboSelect=Eultima; break;
        default: break;

        }


        QSqlQuery qry;
        qDebug() << "QRY:" << qry.exec(cadena);
        int cada2= 2;
        bool TituloTank= true;
        while (qry.next())
        {
            if (cada2 == 2)
            {
                if (TituloTank && ui->ComboSeleccion->currentIndex() != EActual)
                {
                    ui->Tab_entregas->horizontalHeaderItem(0)->setText("Inicio/Fin Fecha&Hora");

                    ui->Tab_entregas->insertRow(ui->Tab_entregas->rowCount());
                    ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1,0, new QTableWidgetItem(qry.value(2).toString()));
                    ui->Tab_entregas->insertRow(ui->Tab_entregas->rowCount());
                    ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 0, new QTableWidgetItem(qry.value(9).toString()+ " : "+ qry.value(8).toDateTime().toString("dd/MM/yyyy HH:mm:ss")));
                    TituloTank = false;
                    cada2 = 1;
                    ui->SelecTank->show();

                }else {
                    ui->Tab_entregas->insertRow(ui->Tab_entregas->rowCount());
                    ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 0, new QTableWidgetItem(qry.value(9).toString()+ " : "+ qry.value(8).toDateTime().toString("dd/MM/yyyy HH:mm:ss")));
                }


            }else {ui->Tab_entregas->insertRow(ui->Tab_entregas->rowCount());
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 0, new QTableWidgetItem(qry.value(9).toString()+ " : "+ qry.value(8).toDateTime().toString("dd/MM/yyyy HH:mm:ss")));
            }
            if(ui->ComboSeleccion->currentIndex() != 0){
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 1, new QTableWidgetItem(QString::number(qry.value(3).toInt())));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 2, new QTableWidgetItem(QString::number(qry.value(4).toInt())));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 3, new QTableWidgetItem(QString::number(qry.value(6).toInt())));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1,4 , new QTableWidgetItem(QString::number(qry.value(7).toInt())));
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 3)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
                //ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 5)->setTextAlignment(Qt::AlignCenter);
            }
            else {
                ui->Tab_entregas->horizontalHeaderItem(0)->setText("Nombre del Tanque");
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 0, new QTableWidgetItem(qry.value(2).toString()));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 1, new QTableWidgetItem(QString::number(qry.value(3).toInt())));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 2, new QTableWidgetItem(QString::number(qry.value(4).toInt())));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 3, new QTableWidgetItem(QString::number(qry.value(5).toInt())));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 4, new QTableWidgetItem(QString::number(qry.value(6).toInt())));
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 3)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
                //ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 5)->setTextAlignment(Qt::AlignCenter);
                ui->SelecTank->hide();

            }
            if(cada2==2 && !TituloTank && ui->ComboSeleccion->currentIndex() !=0 && qry.value(9) != "C.E.")
            {
                ui->Tab_entregas->insertRow(ui->Tab_entregas->rowCount());
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 0, new QTableWidgetItem("Volumen Entregado : "));
                ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 1 , new QTableWidgetItem(QString::number(qry.value(5).toInt())));
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
                ui->Tab_entregas->insertRow(ui->Tab_entregas->rowCount());
                cada2 = 0;
            }


            qDebug() << qry.value(0).toInt() << qry.value(1).toString() << qry.value(2).toInt() << qry.value(4).toInt();
            if (qry.value(9).toString() != "C.E.")
            {
                cada2++;
            }
        }

    });

}

void MainWindow::Botones()
{
    qDebug() << "Hola Desde Stalked:" << frame;

    ui->Btn_user->setVisible(false);
    ui->Regresar->setVisible(false);
    ui->Btn_Guardar->setVisible(false);
    ui->btn_menu->setVisible(false);
    ui->ComboSeleccion->setVisible(false);
    ui->SelecTank->setVisible(false);
    ui->Btn_CambioTurno->setVisible(false);

    QObject::disconnect( combo_connect1 ); // btn reporte
    QObject::disconnect( combo_connect2 ); // barra estados
    QObject::disconnect( combo_connect7 ); // reportes sensores
    if(frame != sInventario)QObject::disconnect(combo_connect5);//Combo de Inventario
    if(frame != SEntregas)
    {
        QObject::disconnect(combo_connect8);//Combo Entrega
        QObject::disconnect(combo_connect9);//Comto Select Tank en entrega
    }

    if(frame != STanque && frame != Slimites)  QObject::disconnect( combo_connect3 ); // tanque
    if(frame != SComunicacion && frame != SComunicador) QObject::disconnect( combo_connect4 ); // comunicador

    switch (frame) {

    case SHome: case SHome2: ui->Btn_CambioTurno->setVisible(true); ui->btn_menu->setVisible(true); ui->Btn_user->setVisible(true); break;
    case SMenuPub: case SLogin: case SMenu: case STMaxi: break;
    case STablaCub: case SVialarmas:case SEntregas: ui->Regresar->setVisible(true); break;
    case sInventario:ui->ComboSeleccion->setVisible(true);ui->SelecTank->setVisible(true);ui->Regresar->setVisible(true);break;
    case SComunicador:  case SComunicacion : case Slimites : case STanque: ui->ComboSeleccion->setVisible(true); ui->Regresar->setVisible(true);ui->Btn_Guardar->setVisible(true); break;
    default: ui->Regresar->setVisible(true);ui->Btn_Guardar->setVisible(true); break;
    case SReportes: case SSensor_rep:  ui->Regresar->setVisible(true); ui->ComboSeleccion->setVisible(true); break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    frame = SHome;
    ui->stackedWidget->setCurrentIndex(SHome);
}

void MainWindow::on_Combo_IdTanque_activated(int index)
{
    S=index;
    QSqlQuery qry;
    QString A  = "SELECT * FROM  cistem.tanques WHERE Id_Taque = "+QString::number(index)+";";
    qry.exec(A);
    qDebug () << A;
    while (qry.next())
    {
        if(qry.value(2).toBool()) ui->RHabilitado->setChecked(true);
        else  ui->RDeshabilitado->setChecked(true);
        ui->Line_Nombre->setText( qry.value(3).toString());
        ui->Line_Codigo_producto->setText(qry.value(4).toString());
        ui->Combo_Color->activated(qry.value(5).toInt());
        ui->Line_CodigoCombustible->setText(qry.value(6).toString());
        ui->Combo_Sonda->setCurrentIndex(ui->Combo_Sonda->findText(qry.value(7).toString(),Qt::MatchWrap));
        ui->Line_AjusteAltura->setText(qry.value(8).toString());
        ui->Line_Diametro->setText(qry.value(9).toString());
        ui->Line_Capacidad->setText(qry.value(10).toString());
        ui->Combo_Tipo->setCurrentIndex(qry.value(11).toInt());
        ui->Line_Angulo->setText(qry.value(12).toString());
        ui->Line_Distancia->setText(qry.value(13).toString());
        ui->Line_coeficiente->setText(qry.value(14).toString());
        ui->Combo_Producto->setCurrentText(qry.value(15).toString());
        ui->Line_TiempoEntrega_2->setText(qry.value(16).toString());
        ui->Line_MinimoEntrega_2->setText(qry.value(17).toString());
    }

}

void MainWindow::btn_clicked()
{   if(Btn_select_rango->text() == "Select Range")
    {
        QString cadena;
        Select_fechas *dlg_rango  = new Select_fechas(this);
        int res = dlg_rango->exec();
        if(res == QDialog::Accepted)
        {
            cadena.append("Desde: ");
            cadena.append(dlg_rango->getFecha_desde());
            cadena.append("  Hasta: ");
            cadena.append(dlg_rango->getFecha_hasta() + "      ");
            ui->Lab_Rango_Fecha->setText(cadena);
            rellenar_incidentes(dlg_rango->getFecha_desde(),dlg_rango->getFecha_hasta(),ui->ComboSeleccion->currentIndex());
        }
        delete dlg_rango;
    }//   Desde: 2021/29/4 00:00  HASTA: 2021/04/29 12:00
    else if(Btn_select_rango->text() == "Limpiar Activos"){
        limpiar_Activos();
    }
}

void MainWindow::Leer_GPIO()
{
    QString Gpio_status;
    QSqlQuery qry;
    QString cadena;

    for (int i = 0; i <= 15; i++) {

        if(S_config[i] == true ){
            S_input[i] = digitalRead(Input[i]);
            if(S_input[i] == false) // se activan en false
            {
                Gpio_status.append( "Sensor_");
                Gpio_status.append(QString::number(i+1));
                Gpio_status.append(": Activado");
               // qDebug() << "El sensor" << i+1 << " se Activo";

                    insertar_incidente("Alarma",Gpio_status,"user","1","1",true);
                    Indicadores[0]->setText("Alarmas:   "+QString::number(Alarmas)+"");
                    cadena.append("UPDATE cistem.Sensores SET Sensor_status = 'Activo' WHERE Sensor_num = '"+ QString::number(i) +"' ");
                    qry.exec(cadena);
            }else{
                cadena.append("UPDATE cistem.Sensores SET Sensor_status = 'Normal' WHERE Sensor_num = '"+ QString::number(i) +"' ");
                qry.exec(cadena);
            }
        }
        else {
            cadena.append("UPDATE cistem.Sensores SET Sensor_status = 'Deshabilitado' WHERE Sensor_num = '"+ QString::number(i) +"' ");
            qry.exec(cadena);
        }
        Gpio_status.clear();
        cadena.clear();
    }
}

void MainWindow::everysecond()
{
    Actualizar_Time();
    int IDTurno = 0;

    if (MinutoSec == 60){


           if( IProg == true ){
            int a;
            int  menor;
             menor  = 10000000;
            for (int i = 0; i < TCon; ++i)
            {
                qDebug () << "Indice Actual" << i;
                qDebug () << "Horas" << turnos[i].Horas;
                qDebug () << "Minutos" << turnos[i].minutos;
                QTime t1(turnos[i].Horas,turnos[i].minutos, 0, 0);
            QTime t2 =QTime::currentTime();
            qDebug () << "Tiempo 1" << t1 ;
            qDebug() << "Tiempo  Actual" << t2;
            int secs = t2.secsTo(t1); // secs should be equal to 15.
            secs = t1.secsTo(t2);     // secs should be equal to -15.
             a = secs;
            qDebug() << a;
                if (a < menor && a > 0  )
                { menor = a; qDebug() << "Turno Encontrado"  << turnos[i].Turno <<  "Hora" << turnos[i].Horas;
                TurnoEncontrado = turnos[i].Turno;
                IDTurno = i;
                }
            }
           if (CierreManual)
           {
               TurnoEncontrado = turnos[IDTurno +1].Turno;
           }

            if (TurnoActual == -1 || CierreManual)
            {
               Qry_Turnos(TurnoEncontrado);
               TurnoActual = TurnoEncontrado;
            }
            else if (TurnoActual !=TurnoEncontrado){
               Qry_Turnos(TurnoActual);
               Qry_Turnos(TurnoEncontrado);
               TurnoActual = TurnoEncontrado;  }
           }
            MinutoSec = 0;}

    else  MinutoSec ++;

    if (EntregaEnprocesoDurantecierre)
    {
        QSqlQuery qry;
        qry.exec("SELECT COUNT(STATUS) FROM cistem.Entregando WHERE STATUS='Entregando';");

        while(qry.next())
        {
            if (qry.value(0)== 0)
            {
                EntregaEnprocesoDurantecierre= false;
               CierreManual = true;
            }
        }

    }
}

void MainWindow::on_Combo_tanque_limites_currentIndexChanged(const QString &arg1)
{
    qDebug() << "tanque seleccionando en limites " << arg1;
    rellenar_limites();
}
void MainWindow::Qry_Entrega(QString SeRealizoEntrega)
{
    insertar_incidente("Alarma",SeRealizoEntrega,"user","1","1",true);
}

void MainWindow::on_Btn_inventarioConfig_clicked()
{ frame = SInventoryConfig; ui->stackedWidget->setCurrentIndex(SInventoryConfig);
    QSqlQuery qry;
    qry.exec("SELECT * FROM `cistem`.`configuracioninv` LIMIT 1000");
    int Inter=0;
    while(qry.next())
    {
        ui->Line_Intervalo->setText(qry.value(1).toString());

        for (int i = 0; i <= 13; i++) { if(Bitso[i] == qry.value(1).toString()){ Inter = i;break;} }
        SelecIntervalo = Inter;
        on_Line_Intervalo_textChanged(qry.value(1).toString());

      }

}

void MainWindow::on_BtnMasIntervalo_clicked()
{
    ui->Line_Intervalo->clear();
    if(SelecIntervalo == 12) SelecIntervalo= 0;
    else SelecIntervalo ++;
    ui->Line_Intervalo->setText(Bitso[SelecIntervalo]);
}

void MainWindow::on_Btn_IntervaloMenos_clicked()
{
    ui->Line_Intervalo->clear();
    if(SelecIntervalo == 0)  SelecIntervalo= 12;
    else SelecIntervalo --;
    ui->Line_Intervalo->setText(Bitso[SelecIntervalo]);
}

void MainWindow::on_BtnMasIntervalo_pressed()
{connect(Time1,&QTimer::timeout,this,&MainWindow::on_BtnMasIntervalo_clicked);}

void MainWindow::on_BtnMasIntervalo_released()
{disconnect(Time1,&QTimer::timeout,this,&MainWindow::on_BtnMasIntervalo_clicked);}

void MainWindow::on_Btn_IntervaloMenos_pressed()
{connect(Time1,&QTimer::timeout,this,&MainWindow::on_Btn_IntervaloMenos_clicked);}

void MainWindow::on_Btn_IntervaloMenos_released()
{disconnect(Time1,&QTimer::timeout,this,&MainWindow::on_Btn_IntervaloMenos_clicked);}

void MainWindow::on_Line_Intervalo_textChanged(const QString &arg1)
{
    if (SelecIntervalo >6)
    {
        ui->Lab_HoraInicio->setEnabled(true);
        ui->Lab_Horas->setEnabled(true);
        ui->Lab_Minutos->setEnabled(true);
        ui->WidgetHora->setIsSelect(false);

    }else{
         ui->Lab_HoraInicio->setEnabled(false);
         ui->Lab_Horas->setEnabled(false);
         ui->Lab_Minutos->setEnabled(false);
         ui->WidgetHora->setIsSelect(true);

    }
}

void MainWindow::InventoryTank()
{
    for (int i= 0;i<=numerodetanques-1;i++) {QSqlQuery qry; qry.exec(tanques[i]->ActualInventory(false)); }
}

void MainWindow::btn_Habilitado(QPushButton *Boton,bool hab )
{   if(hab) Boton->setStyleSheet("QPushButton{color:white;border-radius: 53px;border: 2px solid  gray ; background: royalblue; margin: 0px 0 0px 0; border-radius: 10px; }");
    else    Boton->setStyleSheet("QPushButton{color:white; border-radius: 53px;border: 2px solid  gray ;background: gray; margin: 0px 0 0px 0;  border-radius: 10px; }");
    Boton->setEnabled(hab);
}

void MainWindow::GuardarConfigInv()
{
    QSqlQuery qry;
    if(SelecIntervalo <+ 6)
    qry.exec("UPDATE `cistem`.`configuracioninv` SET `Intervalo`='"+Bitso[SelecIntervalo]+"', `Horas`='00', `Minutos`='00' WHERE  `Id`=1;");
    else
    qry.exec("UPDATE `cistem`.`configuracioninv` SET `Intervalo`='"+Bitso[SelecIntervalo]+"', `Horas`='"+ QString::number(SelecHora)+"', `Minutos`='"+QString::number(SelecMinutos)+"' WHERE  `Id`=1;");

}

void MainWindow::TimerConfigInventoryDB()
{
    Inventory_Timer->stop();
    disconnect(Inventory_Timer,&QTimer::timeout,this,&MainWindow::InventoryTank);
    connect(Inventory_Timer,&QTimer::timeout,this,&MainWindow::InventoryTank);
    QSqlQuery qry;
    qry.exec("SELECT * FROM `cistem`.`configuracioninv` LIMIT 1000;");

    while(qry.next())
    {
        int Inter=0;
        for (int i = 0; i <= 13; i++) {
            if(Bitso[i] == qry.value(1).toString())
            {
                Inter = i;qDebug() << " posicion en el arreglo tanques[] de tanque ID :" << i; break;
            }
        }
        qDebug() <<"QRY VALUE:" <<qry.value(1).toString();

        switch (Inter)
        {

//        QString Bitso [13]= {"Deshabilitado","5 Minutos","10 Minutos","15 Minutos","20 Minutos","30 Minutos",
//                       "1 Hora","2 Horas","4 Horas","6 Horas","8 Horas", "12 Horas", "24 Horas"};
        case 0:Inventory_Timer->stop(); break;
        case 1:Inventory_Timer->stop();Inventory_Timer->start(300000); break;//5 Min
        case 2:Inventory_Timer->stop();Inventory_Timer->start(600000); break;//10 Min
        case 3:Inventory_Timer->stop();Inventory_Timer->start(900000); break;//15 Min
        case 4:Inventory_Timer->stop();Inventory_Timer->start(1200000); break;//20 Min
        case 5:Inventory_Timer->stop();Inventory_Timer->start(1800000); break;//30 Min
        default: Inventory_Timer->stop();Inventory_Timer->start(3600000); break;//1 H
        case 7:Inventory_Timer->stop();Inventory_Timer->start(7200000); break;//2 H
        case 8:Inventory_Timer->stop();Inventory_Timer->start(14400000); break;//4H
        case 9:Inventory_Timer->stop();Inventory_Timer->start(21600000); break;//6H
        case 10:Inventory_Timer->stop();Inventory_Timer->start(28800000); break;//8H
        case 11:Inventory_Timer->stop();Inventory_Timer->start(43200000); break;//12H
        case 12:Inventory_Timer->stop();Inventory_Timer->start(86400000); break;//24H
        }

    }
}


void MainWindow::InVentoryHistory(int IDTank, int ComboInventory, bool Porfecha, QString QryFecha)
{
    limpiar_tabla(ui->Tabla_Inventario,ui->Tabla_Inventario->rowCount());
    QSqlQuery qry;
      if (!Porfecha){
        QString cadena;

        limpiar_tabla(ui->Tabla_Inventario,ui->Tabla_Inventario->rowCount());
      if(IDTank==0)
           cadena = ("SELECT * FROM `cistem`.`"+SelecionInventario[ComboInventory]+"` ORDER  BY  Fecha DESC;");
          else
        cadena = ("SELECT * FROM `cistem`.`"+SelecionInventario[ComboInventory]+"`  WHERE IDTank ='"+QString::number(IDTank)+"'ORDER  BY  Fecha DESC;");

      qry.exec(cadena);
      }
      else {
           qry.exec(QryFecha);
      }

      int cada2= 2;
int uno= 0;
int cada1=1;
int merma[]= {0,0};
int Cmerma=0;


       // qDebug() << cadena;
        while(qry.next())
        {
            if (IDTank == 0 || SelecionInventario[ComboInventory]== "InventarioMin" )
            {
                if(cada1== 1)
                {
                    ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                    ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(2).toString()));
                    cada1 =0;
                }
                 ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());

            }

           if (SelecionInventario[ComboInventory]== "InventarioCortes" )
             {
                  ui->Tabla_Inventario->setColumnWidth(0,650);

                 if (cada2 == 2){
                     if(Cmerma != 0 )
                     {
                     ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem("Cambio De Volumen Bruto"));

                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 1, new QTableWidgetItem(QString::number(merma[1]-merma[0])));
                     ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
                    }
            ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
            ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(2).toString()));
            cada2 =0;
            Cmerma =0;

                 }
                 ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(8).toString()+ "  :  "+ qry.value(7).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));

                 merma[Cmerma]= qry.value(3).toInt();
                 Cmerma++;
             }
             else if (SelecionInventario[ComboInventory]== "inventario" && IDTank !=0)
             {
                ui->Tabla_Inventario->setColumnWidth(0,380);
                ui->Tabla_Inventario->horizontalHeaderItem(0)->setText("Fecha/Hora");
                 if( uno == 0){
                 ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(2).toString()));
                          }
                 ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(7).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
             }

            else if(SelecionInventario[ComboInventory] == "InventarioTurnos")
             {

                 ui->Tabla_Inventario->setColumnWidth(0,100);
                 ui->Tabla_Inventario->horizontalHeaderItem(0)->setText("Turno");
                 ui->Tabla_Inventario->setColumnWidth(1,500);
                 ui->Tabla_Inventario->horizontalHeaderItem(1)->setText("Fecha de Turno");
                 ui->Tabla_Inventario->setColumnWidth(2,380);
                 ui->Tabla_Inventario->horizontalHeaderItem(2)->setText("Volumen de Combustible");
                 ui->Tabla_Inventario->setColumnWidth(3,280);
                 ui->Tabla_Inventario->horizontalHeaderItem(3)->setText("Temperatura");
                 ui->Tabla_Inventario->setColumnWidth(4,280);
                 ui->Tabla_Inventario->horizontalHeaderItem(4)->setText("Altura de Combustible");
                 ui->Tabla_Inventario->setColumnWidth(5,280);
                 ui->Tabla_Inventario->horizontalHeaderItem(5)->setText("Altura de Agua");

                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(8).toString()));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 1, new QTableWidgetItem(qry.value(7).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 2, new QTableWidgetItem(qry.value(3).toString()));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 3, new QTableWidgetItem(qry.value(4).toString()));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 4, new QTableWidgetItem(qry.value(5).toString()));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 5, new QTableWidgetItem(qry.value(6).toString()));
           }

             if (SelecionInventario[ComboInventory] != "InventarioTurnos")
             {
                 if (SelecionInventario[ComboInventory] == "InventarioCortes")
                 ui->Tabla_Inventario->setColumnWidth(0,650);
                 else {
                      ui->Tabla_Inventario->setColumnWidth(0,400);
                 }
                 if (SelecionInventario[ComboInventory] == "InventarioMin")
                 {

                     ui->Tabla_Inventario->horizontalHeaderItem(0)->setText("Volumen de Combustible");
                     ui->Tabla_Inventario->setColumnWidth(1,280);
                     ui->Tabla_Inventario->horizontalHeaderItem(1)->setText("Temperatura");
                     ui->Tabla_Inventario->setColumnWidth(2,380);
                     ui->Tabla_Inventario->horizontalHeaderItem(2)->setText("Altura de combustible");
                     ui->Tabla_Inventario->setColumnWidth(3,280);
                     ui->Tabla_Inventario->horizontalHeaderItem(3)->setText("Altura de Agua");
                     ui->Tabla_Inventario->setColumnWidth(4,280);
                     ui->Tabla_Inventario->horizontalHeaderItem(4)->setText("100% Vacio");
                     ui->Tabla_Inventario->setColumnWidth(5,280);
                     ui->Tabla_Inventario->horizontalHeaderItem(5)->setText("90% Vacio");

                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(3).toString()));
                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 1, new QTableWidgetItem(qry.value(4).toString()));
                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 2, new QTableWidgetItem(qry.value(5).toString()));
                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 3, new QTableWidgetItem(qry.value(6).toString()));
                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 4, new QTableWidgetItem(qry.value(7).toString()));
                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 5, new QTableWidgetItem(qry.value(8).toString()));

                     ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
                     ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 5)->setTextAlignment(Qt::AlignCenter);

                 }
                 else{

                 ui->Tabla_Inventario->horizontalHeaderItem(0)->setText("Fecha/Hora");
                 ui->Tabla_Inventario->setColumnWidth(1,380);
                 ui->Tabla_Inventario->horizontalHeaderItem(1)->setText("Volumen de Combustible");
                 ui->Tabla_Inventario->setColumnWidth(2,280);
                 ui->Tabla_Inventario->horizontalHeaderItem(2)->setText("Temperatura");
                 ui->Tabla_Inventario->setColumnWidth(3,380);
                 ui->Tabla_Inventario->horizontalHeaderItem(3)->setText("Altura de Combustible");
                 ui->Tabla_Inventario->setColumnWidth(4,280);
                 ui->Tabla_Inventario->horizontalHeaderItem(4)->setText("Altura de Agua");
                  ui->Tabla_Inventario->setColumnWidth(5,0);
                    ui->Tabla_Inventario->horizontalHeaderItem(5)->setText("");
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 1, new QTableWidgetItem(qry.value(3).toString()));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 2, new QTableWidgetItem(qry.value(4).toString()));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 3, new QTableWidgetItem(qry.value(5).toString()));
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 4, new QTableWidgetItem(qry.value(6).toString()));
                  ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
                 }
                 ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
                 ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
                 ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
                 ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 3)->setTextAlignment(Qt::AlignCenter);

                // ui->Tabla_Inventario->item(ui->Tabla_Inventario->rowCount() - 1, 5)->setTextAlignment(Qt::AlignCenter);

             }
             else
             {
                 ui->Tabla_Inventario->setColumnWidth(0,100);

                 if (cada2 == 2){
                     if(Cmerma != 0 )
                     {
                         ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                     }
                     else{
                     ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                     ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));

                     }
                     cada2 =0;
                     Cmerma =0;


                 }
                 ui->Tabla_Inventario->insertRow(ui->Tabla_Inventario->rowCount());
                 ui->Tabla_Inventario->setItem(ui->Tabla_Inventario->rowCount() - 1, 0, new QTableWidgetItem(qry.value(8).toString()+ "  :  "+ qry.value(7).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));


                 merma[Cmerma]= qry.value(3).toInt();
                 Cmerma++;
             }
               uno++;
            cada2++;
            cada1++;
        }

}

void MainWindow::estado_sistema(QPushButton *btn, QString estado)
{
    if(estado == "warning" && Alarmas <= 0)
    { btn->setStyleSheet("background-color: green; background-color: qconicalgradient(cx:0.680, cy:0, angle:19.3, stop:0 rgba(250, 220, 0, 250), stop:0.691897 rgba(250,200,0, 160), stop:0.691964  gray , stop:1 gray); color:white");}
    else if (estado == "alarma")
    {btn->setStyleSheet("background-color: green; background-color: qconicalgradient(cx:0.680, cy:0, angle:19.3, stop:0 rgba(255,0,0, 250), stop:0.691897 rgba(180,0,0, 160), stop:0.691964  gray , stop:1 gray); color:white"); }
    else if(estado == "normal")
    {  btn->setStyleSheet("background-color: green; background-color: qconicalgradient(cx:0.680, cy:0, angle:19.3, stop:0 rgba(0, 250, 0, 250), stop:0.691897 rgba(0,180,0, 160), stop:0.691964  gray , stop:1 gray); color:white"); }

}

void MainWindow::guardar_station()
{
    QSqlQuery qry;
    QString cadena;

    cadena.append("UPDATE cistem.printer SET "
                  "station_code = '"+ui->Line_StationCode->text()+"', "
                  "station_name = '"+ui->Line_StationName->text()+"', "
                  "usuario = '"+ui->Line_User->text()+"', "
                  "memo = '"+ui->Line_Memo->text()+"', "
                  "cb_station_code = '"+QString::number(ui->Combo_StationCode->currentIndex())+"', "
                  "cb_station_name = '"+QString::number(ui->Combo_StationName->currentIndex())+"', "
                  "cb_usuario = '"+QString::number(ui->Combo_User->currentIndex())+"', "
                  "cb_memo = '"+QString::number(ui->Combo_Memo->currentIndex())+"' "
                  "WHERE Id_station = 1;");

    qDebug() << cadena;
    qDebug() << qry.exec(cadena);
    frame = SMenu;
    ui->stackedWidget->setCurrentIndex(SMenu);
    ui->Lab_Titulo->setText("Menu Principal");
}

void MainWindow::guardar_Turnos()
{
    QSqlQuery qry;
   if (ui->Combo_MetodoCierre->currentIndex() == 0)
   {

   Turnohabilitado(ui->Turno1,"1");
   Turnohabilitado(ui->Turno2,"2");
   Turnohabilitado(ui->Turno3,"3");
   Turnohabilitado(ui->Turno4,"4");
   Turnohabilitado(ui->Turno5,"5");
   Turnohabilitado(ui->Turno6,"6");
   Turnohabilitado(ui->Turno7,"7");
   Turnohabilitado(ui->Turno8,"8");

   qry.exec("UPDATE `cistem`.`ConfigTurnos` SET `MetodoCierre`='Programado', `TiempoMuerto`='0', `MaxTurnos`='0' WHERE  `ID`=1;");

   }
   else{
     qry.exec("UPDATE `cistem`.`ConfigTurnos` SET `MetodoCierre`='Manual', `TiempoMuerto`='"+ui->LineTMuerto->text()+"', `MaxTurnos`='"+ui->LineNMT->text()+"' WHERE  `ID`=1;");
   }
   MetodoCierre = ui->Combo_MetodoCierre->currentIndex();
}

void MainWindow::Turnohabilitado(Butons *T,QString TID)
{     QSqlQuery qry;
    if(T->getIsHabilitado()) qry.exec("UPDATE `cistem`.`Turnos` SET `Hora`='"+T->getLineHorText()+"', `Minutos`='"+T->getLineMinText()+"', `Habilitado`=b'1' WHERE  `ID`="+TID+";");
    else qry.exec("UPDATE `cistem`.`Turnos` SET `Hora`='00', `Minutos`='00', `Habilitado`=b'0' WHERE  `ID`="+TID+";");
}

void MainWindow::guardar_FormatoFecha()
{

    disconnect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));
//*** Configurar --- 0 Habilitado o Deshabilitado --- ***//
 int hab = 0;
 QSqlQuery qry;
 qry.exec("SELECT COUNT(1) FROM cistem.tanques WHERE Configurado = 1;");
 while(qry.next())  { S = qry.value(0).toInt();

     qDebug() << "La S en este moento es igual a" << S;

       for (int i = 0 ;i < S;i++) { tanques[i]->Delate(); qDebug()<< "Tanque Borrado:" <<i; }
 }


qDebug() << "Modificando Formato fecha";

  //   qry.exec("UPDATE `cistem`.`FormatoFH` SET `Formato_Fecha`='sd', `Separador`='sd', `Formato_Hora`='sd' WHERE  `ID`=1;");
     qry.exec("UPDATE cistem.Interfaz SET `Formato_Fecha`= '"+ui->Combo_FormatoFecha->currentText()+"',"
                                          " `Separador` ='"+ui->Combo_FormatoSeparador->currentText()+"',"
                                          " `Formato_Hora`='"+ui->Combo_FormatoHora->currentText()+"' , "
                                          " `SistemaUnidades`='"+ui->Combo_SistemaUnidades->currentText()+"'"
                                          " WHERE ID='1';");

     DescargarFormatoFecha();
     consultaBD();

     Descargar();
}

void MainWindow::Qry_Turnos(int Turno)
{
    for (int i= 0;i<=numerodetanques-1;i++) {
        QSqlQuery qry;
        qry.exec(tanques[i]->InventoryTurnos(Turno));
        qDebug() << tanques[i]->InventoryTurnos(Turno);
        qDebug() << "-----------------------------------------------------------------------";
    }

}

void MainWindow::on_pushButton_5_clicked()
{
   close();
}



void MainWindow::on_Btn_Station_clicked()
{
    frame = SStation;
    ui->stackedWidget->setCurrentIndex(SStation);
}

void MainWindow::on_Btn_Turnos_clicked()
{
    ui->LineTMuerto->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray;}");
    ui->LineNMT->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray;}");
    ui->LineTMuerto->setEnabled(false);
    ui->LineNMT->setEnabled(false);
    frame = STurnos;
    ui->stackedWidget->setCurrentIndex(STurnos);
}
void MainWindow::on_Combo_StationCode_activated(int index)
{
    if(index == 1){
    ui->Line_StationCode->clear();
    ui->Line_StationCode->setEnabled(false);
    ui->Line_StationCode->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray ;}");
    }else{
    ui->Line_StationCode->setEnabled(true);
    ui->Line_StationCode->setStyleSheet("QLineEdit{border-radius: 10px;border: 2px solid  gray ;}");
    }
}

void MainWindow::on_Combo_StationName_activated(int index)
{
    if(index == 1){
    ui->Line_StationName->clear();
    ui->Line_StationName->setEnabled(false);
    ui->Line_StationName->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray ;}");
    }else{
     ui->Line_StationName->setEnabled(true);
     ui->Line_StationName->setStyleSheet("QLineEdit{border-radius: 10px;border: 2px solid  gray ;}");
    }
}

void MainWindow::on_Combo_User_activated(int index)
{
    if(index == 1){
    ui->Line_User->clear();
    ui->Line_User->setEnabled(false);
    ui->Line_User->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray ;}");
    }else{
     ui->Line_User->setEnabled(true);
     ui->Line_User->setStyleSheet("QLineEdit{border-radius: 10px;border: 2px solid  gray ;}");
    }
}

void MainWindow::on_Combo_Memo_activated(int index)
{
    if(index == 1){
    ui->Line_Memo->clear();
    ui->Line_Memo->setEnabled(false);
    ui->Line_Memo->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray ;}");
    }else{
    ui->Line_Memo->setEnabled(true);
    ui->Line_Memo->setStyleSheet("QLineEdit{border-radius: 10px;border: 2px solid  gray ;}");
    }
}

void MainWindow::on_Btn_SelecRange_clicked()
{

    QString cadena;
    Select_fechas *dlg_rango  = new Select_fechas(this);
    int res = dlg_rango->exec();
    if(res == QDialog::Accepted)
    {
        cadena.append("Desde: ");
        cadena.append(dlg_rango->getFecha_desde());
        cadena.append("  Hasta: ");
        cadena.append(dlg_rango->getFecha_hasta() + "      ");
        ui->Lab_Rango_Fecha->setText(cadena);
      //  rellenar_incidentes(dlg_rango->getFecha_desde(),dlg_rango->getFecha_hasta(),ui->ComboSeleccion->currentIndex());
       RellenarInventario(dlg_rango->getFecha_desde(), dlg_rango->getFecha_hasta(),ui->ComboSeleccion->currentIndex());
    }

    delete dlg_rango;
}

void MainWindow::RellenarInventario(QString FechaInicio, QString FechaFin, int index)
{
    QString cadena1;
    QSqlQuery qry;
    limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());

    switch (index) {

    case 1:    cadena1 = (//"SELECT * FROM cistem.inventario WHERE (Fecha BETWEEN ('2021-05-18 02:28:03') AND ('2021-05-19 23:18:53')) AND IDTank='1';");break;//
                          "SELECT * FROM cistem.inventario WHERE (Fecha BETWEEN "
                          "('"+FechaInicio+"') AND ('"+FechaFin+"')) AND IDTank='"+QString::number(ui->SelecTank->currentIndex())+"' "
                          "ORDER BY Fecha_incidente DESC;"); break;

    case 2:    cadena1 = ( "SELECT * FROM cistem.InventarioCortes WHERE (Fecha BETWEEN "
                           "('"+FechaInicio+"') AND ('"+FechaFin+"')) AND IDTank='"+QString::number(ui->SelecTank->currentIndex())+"'"
                           " ORDER BY Fecha_incidente DESC;"); break;


    case 3:    cadena1 = ("SELECT * FROM cistem.InventarioTurnos WHERE (Fecha BETWEEN "
                          "('"+FechaInicio+"') AND ('"+FechaFin+"')) AND IDTank='"+QString::number(ui->SelecTank->currentIndex())+"' "
                          "ORDER BY Fecha_incidente DESC;"); break;
    default:break;
    }

    InVentoryHistory(ui->SelecTank->currentIndex(), ui->ComboSeleccion->currentIndex(),true,cadena1);
    qDebug() << cadena1;

}

void MainWindow::on_Btn_sensor_conf_clicked()
{
    frame = SSensor_confi;
    ui->stackedWidget->setCurrentIndex(SSensor_confi);
    ui->Combo_sensor_dir->activated(0);
    ui->Combo_sensor_dir->setCurrentIndex(0);
    ui->Combo_sensor_dir->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->Combo_sensor_dir->view()->setAlternatingRowColors(true);
}

void MainWindow::on_Btn_sensor_rep_clicked()
{
    frame = SSensor_rep;
    ui->stackedWidget->setCurrentIndex(SSensor_rep);
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->addItem("Sensor Status");
    ui->ComboSeleccion->addItem("Sensor History");
    ui->ComboSeleccion->addItem("Sensor by period");

    combo_connect7 = QObject::connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
            [=](int index){
        switch (index)
         {case 0:  rellenar_tabla_sensores(1); break;
          case 1:  rellenar_tabla_sensores(2); break;
          case 2:  rellenar_tabla_sensores(3); break;
         }    });
        ui->ComboSeleccion->activated(0);

}
void MainWindow::on_Combo_impresora_activated(int index)
{   Q_UNUSED(index);
    buscar_impresora();
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{   QSqlQuery qry;
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), Impresora->errorString());
        Impresora->close();
        ui->Rb_deshabilitado->setChecked(true);
        btn_Habilitado(ui->Btn_impresion_prueba,false);
        qry.exec("UPDATE cistem.Impresoras SET impresora_config = 0 , impresora_status = 0 WHERE Id = 1;");
        buscar_impresora();
        insertar_incidente("Warning","Impresora desconectada","user","0","1",false);

    }
}

void MainWindow::on_Btn_impresion_prueba_clicked()
{   qDebug() << "imprimiendo prueba";
    QByteArray prueba;
    prueba.append(0x1b);
    prueba.append(0x40);

    prueba.append(0x12);
    prueba.append(0x54);
    if(Impresora->isOpen())
    {   if(papel_out() == false) Impresora->write(prueba);
    }
}

void MainWindow::on_Btn_Impresora_clicked()
{
    frame = SPrinter;
    ui->stackedWidget->setCurrentIndex(SPrinter);
    ui->Line_controlador->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray;}");
    ui->Line_ubicacion->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray;}");
    buscar_impresora();
    consultar_impresora();
}

bool MainWindow::papel_out()
{
    bool papel_out = true;
    QByteArray cmd2;
    cmd2.append(0x1D);
    cmd2.append(0x72);
    cmd2.append(0x01);
    if(Impresora->isOpen())
    {   qDebug() << " mandando primer dato************************************************";

       Impresora->write(cmd2);
      while(Impresora->waitForReadyRead(250))
       {
          if(sensor_papel){
              papel_out = false;
          }else{
              papel_out = true;
              QMessageBox::critical(this, tr("Critical Error"),"No hay papel");
              insertar_incidente("Warning","Impresora sin papel","user","0","1",true);
          }
       }
    }
    return papel_out;
}

void MainWindow::guardar_sensores()
{
    QSqlQuery qry;
    QString cadena;
    QString config;
    QString status;
    if(ui->Rb_sensor_habilitado->isChecked()){
        config = "1";
        S_config[ui->Combo_sensor_dir->currentIndex()] = true;
    }
    else if (ui->Rb_sensor_deshabilitado->isChecked()){
        config = "0";
        status =  "Deshabilitado";
        S_config[ui->Combo_sensor_dir->currentIndex()] = false;
    }
    cadena.append("UPDATE cistem.Sensores SET "
                  "Sensor_label = '"+ ui->Line_Sensor_conf->text() +"', "
                  "Sensor_model = '"+ui->Combo_sensor_model->currentText()+"', "
                  "Sensor_cat = '"+ui->Combo_sensor_cat->currentText()+"', "
                  "Sensor_confi = '"+config+"', "
                  "Sensor_model_index = '"+QString::number(ui->Combo_sensor_model->currentIndex())+"', "
                  "Sensor_cat_index = '"+QString::number(ui->Combo_sensor_cat->currentIndex())+"', "
                  "Sensor_status = '"+status+"' "
                  "WHERE Sensor_num = '"+ ui->Combo_sensor_dir->currentText() +"';");
    qry.exec(cadena);
    qDebug() << cadena;
}

void MainWindow::consultar_sensores(int index)
{
    QSqlQuery qry;
    QString cadena;
    cadena.append("SELECT * FROM cistem.Sensores WHERE Sensor_num = '"+ QString::number(index) +"';");
    qry.exec(cadena);
    while(qry.next())
    {
        if(qry.value(5).toInt() == 0){
            ui->Rb_sensor_deshabilitado->setChecked(true);
        }else {
            ui->Rb_sensor_habilitado->setChecked(true);
        }
        ui->Line_Sensor_conf->setText(qry.value(2).toString());
        ui->Combo_sensor_model->setCurrentIndex(qry.value(7).toInt());
        ui->Combo_sensor_cat->setCurrentIndex(qry.value(8).toInt());
    }
}

void MainWindow::consultar_sensores()
{
    QSqlQuery qry;
    QString cadena;
    cadena.append("SELECT * FROM cistem.Sensores;");
    qry.exec(cadena);
    while(qry.next())
    {
        if(qry.value(5).toInt() == 1) S_config[qry.value(1).toInt()] = true;
        else S_config[qry.value(1).toInt()] = false;
    }
}

void MainWindow::rellenar_tabla_sensores(int caso)
{
    QSqlQuery qry;
    QString cadena;
    limpiar_tabla(ui->Tabla_Sensores,ui->Tabla_Sensores->rowCount());
    cadena.append("SELECT * FROM cistem.Sensores;");
    qry.exec(cadena);
    while(qry.next())
    {
        switch (caso) {
        case 1:
            ui->Tabla_Sensores->insertRow(ui->Tabla_Sensores->rowCount());
            ui->Tabla_Sensores->setItem(ui->Tabla_Sensores->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
            ui->Tabla_Sensores->setItem(ui->Tabla_Sensores->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));
            ui->Tabla_Sensores->setItem(ui->Tabla_Sensores->rowCount() - 1, 2, new QTableWidgetItem(qry.value(6).toString()));
//            ui->Tabla_Sensores->item(ui->Tabla_Sensores->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
//            ui->Tabla_Sensores->item(ui->Tabla_Sensores->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
//            ui->Tabla_Sensores->item(ui->Tabla_Sensores->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
            configurar_tablas(ui->Tabla_Sensores,2,2);
            break;
        case 2: break;
        case 3: break;
        }
    }
}

void MainWindow::conectar_signals()
{
    connect(ui->Combo_sensor_dir, QOverload<int>::of(&QComboBox::activated),[=](int index){ consultar_sensores(index);  });


 int por1 = static_cast<int>((100 * tanques[indiceM]->GetVolMax())/38439.85);
 int y = static_cast<int>(430 * por1/38439.85);
 int y1 = calcY(static_cast<int>(tanques[indiceM]->GetProducto_Alto()));
 int y2 = calcY(static_cast<int>(tanques[indiceM]->GetDesbordamiento()));
 int y3 = calcY(static_cast<int>(tanques[indiceM]->GetNecesitaProducto()));
 int por = static_cast<int>(100 * tanques[indiceM]->GetProductoBajo()/38439.85);
 int y4 = static_cast<int>((430 * por) /38439.85);

connect(ui->tabWidget_2, QOverload<int>::of(&QTabWidget::currentChanged),
      [=](int index){
  switch (index) {

  case 0:
      ui->PuntoVolMax->setGeometry(200,200,50,50);
      ui->Bola_ProductoAlto->setGeometry(200,200,50,50);
      ui->Bola_Desbordamiento->setGeometry(200,200,50,50);
      ui->Bola_NecesitaEntrega->setGeometry(200,300,50,50);
      ui->Bola_ProductoBajo->setGeometry(200,350,50,50);
      ui->Bola_AlarmaAgua->setGeometry(200,380,50,50);
      ui->Bola_Advertencia_Agua->setGeometry(200,400,50,50);


      qDebug()<< "Index Cambio 0"; break;
   case 1:

      ui->PuntoVolMax->setGeometry(X(y),y,50,50);
      ui->Bola_ProductoAlto->setGeometry(X(y1),y1,50,50);
      ui->Bola_Desbordamiento->setGeometry(X(y2),y2,50,50);
      ui->Bola_NecesitaEntrega->setGeometry(X(y3),y3,50,50);
      ui->Bola_ProductoBajo->setGeometry(X(430-y4),430-y4,50,50);
      ui->Bola_AlarmaAgua->setGeometry(X(380),380,50,50);
      ui->Bola_Advertencia_Agua->setGeometry(X(400),400,50,50);

      qDebug() << "Index Cambio 1"; break;
  default:
      qDebug() << "Entre en el default"; break;

  }

});


}

void MainWindow::guardar_alarmas_config()
{

}

void MainWindow::configurar_tablas(QTableWidget *tabla,int n_colum,int n_tabla)
{
    QFont Font;
    Font.setPointSize(26);
    Font.setBold(true);
    tabla->setFont(Font);
    tabla->horizontalHeader()->setFixedHeight(70);                  // altura de header horizontal
    tabla->setRowHeight(tabla->rowCount() - 1,70);                  // altura de fila
    tabla->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);  // aliniacion centrada del vertical header

    for (int i = 0;i <= n_colum; i++) {
        tabla->item(tabla->rowCount() - 1, i)->setTextAlignment(Qt::AlignCenter);
    }

    switch (n_tabla) {
        case 0:
        tabla->setColumnWidth(0,150);
        tabla->setColumnWidth(1,600);
        tabla->setColumnWidth(2,400);
        tabla->setColumnWidth(3,400);
        tabla->setColumnWidth(4,150);

            if(ui->ComboSeleccion->currentIndex() == 0)
            {
                tabla->setSelectionMode(QAbstractItemView::SingleSelection);
                tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
            }
            else{
                tabla->setSelectionMode(QAbstractItemView::NoSelection);
                tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
            }
        break;

        case 1:
        tabla->setColumnWidth(0,300);
        tabla->setColumnWidth(1,300);
        tabla->setColumnWidth(2,300);

        break;

    default: break;
    }

}


void MainWindow::guardar_impresora()
{
    QString cadena;
    QString config;
    QString status;
    QSqlQuery qry;
    if(ui->Rb_habilitado->isChecked()){
        config = "1";
        if(Impresora->open(QIODevice::ReadWrite)){
            status = "1";
            btn_Habilitado(ui->Btn_impresion_prueba,true);
            qDebug() << "impresora conectada";
        }else status = "0";
    }
    else if(ui->Rb_deshabilitado->isChecked()){
        if(Impresora->isOpen()){
           Impresora->close();
           status = "0";
           btn_Habilitado(ui->Btn_impresion_prueba,false);
           qDebug() << status;
        }
        config = "0";
        ui->Btn_impresion_prueba->setEnabled(false);
    }
    cadena.append("UPDATE cistem.Impresoras SET Impresora_label = '"+ ui->Line_Etiqueta->text() +"', "
                                               "impresora_config = '"+config+"', "
                                               "impresora_status = '"+status+"' "
                                               "WHERE ID = 1;");
    qDebug() << cadena;
    qry.exec(cadena);

}

void MainWindow::buscar_impresora()
{

    QStringList puertos;
    QStringList controlador;
    QStringList descripcion;
    QStringList ubicacion;

    foreach (const QSerialPortInfo &puerto_info, QSerialPortInfo::availablePorts()) {
       puertos << puerto_info.portName();
       descripcion << puerto_info.description();
       controlador << puerto_info.manufacturer();
       ubicacion << puerto_info.systemLocation();
    }
    QString program = "/bin/sh";
    QStringList cadenas;
    QStringList arguments;
    arguments << "-c"<< "sudo dmesg | grep -i tty";
    QProcess *myProcess = new QProcess();
    myProcess->start(program, arguments);
    myProcess->waitForFinished();
    QByteArray output = myProcess->readAll();
    int j = 0;
    while ((j = output.indexOf("usb 1-1.1", j)) != -1) {
        cadenas << output.mid(j,output.indexOf('\n',j)-j);
        ++j;
    }
    if(cadenas.last().contains("pl2303") && puertos.contains(cadenas.last().mid(cadenas.last().indexOf("ttyUSB"),7))){
       portname = cadenas.last().mid(cadenas.last().indexOf("ttyUSB"),7);
       for (int i=0 ; i < puertos.size();i++) {
           if(portname == puertos[i]) {
               ui->Line_ubicacion->setText(ubicacion[i]);
               ui->Line_controlador->setText(descripcion[i]);
               break;
           }
       }
       ui->Combo_impresora->clear();
       ui->Combo_impresora->addItem("Impresora " + impresora_name);
       qDebug() << portname;
       Impresora->setPortName(portname);
       ui->Rb_habilitado->setEnabled(true);
    }else{
       qDebug() << " impresora no encontrada";
       ui->Combo_impresora->clear();
       ui->Combo_impresora->addItem("Impresora no encontrada");
       ui->Line_ubicacion->clear();
       ui->Line_controlador->clear();
       ui->Rb_habilitado->setEnabled(false);
       ui->Rb_deshabilitado->setChecked(true);
    }
}

void MainWindow::consultar_impresora()
{
    QString cadena;
    QSqlQuery qry;
    cadena.append("SELECT * FROM cistem.Impresoras;");
    qDebug() << cadena;
    qDebug() << qry.exec(cadena);
    while(qry.next()){
        ui->Line_Etiqueta->setText(qry.value(1).toString());
        if(qry.value(2).toInt() == 1){
            ui->Rb_habilitado->setChecked(true);
            btn_Habilitado(ui->Btn_impresion_prueba,true);
            if(!Impresora->isOpen()) Impresora->open(QIODevice::ReadWrite);
        }
        else {
            ui->Rb_deshabilitado->setChecked(true);
            btn_Habilitado(ui->Btn_impresion_prueba,false);
            if(Impresora->isOpen()) Impresora->close();
        }
    }
}

void MainWindow::leer_impresora()
{    qDebug() << "leyendo impresora******************************************************";

     QByteArray papel;
      papel[0] = 0x60;
       QByteArray data;
        data = Impresora->readAll();
         qDebug() << "data from leer impresosa" << data << "//////////////////////////////////////////////////////////////";
          if(data == papel) sensor_papel = true;
          else {
              sensor_papel = false;
          }

}

void MainWindow::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void MainWindow::updateSettings()
{
      QSqlQuery qry;
      qry.exec("SELECT * FROM cistem.config_sonda;");

      while (qry.next())

      {
          qDebug() << qry.value(1)<< "Baudios888888888";
    if (ui->baudRateBox->currentIndex() == 4) {
        m_currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        qDebug() << "Baudios3333300" << ui->baudRateBox->findText("9600",Qt::MatchWrap);
        m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
       ui->baudRateBox->itemData(ui->baudRateBox->findText(qry.value(1).toString(),Qt::MatchWrap)).toInt());
    }
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
    ui->dataBitsBox->itemData(ui->dataBitsBox->findText(qry.value(1).toString(),Qt::MatchWrap)).toInt());

    m_currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
    ui->dataBitsBox->itemData(ui->dataBitsBox->findText(qry.value(4).toString(),Qt::MatchWrap)).toInt());
    m_currentSettings.stringParity = ui->parityBox->currentText();

    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
    ui->dataBitsBox->itemData(ui->dataBitsBox->findText(qry.value(3).toString(),Qt::MatchWrap)).toInt());

    m_currentSettings.stringStopBits = ui->stopBitsBox->currentText();
      }

    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
          ui->flowControlBox->itemData(ui->flowControlBox->findText(qry.value(5).toString(),Qt::MatchWrap)).toInt());

    m_currentSettings.stringFlowControl = ui->flowControlBox->currentText();

      }

void MainWindow::openSerialPort()
{
    //puertoserie->setPortName("ttyUSB0");
    puertoserie->setPortName("ttyAMA3");
    puertoserie->setBaudRate(m_currentSettings.baudRate);
    puertoserie->setDataBits(m_currentSettings.dataBits);
    puertoserie->setFlowControl(m_currentSettings.flowControl);
    puertoserie->setParity(m_currentSettings.parity);
    puertoserie->setStopBits(m_currentSettings.stopBits);
    puertoserie->open(QIODevice::ReadWrite);
    connect(puertoserie, &QSerialPort::readyRead, this, &MainWindow::Leer_datos);
}

void MainWindow::closeSerialPort()
{
    if (puertoserie->isOpen())
    {
           puertoserie->close();
           disconnect(puertoserie, &QSerialPort::readyRead, this, &MainWindow::Leer_datos);
    }

}

void MainWindow::DescargarFormatoFecha()
{
    QSqlQuery qry;
    qry.exec("SELECT *From cistem.Interfaz  WHERE ID='1';");

    while (qry.next())
    {
        Formato_Fecha = qry.value(1).toString();
        Separador_Fecha = qry.value(2).toString();
        Formato_Hora = qry.value(3).toString();

        Formato_FH = Formato_Fecha + " " +  Formato_Hora ;
        Formato_FH.replace("_",Separador_Fecha);

        ui->Combo_FormatoFecha->setCurrentIndex(ui->Combo_FormatoFecha->findText(qry.value(1).toString(),Qt::MatchWrap));
        ui->Combo_FormatoSeparador->setCurrentIndex(ui->Combo_FormatoSeparador->findText(qry.value(2).toString(),Qt::MatchWrap));
        ui->Combo_FormatoHora->setCurrentIndex(ui->Combo_FormatoHora->findText(qry.value(3).toString(),Qt::MatchWrap));
    }
}


void MainWindow::on_Combo_MetodoCierre_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->LineTMuerto->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray;}");
        ui->LineNMT->setStyleSheet("QLineEdit{border-radius: 10px;background-color: rgb(235, 235, 235);border: 2px solid  gray;}");
        ui->LineTMuerto->setEnabled(false);
        ui->LineNMT->setEnabled(false);
        ui->Turno1->setChekBoxishabilitado(true);
        ui->Turno2->setChekBoxishabilitado(true);
        ui->Turno3->setChekBoxishabilitado(true);
        ui->Turno4->setChekBoxishabilitado(true);
        ui->Turno5->setChekBoxishabilitado(true);
        ui->Turno6->setChekBoxishabilitado(true);
        ui->Turno7->setChekBoxishabilitado(true);
        ui->Turno8->setChekBoxishabilitado(true);
        break;
    case 1:
        ui->LineTMuerto->setStyleSheet("background-color: rgb(255,255,255);");
        ui->LineNMT->setStyleSheet("background-color: rgb(255,255,255);");
        ui->LineTMuerto->setEnabled(true);
        ui->LineNMT->setEnabled(true);
        ui->Turno1->setChekBoxishabilitado(false);
        ui->Turno2->setChekBoxishabilitado(false);
        ui->Turno3->setChekBoxishabilitado(false);
        ui->Turno4->setChekBoxishabilitado(false);
        ui->Turno5->setChekBoxishabilitado(false);
        ui->Turno6->setChekBoxishabilitado(false);
        ui->Turno7->setChekBoxishabilitado(false);
        ui->Turno8->setChekBoxishabilitado(false);
        break;
    }
}


void MainWindow::on_Btn_Fechayhora_clicked()
{
    frame = SFecha_Hora;
     MainWindow::setFocus();
     ui->stackedWidget->setCurrentIndex(SFecha_Hora);
     ui->Btns_Fechayhora->setIsSelect(false);
     ui->Lab_Titulo->setText("Fecha & hora");
     frame = SFecha_Hora;
     qDebug() << frame << "------------------------------------Hola Estoy en ek Frame Fecha y hora";
}

void MainWindow::on_tabWidget_2_currentChanged(int index)
{

}

void MainWindow::on_Btn_Display_clicked()
{
    frame = SFormato_FechaHora;
    ui->stackedWidget->setCurrentIndex(SFormato_FechaHora);

}

void MainWindow::Guardar_FechaHora()
{
    QStringList arg; QProcess pro;
    QString strDateTime = ui->dateEdit->text() + " " + ui->Btns_Fechayhora->getLineHorText() + ":" + ui->Btns_Fechayhora->getLineMinText() + ":00";
    //QString strDateTime = ui->txtMonth->text() + ui->txtDay->text() + ui->txtHour->text() + ui->txtMinute->text() + ui->txtYear->text();
    QString str = QString("sudo hwclock --set --date '%1'").arg(strDateTime);
    arg << "-c" << QString("sudo hwclock --set --date '%1'").arg(strDateTime);
    pro.start("/bin/sh", arg); pro.waitForFinished();
    arg.clear();
    arg << "-c" << QString("sudo hwclock -s").arg(strDateTime);
    pro.start("/bin/sh", arg); pro.waitForFinished();
    QString st = pro.readAllStandardOutput();
    qDebug() << str.toUtf8() << st;

}

void MainWindow::on_Btn_CambioTurno_clicked()
{
    QSqlQuery qry;
    qry.exec("SELECT * FROM cistem.ConfigTurnos;");

    while(qry.next())
    {
       if (qry.value(1).toString() == "Manual" )
       {
           CambioDeTurnoManual();

       }

           else {
           CambioDeTurnoProgramado();
       }
    }

    }

    void MainWindow::CambioDeTurnoManual()
    {

    }

    void MainWindow::CambioDeTurnoProgramado()
    {

        QSqlQuery qry;
        qry.exec("SELECT COUNT(STATUS) FROM cistem.Entregando WHERE STATUS='Entregando' ;");

       while(qry.next())
       {
           if (qry.value(0)== 0)
           {
               CierreManual= true;
           }
           else {
               EntregaEnprocesoDurantecierre= true;

           }
       }

    }



    void MainWindow::Guardar_ConfigProducto()
    {
        QSqlQuery qry;
        qry.exec("UPDATE `cistem`.`Productos` SET `Nombre`='"+ui->Line_EditProducto->text()+"' WHERE  `id`="+QString::number(ui->Combo_selcProducto->currentIndex()+1)+"");

    }

void MainWindow::on_Btn_Productos_clicked()
{
    frame = SProductos;
    ui->stackedWidget->setCurrentIndex(SProductos);
    ui->Combo_selcProducto->clear();
     QSqlQuery qry;
     qry.exec("SELECT Nombre  FROM cistem.Productos;");
     while(qry.next()){ ui->Combo_selcProducto->addItem(qry.value(0).toString());   }
    // ui->Combo_selcProducto->activated(0);
     ui->Combo_selcProducto->setCurrentIndex(0);
     ui->Line_EditProducto->setText(ui->Combo_selcProducto->currentText());

}


void MainWindow::on_Btn_ActualizarProducto_clicked()
{
    ui->Combo_selcProducto->setItemText(ui->Combo_selcProducto->currentIndex(),ui->Line_EditProducto->text());
}

void MainWindow::on_Combo_selcProducto_activated(const QString &arg1)
{
    ui->Line_EditProducto->setText(arg1);
}

