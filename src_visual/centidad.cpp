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

list<CAtributo> *CEntidad::Dame_listaAtr()
{
    return this->lista_atributos;
}

void CEntidad::pon_Nombre(char n[20])
{
    strcpy(this->nombre, n);
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
    return strcmp(this->nombre, aux.dame_Nombre()) < 0;
}

bool CEntidad::operator ==(const CEntidad &n) const
{
    CEntidad p = n;
    return (strcmp(p.dame_Nombre(), this->nombre) == 0)
            && (p.dameDir_Atributo() == this->dir_atributo)
            && (p.dameDir_Dato() == this->dir_dato)
            && (p.dameDir_Entidad() == this->dir_entidad)
            && (p.dameDir_Siguiente() == this->dir_siguiente);
}

void CEntidad::inicia_ListaAtributos()
{
    this->lista_atributos = new list<CAtributo>();
    return;
}

void CEntidad::carga_Atributos(FILE *ptr_arch)
{
    CAtributo aux_entidad;

    if(ptr_arch)
    {
        if(this->dir_atributo != -1)
        {
            fseek(ptr_arch, this->dir_atributo, SEEK_SET);
            fread(&aux_entidad, sizeof(CAtributo), 1, ptr_arch);
            this->lista_atributos->push_back(aux_entidad);

            while(aux_entidad.dameDir_Siguiente() != -1)
            {
                fseek(ptr_arch, aux_entidad.dameDir_Siguiente(), SEEK_SET);
                fread(&aux_entidad, sizeof(CAtributo), 1, ptr_arch);
                this->lista_atributos->push_back(aux_entidad);
            }
        }
    }
}

int CEntidad::lista_Atributos()
{
    int opcion = -1;
    int contador;
    list<CAtributo>::iterator i;

    if(!this->lista_atributos->empty())
    {
        cout << "Selecciona Atributo" << endl;
        i = this->lista_atributos->begin();
        contador = 1;

        do
        {
            while(i != this->lista_atributos->end())
            {
                cout << contador
                          << " - "
                          << i->dame_Nombre() << " ";
                switch (i->dame_Tipo()) {
                case 1:
                    cout << "CHAR" << endl;
                    break;
                case 2:
                    cout << "INT" << endl;
                    break;
                case 3:
                    cout << "FLOAT" << endl;
                    break;
                case 4:
                    cout << "CHAR(" << i->dame_Tamanio() << ")" << endl;
                default:
                    break;
                }
                i++;
                contador++;
            }
            cout << "> ";
            try
            {
                cin >> opcion;
            }catch(int op)
            {
                cout << "Valor incorrecto" << endl;
            }
        }while(opcion >= 4 && opcion <=1);
    }
    return opcion;
}

int CEntidad::lista_datos(FILE *ptr_arch)
{
    list<CAtributo>::iterator iterador;
    int opcion = -1;
    int contador = 1;
    int aux_direccion;
    int tam_atributos = 0;
    void *bloque;

    if(this->dir_dato != -1)
    {
        /*Ciclo para caluar el tamaño que ocupan los atributos*/
        iterador = this->lista_atributos->begin();
        while(iterador != this->lista_atributos->end())
        {
            tam_atributos += iterador->dame_Tamanio();
            iterador++;
        }
        /*Reserva memoria del tamaño de los atributos*/
        tam_atributos+=sizeof(long);
        aux_direccion = this->dir_dato;
        do
        {
            bloque = malloc(tam_atributos);
            /*Ubica el apuntador en la direccion del primer dato*/
            fseek(ptr_arch, aux_direccion, SEEK_SET);
            /*Lee el primer dato*/
            fread(bloque, tam_atributos, 1, ptr_arch);
            /*Lee la direccion del siguiente dato*/
            aux_direccion = *((long*)((uint8_t*)bloque+tam_atributos-sizeof(long)));
            cout << contador << ". --------" << endl;
            this->imprime_dato(bloque);
            free(bloque);
            bloque = nullptr;
            contador++;
        }while(aux_direccion != -1);

        cout << "0. Salir" << endl;
        cout << "> ";
        try
        {
            cin >> opcion;
        }catch(int op)
        {
            cout << "Valor incorrecto" << endl;
        }
    }
    return opcion;
}

