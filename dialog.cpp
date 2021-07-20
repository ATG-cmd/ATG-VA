#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window |Qt::FramelessWindowHint);

    setMaximumWidth(1860);
    setMaximumHeight(735);

    Fontstyle.setPointSize(40);
    Fontstyle.setBold(true);
    debug = new QLabel(this);
    grupo =new QGroupBox(this);
    label = new QLabel(this);
    line = new QLineEdit(this);

    for (int i = 0; i <= 37;i++) {
        btn[i] = new QPushButton(grupo);
        btn[i]->setFont(Fontstyle);
    }

    mostrar_tipo_teclado(1);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::btn_setText()
{
    btn[0]->setText(getNivel() == 0 ? "Q":"q");
    btn[1]->setText(getNivel() == 0 ? "W":"w");
    btn[2]->setText(getNivel() == 0 ? "E":"e");
    btn[3]->setText(getNivel() == 0 ? "R":"r");
    btn[4]->setText(getNivel() == 0 ? "T":"t");
    btn[5]->setText(getNivel() == 0 ? "Y":"y");
    btn[6]->setText(getNivel() == 0 ? "U":"u");
    btn[7]->setText(getNivel() == 0 ? "I":"i");
    btn[8]->setText(getNivel() == 0 ? "O":"o");
    btn[9]->setText(getNivel() == 0 ? "P":"p");
    btn[10]->setText(getNivel() == 0 ? "←":"←");
    btn[11]->setText(getNivel() == 0 ? "A":"a");
    btn[12]->setText(getNivel() == 0 ? "S":"s");
    btn[13]->setText(getNivel() == 0 ? "D":"d");
    btn[14]->setText(getNivel() == 0 ? "F":"f");
    btn[15]->setText(getNivel() == 0 ? "G":"g");
    btn[16]->setText(getNivel() == 0 ? "H":"h");
    btn[17]->setText(getNivel() == 0 ? "J":"j");
    btn[18]->setText(getNivel() == 0 ? "K":"k");
    btn[19]->setText(getNivel() == 0 ? "L":"l");
    btn[20]->setText(getNivel() == 0 ? "Enter":"Enter");
    btn[21]->setText(getNivel() == 0 ? "SHIFT":"SHIFT");
    btn[22]->setText(getNivel() == 0 ? "Z":"z");
    btn[23]->setText(getNivel() == 0 ? "X":"x");
    btn[24]->setText(getNivel() == 0 ? "C":"c");
    btn[25]->setText(getNivel() == 0 ? "V":"v");
    btn[26]->setText(getNivel() == 0 ? "B":"b");
    btn[27]->setText(getNivel() == 0 ? "N":"n");
    btn[28]->setText(getNivel() == 0 ? "M":"m");
    btn[29]->setText(getNivel() == 0 ? ",":",");
    btn[30]->setText(getNivel() == 0 ? ".":".");
    btn[31]->setText(getNivel() == 0 ? "SHIFT":"SHIFT");
    btn[32]->setText(getNivel() == 0 ? "&123":"&123");
    btn[33]->setText(getNivel() == 0 ? "@":"@");
    btn[34]->setText(getNivel() == 0 ? "SPACE":"SPACE");
    btn[35]->setText(getNivel() == 0 ? "'":"'");
    btn[36]->setText(getNivel() == 0 ? "/":"/");
    btn[37]->setText(getNivel() == 0 ? "#":"#");

}

