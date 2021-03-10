#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <tanque.h>
#include <QLineEdit>
#include <sonda.h>
#include <dialog.h>

#define SOH 0x01
const int lenbuff1 = 1024;              // Longitud de buffer, Ajustar
int xbuff1 = 0x00;                      // Índice: siguiente CHAR en cbuff
char cbuff1[lenbuff1];                  // Buffer
char rcvchar1 = 0x00;                   // último carácter recibido
//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Time2 = new QTimer();
    Time3 = new QTimer();

    QFont Fonttitle;
    Fonttitle.setPointSize(30);
    Fonttitle.setBold(true);
    Lab_Title = new QLabel(ui->Btn_Barra_Estados);
    Lab_Title->setGeometry(QRect(200,2,500,50));
    Lab_Title->setText("Estado Sistema   (▀̿̿Ĺ̯̿▀̿ ̿) ");
    Lab_Title->setFont(Fonttitle);

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

    for (int i=0; i < 3; i++)
    {
    Indicadores[i] = new QLabel(ui->Btn_Barra_Estados);
    Indicadores[i]->setGeometry(QRect(x,10,40,29));
    Indicadores[i]->setStyleSheet("color: white; background-color:cyan ; border: 3px solid gray; border-radius: 35px; padding: 2px;");
     x+=45;
    }


    Tconf = new Tanque(ui->Tanque,false);
    Maximizado = new Tanque(ui->Tanque_Maximizado,true);
    Tconf->Setgeometry(500,5,100,80);
    Tconf->SetnameTank("Sin Nombre");
    ConCombocol(ui->Combo_Color);

    connect(Time2,SIGNAL(timeout()),this,SLOT(Actualizar_Time()));
    Time2->start(1000);

     MainWindow::setFocus();
    ui->Line_AjusteAltura->installEventFilter(this);
    ui->Line_Licencia->installEventFilter(this);
    ui->line_Serie->installEventFilter(this);
    ui->Line_Usuario->installEventFilter(this);
    ui->Line_Contra->installEventFilter(this);
    ui->Line_Medida1->installEventFilter(this);
    ui->Line_Medida2->installEventFilter(this);
    ui->Line_Nombre->installEventFilter(this);
    //ui->lineEdit->installEventFilter(this);

   // ui->stackedWidget->setCurrentIndex(1);
    ui->Lab_Titulo->setText("Inicio");
    ocultar();

        puertoserie = new QSerialPort();
        Time1 = new QTimer();

        ui->stackedWidget->setCurrentIndex(3);

        //puertoserie->setPortName("Com3");
        puertoserie->setPortName("ttyUSB0");
        puertoserie->setBaudRate(QSerialPort::Baud9600);
        puertoserie->setDataBits(QSerialPort::Data8);
        puertoserie->setFlowControl(QSerialPort::NoFlowControl);
        puertoserie->setParity(QSerialPort::NoParity);
        puertoserie->setStopBits(QSerialPort::OneStop);
        puertoserie->open(QIODevice::ReadWrite);
        connect(puertoserie, &QSerialPort::readyRead, this, &MainWindow::Leer_datos);

        DB = QSqlDatabase::addDatabase("QMYSQL");
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
      Descargar();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Btn_Home_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->Lab_Titulo->setText("Inicio");
    ui->Btn_Guardar->setVisible(false);
    //ui->Btn_Config->setVisible(false);
    ui->Btn_user->setVisible(true);
    ui->Regresar->setVisible(false);
    ui->ComboSeleccion->setVisible(false);

    if (Maxi == true)
      { Maxi = false;

        qDebug() << "ando en el if de home";
        for (int i=0;i <= IDSerie-1; i++ )
        { tanques[i]->setTMaximizado(false);
               qDebug() << "entre al for";
               qDebug() << "Tanque" +QString::number(i)+ ":false";
        }

    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->Lab_Titulo->setText("Menu Principal");
}

