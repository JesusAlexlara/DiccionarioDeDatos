#include "centidad.h"

CEntidad::CEntidad()
{
    this->dir_entidad = -1;
    this->dir_atributo = -1;
    this->dir_dato = -1;
    this->dir_siguiente = -1;
    this->inicia_ListaAtributos();
}

CEntidad::CEntidad(char n[20], long dir_ant)
{
    this->pon_Nombre(n);
    this->dir_entidad = dir_ant;
    this->dir_atributo = -1;
    this->dir_dato = -1;
    this->dir_siguiente = -1;
    this->inicia_ListaAtributos();
}

void CEntidad::pon_Nombre(char n[20])
{
    std::strcpy(this->nombre, n);
    return;
}

void CEntidad::ponDir_Entidad(long n)
{
    this->dir_entidad = n;
    return;
}

void CEntidad::ponDir_Atributo(long n)
{
    this->dir_atributo = n;
    return;
}

void CEntidad::ponDir_dato(long n)
{
    this->dir_dato = n;
    return;
}

void CEntidad::ponDir_Siguiente(long n)
{
    this->dir_siguiente = n;
    return;
}

char *CEntidad::dame_Nombre()
{
    return this->nombre;
}

long CEntidad::dameDir_Entidad()
{
    return this->dir_entidad;
}

long CEntidad::dameDir_Atributo()
{
    return this->dir_atributo;
}

long CEntidad::dameDir_Dato()
{
    return this->dir_dato;
}

long CEntidad::dameDir_Siguiente()
{
    return this->dir_siguiente;
}

bool CEntidad::operator <(const CEntidad &n) const
{
    CEntidad aux = n;
    return std::strcmp(this->nombre, aux.dame_Nombre()) < 0;
}

bool CEntidad::operator ==(const CEntidad &n) const
{
    CEntidad p = n;
    return (std::strcmp(p.dame_Nombre(), this->nombre) == 0)
            && (p.dameDir_Atributo() == this->dir_atributo)
            && (p.dameDir_Dato() == this->dir_dato)
            && (p.dameDir_Entidad() == this->dir_entidad)
            && (p.dameDir_Siguiente() == this->dir_siguiente);
}

void CEntidad::inicia_ListaAtributos()
{
    this->lista_atributos = new std::list<CAtributo>();
    return;
}

void CEntidad::carga_Atributos(FILE *ptr_arch)
{
    CAtributo aux_entidad;

    if(ptr_arch)
    {
        if(this->dir_atributo != -1)
        {
            std::fseek(ptr_arch, this->dir_atributo, SEEK_SET);
            std::fread(&aux_entidad, sizeof(CAtributo), 1, ptr_arch);
            this->lista_atributos->push_back(aux_entidad);

            while(aux_entidad.dameDir_Siguiente() != -1)
            {
                std::fseek(ptr_arch, aux_entidad.dameDir_Siguiente(), SEEK_SET);
                std::fread(&aux_entidad, sizeof(CAtributo), 1, ptr_arch);
                this->lista_atributos->push_back(aux_entidad);
            }
        }
    }
}

int CEntidad::lista_Atributos()
{
    int opcion = -1;
    int contador;
    std::list<CAtributo>::iterator i;

    if(!this->lista_atributos->empty())
    {
        std::cout << "Selecciona Atributo" << std::endl;
        i = this->lista_atributos->begin();
        contador = 1;

        do
        {
            while(i != this->lista_atributos->end())
            {
                std::cout << contador
                          << " - "
                          << i->dame_Nombre() << " ";
                switch (i->dame_Tipo()) {
                case 1:
                    std::cout << "CHAR" << std::endl;
                    break;
                case 2:
                    std::cout << "INT" << std::endl;
                    break;
                case 3:
                    std::cout << "FLOAT" << std::endl;
                    break;
                case 4:
                    std::cout << "CHAR(" << i->dame_Tamanio() << ")" << std::endl;
                default:
                    break;
                }
                i++;
                contador++;
            }
            std::cout << "> ";
            try
            {
                std::cin >> opcion;
            }catch(int op)
            {
                std::cout << "Valor incorrecto" << std::endl;
            }
        }while(opcion >= 4 && opcion <=1);
    }
    return opcion;
}

