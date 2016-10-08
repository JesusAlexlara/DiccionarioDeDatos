#ifndef CDICCIONARIO_H
#define CDICCIONARIO_H

#include<string>
#include<sstream>
#include<cstdio>
#include<iostream>
#include<list>
#include<QDebug>
#include<QTableWidget>

#include"centidad.h"

using namespace std;

class CDiccionario
{
private:
    long cabecera;
    FILE *ptr_Archivo;
    list<CEntidad> lista_entidades;

public:
    CDiccionario();
    ~CDiccionario();

    bool abrir_Diccionario(string nombre);
    bool nuevo_Diccionario(string nombre);

    void ponTabla_Atributos(QTableWidget *tabla, int indexEntidad);
    QStringList Dame_Entidades();

    //string agrega_Entidad(char n[20]);
    //string edita_Entidad(int index, char n[20]);
    //string elimina_Entidad(int n);

    //int lista_EntidadesMenu();
    //string selecciona_Entidad(int index);
    //int menu_Entidad(char n[20], string buf);
    //bool existe_Diccionario();
};

#endif // CDICCIONARIO_H