void MainWindow::Botones_Barra()
{   MainWindow::setFocus();
    ui->Btn_Guardar->setVisible(true);
    ui->Regresar->setVisible(true);
    ui->Btn_user->setVisible(false);
}

void MainWindow::on_Btb_Sonda_clicked()
{
    Botones_Barra();
    ui->stackedWidget->setCurrentIndex(2);
    ui->Lab_Titulo->setText("Sonda");
    frame = 1;
}

/*--------------------------------------------------------------------------------------------------------
 * En esta parte del codigo se maneja el stalked del tanque, primeramente se le da el valor 2 a frame para
 * saber que estamos en el stalked 2 a la hora de guardar, se le da un focus al mainwindos para que el teclado
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
     frame = 2;
     Botones_Barra();
     ui->Combo_Sonda->clear();
     ui->ComboSeleccion->clear();
     ui->ComboSeleccion->setVisible(true);
     ui->stackedWidget->setCurrentIndex(3);
     ui->ComboSeleccion->addItem("General");
     ui->ComboSeleccion->addItem("Limites");

     connect(ui->ComboSeleccion, QOverload<int>::of(&QComboBox::activated),
             [=](int index){

         switch (index)
          {
          case 0:ui->stackedWidget->setCurrentIndex(3); break;
          case 1: ui->stackedWidget->setCurrentIndex(10);break;
          case 2: Vertical();break;
          case 3:
             ui->stackedWidget->setCurrentIndex(4);
             ui->Lab_Titulo->setText("Tabla De Cubicacion");
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
     connect(ui->Line_Nombre,&QLineEdit::textChanged,this,&MainWindow::Modificar_TextoTank);
     connect(ui->Combo_Tipo, QOverload<int>::of(&QComboBox::activated),
             [=](int index){

         switch (index)
          {
         // case 0: Rectangular();break;
          case 1: Horizontal();break;
          case 2: Vertical();break;
          case 3:
             ui->stackedWidget->setCurrentIndex(4);
             ui->Lab_Titulo->setText("Tabla De Cubicacion");
         } });

}//Fin del StackedTanque



/*--------------------------------------------------------------------------------------------------------------
 * Esta parte del codigo es para ocultar y mostrar los TextEdit del frame de configuracion de tanques estos metodos son
 * llamados en un switch  que esta en la parte superior, tambien consta de un slot local que es activado
 * cuando el combo es presionado.
 * -------------------------------------------------------------------------------------------------------------*/

// Inicio de ocultar y Mostrar

void MainWindow::Horizontal()
{
    ocultar();
    ui->Lab_Medida1->setText("Diametro");
    ui->Lab_Medida1->setGeometry(0,300,175,52);
    ui->Lab_Medida1->setVisible(true);
    ui->Line_Medida1->setVisible(true);
    ui->Lab_Medida2->setText("Largo");
    ui->Lab_Medida2->setVisible(true);
    ui->Line_Medida2->setVisible(true);
  //  ui->lineEdit->setText("0");
    G = 1;

}

void MainWindow::Vertical()
{
    ocultar();
    ui->Lab_Medida1->setText("Diametro");
    ui->Lab_Medida1->setGeometry(0,300,175,52);
    ui->Lab_Medida1->setVisible(true);
    ui->Line_Medida1->setVisible(true);
    ui->Lab_Medida2->setText("Alto");
    ui->Lab_Medida2->setVisible(true);
    ui->Line_Medida2->setVisible(true);
  //  ui->lineEdit->setText("0");
    G=0;

}

//void MainWindow::Rectangular()
//{
//    ocultar();
//    ui->Lab_Medida1->setText("Altura");
//    ui->Lab_Medida1->setGeometry(48,300,120,52);
//    ui->Lab_Medida1->setVisible(true);
//    ui->Line_Medida1->setVisible(true);
//    ui->Lab_Medida2->setText("Largo");
//    ui->Lab_Medida2->setVisible(true);
//    ui->Line_Medida2->setVisible(true);
//    ui->Lab_Medida3->setText("Ancho");
//    ui->Lab_Medida3->setVisible(true);
//    ui->lineEdit->setVisible(true);
//    G=2;

