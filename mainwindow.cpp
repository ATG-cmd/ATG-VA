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

#define INPUT_1 4
#define INPUT_2 2
#define INPUT_3 1
#define INPUT_4 0
#define INPUT_5 14
#define INPUT_6 6
#define INPUT_7 5
#define INPUT_8 3
#define INPUT_9 21
#define INPUT_10 31
#define INPUT_11 30
#define INPUT_12 11
#define INPUT_13 23
#define INPUT_14 27
#define INPUT_15 22
#define INPUT_16 26
#define CTRL 12
#define Buzzer 29
#define RX4 13
#define TX4 8
#define TX1 15
#define RX1 16
#define LD 7
#define SDA1 8
#define SCL1 9

#define SOH 0x01
const int lenbuff1 = 1024;              // Longitud de buffer, Ajustar
int xbuff1 = 0x00;                      // Índice: siguiente CHAR en cbuff
char cbuff1[lenbuff1];                  // Buffer
char rcvchar1 = 0x00;                   // último carácter recibido
double ProGaugeCapacidad = 40001.0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Time2 = new QTimer();
    Time3 = new QTimer();
    Gpio_timer = new QTimer();

    wiringPiSetup();
    pinMode(CTRL,OUTPUT);
    pinMode(Buzzer,OUTPUT);
    pinMode(LD,OUTPUT);
    pinMode(INPUT_1,INPUT);
    pinMode(INPUT_2,INPUT);
    pinMode(INPUT_3,INPUT);
    pinMode(INPUT_4,INPUT);
    pinMode(INPUT_5,INPUT);
    pinMode(INPUT_6,INPUT);
    pinMode(INPUT_7,INPUT);
    pinMode(INPUT_8,INPUT);
    pinMode(INPUT_9,INPUT);
    pinMode(INPUT_10,INPUT);
    pinMode(INPUT_11,INPUT);
    pinMode(INPUT_12,INPUT);
    pinMode(INPUT_13,INPUT);
    pinMode(INPUT_14,INPUT);
    pinMode(INPUT_15,INPUT);
    pinMode(INPUT_16,INPUT);

    QFont Fonttitle;
    Fonttitle.setPointSize(30);
    Fonttitle.setBold(true);
    Lab_Title = new QLabel(ui->Btn_Barra_Estados);
    Lab_Title->setGeometry(QRect(200,2,500,50));
    Lab_Title->setText("Estado Sistema ");
    Lab_Title->setFont(Fonttitle);
    ui->lbl_ProGaugeDeliveryInProccess->hide();

    QFont FontReloj;
    FontReloj.setPointSize(20);
    FontReloj.setBold(true);
    Reloj= new QLabel(ui->Btn_Barra_Estados);
    Reloj->setGeometry(QRect(860,2,400,50));
    Reloj->setFont(FontReloj);
    Reloj->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss ap"));

    Btn_select_rango = new QPushButton(ui->Lab_Rango_Fecha);
    Btn_select_rango->setText("Select Range");
    Btn_select_rango->setGeometry(30,5,200,40);
    Btn_select_rango->setFont(FontReloj);

    Indicadores[0] = new QLabel(ui->Btn_Barra_Estados); // Alarma
    Indicadores[1] = new QLabel(ui->Btn_Barra_Estados); // warnings

    QFont fontAlarmas;
    fontAlarmas.setPointSize(14);
    fontAlarmas.setBold(true);

    Indicadores[0]->setGeometry(650,3,120,18);
    Indicadores[0]->setFont(fontAlarmas);
    Indicadores[0]->setText("Alarmas:   "+QString::number(Alarmas)+"");

    Indicadores[1]->setGeometry(650,22,120,25);
    Indicadores[1]->setFont(fontAlarmas);
    Indicadores[1]->setText("Warnings: "+QString::number(warnings)+"");



   connect(ui->Regresar_Home,&QPushButton::clicked,this,&MainWindow::on_Btn_Home_clicked);
   connect(ui->stackedWidget,&QStackedWidget::currentChanged,this,&MainWindow::Botones);
   connect(Btn_select_rango,&QPushButton::clicked,this,&MainWindow::btn_clicked);
    Maximizado = new Tanque(ui->Tanque_Maximizado,false);

    ConCombocol(ui->Combo_Color);

    //connect(Time2,SIGNAL(timeout()),this,SLOT(Actualizar_Time()));
    //Time2->start(1000);

    deliveryProGaugeTimer = new QTimer(this);
    connect(Gpio_timer,SIGNAL(timeout()),this,SLOT(Leer_GPIO()));
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
    //ui->lineEdit->installEventFilter(this);

    // ui->stackedWidget->setCurrentIndex(1);
    ui->Lab_Titulo->setText("Inicio");
   // ocultar();

    puertoserie = new QSerialPort();
    Time1 = new QTimer();

    ui->stackedWidget->setCurrentIndex(3);

    //puertoserie->setPortName("Com3");
    puertoserie->setPortName("ttyAMA3");
    puertoserie->setBaudRate(QSerialPort::Baud9600);
    puertoserie->setDataBits(QSerialPort::Data8);
    puertoserie->setFlowControl(QSerialPort::NoFlowControl);
    puertoserie->setParity(QSerialPort::NoParity);
    puertoserie->setStopBits(QSerialPort::OneStop);
    puertoserie->open(QIODevice::ReadWrite);
    connect(puertoserie, &QSerialPort::readyRead, this, &MainWindow::Leer_datos);

    DB = QSqlDatabase::addDatabase("QMYSQL");
    // DB.setHostName("192.168.10.104");
    DB.setHostName("192.168.100.216");
    DB.setDatabaseName("mysql");
    DB.setPort(3306);
    DB.setUserName("root");
    DB.setPassword("flores45");
    if(!DB.open()){
        QMessageBox::critical(this, "Error", DB.lastError().text());
        return;
    }

    consultaBD();
    Buscar_Tanques();
    Descargar();
 qDebug () << "Sali de descargar";
   ui->Tab_entregas->horizontalHeader()->setVisible(true);
   ui->tableWidget->horizontalHeader()->setVisible(true);
   ui->tabla_incidentes->horizontalHeader()->setVisible(true);
   ui->stackedWidget->setCurrentIndex(SHome);
   ui->Btn_Guardar->setVisible(false);
   ui->Regresar->setVisible(false);
   ui->Btn_user->setVisible(true);
   ui->btn_menu->setVisible(true);
   ui->SelecTank->setVisible(false);
   qDebug () << "Aqui ando";
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
     connect(ui->Combo_Tipo, QOverload<int>::of(&QComboBox::activated),
             [=](int index){

    ui->Lab_Titulo->setText("Tanque");
    ui->Combo_Sonda->addItem(" ");

    QSqlQuery qry;
    qry.exec("SELECT Serie FROM `cistem`.`sonda` LIMIT 100 ;");
    while(qry.next())
    {
        ui->Combo_Sonda->addItem(qry.value(0).toString());
        qDebug() << qry.value(0);
    }  });

    connect(ui->Combo_IdTanque, QOverload<int>:: of(&QComboBox::activated),[=](int index){


     } );

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

