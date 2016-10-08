#ifndef CATRIBUTO_H
#define CATRIBUTO_H

#include<iostream>
#include<cstdio>
#include<cstring>

class CAtributo
{
private:
    char nombre[20];
    int tamanio;
    int tipo;
    bool indice;
    bool clave_primaria;
    long dir_atributo;
    long dir_siguiente;

public:
    CAtributo();
    CAtributo(char n[20], int tam, int tipo, long dir);
    void pon_Nombre(char n[20]);
    void ponDir_Atributo(long n);
    void ponDir_Siguiente(long n);
    void pon_Tipo(int tipo);
    void pon_Tamanio(int tam);
    void pon_Indice(bool i);
    void pon_ClavePrimaria(bool i);

    char *dame_Nombre();
    long dameDir_Atributo();
    long dameDir_Siguiente();
    int dame_Tipo();
    int dame_Tamanio();
    bool dame_Indice();
    bool dame_ClavePrimaria();

    bool operator==(const CAtributo &a) const;
};

#endif // CATRIBUTO_H