//}

void MainWindow::ocultar()
{

    ui->Lab_Medida1->setVisible(false);
    ui->Line_Medida1->setVisible(false);
    ui->Lab_Medida2->setVisible(false);
    ui->Line_Medida2->setVisible(false);
   // ui->Lab_Medida3->setVisible(false);
    //ui->lineEdit->setVisible(false);
    ui->ComboSeleccion->setVisible(false);



} //Fin de de ocultar y Mostrar


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
           combo->setStyleSheet( "color: gray ;");
            Tconf->color("gray",false);
            combo->setItemText(1,"█████████");
            combo->setItemText(2,"");
            combo->setItemText(3,"");
            combo->setItemText(4,"");
                                                break;

       case 2:
             combo->setStyleSheet("color: green;");
             Tconf->color("green",false);
             combo->setItemText(1,"");
             combo->setItemText(2,"█████████");
             combo->setItemText(3,"");
             combo->setItemText(4,"");
                                                     break;
       case 3:
             combo->setStyleSheet("color: yellow;");
             Tconf->color("yellow",false);
             combo->setItemText(1,"");
             combo->setItemText(2,"");
             combo->setItemText(3,"█████████");
             combo->setItemText(4,"");
                                                     break;

       case 4:
             combo->setStyleSheet("QComboBox{color: cyan;}");
             Tconf->color("cyan",false);
             combo->setItemText(1,"");
             combo->setItemText(2,"");
             combo->setItemText(3,"");
             combo->setItemText(4,"█████████");

                                                     break;
       default:
           break;
       }

    });

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

void MainWindow::on_Btn_Guardar_clicked()
{ switch(frame){ case 1 : Guardar_Sonda(); break; case 2 : Guardar_Tanque(); break;} }

void MainWindow::Actualizar_Time()
{
     Reloj->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss ap"));
}

void MainWindow::on_Btn_Config_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->Btn_Guardar->setVisible(false);
    //ui->Btn_Config->setVisible(false);
    ui->Btn_user->setVisible(false);
    ui->Lab_Titulo->setText("Configuracion");

}