/*----------------------------------------------------------------------------------------------------------
 * En esta parte se modifica el tecto del tanque que aparece en el stalck de configuracion del tanque,
 * este slot es activado con la señal texchanged, que se activa al modificar el texedit de nombre, en este slot
 * se toma el texto del textedit Nombre para asignalo al tanque mediante el metodo setnameTank
 * -------------------------------------------------------------------------------------------------------*/

// Inicio de Modificacion de texto de Tanque Configuracion

void MainWindow::Modificar_TextoTank()
{
    Tconf->SetnameTank(ui->Line_Nombre->text());

}// FIN de Modificacion de texto de Tanque Configuraci

/*-------------------------------------------------------------------------------------------------------------
 * Cada stalcked tiene su bonton de guardar y volver al momento de de entrar a un stalcked mediante el munu dentro del
 * slot del boton se asigna un  numero de frame para a la hora de guardar o regresar este entre al casecon el numero de frame
 * y gurade correctamente los datos del stalcked seleccionado
 * ------------------------------------------------------------------------------------------------------------------*/
//Inicio de boton de Guardar
void MainWindow::on_Btn_Guardar_clicked()
{
    qDebug() << frame;
    switch(frame){
    case SSonda : Guardar_Sonda(); break;
    case STanque: on_Btn_SaveTank_clicked(); break;
    case SComunicacion: Guardar_Comunicacion(); break;
    case Slimites : guardar_limites(); break;
    }

}
//Fin de boton de Guardar
/*--------------------------------------------------------------------------------------------------------------------
 * Este Timer es activado Cada Segundo para actualizar el reloj de la parte superior derecha de la pantalla
 * -----------------------------------------------------------------------------------------------------------------*/
//Inicio de timer que actualiza reloj
void MainWindow::Actualizar_Time()
{
    Reloj->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss ap"));
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
        Dialog *dlg = new Dialog(this);
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
        dlg->set_etiqueta("Ingrese "+ ui->Lab_Medida1->text());
        //dlg->use_validator(val_1);
        //QValidator *val_1 = new QIntValidator(100,2000,this);
        dlg->validador(2,0,2500,2);
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
        dlg->set_etiqueta("Ingrese " + ui->Lab_Medida2->text());
        //dlg->use_validator(val_1);
        //QValidator *val_1 = new QIntValidator(100,2000,this);
        dlg->validador(2,0,2500,2);
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
    RX=true;
    QByteArray data;
    char dato;
    while( puertoserie->bytesAvailable())
    {
        data =  puertoserie->read(1);
        qDebug() << data;
        dato = data.at(0);
        qDebug()<< dato;
        addcbuff1(dato);
    }

}