string CEntidad::agregar_Atributo(char nombre[20], int tipo, int tam, FILE *ptr_arch)
{
    list<CAtributo>::iterator atras_iterador;
    CAtributo *nuevo_atributo, aux_atributo;
    stringstream b;
    long dir_nueva;
    int tam_aux;

    if(this->dir_dato == -1)
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
        if(ptr_arch != NULL)
        {
            fseek(ptr_arch, 0, SEEK_END);
            dir_nueva = ftell(ptr_arch);
            nuevo_atributo =
                    new CAtributo(nombre, tam_aux, tipo, dir_nueva);
            fwrite(nuevo_atributo, sizeof(CAtributo), 1, ptr_arch);

            if(this->lista_atributos->empty())
            {
                this->dir_atributo = dir_nueva;
                this->lista_atributos->push_back(*nuevo_atributo);
                b << "Se agrego el atributo " << nuevo_atributo->dame_Nombre()
                       << " a la entidad" << endl;
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
                    fseek(ptr_arch, atras_iterador->dameDir_Atributo(), SEEK_SET);
                    atras_iterador->ponDir_Siguiente(dir_nueva);
                    aux_atributo = *atras_iterador;
                    fwrite(&aux_atributo, sizeof(CAtributo), 1, ptr_arch);
                    this->lista_atributos->push_back(*nuevo_atributo);
                }
            }
        }
    }
    else
    {
        b << "No se pueden agregar mas atributos" << endl;
    }
    return b.str();
}

string CEntidad::imprime_Atributos()
{
    stringstream buffer;
    list<CAtributo>::iterator iterador;

    iterador = this->lista_atributos->begin();

    while(iterador != this->lista_atributos->end())
    {
        buffer << "  Nombre  "
                  << iterador->dame_Nombre() << endl
                  << "\tTipo   ";
        switch (iterador->dame_Tipo())
        {
        case 1:
            buffer << "CHAR" << endl;
            break;
        case 2:
            buffer << "INT" << endl;
            break;
        case 3:
            buffer << "FLOAT" << endl;
            break;
        case 4:
            buffer << "CHAR(" << iterador->dame_Tamanio() << ")\n";
            break;
        default:
            break;
        }
        buffer << "\tTamaño        "
               << iterador->dame_Tamanio() << endl
               << "\tdir_Atributo  "
               << iterador->dameDir_Atributo() << endl
               << "\tdir_Siguiente "
               << iterador->dameDir_Siguiente() << endl
               << endl;
        iterador++;
    }
    if(this->dir_atributo == -1)
    {
        buffer << "No hay atributos" << endl;
    }
    return buffer.str();
}

string CEntidad::mofica_Atributos(char nombre[20], int tipo, int tam, FILE *ptr_arch, int index)
{
    stringstream buffer;
    list<CAtributo>::iterator iterador;
    CAtributo aux_atributo;
    char aux_nombre[20];
    int tam_aux;

    if(ptr_arch != NULL)
    {
        if(this->dir_dato == -1)
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
                advance(iterador, index-1);
                strcpy(aux_nombre,iterador->dame_Nombre());
                iterador->pon_Nombre(nombre);
                iterador->pon_Tamanio(tam_aux);
                iterador->pon_Tipo(tipo);
                aux_atributo = *iterador;
                fseek(ptr_arch, aux_atributo.dameDir_Atributo(), SEEK_SET);
                fwrite(&aux_atributo, sizeof(CAtributo), 1, ptr_arch);

                buffer << "Se modifico el Atributo"
                                  << aux_nombre << " > " << aux_atributo.dame_Nombre()
                                  << endl;
            }
            else
            {
                buffer << "La lista esta vacia" << endl;
            }
        }
        else
        {
            buffer << "No se puede eliminar la entidad tiene datos" << endl;
        }
    }
    return buffer.str();
}

