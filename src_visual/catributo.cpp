#include "catributo.h"

CAtributo::CAtributo()
{
    this->dir_atributo = -1;
    this->dir_siguiente = -1;
    this->tipo = 0;
    this->tamanio = 0;
    this->clave_primaria = false;
    this->indice = false;
}

CAtributo::CAtributo(char n[20], int tam, int tipo, long dir)
{
    this->pon_Nombre(n);
    this->dir_atributo = dir;
    this->dir_siguiente = -1;
    this->tipo = tipo;
    this->tamanio = tam;
    this->clave_primaria = false;
    this->indice = false;
}

void CAtributo::pon_Nombre(char n[20])
{
    strcpy(this->nombre, n);
    return;
}

void CAtributo::ponDir_Atributo(long n)
{
    this->dir_atributo = n;
    return;
}

void CAtributo::ponDir_Siguiente(long n)
{
    this->dir_siguiente = n;
    return;
}

void CAtributo::pon_Tipo(int tipo)
{
    this->tipo = tipo;
    return;
}

void CAtributo::pon_Tamanio(int tam)
{
    this->tamanio = tam;
    return;
}

void CAtributo::pon_Indice(bool i)
{
    this->indice = i;
    return;
}

void CAtributo::pon_ClavePrimaria(bool i)
{
    this->clave_primaria = i;
    return;
}

char *CAtributo::dame_Nombre()
{
    return this->nombre;
}

long CAtributo::dameDir_Atributo()
{
    return this->dir_atributo;
}

long CAtributo::dameDir_Siguiente()
{
    return this->dir_siguiente;
}

int CAtributo::dame_Tipo()
{
    return this->tipo;
}

int CAtributo::dame_Tamanio()
{
    return this->tamanio;
}

bool CAtributo::dame_Indice()
{
    return this->indice;
}

bool CAtributo::dame_ClavePrimaria()
{
    return this->clave_primaria;
}

bool CAtributo::operator==(const CAtributo &a) const
{
    CAtributo p = a;
    return (strcmp(p.dame_Nombre(), this->nombre) == 0)
            && (p.dame_Tamanio() == this->tamanio)
            && (p.dame_Tipo() == this->tipo)
            && (p.dameDir_Atributo() == this->dir_atributo)
            && (p.dameDir_Siguiente() == this->dir_siguiente)
            && (p.dame_Indice() == this->indice)
            && (p.dame_ClavePrimaria() == this->clave_primaria);
}