void MainWindow::inicbuff1(){
    for (int i = 0; i < lenbuff1; i++){ cbuff1[i] = 0x00; } xbuff1 = 0x00;
}

void MainWindow::addcbuff1(char c){
    switch (c){
    case 0x0D: cbuff1[xbuff1++] = c; Protocolo(cbuff1); qDebug() << cbuff1; break;
    case SOH: inicbuff1(); cbuff1[xbuff1++] = c; qDebug() << c; break;
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

    qry.exec("INSERT INTO `cistem`.`sonda` (`Serie`, `Protocolo`, `Flotadores`, `Licencia`, `fecha`) VALUES ('"+ui->line_Serie->text()+"', '"+ui->Combo_protocolo->currentText()+"', '"+ui->Combo_protocolo->currentText()+"', '"+ui->Line_Licencia->text()+"', '2020-01-14 00:03:00' );");

//    QString A("INSERT INTO `cistem`.`sonda` (`Serie`, `Protocolo`, `Flotadores`, `Licencia`, `Ajuste Altura`, `fecha`)"
//    " VALUES ('"+ui->line_Serie->text()+"'"
//           ", '"+ui->Combo_protocolo->currentText()+"'"
//           ", '"+ui->Line_Licencia->text()+"'"
//           ", '"+ui->Line_Licencia->text()+"'"
//           ", "+ui->Line_AjusteAltura->text()+""
//           ", '2020-01-14 00:03:00');");

//    qry.exec(A);
//    qDebug () << A;

    ui->line_Serie->clear();
    ui->Combo_protocolo->setCurrentIndex(0);
    ui->ComboFlotadores->setCurrentIndex(0);
    ui->Line_Licencia->clear();
    ui->ComboUDistancia->setCurrentIndex(0);
    ui->Combo_UTemperatura->setCurrentIndex(0);
    ui->Combo_UVolumen->setCurrentIndex(0);
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
    qDebug() << "Indice:" << indice;
    qDebug()<< qry.exec("SELECT Protocolo FROM `cistem`.`sonda` WHERE Serie = '"+ProGaugeId[indice]+"' ;");
    while(qry.next())
    {
        N= qry.value(0).toInt();
        qDebug() << "Protocolo:"<< N;
    }
    connect(tanques[indice],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);
    switch (N) {

    case 1:
        if (cad[7]== "=" && cad[12]== "=" && cad[18] =="=")
        {
            tanques[indice]->SetAltura(cad.mid(13,5).toDouble(), cad.mid(19,4).toDouble());
            tanques[indice]->SetTemperatura(cad.mid(9,3).toDouble());
        }
        inicbuff1();
        break;

    case 2:
        if (cad[7]== "=" && cad[12]== "=" && cad[21] =="=" && cad.length() == 36)
        {
            tanques[indice]->SetAltura(cad.mid(13,8).toDouble(),cad.mid(22,8).toDouble());
            tanques[indice]->SetTemperatura(cad.mid(9,3).toDouble()/10);


//            if(tanques[indice]->GetVolumen() > deliveryMaxVolumeRead || deliveryCountIncrement == 0){
//                qDebug() << "ProGaugeVolumen:" << tanques[indice]->GetVolumen() << "deliveryMaxVolumeRead:" << deliveryMaxVolumeRead;

//                deliveryMaxVolumeRead = tanques[indice]->GetVolumen();
//                // ui->lbl_deliveryMaxVolumeRead->setText(QString::number(deliveryMaxVolumeRead));
//            }


            qDebug() << "Tamano de cadena:"<< cad.length();
        }
        inicbuff1();
        break;
    default:
        inicbuff1();
        break;
    }

    if(Maxi)
        Tanque_Maximisado();
}

void MainWindow::Descargar()
{

    connect(deliveryProGaugeTimer, SIGNAL(timeout()), this, SLOT(deliveryProGaugeCountIncrement()));
    deliveryProGaugeTimer->start(1000);
    qDebug() << "Descargado Tanques .....";
    S=0;
    QSqlQuery qry;
    qry.exec("SELECT COUNT(1) FROM `cistem`.`tanques` WHERE Configurado = 1;");
  while(qry.next())   {
   qDebug() << "Que esta pasando";
   if (qry.value(0).toInt() > 0)
   {  numerodetanques = qry.value(0).toInt();
  qDebug () << "En este momento S:" << S;
    if(qry.exec("SELECT * FROM `cistem`.`tanques` where configurado = 1 ;"))
    {
        while (qry.next())
        {
            Geometrytank();
            connect(tanques[S],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);
            connect(tanques[S],&Tanque::Entrega,this,&MainWindow::Qry_Entrega);
            ui->stackedWidget->setCurrentIndex(3);
           // ProGaugeId[S]=qry.value(1).toString();
            qDebug() <<   "ID del tanque dw:" <<  qry.value(1).toInt();
            tanques[S]->setIdTanque(qry.value(1).toInt());
            tanques[S]->setIshabilitado(qry.value(2).toBool());
            tanques[S]->SetnameTank(qry.value(3).toString());
            tanques[S]->setCodigoProducto(qry.value(4).toInt());

            switch (qry.value(5).toInt())
            {
            case 1: tanques[S]->color("gray", true);  break;
            case 2: tanques[S]->color("green",true);  break;
            case 3: tanques[S]->color("yellow",true); break;
            case 4: tanques[S]->color("cyan",true);   break;
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

            S++;
            ui->stackedWidget->setCurrentIndex(SHome);
        }
    }
    else
    {
        QMessageBox::critical(this, "Error",tr(qry.lastError().text().toUtf8()));
    }
    for (int i=0;i < S;i++) {
        if(qry.exec("SElECT * FROM CISTEM.LIMITES WHERE Id_Taque = " +  QString::number(tanques[i]->getIdTanque())))
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
    case 0:case 4: tanques[S]->Setgeometry(80,3,480,240);   break;
    case 1:case 5: tanques[S]->Setgeometry(600,3,480,240);  break;
    case 2:case 6: tanques[S]->Setgeometry(80,240,480,240); break;
    case 3:case 7: tanques[S]->Setgeometry(600,240,480,240);break;
    }
}

void MainWindow::Estados()
{
    switch(ProGaugeCount){
    case 0:
        connect(Time3,SIGNAL(timeout()),this,SLOT(SendCMD()));
        Time3->start(50); Time1->stop();
        disconnect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));
        break;
    case 1:  if (RX == true ) { ProGaugeCount ++ ; RX=false; }
        else if (intento >= 3)
        {
            ProGaugeCount ++ ; RX=false; intento = 0;
            qDebug () << "Sonda Fuera de Linea: " << ProGaugeId[ProGaugeCount1];
            offlineSonda(ProGaugeId[ProGaugeCount1]);
            SondasOnline[ProGaugeCount1] = false;
        }
        else { intento ++; ProGaugeCount--; qDebug () << intento; } break;
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
        QMessageBox::information(this,"EXITO", "Los datos se guardaron correctamente");
        return;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    frame = SHome2;
    ui->stackedWidget->setCurrentIndex(SHome2);
}

void MainWindow::Tanque_Maximisado()
{
    if(!Maxi)
    {
        indiceM =0;
        ui->stackedWidget->setCurrentIndex(0);
        qDebug() << "Hola desde tanque maximizado";

        for(int i=0; i <= 8; i++)
        {
            qDebug() << i;
            if (tanques[i]->getTMaximizado() == false)
            {
                qDebug() << "hola desde este if :)" << i;
                indiceM = i;
                Maxi = true;
                break;
            }
        }

        qDebug()<< "indiceM"<< indiceM;

        Maximizado->Setgeometry(19,-11,500,580);
        Maximizado->setID(tanques[indiceM]->getID());
        ui->Lab_Titulo->setText(tanques[indiceM]->GetNameTank());
        Maximizado->SetnameTank("");
        Maximizado->color(tanques[indiceM]->GetColor(),false);
        Maximizado->SetVolMax(tanques[indiceM]->GetVolMax());
        Maximizado->SetProducto_Alto(tanques[indiceM]->GetProducto_Alto());
        Maximizado->SetDesbordamiento(tanques[indiceM]->GetDesbordamiento());
        Maximizado->SetNecesitaProducto(tanques[indiceM]->GetNecesitaProducto());
        Maximizado->SetProductoBajo(tanques[indiceM]->GetProductoBajo());
        Maximizado->SetAlarma_de_Agua(tanques[indiceM]->GetAlarma_de_Agua());
        Maximizado->SetAdvertencua_de_Agua(tanques[indiceM]->GetAdvertencia_de_Agua());
        Maximizado->setTankTiempoEntrega(tanques[indiceM]->getTankTiempoEntrega());
        Maximizado->setTankMinimoEntrega(tanques[indiceM]->getTankMinimoEntrega());

        qDebug() << "ID" << Maximizado->getID();

        Maximizado->setCapacidad(tanques[indiceM]->getCapacidad());
        Maximizado->SetTankDiametro(tanques[indiceM]->GetTankDiametro());
        Maximizado->setTipo(tanques[indiceM]->getTipo());


    }
       int por1 = (100 * tanques[indiceM]->GetVolMax())/38439.85;
       int y = 430 * por1/38439.85;
       int y1 = calcY(tanques[indiceM]->GetProducto_Alto());
       int y2 = calcY(tanques[indiceM]->GetDesbordamiento());
       int y3 = calcY(tanques[indiceM]->GetNecesitaProducto());
       int por = (100 * tanques[indiceM]->GetProductoBajo()/38439.85);
       int y4 = ((430 * por) /38439.85);

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
//Aqui me Quede
    Maximizado->setTMaximizado(false);
    tanques[indiceM]->setTMaximizado(false);
    Maximizado->SetAltura(tanques[indiceM]->GetAltura(),tanques[indiceM]->getNivelAgua());
    Maximizado->SetTemperatura(tanques[indiceM]->GetTemperatura());
    ui->Lab_VolMaxi->setText(QString::number(tanques[indiceM]->getVolumenCon()));
    ui->Lab_AlturaMaxi->setText(QString::number(tanques[indiceM]->GetAltura()));
    ui->Lab_VolAgua->setText(QString::number(tanques[indiceM]->getVolumenA()));
    ui->Lab_AlturaAgua->setText(QString::number(tanques[indiceM]->getNivelAgua()));
    ui->Lab_Maxitemp->setText(QString::number(tanques[indiceM]->GetTemperatura()));
    ui->Lab_Diametro->setText(QString::number(tanques[indiceM]->GetTankDiametro()));
    ui->Lab_VolMaximo->setText(QString::number(tanques[indiceM]->GetVolMax()));
    ui->Lab_DesbordamientomMaxi->setText(QString::number(tanques[indiceM]->GetDesbordamiento()));
    ui->Lab_ProductoAlto->setText(QString::number(tanques[indiceM]->GetProducto_Alto()));
    ui->Lab_NecesitaEntrega->setText(QString::number(tanques[indiceM]->GetNecesitaProducto()));
    ui->Lab_ProductoBajo->setText(QString::number(tanques[indiceM]->GetProductoBajo()));
    ui->Lab_AlarmaAguaAlta->setText(QString::number(tanques[indiceM]->GetAlarma_de_Agua()));
    ui->Lab_AdvertenciaAlarmaAlta->setText(QString::number(tanques[indiceM]->GetAdvertencia_de_Agua()));

    inicbuff1();
    frame = STMaxi;
    qDebug() << "Hola desde Tanque maximizado ;p";
    ui->stackedWidget->setCurrentIndex(STMaxi);

}

void MainWindow::on_Regresar_clicked()
{
    if(frame > SMenuPub || frame == SEntregas) frame =SMenuPub;
    else frame = SMenu;

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
        if (IDactual == busqueda) {  indice = i; break;  }
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
      //  puertoserie->setDataTerminalReady(true);
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
           Time1->start(1500 /(qry.value(0).toInt()*2));
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
    QSqlQuery qry;
   QString A = "UPDATE `cistem`.`config_sonda` SET "
               "`Baudios`='"+ui->SCombo_Baudios->currentText()+"',"
               " `Datos`='"+ui->SCombo_Datos->currentText()+"',"
               " `Stop`='"+ui->SCombo_Stop->currentText()+"', "
               "`Paridad`='"+ui->SCombo_Paridad->currentText()+"', "
               "`Unidades Sistema`='"+ui->SUnidadesSistema->currentText()+"' "
               "WHERE  `ID`=0;";
   qDebug() << "Hola desde  Guardar Comunicacion" << A;
   qry.exec(A);

}
void MainWindow::on_Btn_tabla_cubicacion_clicked()
{
    frame = STablaCub;
    ui->stackedWidget->setCurrentIndex(STablaCub);
    ui->Lab_Titulo->setText("Tabla De Cubicacion");
    enableCubicTableFields(false);
    enableCubicTableBtn(true,false,false);
    clearCubicTableFields();

}

void MainWindow::on_Btn_Alarmas_clicked()
{

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
    QString cadena;
    cadena.append("SELECT Punto, Altura, Volumen FROM cistem.tablacubicacion WHERE "
                  "TanqueId = '" + QString::number(Id_tanque) +"';");
    QSqlQuery qry;
    qDebug() << qry.exec(cadena);
    while (qry.next())
    {
        ui->tableWidget->removeRow(0);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(qry.value(0).toInt())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(qry.value(1).toDouble(), 'f', 3)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(qry.value(2).toDouble(), 'f', 3)));
        qDebug() << qry.value(0).toInt() << qry.value(1).toInt() << qry.value(2).toInt();
    }

}

void MainWindow::Rellenar_campos_cubicacion(QString p, QString a, QString v)
{
    ui->Line_Punto->setText(p);
    ui->Line_Altura->setText(a);
    ui->Line_Volumen->setText(v);
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


void MainWindow::on_Combo_CubTanque_currentIndexChanged(int index)
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
                ui->tableWidget->item(row,2)->text()
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

//    qDebug() << "este es el ID del tenque que guardo limites : " << tanques[tank_id]->getIdTanque();

    tanques[tank_id]->SetVolMax(ui->Line_volumen_maximo->text().toDouble());
    tanques[tank_id]->SetProducto_Alto(ui->Line_producto_alto->text().toDouble());
    tanques[tank_id]->SetDesbordamiento(ui->Line_desbordamiento->text().toDouble());
    tanques[tank_id]->SetNecesitaProducto(ui->Line_limite_entrega->text().toDouble());
    tanques[tank_id]->SetProductoBajo(ui->Line_producto_bajo->text().toDouble());
    tanques[tank_id]->SetAlarma_de_Agua(ui->Line_alarma_agua->text().toDouble());
    tanques[tank_id]->SetAdvertencua_de_Agua(ui->Line_advertencia_agua->text().toDouble());

    ui->stackedWidget->setCurrentIndex(0);
    frame = 0;
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
    int porcentaje = ((tanque->getVolumenCon() * 100) / tanque->getCapacidad());
    qDebug() << "porcentaje de volumen de tanque: " << porcentaje;
//    tanques[tank_id]->SetVolMax(ui->Line_volumen_maximo->text().toDouble());
//    tanques[tank_id]->SetProducto_Alto(ui->Line_producto_alto->text().toDouble());
//    tanques[tank_id]->SetDesbordamiento(ui->Line_desbordamiento->text().toDouble());
//    tanques[tank_id]->SetNecesitaProducto(ui->Line_limite_entrega->text().toDouble());
//    tanques[tank_id]->SetProductoBajo(ui->Line_producto_bajo->text().toDouble());
//    tanques[tank_id]->SetAlarma_de_Agua(ui->Line_alarma_agua->text().toDouble());
//    tanques[tank_id]->SetAdvertencua_de_Agua(ui->Line_advertencia_agua->text().toDouble());


//    if(((tanque->getVolumenCon() * 100) / tanque->getCapacidad()) >= tanque->GetProducto_Alto()){}
//    if(tanque->getVolumenCon() >= tanque->GetDesbordamiento()){}
}

void MainWindow::insertar_incidente(QString tipo, QString Descripcion, QString usuario,QString Prioridad,QString Activo)
{
    QSqlQuery qry;
    QString cadena;
    cadena.append("INSERT INTO cistem.incidentes (Tipo_incidente, Descripcion, usuario, Fecha_incidente,Prioridad,Activo)"
                  " VALUES ('"+tipo+"', '"+Descripcion+"', '"+usuario+"', '"+ QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") +"','"+ Prioridad+"', '"+Activo+"');");

    qDebug() << cadena;
    qry.exec(cadena);
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
                                  "BETWEEN ('"+T_inicial+"') AND ('"+T_Final+"'));"); break;

    case 2:    cadena1 = ("SELECT * FROM cistem.incidentes WHERE "
                                  "Prioridad = '0' AND (Fecha_incidente "
                                  "BETWEEN ('"+T_inicial+"') AND ('"+T_Final+"'));"); break;

    case 3:    cadena1 = ("SELECT * FROM cistem.incidentes WHERE Fecha_incidente "
                                      "BETWEEN ('"+T_inicial+"') AND ('"+T_Final+"');"); break;
    default:break;
    }
    qry.exec(cadena1);
   // qDebug() << cadena1;
    while(qry.next())
    {
//        ui->tabla_incidentes->removeRow(0);
        ui->tabla_incidentes->insertRow(ui->tabla_incidentes->rowCount());
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 2, new QTableWidgetItem(qry.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 3, new QTableWidgetItem(qry.value(5).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 4, new QTableWidgetItem(qry.value(3).toString()));
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
        //qDebug() << qry.value(0).toString() << qry.value(1).toString() << qry.value(2).toString() << qry.value(3).toString() << qry.value(4).toString();
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
    QString cadena;
    QSqlQuery qry;
    limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());
    //Btn_select_rango->setGeometry(30,5,200,40);
    Btn_select_rango->setText("Select Range");
    cadena = ("SELECT * FROM cistem.incidentes WHERE Prioridad = '"+ priodidad +"' ;");

    qry.exec(cadena);
   // qDebug() << cadena;
    while(qry.next())
    {
        ui->tabla_incidentes->insertRow(ui->tabla_incidentes->rowCount());
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 2, new QTableWidgetItem(qry.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 3, new QTableWidgetItem(qry.value(5).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 4, new QTableWidgetItem(qry.value(3).toString()));
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
    }
}

void MainWindow::rellenar_activos(QPushButton *btn)
{
    btn->setText("Limpiar Activos");
    limpiar_tabla(ui->tabla_incidentes,ui->tabla_incidentes->rowCount());
    QString cadena;
    cadena.append("SELECT * FROM cistem.incidentes WHERE Activo = '1';");

    QSqlQuery qry;
    qry.exec(cadena);

    while (qry.next()) {
        ui->tabla_incidentes->insertRow(ui->tabla_incidentes->rowCount());
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 1, new QTableWidgetItem(qry.value(2).toString()));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 2, new QTableWidgetItem(qry.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 3, new QTableWidgetItem(qry.value(5).toDateTime().toString("yyyy-MM-dd HH:mm:ss")));
        ui->tabla_incidentes->setItem(ui->tabla_incidentes->rowCount() - 1, 4, new QTableWidgetItem(qry.value(3).toString()));
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tabla_incidentes->item(ui->tabla_incidentes->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
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

}

void MainWindow::buscar_alarmas()
{

}

/* Este Metodo es para colocar las bolitas de colores en su lugar
  Respecto al limite que le corresponde, la siguiente formula es la
  formula del semicirculo
 Referencia : https://www.youtube.com/watch?v=uDS4Q-QCU8M */

int MainWindow::X(int Y)
{
    return  qSqrt(50625 - qPow(Y-215,2))+270;
}

int MainWindow::calcY(int y)
{
    return  430-(430 * y )/100;
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

}
void MainWindow::on_Btn_CubGenerar_clicked()
{

}

/* Termina la configuracion de la tabla de cubicacion*/

//void MainWindow::guardar_limites()
//{
//    QSqlQuery qry;
//    QString cadena;
//    cadena.append("UPDATE cistem.limites SET Volumen_total = '" + ui->Line_volumen_total->text() + "',"
//                  " Volumen_maximo = '" + ui->Line_volumen_maximo->text() + "',"
//                  " Producto_alto = '" + ui->Line_producto_alto->text() + "',"
//                  " Desbordamiento = '" + ui->Line_desbordamiento->text() + "',"
//                  " Entrega_necesaria = '" + ui->Line_limite_entrega->text() + "',"
//                  " Producto_bajo = '" + ui->Line_producto_bajo->text() + "',"
//                  " Alarma_agua_alta = '" + ui->Line_alarma_agua->text() + "',"
//                  " Advertencia_agua_alta = '" + ui->Line_advertencia_agua->text() + "'"
//                  " WHERE Id_Taque = '" + ui->Combo_taque_limites->currentText() + "';");

//    qDebug() << cadena;
//    qDebug() << qry.exec(cadena);
//}
void MainWindow::on_Btn_Entregas_clicked()
{
}
void MainWindow::deliveryProGaugeCountIncrement(){
    if (TanqueActual == numerodetanques-1) { TanqueActual = 0;}
    else { TanqueActual++;
  }
      qDebug() << "Tanque Actual"<< TanqueActual;
    tanques[TanqueActual]->deliveryProGaugeCountIncrement();
    QSqlQuery qry;
    qry.exec(tanques[TanqueActual]->ActualInventory());
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
                  "`Producto`='Disel' WHERE  `Id_Taque`= '"+QString::number(ui->Combo_IdTanque->currentIndex())+"' ;");

      consultaBD();
      Buscar_Tanques();
      Descargar();

    frame = SMenu;
    ui->stackedWidget->setCurrentIndex(SMenu);

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

        switch (index)
         {
         case 0:   break;
         case 1:   break;
         case 2:   break;
         case 3:   break;
        } });
    frame = sInventario; ui->stackedWidget->setCurrentIndex(sInventario);
}

