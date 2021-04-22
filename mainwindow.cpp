#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <tanque.h>
#include <QLineEdit>
#include <sonda.h>
#include <dialog.h>
#include <QFrame>
#include <qmath.h>
#include "wiringPi.h"

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


#define SOH 0x01
const int lenbuff1 = 1024;              // Longitud de buffer, Ajustar
int xbuff1 = 0x00;                      // Índice: siguiente CHAR en cbuff
char cbuff1[lenbuff1];                  // Buffer
char rcvchar1 = 0x00;                   // último carácter recibido
double ProGaugeCapacidad = 40001.0;

int    deliveryTimeOut = 0;
double deliveryMinimunVolume = 0;
//double deliveryVolumeRead = 0;
double deliveryMaxVolumeRead = 0;
int    deliveryInProcess = 0;
double deliveryLastInventoryRead = 0;
double deliveryCountIncrement = 0;
double deliveryCountDecrement = 0;
double deliveryInventoryStart = 0;
double deliverySensivilityVolume = ProGaugeCapacidad * 0.0001;
//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Time2 = new QTimer();
    Time3 = new QTimer();
    wiringPiSetup();
    pinMode(12,OUTPUT);

    QFont Fonttitle;
    Fonttitle.setPointSize(30);
    Fonttitle.setBold(true);
    Lab_Title = new QLabel(ui->Btn_Barra_Estados);
    Lab_Title->setGeometry(QRect(200,2,500,50));
    Lab_Title->setText("Estado Sistema ");
    Lab_Title->setFont(Fonttitle);
    ui->lbl_ProGaugeDeliveryInProccess->hide();

    Avisos = new QLabel(ui->Btn_Barra_Estados);
    Avisos->setGeometry(QRect(700,2,105,50));
    Avisos->setText("Alarmas 0 <br/> Advertencias 0");

    QFont FontReloj;
    FontReloj.setPointSize(20);
    FontReloj.setBold(true);
    Reloj= new QLabel(ui->Btn_Barra_Estados);
    Reloj->setGeometry(QRect(860,2,400,50));
    Reloj->setFont(FontReloj);
    Reloj->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss ap"));

    int x=10;

//    for (int i=0; i < 3; i++)
//    {
//        Indicadores[i] = new QLabel(ui->Btn_Barra_Estados);
//        Indicadores[i]->setGeometry(QRect(x,10,40,29));
//        Indicadores[i]->setStyleSheet("color: white; background-color:cyan ; border: 3px solid gray; border-radius: 35px; padding: 2px;");
//        x+=45;
//    }

  connect(ui->Regresar_Home,&QPushButton::clicked,this,&MainWindow::on_Btn_Home_clicked);
  connect(ui->stackedWidget,&QStackedWidget::currentChanged,this,&MainWindow::Botones);

//    Tconf = new Tanque(ui->Tanque,false);
    Maximizado = new Tanque(ui->Tanque_Maximizado,false);
//    Tconf->Setgeometry(500,5,100,80);
//    Tconf->SetnameTank("Sin Nombre");
    ConCombocol(ui->Combo_Color);

    //connect(Time2,SIGNAL(timeout()),this,SLOT(Actualizar_Time()));
    //Time2->start(1000);

    deliveryProGaugeTimer = new QTimer(this);
    connect(deliveryProGaugeTimer, SIGNAL(timeout()), this, SLOT(deliveryProGaugeCountIncrement()));
    deliveryProGaugeTimer->start(1000);


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

   ui->Tab_entregas->horizontalHeader()->setVisible(true);
   ui->tableWidget->horizontalHeader()->setVisible(true);

   ui->stackedWidget->setCurrentIndex(SHome);
   ui->Btn_Guardar->setVisible(false);
   ui->Regresar->setVisible(false);
   ui->Btn_user->setVisible(true);
   ui->btn_menu->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Btn_Home_clicked()
{
    frame =SHome;
    ui->stackedWidget->setCurrentIndex(SHome);
    ui->Lab_Titulo->setText("Inicio");
    ui->Btn_Guardar->setVisible(false);
    //ui->Btn_Config->setVisible(false);
    ui->Btn_user->setVisible(true);
    ui->Regresar->setVisible(false);
    //ui->ComboSeleccion->setVisible(false);

    if (Maxi == true)
    { Maxi = false;

        qDebug() << "ando en el if de home";
        for (int i=0;i <= IDSerie-1; i++ )
        { tanques[i]->setTMaximizado(false);
            qDebug() << "entre al for";
            qDebug() << "Tanque" + QString::number(i)+ ":false";
        }

    }
    frame = SHome;
}