std::__cxx11::string CEntidad::agregar_Atributo(char nombre[20], int tipo, int tam, FILE *ptr_arch)
{
    std::list<CAtributo>::iterator atras_iterador;
    CAtributo *nuevo_atributo, aux_atributo;
    std::stringstream b;
    long dir_nueva;
    int tam_aux;

    switch (tipo)
    {
    case 1:
        tam_aux = sizeof(char);
        break;
    case 2:
        tam_aux = sizeof(int);
        break;
    case 3:
        tam_aux = sizeof(float);
        break;
    case 4:
        tam_aux = sizeof(char) * tam;
        break;
    }
    if(ptr_arch != NULL)
    {
        std::fseek(ptr_arch, 0, SEEK_END);
        dir_nueva = std::ftell(ptr_arch);
        nuevo_atributo =
                new CAtributo(nombre, tam_aux, tipo, dir_nueva);
        std::fwrite(nuevo_atributo, sizeof(CAtributo), 1, ptr_arch);

        if(this->lista_atributos->empty())
        {
            this->dir_atributo = dir_nueva;
            this->lista_atributos->push_back(*nuevo_atributo);
            b << "Se agrego el atributo " << nuevo_atributo->dame_Nombre()
                   << " a la entidad" << std::endl;
        }
        else
        {
            atras_iterador = this->lista_atributos->begin();
            while(atras_iterador != this->lista_atributos->end()
                  && atras_iterador->dameDir_Siguiente() != -1)
            {
                atras_iterador++;
            }
            if(atras_iterador->dameDir_Siguiente() == -1)
            {
                std::fseek(ptr_arch, atras_iterador->dameDir_Atributo(), SEEK_SET);
                atras_iterador->ponDir_Siguiente(dir_nueva);
                aux_atributo = *atras_iterador;
                std::fwrite(&aux_atributo, sizeof(CAtributo), 1, ptr_arch);
                this->lista_atributos->push_back(*nuevo_atributo);
            }
        }
    }
    return b.str();
}

std::__cxx11::string CEntidad::imprime_Atributos()
{
    std::stringstream buffer;
    std::list<CAtributo>::iterator iterador;

    iterador = this->lista_atributos->begin();

    while(iterador != this->lista_atributos->end())
    {
        buffer << "  Nombre  "
                  << iterador->dame_Nombre() << std::endl
                  << "\tTipo   ";
        switch (iterador->dame_Tipo())
        {
        case 1:
            buffer << "CHAR" << std::endl;
            break;
        case 2:
            buffer << "INT" << std::endl;
            break;
        case 3:
            buffer << "FLOAT" << std::endl;
            break;
        case 4:
            buffer << "CHAR(" << iterador->dame_Tamanio() << ")\n";
            break;
        default:
            break;
        }
        buffer << "\tTamaño        "
               << iterador->dame_Tamanio() << std::endl
               << "\tdir_Atributo  "
               << iterador->dameDir_Atributo() << std::endl
               << "\tdir_Siguiente "
               << iterador->dameDir_Siguiente() << std::endl
               << std::endl;
        iterador++;
    }
    if(this->dir_atributo == -1)
    {
        buffer << "No hay atributos" << std::endl;
    }
    return buffer.str();
}

std::__cxx11::string CEntidad::mofica_Atributos(char nombre[20], int tipo, int tam, FILE *ptr_arch, int index)
{
    std::stringstream buffer;
    std::list<CAtributo>::iterator iterador;
    CAtributo aux_atributo;
    char aux_nombre[20];
    int tam_aux;

    if(ptr_arch != NULL)
    {
        iterador = this->lista_atributos->begin();

        if(iterador != this->lista_atributos->end())
        {
            switch (tipo)
            {
            case 1:
                tam_aux = sizeof(char);
                break;
            case 2:
                tam_aux = sizeof(int);
                break;
            case 3:
                tam_aux = sizeof(float);
                break;
            case 4:
                tam_aux = sizeof(char) * tam;
                break;
            }
            std::advance(iterador, index-1);
            std::strcpy(aux_nombre,iterador->dame_Nombre());
            iterador->pon_Nombre(nombre);
            iterador->pon_Tamanio(tam_aux);
            iterador->pon_Tipo(tipo);
            aux_atributo = *iterador;
            std::fseek(ptr_arch, aux_atributo.dameDir_Atributo(), SEEK_SET);
            std::fwrite(&aux_atributo, sizeof(CAtributo), 1, ptr_arch);

            buffer << "Se modifico el Atributo"
                              << aux_nombre << " > " << aux_atributo.dame_Nombre()
                              << std::endl;
        }
        else
        {
            buffer << "La lista esta vacia" << std::endl;
        }
    }
    return buffer.str();
}

std::__cxx11::string CEntidad::elimina_Atributo(int index, FILE *ptr_arch)
{
    std::stringstream buffer;
    std::list<CAtributo>::iterator iterador;
    CAtributo aux_atributo, aux_anterior;

    if(ptr_arch != NULL)
    {
        if(index-1 <= (int)this->lista_atributos->size())
        {
            iterador = this->lista_atributos->begin();
            if(iterador != this->lista_atributos->end())
            {
                std::advance(iterador, index-1);
                aux_atributo = *iterador;
                aux_anterior = *iterador;
                iterador = this->lista_atributos->begin();
                while(iterador->dameDir_Siguiente() != aux_atributo.dameDir_Atributo()
                      && iterador != this->lista_atributos->end())
                {
                    iterador++;
                }
                if(iterador->dameDir_Siguiente() == aux_atributo.dameDir_Atributo())
                {
                    iterador->ponDir_Siguiente(aux_atributo.dameDir_Siguiente());
                    std::fseek(ptr_arch, iterador->dameDir_Atributo(), SEEK_SET);
                    aux_atributo = *iterador;
                    std::fwrite(&aux_atributo, sizeof(CAtributo), 1, ptr_arch);
                    buffer << "Se elimino el Atributo " << aux_anterior.dame_Nombre();
                    this->lista_atributos->remove(aux_anterior);

                }
                else if(aux_atributo.dameDir_Atributo() == this->dir_atributo)
                {
                    this->dir_atributo = aux_atributo.dameDir_Siguiente();
                    buffer << "Se elimino el Atributo " << aux_anterior.dame_Nombre();
                    this->lista_atributos->remove(aux_anterior);
                }
                else
                {
                    buffer << "Error!! ";
                }
            }
        }
    }
    return buffer.str();
}