void MainWindow::on_Btn_Reports_clicked()
{
    frame = SReportes;
    ui->stackedWidget->setCurrentIndex(SReportes);
    Btn_select_rango->setGeometry(30,5,200,40);
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->addItem("Activos");
    ui->ComboSeleccion->addItem("Prioritarios");
    ui->ComboSeleccion->addItem("No Prioritarios");
    ui->ComboSeleccion->addItem("Historico");

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

    QString cadena;
     ui->CSelecTank->clear();
     ui->CSelecTank->addItem("ALL");
    cadena.append("SELECT DISTINCT Tanque_Nombre FROM `cistem`.`entregas`;");
    QSqlQuery qry;
    qDebug() << "QRY:" << qry.exec(cadena);
    while (qry.next()) {
       ui->CSelecTank->addItem(qry.value(0).toString());

    }

    connect(ui->CSelecTank, QOverload<int>::of(&QComboBox::activated),
            [=](int index){
        QString cadena;
        ui->Tab_entregas->clearContents();
        ui->Tab_entregas->setRowCount(0);

        switch (index)
         {
         case 0: cadena.append("SELECT * FROM `cistem`.`entregas` LIMIT 1000;");break;
         default: qDebug() << "Texto del index:" << ui->CSelecTank->itemText(index);
cadena.append("SELECT * FROM `cistem`.`entregas` where Tanque_Nombre = '"+ui->CSelecTank->itemText(index)+"';"); break;

        }

        //
         QSqlQuery qry;
         qDebug() << "QRY:" << qry.exec(cadena);
         while (qry.next())
         {
             //ui->Tab_entregas->removeRow(0);


             ui->Tab_entregas->insertRow(ui->Tab_entregas->rowCount());
             //ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 0, new QTableWidgetItem(QString::number(qry.value(0).toInt())));
             ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 0, new QTableWidgetItem(qry.value(1).toString()));
             ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 1, new QTableWidgetItem(QString::number(qry.value(2).toInt())));
             ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 2, new QTableWidgetItem(QString::number(qry.value(3).toInt())));
             ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 3, new QTableWidgetItem(QString::number(qry.value(4).toInt())));
             ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1,4 , new QTableWidgetItem(QString::number(qry.value(5).toInt())));
             ui->Tab_entregas->setItem(ui->Tab_entregas->rowCount() - 1, 6, new QTableWidgetItem(qry.value(7).toDateTime().toString("dd/MM/yyyy HH:mm:ss")));
             ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
             ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 1)->setTextAlignment(Qt::AlignCenter);
             ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 2)->setTextAlignment(Qt::AlignCenter);
             ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 3)->setTextAlignment(Qt::AlignCenter);
             ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);
             //ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 5)->setTextAlignment(Qt::AlignCenter);

             ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 6)->setTextAlignment(Qt::AlignCenter);
             //  ui->Tab_entregas->item(ui->Tab_entregas->rowCount() - 1, 6)->setFont(A);
             qDebug() << qry.value(0).toInt() << qry.value(1).toString() << qry.value(2).toInt() << qry.value(4).toInt();
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
  // ui->ComboSeleccion->clear();

   QObject::disconnect( combo_connect1 ); // btn reporte
   QObject::disconnect( combo_connect2 ); // barra estados
   if(frame != STanque && frame != Slimites)  QObject::disconnect( combo_connect3 ); // tanque
   if(frame != SComunicacion && frame != SComunicador) QObject::disconnect( combo_connect4 ); // comunicador


   switch (frame) {
  // case 1: ui->Regresar->setVisible(true); ui->btn_menu->setVisible(true); break;
   case SHome: case SHome2: ui->btn_menu->setVisible(true); ui->Btn_user->setVisible(true); break;
   case SMenuPub: case SLogin: case SMenu: case STMaxi: break;
   case STablaCub: case SVialarmas:case SEntregas:
        ui->Regresar->setVisible(true); break;
    case sInventario:ui->ComboSeleccion->setVisible(true);ui->SelecTank->setVisible(true);ui->Regresar->setVisible(true);break;
   case SComunicador:  case SComunicacion : case Slimites : case STanque: ui->ComboSeleccion->setVisible(true); ui->Regresar->setVisible(true);ui->Btn_Guardar->setVisible(true); break;
   default: ui->Regresar->setVisible(true);ui->Btn_Guardar->setVisible(true); break;
   case SReportes: ui->Regresar->setVisible(true); ui->ComboSeleccion->setVisible(true); break;
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
  ui->Combo_Color->setCurrentIndex(qry.value(5).toInt());
  ui->Line_CodigoCombustible->setText(qry.value(6).toString());
  ui->Combo_Sonda->setCurrentText(qry.value(7).toString());
  ui->Line_AjusteAltura->setText(qry.value(8).toString());
  ui->Line_Diametro->setText(qry.value(9).toString());
  ui->Line_Capacidad->setText(qry.value(10).toString());
  ui->Combo_Tipo->setCurrentIndex(qry.value(11).toInt());
  ui->Line_Angulo->setText(qry.value(12).toString());
  ui->Line_Distancia->setText(qry.value(13).toString());
  ui->Line_coeficiente->setText(qry.value(14).toString());
  ui->Combo_Producto->setCurrentText(qry.value(15).toString());
}


}