void MainWindow::on_Btb_Sonda_clicked()
{   frame = SSonda;
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
     ui->Combo_Sonda->clear();
     ui->ComboSeleccion->clear();
     ui->ComboSeleccion->setVisible(true);
     ui->stackedWidget->setCurrentIndex(STanque);
     ui->ComboSeleccion->addItem("General");
     ui->ComboSeleccion->addItem("Limites");

     connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
             [=](int index){

         switch (index)
          {
          case 0:ui->stackedWidget->setCurrentIndex(STanque); break;
          case 1: ui->stackedWidget->setCurrentIndex(Slimites); frame = Slimites;break; // //////////////////////////
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
    //connect(ui->Line_Nombre,&QLineEdit::textChanged,this,&MainWindow::Modificar_TextoTank);
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
    }
   
});
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

    connect(combo, QOverload<int>::of(&QComboBox::activated),
            [=](int index){

        switch(index){
        case 1 :
            combo->setStyleSheet(
                        "QComboBox{background-color: gray ;}"
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
                        "border-right: 10px solid gray;"
                        "border-bottom: 5px solid gray; ;"
                        " border-left: 10px  solid gray; ;"
                        "}");

            break;

        case 2:
            combo->setStyleSheet(
                        "QComboBox{background-color: green ;}"
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
                        "border-right: 10px solid green ;"
                        "border-bottom: 5px solid green ; ;"
                        " border-left: 10px  solid green ; ;"
                        "}"
                        );

            break;
        case 3:
            combo->setStyleSheet(
                        "QComboBox{background-color: yellow;}"
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
                        "border-right: 10px solid yellow;"
                        "border-bottom: 5px solid yellow;"
                        " border-left: 10px  solid yellow;"
                        "}"


                        );

            break;

        case 4:
            combo->setStyleSheet(            "QComboBox{background-color: cyan;}"
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
                                             "border-right: 10px solid cyan;"
                                             "border-bottom: 5px solid cyan;"
                                             " border-left: 10px  solid cyan;"
                                             "}"

                                             );


            break;
        default:
            break;
        }

    });

}



// Final  Del Combo Color


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
    case 1 : Guardar_Sonda(); break;
    case 2 : Guardar_Tanque(); break;
    case 10: guardar_limites(); break;
    case 4: Guardar_Comunicacion(); break;
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
    ui->Combo_Sonda->addItem(" ");
    Sondas[N] = new Sonda(this);
    Sondas[N]->setSerie(ui->line_Serie->text().toInt());
    Sondas[N]->setProtocolo(ui->Combo_protocolo->currentText().toInt());
    Sondas[N]->setFlotadores(ui->ComboFlotadores->currentText().toInt());
    Sondas[N]->setLicencia(ui->Line_Licencia->text());
//    Sondas[N]->setUnidad_Distancia(QString::number(ui->ComboUDistancia->currentIndex()));
//    Sondas[N]->setUnidad_Temperatura(QString::number(ui->Combo_UTemperatura->currentIndex()));
//    Sondas[N]->setUnidad_Volumen(QString::number(ui->Combo_UVolumen->currentIndex()));
    Sondas[N]->setAjuste_Altura(ui->Line_AjusteAltura->text().toInt());

    QSqlQuery qry;

    QString A("INSERT INTO `cistem`.`sonda` "
              "(`Serie`, `Protocolo`, `Flotadores`, "
              "`Licencia`, `Unidad Distancia`, `Unidad Temperatura`, "
              "`Unidad Volumen`, `Ajuste Altura`)"
              " VALUES ('"+ui->line_Serie->text()+"',"
                                                  " "+ui->Combo_protocolo->currentText()+","
                                                  " "+ui->ComboFlotadores->currentText()+","
                                                  " '"+ui->Line_Licencia->text()+"', "
                                                  "'"+ui->ComboUDistancia->currentText()+"'"
                                                  ", '"+ui->Combo_UTemperatura->currentText()+"',"
                                                  "'"+ui->Combo_UVolumen->currentText()+"',"
                                                  " "+ui->Line_AjusteAltura->text()+");");
    qDebug() << A;

    qry.exec(A);

    ui->line_Serie->clear();
    ui->Combo_protocolo->setCurrentIndex(0);
    ui->ComboFlotadores->setCurrentIndex(0);
    ui->Line_Licencia->clear();
    ui->ComboUDistancia->setCurrentIndex(0);
    ui->Combo_UTemperatura->setCurrentIndex(0);
    ui->Combo_UVolumen->setCurrentIndex(0);
    ui->Line_AjusteAltura->clear();

    ui->stackedWidget->setCurrentIndex(0);
    ui->Lab_Titulo->setText("Menu Principal");

    N++;
}