void Dialog::btn_setNum()
{
    btn[0]->setText(getNivel() == 0 ? "1":"~");
    btn[1]->setText(getNivel() == 0 ? "2":"");
    btn[2]->setText(getNivel() == 0 ? "3":"");
    btn[3]->setText(getNivel() == 0 ? "4":"/");
    btn[4]->setText(getNivel() == 0 ? "5":"x");
    btn[5]->setText(getNivel() == 0 ? "6":"");
    btn[6]->setText(getNivel() == 0 ? "7":"{");
    btn[7]->setText(getNivel() == 0 ? "8":"}");
    btn[8]->setText(getNivel() == 0 ? "9":"$");
    btn[9]->setText(getNivel() == 0 ? "0":"");
    btn[10]->setText(getNivel() == 0 ? "←":"←");
    btn[11]->setText(getNivel() == 0 ? "@":"a");
    btn[12]->setText(getNivel() == 0 ? "#":"");
    btn[13]->setText(getNivel() == 0 ? "%":"");
    btn[14]->setText(getNivel() == 0 ? "&":"");
    btn[15]->setText(getNivel() == 0 ? "*":"");
    btn[16]->setText(getNivel() == 0 ? "-":"");
    btn[17]->setText(getNivel() == 0 ? "+":"");
    btn[18]->setText(getNivel() == 0 ? "(":"");
    btn[19]->setText(getNivel() == 0 ? ")":"");
    btn[20]->setText(getNivel() == 0 ? "Enter":"Enter");
    btn[21]->setText(getNivel() == 0 ? "1/2":"2/2");
    btn[22]->setText(getNivel() == 0 ? "!":"");
    btn[23]->setText(getNivel() == 0 ? "''":"");
    btn[24]->setText(getNivel() == 0 ? "<":"");
    btn[25]->setText(getNivel() == 0 ? ">":"");
    btn[26]->setText(getNivel() == 0 ? "'":"");
    btn[27]->setText(getNivel() == 0 ? ":":"");
    btn[28]->setText(getNivel() == 0 ? ";":"");
    btn[29]->setText(getNivel() == 0 ? "/":"");
    btn[30]->setText(getNivel() == 0 ? "?":"");
    btn[31]->setText(getNivel() == 0 ? "1/2":"2/2");
    btn[32]->setText(getNivel() == 0 ? "ABC":"ABC");
    btn[33]->setText(getNivel() == 0 ? "@":"@");
    btn[34]->setText(getNivel() == 0 ? "SPACE":"SPACE");
    btn[35]->setText(getNivel() == 0 ? ".":"");
    btn[36]->setText(getNivel() == 0 ? ":)":":(");
    btn[37]->setText(getNivel() == 0 ? "#":"#");


}

void Dialog::btn_char()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;
    line->insert(btn->text());
    line->setFocus();               // revisar los efectos del setfocus
                                    // porque deja activado el el focus en el btn[0]

}

void Dialog::btn_SHIFT()
{

    switch (shift) {
    // caso Mayus unitario
        case 0:
        setNivel(0);
        if(getTypekeyboar() == 0){
            btn[21]->setStyleSheet("color: green; background-color: rgb(97, 97, 97);");
            btn[31]->setStyleSheet("color: green; background-color: rgb(97, 97, 97);");
            btn_setText();
        }else {
            btn_setNum();
        }
        setShift(1);
        break;
    // caso minus
        case 1:
        setNivel(1);
        if(getTypekeyboar() == 0){
            btn[21]->setStyleSheet(" background-color: rgb(97, 97, 97);");
            btn[31]->setStyleSheet(" background-color: rgb(97, 97, 97);");
            btn_setText();
            setShift(2);
        }else {
            btn_setNum();
            setShift(0);
        }
        setNivel(1);
        break;
    // caso mayus
        case 2:
        btn[21]->setStyleSheet("color: green; background-color: rgb(160, 160, 160);");
        btn[31]->setStyleSheet("color: green; background-color: rgb(160, 160, 160);");
        setNivel(0);
        btn_setText();
        setShift(0);
        break;
    default:break;
    }

}



void Dialog::btn_num()          //cambio de teclado
{
    if(getTypekeyboar() == 0){
        //setShift(0);
        setTypekeyboar(1);
        setNivel(0);
        btn[21]->setStyleSheet(" background-color: rgb(97, 97, 97);");
        btn[31]->setStyleSheet(" background-color: rgb(97, 97, 97);");
        btn_setNum();
    }
    else if(getTypekeyboar() == 1){
        btn_setText();
        setTypekeyboar(0);
    }

}

void Dialog::btn_exit()
{
    reject();
}

void Dialog::btn_SPACE()
{
    line->insert(" ");
}