std::__cxx11::string CEntidad::agrega_dato(FILE *ptr_arch)
{
    std::stringstream buffer;
    std::list<CAtributo>::iterator iterador;
    void *bloque;
    //void *aux_bloque;
    char *aux_cadena;
    int aux_entero;
    float aux_float;
    char aux_char;
    int tam_atributos = 0;
    //int aux_direccion;

    if(ptr_arch != NULL)
    {
        iterador = this->lista_atributos->begin();

        if(iterador != this->lista_atributos->end())
        {
            //Saca el tamaño.
            while(iterador != this->lista_atributos->end())
            {
                tam_atributos += iterador->dame_Tamanio();
                iterador++;
            }
            tam_atributos += sizeof(long);
            //reserva memoria
            bloque = std::malloc(tam_atributos);

            //Captura Datos
            tam_atributos = 0;
            iterador = this->lista_atributos->begin();
            while(iterador != this->lista_atributos->end())
            {
                switch (iterador->dame_Tipo()) {
                case 1:
                    std::cout << "CHAR " <<"Dame " << iterador->dame_Nombre() << ": ";
                    std::cin >> aux_char;
                    //actualiza bloque
                    *((char*)(bloque)+(tam_atributos)) = aux_char;
                    tam_atributos += sizeof(char);
                    break;
                case 2:
                    std::cout << "INT " << "Dame " << iterador->dame_Nombre() << ": ";
                    std::cin >> aux_entero;
                    //actuza bloque
                    *((int*)(bloque)+(tam_atributos)) = aux_entero;
                    tam_atributos += sizeof(int);
                    break;
                case 3:
                    std::cout << "FLOAT " << "Dame " << iterador->dame_Nombre() << ": ";
                    std::cin >> aux_float;
                    //actualiza bloque
                    *((float*)(bloque)+(tam_atributos)) = aux_float;
                    tam_atributos += sizeof(float);
                    break;
                case 4:
                    aux_cadena = (char*)std::malloc(iterador->dame_Tamanio());
                    std::cout << "CHAR (" << iterador->dame_Tamanio() << ") "
                              << "Dame " << iterador->dame_Nombre() << ": ";
                    std::cin >> aux_cadena;
                    std::strcpy((char*)(bloque)+(tam_atributos), aux_cadena);
                    tam_atributos += iterador->dame_Tamanio();
                    std::free(aux_cadena);
                    break;
                default:
                    break;
                }
                std::cin.ignore(256, '\n');
                iterador++;
            }
            if(this->dir_dato != -1)
            {
                this->dir_dato = std::ftell(ptr_arch);
            }
            else
            {
                std::fseek(ptr_arch, this->dir_dato, SEEK_SET);
                //std::fread()
            }
            *((long*)(bloque)+(tam_atributos)) = -1;
            tam_atributos += sizeof(long);
            //escribe en el archivo
            std::fwrite(bloque, tam_atributos, 1, ptr_arch);
            this->imprime_dato(bloque);
            std::free(bloque);
        }
        else
        {
            buffer << "No hay atributos!" << std::endl;
        }
    }
    return buffer.str();
}

void CEntidad::imprime_dato(void *bloque)
{
    int tam_dato = 0;
    std::list<CAtributo>::iterator it;
    char aux_char;
    int aux_int;
    float aux_float;
    char *aux_cad;

    it = this->lista_atributos->begin();
    while(it != this->lista_atributos->end())
    {
        switch (it->dame_Tipo()) {
        case 1:
            aux_char = *((char*)(bloque)+(tam_dato));
            tam_dato += sizeof(char);
            std::cout << it->dame_Nombre()<<": "<< aux_char << std::endl;
            break;
        case 2:
            aux_int = *((int*)(bloque)+(tam_dato));
            tam_dato += sizeof(int);
            std::cout << it->dame_Nombre() << ": " << aux_int << std::endl;
            break;
        case 3:
            aux_float = *((float*)(bloque)+(tam_dato));
            tam_dato+= sizeof(float);
            std::cout << it->dame_Nombre() << ": " << aux_float << std::endl;
            break;
        case 4:
            aux_cad = (char*)std::malloc(it->dame_Tamanio());
            std::strcpy(aux_cad, ((char*)(bloque)+(tam_dato)));
            std::cout << it->dame_Nombre() << ": " << aux_cad << std::endl;
            std::free(aux_cad);
            tam_dato += it->dame_Tamanio();
            break;
        default:
            break;
        }
        it++;
    }
}