void MainWindow::Guardar_Tanque()
{
    Geometrytank();
    connect(tanques[S],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);

    ui->stackedWidget->setCurrentIndex(3);
    //ProGaugeId[N]=ID->toPlainText();
    //tanques[N]->setProtocolo(QcomboProtocolo->currentIndex());
    tanques[S]->setID(ui->Combo_Sonda->currentText());
    tanques[S]->SetnameTank(ui->Line_Nombre->text());
    //tanques[S]->color(Tconf->GetColor(),true);

    switch(G){
    case 1:
        tanques[S]->SetTankAltura(ui->Line_Diametro->text().toDouble());
        tanques[S]->SetTankLargo(ui->Line_Capacidad->text().toDouble());
        //     tanques[S]->SetTankAncho(ui->lineEdit->text().toDouble());
        tanques[S]->setTipo(2);
        break;
    case 2:
        tanques[S]->SetTankDiametro(ui->Line_Diametro->text().toDouble());
        tanques[S]->SetTankLargo(ui->Line_Capacidad->text().toDouble());
        tanques[S]->setTipo(1);                                                                           break;
    case 3:
        tanques[S]->SetTankDiametro(ui->Line_Diametro->text().toDouble());
        tanques[S]->SetTankAltura(ui->Line_Capacidad->text().toDouble());
        tanques[S]->setTipo(0);
        break;
    }
    Enviar_qry("INSERT INTO `cistem`.`tanques` "
               "(`Serie_Sonda`, `Nombre`, `Color`,`Tipo`,`Medida 1`,`Medida 2`) "
               "VALUES ('"+ui->Combo_Sonda->currentText()+"'"
                      ", '"+ui->Line_Nombre->text()+"'"
                      ", '"+QString::number(ui->Combo_Color->currentIndex())+"'"
                      ", '"+QString::number(ui->Combo_Tipo->currentIndex())+"'"
                      ", '"+ui->Line_Diametro->text()+"'"
                      ", '"+ui->Line_Capacidad->text()+"'"
                      ";");

    Descargar();
    // S++;
    ui->stackedWidget->setCurrentIndex(0);
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


            if(tanques[indice]->GetVolumen() > deliveryMaxVolumeRead || deliveryCountIncrement == 0){
                qDebug() << "ProGaugeVolumen:" << tanques[indice]->GetVolumen() << "deliveryMaxVolumeRead:" << deliveryMaxVolumeRead;

                deliveryMaxVolumeRead = tanques[indice]->GetVolumen();
                // ui->lbl_deliveryMaxVolumeRead->setText(QString::number(deliveryMaxVolumeRead));
            }

            evaluar_limites(tanques[indice]);
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
    QSqlQuery qry;
    
    if(qry.exec("SELECT * FROM `cistem`.`tanques`;"))
    {
        while (qry.next())
        {
            Geometrytank();
            connect(tanques[S],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);

            ui->stackedWidget->setCurrentIndex(3);
           // ProGaugeId[S]=qry.value(1).toString();
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
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
    else
    {
        QMessageBox::critical(this, "Error",tr(qry.lastError().text().toUtf8()));
    }
    if(qry.exec("SElECT * FROM CISTEM.LIMITES WHERE Id_Taque = 2"))
    {
       while(qry.next())
       {

           tanques[0]->SetVolMax(qry.value(2).toDouble());
           tanques[0]->SetProducto_Alto(qry.value(3).toDouble());
           tanques[0]->SetDesbordamiento(qry.value(4).toDouble());
           tanques[0]->SetNecesitaProducto(qry.value(5).toDouble());
           tanques[0]->SetProductoBajo(qry.value(6).toDouble());
           tanques[0]->SetAlarma_de_Agua(qry.value(7).toDouble());
           tanques[0]->SetAdvertencua_de_Agua(qry.value(8).toDouble());


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

void MainWindow::Geometrytank()
{
    if(S<4)
        tanques[S]= new Tanque(ui->Home,true);
    else
        tanques[S] = new Tanque(ui->Home2,true);
    switch(S){
    case 0:case 4: tanques[S]->Setgeometry(80,3,500,280);   break;
    case 1:case 5: tanques[S]->Setgeometry(600,3,500,280);  break;
    case 2:case 6: tanques[S]->Setgeometry(80,250,500,280); break;
    case 3:case 7: tanques[S]->Setgeometry(600,250,500,280);break;
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
    ui->stackedWidget->setCurrentIndex(6);
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
            if (tanques[i]->getTMaximizado() == true)
            {
                qDebug() << "hola desde este if :)";
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

    qDebug() << "Hola desde Tanque maximizado ;p";

    ui->stackedWidget->setCurrentIndex(STMaxi);
    frame = STMaxi;
}

void MainWindow::on_Regresar_clicked()
{
    if(frame > SMenuPub || frame ==SEntregas) frame =SMenuPub;
    else frame = SMenu;
    ui->Btn_Guardar->setVisible(false);
    ui->Regresar->setVisible(false);
    //ui->ComboSeleccion->setVisible(false);
    ui->stackedWidget->setCurrentIndex(frame);

}

void MainWindow::consultaBD()
{
    int i= 0;

    QSqlQuery qry;
    qry.exec("SELECT Serie_Sonda FROM `cistem`.`tanques` ;");
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

    for (int i = 0; i <= IDSerie; i++) {
        QString IDactual = ProGaugeId[i];
        qDebug() << "ProGaugeID" << ProGaugeId[i];
        if (IDactual == busqueda) {  indice = i; break;  }
    }

    tanques[indice]->offline();
}

void MainWindow::SendCMD()
{
    switch(ProGaugeCountCMD){
    case 0:
       // puertoserie->setDataTerminalReady(false);
        digitalWrite(12,HIGH);
        ProGaugeCountCMD++;
        break;
    case 1:
        puertoserie->write(("M"+ProGaugeId[ProGaugeCount1]).toUtf8()+char(13)+ char(10));
        qDebug() << ("M"+ProGaugeId[ProGaugeCount1]).toUtf8();
        ProGaugeCountCMD++;
        break;

    case 2:
      //  puertoserie->setDataTerminalReady(true);
        digitalWrite(12,LOW);
        ProGaugeCountCMD = 0;
        ProGaugeCount++;
        Time3->stop();
        disconnect(Time3,SIGNAL(timeout()),this,SLOT(SendCMD()));
        connect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));
        Time1->start(1500);                                                                      break;
    }

}

void MainWindow::on_Btn_Comunicacion_clicked()
{
    frame = SComunicacion;
    ui->stackedWidget->setCurrentIndex(SComunicacion);

    ui->Combo_Sonda->clear();
    ui->ComboSeleccion->clear();
    ui->ComboSeleccion->setVisible(true);
    //ui->stackedWidget->setCurrentIndex(3);
    ui->ComboSeleccion->addItem("Sonda");
    ui->ComboSeleccion->addItem("Comunicador");

    connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
            [=](int index){

        switch (index)
         {
         case 0:ui->stackedWidget->setCurrentIndex(8); break;
         case 1: ui->stackedWidget->setCurrentIndex(12); frame = 10;break;
         //case 2: Vertical();break;
         case 3:
            ui->stackedWidget->setCurrentIndex(4);
            ui->Lab_Titulo->setText("Tabla De Cubicacion");
        } });

}

void MainWindow::on_Btn_Barra_Estados_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::Guardar_Comunicacion()
{

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
    cadena = "SELECT Id_Taque FROM cistem.tanques"; // WHERE Configurado = 1;
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

void MainWindow::guardar_limites()
{
    QSqlQuery qry;
    QString cadena;
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

    tanques[0]->SetVolMax(ui->Line_volumen_maximo->text().toDouble());
    tanques[0]->SetProducto_Alto(ui->Line_producto_alto->text().toDouble());
    tanques[0]->SetDesbordamiento(ui->Line_desbordamiento->text().toDouble());
    tanques[0]->SetNecesitaProducto(ui->Line_limite_entrega->text().toDouble());
    tanques[0]->SetProductoBajo(ui->Line_producto_bajo->text().toDouble());
    tanques[0]->SetAlarma_de_Agua(ui->Line_alarma_agua->text().toDouble());
    tanques[0]->SetAdvertencua_de_Agua(ui->Line_advertencia_agua->text().toDouble());

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

}


void MainWindow::on_Combo_tanque_limites_currentIndexChanged(const QString &arg1)
{
    qDebug() << "tanque seleccionando en limites " << arg1;
    rellenar_limites();
}

void MainWindow::on_Btn_Entregas_clicked()
{
}

void MainWindow::deliveryProGaugeCountIncrement(){
    if(deliveryInProcess == 0){
        if(tanques[indice]->GetVolumen() <= deliveryLastInventoryRead){
            deliveryCountDecrement++;
            if(deliveryCountDecrement >= 10){
                deliveryCountDecrement = 0;
                deliveryCountIncrement = 0;
                deliveryInventoryStart = 0;
                //   ui->lbl_deliveryInventoryStart->setText(QString::number(deliveryInventoryStart));
                deliveryMaxVolumeRead = deliveryLastInventoryRead;
                //   ui->lbl_deliveryMaxVolumeRead->setText(QString::number(deliveryMaxVolumeRead));
            }
            if(deliveryCountIncrement >= 30){
                deliveryCountDecrement = 0;
                deliveryCountIncrement = 0;
                deliveryInProcess = 1;
                Volumen_inicial =  tanques[indice]->GetVolumen();
                ui->lbl_ProGaugeDeliveryInProccess->show();
            }
        } else{
            deliveryCountIncrement++;
            if(deliveryCountIncrement == 1.00){
                deliveryInventoryStart = tanques[indice]->GetVolumen();
                //  ui->lbl_deliveryInventoryStart->setText(QString::number(deliveryInventoryStart));
            }
            deliveryCountDecrement = 0;
        }
    }
    else{
        deliveryCountIncrement++;
        if(tanques[indice]->GetVolumen() > (deliveryLastInventoryRead + deliverySensivilityVolume)){
            deliveryCountIncrement = 0;
        }
        if(deliveryCountIncrement >= (1 * 60)){
            deliveryCountIncrement = 0;
            deliveryInProcess = 0;
            int VolumenEntregado=  Volumen_Final -Volumen_inicial;
            QSqlQuery qry;
            qry.exec("INSERT INTO `cistem`.`entregas` (`Tanque_Nombre`, `Volumen Inicial`, `Temperatura Inicial`, `Volumen Final`, `Temperatrura Final`, `Volumen Entregado`, `Fecha`) VALUES ('"+tanques[indice]->GetNameTank()+"', '"+QString::number(Volumen_inicial)+"', '"+QString::number(Temperatura_inicial)+"', '"+QString::number(Volumen_Final)+"', '"+QString::number(Temperatura_Final)+"', '"+QString::number(VolumenEntregado)+"', '"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"');");
            // qry.exec("INSERT INTO `cistem`.`entregas` (`Tanque_Nombre`, `Volumen Inicial`, `Volumen Final`, `Temperatura`, `Fecha`) VALUES ('"+tanques[indice]->GetNameTank()+"', '"+QString::number(Volumen_inicial)+"', '"+QString::number(tanques[indice]->GetVolumen())+"', '"+QString::number(tanques[indice]->GetTemperatura())+"', '"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"');");
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            Volumen_inicial =0;
            ui->lbl_ProGaugeDeliveryInProccess->hide();
            deliveryInventoryStart = 0;
            // ui->lbl_deliveryInventoryStart->setText(QString::number(deliveryInventoryStart));
        }
    }
    qDebug() <<tanques[indice]->GetVolumen() << deliveryLastInventoryRead << deliveryCountDecrement << deliveryCountIncrement;
    deliveryLastInventoryRead = tanques[indice]->GetVolumen();
    //ui->lbl_deliveryCountIncrement->setText(QString::number(deliveryCountIncrement));
    //ui->lbl_deliveryCountDecrement->setText(QString::number(deliveryCountDecrement));
    //ui->lbl_deliveryLastInventoryRead->setText(QString::number(deliveryLastInventoryRead));
}



void MainWindow::on_Btn_SaveTank_clicked()
{
    Geometrytank();
    connect(tanques[S],&Tanque::Camino,this,&MainWindow::Tanque_Maximisado);

//*** Configurar --- 0 Habilitado o Deshabilitado --- ***//

    if(ui->RHabilitado->isChecked())
    {
        tanques[S]->setIshabilitado(true);
    }
    else {
        tanques[S]->setIshabilitado(false);
    }
     // *** --- Etiqueta Tanque --- ***//
      tanques[S]->SetnameTank(ui->Line_Nombre->text());
     // ***---Codigo del Producto ---***
      tanques[S]->setCodigoProducto(ui->Line_Codigo_producto->text().toInt());
     // ***---Color Del Producto ---***//
      switch (ui->Combo_Color->currentIndex())
      {
      case 1: tanques[S]->color("gray", true);  break;
      case 2: tanques[S]->color("green",true);  break;
      case 3: tanques[S]->color("yellow",true); break;
      case 4: tanques[S]->color("cyan",true);   break;
      }
      // --- *** Codigo Combustible ***---//
      tanques[S]->setCodigoCombustible(ui->Line_CodigoCombustible->text().toInt());
      // ---*** Sonda ***---//
       tanques[S]->setID(ui->Combo_Sonda->currentText());
       // --- *** Ajuste De Altura *** --- //
       tanques[S]->setAjusteAltura(ui->Line_AjusteAltura->text().toDouble());
       // --- *** Capacidad ***-- //
       tanques[S]->setCapacidad(ui->Line_Capacidad->text().toDouble());
       // --- *** Diametro *** --- ///
       tanques[S]->SetTankDiametro(ui->Line_Diametro->text().toDouble());
       // --- *** Tipo de Tanque ***--- ///
       tanques[S]->setTipo(G);
       // --- *** Angulo *** --- //
       tanques[S]->setAngle(ui->Line_Angulo->text().toDouble());
       // --- *** Distancia *** ---  //
       tanques[S]->setFrombase(ui->Line_Distancia->text().toDouble());
       // --- *** Coeficiente Termico *** ---
       tanques[S]->setCoeficienteTermico(ui->Line_coeficiente->text().toDouble());
       // --- *** Producto *** --- //
       tanques[S]->setProducto(ui->Combo_Producto->currentText());

      Enviar_qry( "INSERT INTO `cistem`.`tanques` "
                  "(`Id_Taque`, `Configurado`, `Nombre`, "
                  "`CodigoProducto`, `Color`, `CodigoCombustible`, "
                  "`Serie_Sonda`, `AjusteAltura`, `Diametro`, "
                  "`Capacidad`, `Tipo`, `Angulo`, `Frombase`, "
                  "`CoeficienteTermico`, `Producto`) "
                  "VALUES ('"+QString::number(ui->Combo_IdTanque->currentIndex())+"',"
                  ""+ui->RHabilitado->isChecked()+" , 'Tanque 2', '00125', "
                  "'2', '25645', '00100', '0.002', '3500', "
                  "'4000', '2', '12', '100', '10', 'Gasolina');");


    Descargar();
    // S++;
    ui->stackedWidget->setCurrentIndex(SMenu);

}

void MainWindow::on_Btn_SalveComunicacion_clicked()
{
    QSqlQuery qry;
    qry.exec("INSERT INTO `cistem`.`config_sonda` (`Baudios`, `Datos`, `Stop`, `Paridad`) VALUES ('300', '2', '7', 'Igualdad');");

}

//int x(int y){
//    int h = 250;
//    int k = 250;

//    return
//}


void MainWindow::on_btn_menu_clicked()
{
      ui->stackedWidget->setCurrentIndex(SMenuPub);
}

void MainWindow::on_Btn_Inventario_clicked()
{
    frame = sInventario;
    ui->stackedWidget->setCurrentIndex(sInventario);
}

void MainWindow::on_Btn_Reports_clicked()
{
    frame = SReportes;
    ui->stackedWidget->setCurrentIndex(SReportes);
}

void MainWindow::on_Btn_Entregas_or_clicked()
{
    frame = SEntregas;
    ui->stackedWidget->setCurrentIndex(SEntregas);

    QString cadena;

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

   switch (frame) {
  // case 1: ui->Regresar->setVisible(true); ui->btn_menu->setVisible(true); break;
   case SHome: case SHome2: ui->btn_menu->setVisible(true); ui->Btn_user->setVisible(true); break;
   case SMenuPub: case SLogin: case SMenu: case STMaxi: break;
   case sInventario: case SReportes: case STablaCub: case SVialarmas:case SEntregas:
       ui->Regresar->setVisible(true); break;
   case STanque: ui->ComboSeleccion->setVisible(true); ui->Regresar->setVisible(true);ui->Btn_Guardar->setVisible(true); break;
   case Slimites: ui->ComboSeleccion->setVisible(true); ui->Regresar->setVisible(true);ui->Btn_Guardar->setVisible(true); break;
   default: ui->Regresar->setVisible(true);ui->Btn_Guardar->setVisible(true); break;

   }
}


