#ifndef CDICCIONARIO_H
#define CDICCIONARIO_H

#include<string>
#include<sstream>
#include<cstdio>
#include<iostream>
#include<list>

#include"centidad.h"
#include"limpiapantalla.h"

class CDiccionario
{
private:
    long cabecera;
    FILE *ptr_Archivo;
    std::list<CEntidad> lista_entidades;

public:
    CDiccionario();
    ~CDiccionario();

    std::string abrir_Diccionario(char n[20]);
    std::string agrega_Entidad(char n[20]);
    std::string edita_Entidad(int index, char n[20]);
    std::string elimina_Entidad(int n);
    std::string imprime_Entidades();

    int lista_EntidadesMenu();
    std::string selecciona_Entidad(int index);
    int menu_Entidad(char n[20], std::string buf);
    bool existe_Diccionario();
};

#endif // CDICCIONARIO_H
