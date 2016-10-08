#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushNuevo_clicked()
{
    QFileDialog dialogo(this);
    QString ruta;
    bool bandera;

    dialogo.setWindowTitle("Nuevo Diccionario");
    dialogo.setDefaultSuffix("dd");
    dialogo.setAcceptMode(QFileDialog::AcceptSave);
    dialogo.setNameFilter("Diccionario (*.dd)");
    dialogo.setDirectory(QDir::currentPath());

    if (dialogo.exec() == QDialog::Accepted)
    {
        ruta = dialogo.selectedFiles()[0];
        qDebug() << ruta;
        if(!ruta.isEmpty())
        {
            this->ui->label->setText(ruta);
            bandera = this->diccionario.nuevo_Diccionario(ruta.toStdString());
            if(bandera)
            {
                this->inicia_tablaAtributos();
            }
        }
    }
    return;
}

void Dialog::on_pushAbrir_clicked()
{
    QFileDialog dialogo(this);
    QString ruta;
    bool bandera;

    dialogo.setWindowTitle("Abrir Diccionario");
    dialogo.setDefaultSuffix("dd");
    dialogo.setAcceptMode(QFileDialog::AcceptOpen);
    dialogo.setNameFilter("Diccionario (*.dd)");
    dialogo.setDirectory(QDir::currentPath());

    if (dialogo.exec() == QDialog::Accepted)
    {
        ruta = dialogo.selectedFiles()[0];
        qDebug() << ruta;
        if(!ruta.isEmpty())
        {
            this->ui->label->setText(ruta);
            bandera = this->diccionario.abrir_Diccionario(ruta.toStdString());
            if(bandera)
            {
                this->inicia_tablaAtributos();
                this->ui->listEntidades->addItems(this->diccionario.Dame_Entidades());
            }
        }
    }
    return;
}

void Dialog::inicia_tablaAtributos()
{
    QStringList lista_label;

    lista_label.append("Nombre");
    lista_label.append("Tipo");
    lista_label.append("Tamaño");
    lista_label.append("Indice");
    lista_label.append("PK");
    this->ui->tableAtributos->setColumnCount(5);
    this->ui->tableAtributos->setHorizontalHeaderLabels(lista_label);

    this->ui->tableAtributos->setColumnWidth(0, 245);
    this->ui->tableAtributos->setColumnWidth(1, 50);
    this->ui->tableAtributos->setColumnWidth(2, 75);
    this->ui->tableAtributos->setColumnWidth(3, 50);
    this->ui->tableAtributos->setColumnWidth(4, 50);
}

void Dialog::on_listEntidades_clicked(const QModelIndex &index)
{
    qDebug() <<"[DEBUG] Entidad seleccionada" << index.row();

    /*this->ui->tableAtributos->insertRow(0);
    this->ui->tableAtributos->setItem(0, 0, new QTableWidgetItem("Hello"));
    this->ui->tableAtributos->setItem(0, 1, new QTableWidgetItem("Hello"));
    this->ui->tableAtributos->setItem(0, 2, new QTableWidgetItem("Hello"));
    this->ui->tableAtributos->setItem(0, 3, new QTableWidgetItem("Hello"));
    this->ui->tableAtributos->setItem(0, 4, new QTableWidgetItem("Hello"));*/

    this->ui->tableAtributos->clearContents();
    this->diccionario.ponTabla_Atributos(this->ui->tableAtributos, index.row());
}
