#ifndef CDICCIONARIO_H
#define CDICCIONARIO_H

#include<string>
#include<sstream>
#include<cstdio>
#include<iostream>
#include<list>

#include"centidad.h"
#include"limpiapantalla.h"

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

    string abrir_Diccionario(char n[20]);
    string agrega_Entidad(char n[20]);
    string edita_Entidad(int index, char n[20]);
    string elimina_Entidad(int n);
    string imprime_Entidades();

    int lista_EntidadesMenu();
    string selecciona_Entidad(int index);
    int menu_Entidad(char n[20], string buf);
    bool existe_Diccionario();
};

#endif // CDICCIONARIO_H