void MainWindow::on_Btn_user_clicked()
{

    MainWindow::setFocus();
    ui->stackedWidget->setCurrentIndex(5);
    ui->Btn_Guardar->setVisible(false);
    //ui->Btn_Config->setVisible(false);
    ui->Btn_user->setVisible(false);
    ui->Regresar->setVisible(false);
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
        MainWindow::setFocus();
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
    if(obj == ui->Line_Medida1 && event->type() == QEvent::FocusIn)
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
            ui->Line_Medida1->clear();
            ui->Line_Medida1->setText(dlg->getDatos());
        }

        delete dlg;
        return true;
    }
    if(obj == ui->Line_Medida2 && event->type() == QEvent::FocusIn)
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
            ui->Line_Medida2->clear();
            ui->Line_Medida2->setText(dlg->getDatos());
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
        dato = data.at(0);
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
    Sondas[N]->setUnidad_Distancia(QString::number(ui->ComboUDistancia->currentIndex()));
    Sondas[N]->setUnidad_Temperatura(QString::number(ui->Combo_UTemperatura->currentIndex()));
    Sondas[N]->setUnidad_Volumen(QString::number(ui->Combo_UVolumen->currentIndex()));
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
        tanques[S]->color(Tconf->GetColor(),true);

        switch(G){
        case 1:
        tanques[S]->SetTankAltura(ui->Line_Medida1->text().toDouble());
        tanques[S]->SetTankLargo(ui->Line_Medida2->text().toDouble());
   //     tanques[S]->SetTankAncho(ui->lineEdit->text().toDouble());
        tanques[S]->setTipo(2);
            break;
        case 2:
            tanques[S]->SetTankDiametro(ui->Line_Medida1->text().toDouble());
            tanques[S]->SetTankLargo(ui->Line_Medida2->text().toDouble());
            tanques[S]->setTipo(1);                                                                           break;
         case 3:
             tanques[S]->SetTankDiametro(ui->Line_Medida1->text().toDouble());
             tanques[S]->SetTankAltura(ui->Line_Medida2->text().toDouble());
             tanques[S]->setTipo(0);
            break;
        }
        Enviar_qry("INSERT INTO `cistem`.`tanques` "
                "(`Serie_Sonda`, `Nombre`, `Color`,`Tipo`,`Medida 1`,`Medida 2`,`Medida 3`) "
                 "VALUES ('"+ui->Combo_Sonda->currentText()+"'"
                 ", '"+ui->Line_Nombre->text()+"'"
                 ", '"+QString::number(ui->Combo_Color->currentIndex())+"'"
                 ", '"+QString::number(ui->Combo_Tipo->currentIndex())+"'"
                 ", '"+ui->Line_Medida1->text()+"'"
                 ", '"+ui->Line_Medida2->text()+"'"
                 ", '"/*ui->lineEdit->text()*/"');");

       Descargar();
      // S++;
        ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::Protocolo(QString cad)
{
    QSqlQuery qry;

    int indice =0;

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
        ProGaugeId[S]=qry.value(1).toString();
        //tanques[N]->setProtocolo(QcomboProtocolo->currentIndex());
        tanques[S]->setID(qry.value(1).toString());
        tanques[S]->SetnameTank(qry.value(2).toString());
        switch (qry.value(3).toInt())
        {
        case 1: tanques[S]->color("gray", true);  break;
        case 2: tanques[S]->color("green",true);  break;
        case 3: tanques[S]->color("yellow",true); break;
        case 4: tanques[S]->color("cyan",true);   break;
        }

        switch(qry.value(4).toInt()){
        case 1:
        tanques[S]->SetTankAltura(qry.value(5).toDouble());
        tanques[S]->SetTankLargo(qry.value(6).toDouble());
        tanques[S]->SetTankAncho(qry.value(7).toDouble());
        tanques[S]->setTipo(2);
                                                  break;
         case 2:
            tanques[S]->SetTankDiametro(qry.value(5).toDouble());
            tanques[S]->SetTankLargo(qry.value(6).toDouble());
            tanques[S]->setTipo(1);               break;
         case 3:
             tanques[S]->SetTankDiametro(qry.value(5).toDouble());
             tanques[S]->SetTankAltura(qry.value(6).toDouble());
             tanques[S]->setTipo(0);
                                                  break;
        }
        S++;
        ui->stackedWidget->setCurrentIndex(0);
    }
}
else
{
    QMessageBox::critical(this, "Error",tr(qry.lastError().text().toUtf8()));
}
}

void MainWindow::Geometrytank()
{
    if(S<4)
    tanques[S]= new Tanque(ui->Home,true);
    else
   tanques[S] = new Tanque(ui->Home2,true);
    switch(S){
    case 0: tanques[S]->Setgeometry(80,3,500,280);   break;
    case 1: tanques[S]->Setgeometry(600,3,500,280);  break;
    case 2: tanques[S]->Setgeometry(80,250,500,280); break;
    case 3: tanques[S]->Setgeometry(600,250,500,280);break;
    case 4: tanques[S]->Setgeometry(80,3,500,280);   break;
    case 5: tanques[S]->Setgeometry(600,3,500,280);  break;
    case 6: tanques[S]->Setgeometry(80,250,500,280); break;
    case 7: tanques[S]->Setgeometry(600,250,500,280);break;}
      }