void Dialog::btn_bks()
{
    line->backspace();
}


// SLOT QUE SE EJECUTA CON LA SEÑAL RETURN PRESSED
void Dialog::data_out()
{
    setDatos(line->text());
    accept();
}

void Dialog::dobleclick()
{
    debug->setText("se hizo dobleclick");
}

int Dialog::getVal_index() const
{
    return val_index;
}

void Dialog::setVal_index(int value)
{
    val_index = value;
}

bool Dialog::getIsvalidator() const
{
    return isvalidator;
}

void Dialog::setIsvalidator(bool value)
{
    isvalidator = value;
}



void Dialog::set_etiqueta(QString lb)
{
    label->setText(lb);
}

int Dialog::getNivel() const
{
    return nivel;
}

void Dialog::setNivel(int value)
{
    nivel = value;
}

void Dialog::conexiones_alfa()
{
    connect(btn[0],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[1],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[2],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[3],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[4],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[5],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[6],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[7],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[8],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[9],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[11],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[12],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[13],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[14],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[15],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[16],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[17],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[18],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[19],&QPushButton::clicked,this,&Dialog::btn_char);

    connect(btn[22],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[23],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[24],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[25],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[26],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[27],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[28],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[29],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[30],&QPushButton::clicked,this,&Dialog::btn_char);

    connect(btn[10],&QPushButton::clicked,this,&Dialog::btn_bks);
    connect(btn[20],&QPushButton::clicked,this,&Dialog::btn_ENTER);
    connect(btn[21],&QPushButton::clicked,this,&Dialog::btn_SHIFT);
    connect(btn[31],&QPushButton::clicked,this,&Dialog::btn_SHIFT);
    connect(btn[32],&QPushButton::clicked,this,&Dialog::btn_num);
    connect(btn[34],&QPushButton::clicked,this,&Dialog::btn_SPACE);
    connect(btn[37],&QPushButton::clicked,this,&Dialog::btn_exit);

    connect(line,&QLineEdit::returnPressed,this,&Dialog::data_out);
}

void Dialog::conexiones_num()
{

    connect(btn[0],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[1],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[2],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[4],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[5],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[6],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[8],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[9],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[10],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[12],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[13],&QPushButton::clicked,this,&Dialog::btn_char);
    connect(btn[14],&QPushButton::clicked,this,&Dialog::btn_char);

    connect(btn[3],&QPushButton::clicked,this,&Dialog::btn_bks);
    connect(btn[7],&QPushButton::clicked,this,&Dialog::btn_SPACE);
    connect(btn[11],&QPushButton::clicked,this,&Dialog::btn_exit);
    connect(btn[15],&QPushButton::clicked,this,&Dialog::btn_ENTER);

    connect(line,&QLineEdit::returnPressed,this,&Dialog::data_out);
}

void Dialog::desconexiones_alfa()
{
    disconnect(btn[0],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[1],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[2],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[3],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[4],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[5],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[6],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[7],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[8],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[9],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[11],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[12],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[13],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[14],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[15],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[16],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[17],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[18],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[19],&QPushButton::clicked,this,&Dialog::btn_char);

    disconnect(btn[22],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[23],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[24],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[25],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[26],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[27],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[28],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[29],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[30],&QPushButton::clicked,this,&Dialog::btn_char);

    disconnect(btn[10],&QPushButton::clicked,this,&Dialog::btn_bks);
    disconnect(btn[20],&QPushButton::clicked,this,&Dialog::btn_ENTER);
    disconnect(btn[21],&QPushButton::clicked,this,&Dialog::btn_SHIFT);
    disconnect(btn[31],&QPushButton::clicked,this,&Dialog::btn_SHIFT);
    disconnect(btn[32],&QPushButton::clicked,this,&Dialog::btn_num);
    disconnect(btn[34],&QPushButton::clicked,this,&Dialog::btn_SPACE);
    disconnect(btn[37],&QPushButton::clicked,this,&Dialog::btn_exit);

    disconnect(line,&QLineEdit::returnPressed,this,&Dialog::data_out);
}