void MainWindow::btn_clicked()
{
    if(Btn_select_rango->text() == "Select Range")
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

// aqui se leen los sensores
    S_input[0] = digitalRead(INPUT_1);
    S_input[1] = digitalRead(INPUT_2);
    S_input[2] = digitalRead(INPUT_3);
    S_input[3] = digitalRead(INPUT_4);
    S_input[4] = digitalRead(INPUT_5);
    S_input[5] = digitalRead(INPUT_6);
    S_input[6] = digitalRead(INPUT_7);
    S_input[7] = digitalRead(INPUT_8);
    S_input[8] = digitalRead(INPUT_9);
    S_input[9] = digitalRead(INPUT_10);
    S_input[10] = digitalRead(INPUT_11);
    S_input[11] = digitalRead(INPUT_12);
    S_input[12] = digitalRead(INPUT_13);
    S_input[13] = digitalRead(INPUT_14);
    S_input[14] = digitalRead(INPUT_15);
    S_input[15] = digitalRead(INPUT_16);

   for(int i = 0; i <= 3; i++)
   {
      //  qDebug() << "Valor de senor " << i+1 <<  " :" << S_input[i];
       if(S_input[i] == false) // se activan en false
       {   Gpio_status.append( "Sensor_");
           Gpio_status.append(QString::number(i+1));
           Gpio_status.append(": Activado");
           qDebug() << "El sensor" << i+1 << " se Activo";
           if(S_input[0] == false || S_input[1] == false ){
               insertar_incidente("Alarma",Gpio_status,"user","1","1");
               Alarmas ++;
               Indicadores[0]->setText("Alarmas:   "+QString::number(Alarmas)+"");
           }else {
               insertar_incidente("Warning",Gpio_status,"user","0","1");
               warnings ++;
               Indicadores[1]->setText("Warnings: "+QString::number(warnings)+"");
           }
           Gpio_status.clear();
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
    qDebug() << "Qry Entrega" << SeRealizoEntrega;
    QSqlQuery qry;
    qry.exec(SeRealizoEntrega);
}