void MainWindow::Estados()
{
   switch(ProGaugeCount){
    case 0:
           connect(Time3,SIGNAL(timeout()),this,SLOT(SendCMD()));
           Time3->start(22); Time1->stop();
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

    Maximizado->Setgeometry(0,-30,1200,580);
    Maximizado->setID(tanques[indiceM]->getID());
    ui->Lab_Titulo->setText(tanques[indiceM]->GetNameTank());
    Maximizado->SetnameTank("");
    Maximizado->color(tanques[indiceM]->GetColor(),false);
    Maximizado->SetVolMax(tanques[indiceM]->GetVolMax());
    Maximizado->SetProducto_Alto(tanques[indiceM]->GetProducto_Alto());
    Maximizado->SetDesvordamiento(tanques[indiceM]->GetDesbordamiento());
    Maximizado->SetNecesitaProducto(tanques[indiceM]->GetNecesitaProducto());
    Maximizado->SetProductoBajo(tanques[indiceM]->GetProducto());
    Maximizado->SetAlarma_de_Agua(tanques[indiceM]->GetAlarma_de_Agua());
    Maximizado->SetAdvertencua_de_Agua(tanques[indiceM]->GetAdvertencia_de_Agua());
    Maximizado->setTankTiempoEntrega(tanques[indiceM]->getTankTiempoEntrega());
    Maximizado->setTankMinimoEntrega(tanques[indiceM]->getTankMinimoEntrega());

    qDebug() << "ID" << Maximizado->getID();

   switch(tanques[indiceM]->getTipo()){
       case 2:
               Maximizado->SetTankAltura(tanques[indiceM]->GetTanqueAltura());
               Maximizado->SetTankLargo(tanques[indiceM]->GetTankLargo());
               Maximizado->SetTankAncho(tanques[indiceM]->GetTankAncho());
               Maximizado->setTipo(2);
                                         break;
       case 1:
               Maximizado->SetTankDiametro(tanques[indiceM]->GetTankDiametro());
               Maximizado->SetTankLargo(tanques[indiceM]->GetTankLargo());
               Maximizado->setTipo(1);
                                          break;
       case 0:
               Maximizado->SetTankDiametro(tanques[indiceM]->GetTankDiametro());
               Maximizado->SetTankAltura(tanques[indiceM]->GetTanqueAltura());
               Maximizado->setTipo(0);
                                           break;
    }

    }
   Maximizado->SetAltura(tanques[indiceM]->GetAltura(),100.00);
   Maximizado->SetTemperatura(tanques[indiceM]->GetTemperatura());
   inicbuff1();

   qDebug() << "Hola desde Tanque maximizado ;p";

   ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_Regresar_clicked()
{
    frame = 0;
    ui->Btn_Guardar->setVisible(false);
    ui->Regresar->setVisible(false);
    ui->ComboSeleccion->setVisible(false);
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
              Time1->start(1000);
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
           puertoserie->setDataTerminalReady(false);
           ProGaugeCountCMD++;
                                                                          break;
     case 1:
           puertoserie->write(("M"+ProGaugeId[ProGaugeCount1]).toUtf8()+char(13)+ char(10));
           qDebug() << ("M"+ProGaugeId[ProGaugeCount1]).toUtf8();
           ProGaugeCountCMD++;
                                                                           break;
       case 2:
           puertoserie->setDataTerminalReady(true);
           ProGaugeCountCMD = 0;
           ProGaugeCount++;
           Time3->stop();
           disconnect(Time3,SIGNAL(timeout()),this,SLOT(SendCMD()));
           connect(Time1,SIGNAL(timeout()),this,SLOT(Estados()));
           Time1->start(1000);
                                                                           break;
    }

}

void MainWindow::on_Btn_Comunicacion_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}


void MainWindow::on_Btn_Barra_Estados_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::on_Btn_tabla_cubicacion_clicked()
{

}

void MainWindow::on_Btn_Alarmas_clicked()
{

}