string CEntidad::elimina_Atributo(int index, FILE *ptr_arch)
{
    stringstream buffer;
    list<CAtributo>::iterator iterador;
    CAtributo aux_atributo, aux_anterior;

    if(this->dir_dato == -1)
    {
        if(ptr_arch != NULL)
        {
            if(index-1 <= (int)this->lista_atributos->size())
            {
                iterador = this->lista_atributos->begin();
                if(iterador != this->lista_atributos->end())
                {
                    advance(iterador, index-1);
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
                        fseek(ptr_arch, iterador->dameDir_Atributo(), SEEK_SET);
                        aux_atributo = *iterador;
                        fwrite(&aux_atributo, sizeof(CAtributo), 1, ptr_arch);
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
    }
    else
    {
        buffer << "No se puede eliminar el atributo porque la entidad contiene datos"
               << endl;
    }
    return buffer.str();
}

string CEntidad::agrega_dato(FILE *ptr_arch)
{
    /*Funcion donde se agregan los datos*/
    stringstream buffer;
    list<CAtributo>::iterator iterador;
    void *bloque;
    //void *aux_bloque;
    char *aux_cadena;
    int aux_entero;
    float aux_float;
    char aux_char;
    int tam_atributos = 0;
    //auxiliares
    long aux_direccion_ant;
    long aux_direccion = -1;
    long dato_direccion;

    if(ptr_arch != NULL)
    {
        iterador = this->lista_atributos->begin();

        if(iterador != this->lista_atributos->end())
        {
            //Saca el tamaño que ocupara el bloque.
            while(iterador != this->lista_atributos->end())
            {
                tam_atributos += iterador->dame_Tamanio();
                iterador++;
            }
            tam_atributos += sizeof(long);
            //reserva memoria
            bloque = malloc(tam_atributos);

            //Captura Datos
            tam_atributos = 0;
            iterador = this->lista_atributos->begin();
            while(iterador != this->lista_atributos->end())
            {
                switch (iterador->dame_Tipo()) {
                case 1:
                    cout << "CHAR " <<"Dame " << iterador->dame_Nombre() << ": ";
                    cin >> aux_char;
                    //actualiza bloque
                    *((char*)((uint8_t*)bloque+tam_atributos)) = aux_char;
                    tam_atributos += sizeof(char);
                    break;
                case 2:
                    cout << "INT " << "Dame " << iterador->dame_Nombre() << ": ";
                    cin >> aux_entero;
                    //actuza bloque
                    *((int*)((uint8_t*)bloque+tam_atributos)) = aux_entero;
                    tam_atributos += sizeof(int);
                    break;
                case 3:
                    cout << "FLOAT " << "Dame " << iterador->dame_Nombre() << ": ";
                    cin >> aux_float;
                    //actualiza bloque
                    *((float*)((uint8_t*)bloque+tam_atributos)) = aux_float;
                    tam_atributos += sizeof(float);
                    break;
                case 4:
                    aux_cadena = (char*)malloc(iterador->dame_Tamanio());
                    cout << "CHAR (" << iterador->dame_Tamanio() << ") "
                              << "Dame " << iterador->dame_Nombre() << ": ";
                    cin >> aux_cadena;
                    strcpy((char*)((uint8_t*)bloque+tam_atributos), aux_cadena);
                    tam_atributos += iterador->dame_Tamanio();
                    free(aux_cadena);
                    break;
                default:
                    break;
                }
                //Linea para limpiar el buffer.
                cin.ignore(256, '\n');
                iterador++;
            }

            fseek(ptr_arch, 0, SEEK_END);
            dato_direccion = ftell(ptr_arch);
            *((long*)((uint8_t*)bloque+tam_atributos)) = -1;
            tam_atributos += sizeof(long);
            //escribe en el archivo
            fwrite(bloque, tam_atributos, 1, ptr_arch);
            //fwrite(&aux_direccion, sizeof(long), 1, ptr_arch);

            if(this->dir_dato == -1)
            {
                /*Si es el primer dato actualiza la direccion del dato de la entidad*/
                this->dir_dato = dato_direccion;
            }
            else
            {
                tam_atributos -= sizeof(long);
                aux_direccion = this->dir_dato;
                do
                {
                    aux_direccion += tam_atributos;
                    //Se mueve a la ubicacion del bloque en la direccion del siguiente bloque.
                    fseek(ptr_arch, aux_direccion, SEEK_SET);
                    //se guarda la direccion donde esta la direccion del siguiente dato,
                    aux_direccion_ant = ftell(ptr_arch);
                    //Se lee el dato siguiente.
                    fread(&aux_direccion, sizeof(long), 1, ptr_arch);
                }while(aux_direccion != -1);
                if(aux_direccion == -1)
                {
                    //Se regresa el puntero a la direccion anterior
                    fseek(ptr_arch, aux_direccion_ant, SEEK_SET);
                    //Se actualiza la direccion anterior a la nueva direccion del nuevo dato.
                    fwrite(&dato_direccion, sizeof(long), 1, ptr_arch);
                }
            }

            //Solo para ver si escribio bien el bloque..
            this->imprime_dato(bloque);
            free(bloque);
        }
        else
        {
            buffer << "No hay atributos!" << endl;
        }
    }
    return buffer.str();
}

string CEntidad::elimina_dato(int index, FILE *ptr_arch)
{
    stringstream buffer;
    list<CAtributo>::iterator iterador;
    long dir_ant, aux_dir;
    int tam_atributos = 0;
    int contador = 1;

    if(this->dir_dato != -1)
    {
        iterador = this->lista_atributos->begin();
        if(iterador != this->lista_atributos->end())
        {
            /*Bloque que saca el tamaño de los atributos*/
            while(iterador != this->lista_atributos->end())
            {
                tam_atributos += iterador->dame_Tamanio();
                iterador++;
            }
            aux_dir = this->dir_dato;
            dir_ant = aux_dir;

            while(contador != index && aux_dir != -1)
            {
                /*se ubica en la direccion siguiente del dato*/
                fseek(ptr_arch, aux_dir+tam_atributos, SEEK_SET);
                /*se guarda la direccion anterior del dato*/
                dir_ant = aux_dir;
                /*se carga la nueva direccion*/
                fread(&aux_dir, sizeof(long), 1, ptr_arch);
                contador++;
            }
            /*Se verifica porque se salio del ciclo*/
            if(contador == index)
            {
                /*Si el dato anterior es la cabecera*/
                if(dir_ant == this->dir_dato)
                {
                    /*Se mueve el puntero a la direccion del dato actual*/
                    fseek(ptr_arch, dir_ant+tam_atributos, SEEK_SET);
                    /*se actualiza la cabecera al la direccion siguiente*/
                    fread(&this->dir_dato, sizeof(long), 1, ptr_arch);
                }
                else
                {
                    fseek(ptr_arch, aux_dir+tam_atributos, SEEK_SET);
                    fread(&aux_dir, sizeof(long), 1, ptr_arch);
                    /*se mueve el puntero a la direccion del dato anterior*/
                    fseek(ptr_arch, dir_ant+tam_atributos, SEEK_SET);
                    /*se actualiza la direccion del dato anterior*/
                    fwrite(&aux_dir, sizeof(long), 1, ptr_arch);
                }
                buffer << "Se elimino el dato." << endl;
            }
        }
    }            tam_atributos += sizeof(long);
    return buffer.str();
}

string CEntidad::edita_dato(int index, FILE *ptr_arch)
{
    stringstream buffer;
    list<CAtributo>::iterator iterador;
    void *bloque;
    char *aux_cadena;
    int aux_entero;
    float aux_float;
    char aux_char;
    int tam_atributos = 0;
    long aux_dir;
    int contador = 1;

    if(ptr_arch != NULL)
    {
        iterador = this->lista_atributos->begin();
        if(iterador != this->lista_atributos->end())
        {
            //Saca el tamaño que ocupara el bloque.
            while(iterador != this->lista_atributos->end())
            {
                tam_atributos += iterador->dame_Tamanio();
                iterador++;
            }
            //reserva memoria
            bloque = malloc(tam_atributos);

            tam_atributos = 0;
            iterador = this->lista_atributos->begin();
            while(iterador != this->lista_atributos->end())
            {
                switch (iterador->dame_Tipo()) {
                case 1:
                    cout << "CHAR " <<"Dame " << iterador->dame_Nombre() << " nuevo" << ": ";
                    cin >> aux_char;
                    //actualiza bloque
                    *((char*)((uint8_t*)bloque+tam_atributos)) = aux_char;
                    tam_atributos += sizeof(char);
                    break;
                case 2:
                    cout << "INT " << "Dame " << iterador->dame_Nombre() << " nuevo" << ": ";
                    cin >> aux_entero;
                    //actuza bloque
                    *((int*)((uint8_t*)bloque+tam_atributos)) = aux_entero;
                    tam_atributos += sizeof(int);
                    break;
                case 3:
                    cout << "FLOAT " << "Dame " << iterador->dame_Nombre() << " nuevo" << ": ";
                    cin >> aux_float;
                    //actualiza bloque
                    *((float*)((uint8_t*)bloque+tam_atributos)) = aux_float;
                    tam_atributos += sizeof(float);
                    break;
                case 4:
                    aux_cadena = (char*)malloc(iterador->dame_Tamanio());
                    cout << "CHAR (" << iterador->dame_Tamanio() << ") "
                              << "Dame " << iterador->dame_Nombre() << " nuevo" << ": ";
                    cin >> aux_cadena;
                    strcpy((char*)((uint8_t*)bloque+tam_atributos), aux_cadena);
                    tam_atributos += iterador->dame_Tamanio();
                    free(aux_cadena);
                    break;
                default:
                    break;
                }
                //Linea para limpiar el buffer.
                cin.ignore(256, '\n');
                iterador++;
            }
            aux_dir = this->dir_dato;

            while(contador != index && aux_dir != -1)
            {
                /*se ubica en la direccion siguiente del dato*/
                fseek(ptr_arch, aux_dir+tam_atributos, SEEK_SET);
                fread(&aux_dir, sizeof(long), 1, ptr_arch);
                contador++;
            }
            /*Se verifica porque se salio del ciclo*/
            if(contador == index)
            {
                fseek(ptr_arch, aux_dir, SEEK_SET);
                fwrite(bloque, tam_atributos, 1, ptr_arch);
                rewind(ptr_arch);
                buffer << "Se modifico el dato." << endl;
                free(bloque);
            }
        }
    }
    return buffer.str();
}

void CEntidad::imprime_dato(void *bloque)
{
    int tam_dato = 0;
    list<CAtributo>::iterator it;
    char aux_char;
    int aux_int;
    float aux_float;
    long aux_direccion = 0;
    char *aux_cad;

    it = this->lista_atributos->begin();
    while(it != this->lista_atributos->end())
    {
        switch (it->dame_Tipo()) {
        case 1:
            aux_char = *((char*)((uint8_t*)bloque + tam_dato));
            tam_dato += sizeof(char);
            cout << it->dame_Nombre()<<": "<< aux_char << endl;
            break;
        case 2:
            aux_int = *((int*)((uint8_t*)bloque + tam_dato));
            tam_dato += sizeof(int);
            cout << it->dame_Nombre() << ": " << aux_int << endl;
            break;
        case 3:
            aux_float = *((float*)((uint8_t*)bloque + tam_dato));
            tam_dato+= sizeof(float);
            cout << it->dame_Nombre() << ": " << aux_float << endl;
            break;
        case 4:
            aux_cad = (char*)malloc(it->dame_Tamanio());
            strcpy(aux_cad, ((char*)((uint8_t*)bloque + tam_dato)));
            cout << it->dame_Nombre() << ": " << aux_cad << endl;
            free(aux_cad);
            tam_dato += it->dame_Tamanio();
            break;
        default:
            break;
        }
        it++;
    }
    aux_direccion = *((long*)((uint8_t*)bloque+tam_dato));
    cout << "Direccion siguiente: " << aux_direccion << endl;
    return;
}