void Dialog::desconexiones_num()
{
    disconnect(btn[0],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[1],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[2],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[4],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[5],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[6],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[8],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[9],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[10],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[12],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[13],&QPushButton::clicked,this,&Dialog::btn_char);
    disconnect(btn[14],&QPushButton::clicked,this,&Dialog::btn_char);

    disconnect(btn[3],&QPushButton::clicked,this,&Dialog::btn_bks);
    disconnect(btn[7],&QPushButton::clicked,this,&Dialog::btn_SPACE);
    disconnect(btn[11],&QPushButton::clicked,this,&Dialog::btn_exit);
    disconnect(btn[15],&QPushButton::clicked,this,&Dialog::btn_ENTER);

    disconnect(line,&QLineEdit::returnPressed,this,&Dialog::data_out);
}

void Dialog::teclado_alfanum()
{

    grupo->setGeometry(30,240,1800,465);  // grupo->setGeometry(10,90,600,165);
    //grupo->setStyleSheet("border: 2px solid blue;");

    debug->setGeometry(0,0,1800,60);
    debug->setText("aa");
    debug->setFont(Fontstyle);

    label->setGeometry(30,30,1800,75);   //label->setGeometry(180,10,300,20);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(Fontstyle);
   // label->setStyleSheet("border: 2px solid blue;");
   // label->setText("Ingrese un numero en el rango de");
    line->setGeometry(330,135,1200,75);   //line->setGeometry(180,40,220,25);
    line->setFocus();
    line->setFont(Fontstyle);

    for(int i = 0; i <= 10;i++)
   {
      btn[i]->setGeometry((i *(btnX+btnXesp)),(0*(btnY+btnYesp)),btnX,btnY);
   }
    for(int i = 0; i <= 8 ;i++)
   {
      btn[i+11]->setGeometry(20+(i *(btnX+btnXesp)),(1*(btnY+btnYesp)),btnX,btnY);
   }

      btn[20]->setGeometry(20+(9 *(btnX+btnXesp)),(1*(btnY+btnYesp)),btnX+145,btnY);
    for(int i = 0; i <= 10;i++)
   {
      btn[i+21]->setGeometry((i *(btnX+btnXesp)),(2*(btnY+btnYesp)),btnX,btnY);
   }
    btn[32]->setGeometry(( 0*(btnX+btnXesp)),(3*(btnY+btnYesp)),btnX+25,btnY);
    btn[33]->setGeometry(25+(1 *(btnX+btnXesp)),(3*(btnY+btnYesp)),btnX,btnY);
    btn[34]->setGeometry(25+(2 *(btnX+btnXesp)),(3*(btnY+btnYesp)),5*(btnX+btnXesp)+50,btnY);
    btn[35]->setGeometry(90+(7 *(btnX+btnXesp)),(3*(btnY+btnYesp)),btnX,btnY);
    btn[36]->setGeometry(90+(8 *(btnX+btnXesp)),(3*(btnY+btnYesp)),btnX,btnY);
    btn[37]->setGeometry(90+(9 *(btnX+btnXesp)),(3*(btnY+btnYesp)),btnX+75,btnY);

    btn[10]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[20]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[21]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[31]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[32]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[37]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");


    for(int i = 0; i <= 38; i++)
    {
    btn[i]->setFocusPolicy(Qt::NoFocus);
    btn[i]->setFont(Fontstyle);
    }

    conexiones_alfa();
}

