#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <cstdlib>
#include <cstdio>
#include "cdiccionario.h"

using namespace std;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushNuevo_clicked();
    void on_pushAbrir_clicked();

    void on_listEntidades_clicked(const QModelIndex &index);

private:
    void inicia_tablaAtributos();

private:
    Ui::Dialog *ui;
    CDiccionario diccionario;
};

#endif // DIALOG_H
