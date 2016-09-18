#ifndef CENTIDAD_H
#define CENTIDAD_H

#include<cstring>
#include<list>
#include<string>
#include<sstream>
#include"catributo.h"

class CEntidad
{
private:
    char nombre[20];
    long dir_entidad;
    long dir_atributo;
    long dir_dato;
    long dir_siguiente;
    std::list<CAtributo> *lista_atributos;

public:
    CEntidad();
    CEntidad(char n[20], long dir_ant);

    void pon_Nombre(char n[20]);
    void ponDir_Entidad(long n);
    void ponDir_Atributo(long n);
    void ponDir_dato(long n);
    void ponDir_Siguiente(long n);
    char *dame_Nombre();
    long dameDir_Entidad();
    long dameDir_Atributo();
    long dameDir_Dato();
    long dameDir_Siguiente();

    bool operator <(const CEntidad &n) const;
    bool operator ==(const CEntidad &n) const;
    void inicia_ListaAtributos();

    void carga_Atributos(FILE *ptr_arch);
    int lista_Atributos();
    int lista_datos(FILE *ptr_arch);
    std::string agregar_Atributo(char nombre[20], int tipo, int tam, FILE *ptr_arch);
    std::string imprime_Atributos();
    std::string mofica_Atributos(char nombre[20], int tipo, int tam, FILE *ptr_arch, int index);
    std::string elimina_Atributo(int index, FILE *ptr_arch);

    std::string agrega_dato(FILE *ptr_arch);
    std::string elimina_dato(int index, FILE *ptr_arch);
    void imprime_dato(void *bloque);

};

#endif // CENTIDAD_H