void Dialog::teclado_num()
{
    grupo->setGeometry(547,240,765,465);
    label->setGeometry(30,30,1800,75);   //label->setGeometry(180,10,300,20);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(Fontstyle);
    line->setGeometry(547,135,765,75);   //line->setGeometry(180,40,220,25);
    line->setFocus();
    line->setFont(Fontstyle);

    for (int i = 0; i <= 3;i++) {
        btn[i]->setGeometry((i *(btnX1+btnXesp1)),(0*(btnY1+btnYesp1)),btnX1,btnY1);
    }
    for (int i = 0; i <= 3;i++) {
        btn[i+4]->setGeometry((i *(btnX1+btnXesp1)),(1*(btnY1+btnYesp1)),btnX1,btnY1);
    }
    for (int i = 0; i <= 3;i++) {
        btn[i+8]->setGeometry((i *(btnX1+btnXesp1)),(2*(btnY1+btnYesp1)),btnX1,btnY1);
    }
    for (int i = 0; i <= 3;i++) {
        btn[i+12]->setGeometry((i*(btnX1+btnXesp1)),(3*(btnY1+btnYesp1)),btnX1,btnY1);
    }
    for (int j = 16; j <= 37; j++) {
        btn[j]->setGeometry(0,0,0,0);
    }

    btn[3]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[7]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[11]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");
    btn[15]->setStyleSheet(" QPushButton{background-color: rgb(60, 60, 60);}QPushButton:pressed {background-color: rgb(220, 220,220);}");

    btn[0]->setText("1");
    btn[1]->setText("2");
    btn[2]->setText("3");
    btn[3]->setText("←");
    btn[4]->setText("4");
    btn[5]->setText("5");
    btn[6]->setText("6");
    btn[7]->setText("_");
    btn[8]->setText("7");
    btn[9]->setText("8");
    btn[10]->setText("9");
    btn[11]->setText("X");
    btn[12]->setText("");
    btn[13]->setText("0");
    btn[14]->setText("#");
    btn[15]->setText("ENTER");

    conexiones_num();
}

void Dialog::mostrar_tipo_teclado(int tipo)
{
    switch (tipo) {
    case 1: teclado_alfanum(); btn_setNum(); break;  //alfanum
    case 2: desconexiones_alfa();
            teclado_num();
          break;  //num
    default: break;
    }
}

QString Dialog::getDatos() const
{
    return datos;
}

void Dialog::setDatos(const QString &value)
{
    datos = value;
}

// Con esta funcion asignamos la validacion por QVALIDATOR
void Dialog::validador(int index, int bottom, int top, int dec)
{
    setIsvalidator(true);

    switch (index) {
    case 0:
        setVal_index(index);
        line->setValidator(nullptr);
        break;
    case 1:
        setVal_index(index);
        v_int = new QIntValidator(bottom,top,this);
        line->setValidator(v_int);
        break;
    case 2:
        setVal_index(index);
       v_dou = new QDoubleValidator(bottom,top,dec,this);
       line->setValidator(v_dou);
        break;
    }
}

void Dialog::maskarator(int index, QString mask)
{
    setIsvalidator(false);
    qDebug() << mask;
    switch (index) {
    case 0:                // fecha 9999 99 99
        line->setInputMask(mask);
        break;
    case 1:               // ip  000 000 000 000
        line->setInputMask(mask);
        break;
    case 2:              //  mac hh hh hh hh hh hh
        line->setInputMask(mask);

        break;
    case 3:             //  telefono 99 99 99 99 99
        line->setInputMask(mask);
        line->setEchoMode(QLineEdit::Password);
        break;
    }
}

void Dialog::btn_ENTER()
{
    qDebug() << "btn enter";
    int i = 0;
    QString datos = line->text();
    if(getIsvalidator() == true)
    {
        switch (getVal_index())
        {
        case 0:
             qDebug() << "caso 0 aceptable";
            getDatos();
            break;
        case 1:
            if(v_int->validate(datos,i) == QValidator::Acceptable)
            {
                qDebug() << "caso 1 aceptable";

                data_out();
            }
            break;
        case 2:
            if(v_dou->validate(datos,i) == QValidator::Acceptable)
            {
                qDebug() << "caso 2 aceptable";
                data_out();
            }
            break;
        }
    }
    else
    {
         qDebug() << "validando mask";

        if(line->hasAcceptableInput() != false)
        {
         qDebug() << "has acceptable input";
         data_out();
        }
        else qDebug() << line->hasAcceptableInput();
    }

}

int Dialog::getShift() const
{
    return shift;
}

void Dialog::setShift(int value)
{
    shift = value;
}

int Dialog::getTypekeyboar() const
{
    return typekeyboar;
}

void Dialog::setTypekeyboar(int value)
{
    typekeyboar = value;
}


