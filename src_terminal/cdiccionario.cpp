#include "cdiccionario.h"

CDiccionario::CDiccionario()
{
    this->ptr_Archivo = NULL;
    this->cabecera = -1;
    this->lista_entidades.clear();
}

CDiccionario::~CDiccionario()
{
    fclose(this->ptr_Archivo);
}

string CDiccionario::abrir_Diccionario(char n[20])
{
    stringstream buffer;
    CEntidad aux_entidad;
    long auxDir_siguiente;

    this->lista_entidades.clear();
    //Si el archivo esta abierto se cierra
    if(this->ptr_Archivo != NULL)
    {
        fclose(this->ptr_Archivo);
        this->cabecera = -1;
    }
    /*Se abre el archivo considerando que existe*/
    this->ptr_Archivo = fopen(n, "r+b");
    /*Si el archivo se pudo abrir se cargan sus datos*/
    if(this->ptr_Archivo != NULL)
    {
        buffer << "Diccionario " << n << " Abierto!!";
        /*Se lee su cabecera en el archivo y sea actualiza la cabecera de la clase*/
        fread(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
        buffer << " Cabecera en " << this->cabecera  << endl;

        /*Se cargas todos sus Entidades y atributos*/
        if(this->cabecera != -1)
        {
            fseek(this->ptr_Archivo, this->cabecera, SEEK_SET);
            fread(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
            aux_entidad.inicia_ListaAtributos();
            aux_entidad.carga_Atributos(this->ptr_Archivo);
            this->lista_entidades.push_back(aux_entidad);
            while(aux_entidad.dameDir_Siguiente() != -1)
            {
                auxDir_siguiente = aux_entidad.dameDir_Siguiente();
                fseek(this->ptr_Archivo, auxDir_siguiente, SEEK_SET);
                fread(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
                aux_entidad.inicia_ListaAtributos();
                aux_entidad.carga_Atributos(this->ptr_Archivo);
                this->lista_entidades.push_back(aux_entidad);
            }
            /*Cargadas todas las entidades se ordena la lista*/
            this->lista_entidades.sort();
        }
    }
    /*Si el archivo no se pudo abrir se crea automaticamente*/
    else
    {
        this->ptr_Archivo = fopen(n, "w+b");
        /*Si el archivo se pudo crear*/
        if(this->ptr_Archivo != NULL)
        {
            buffer << "Diccionario " << n << " Creado!!" << endl;
            /*Escribimos la cabecera vacia en el archivo nuevo*/
            fwrite(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
        }
        /*Si no se pudo crear el diccionario*/
        else
        {
            buffer << "No se pudo crear el diccionario " << n << endl;
        }
    }
    return buffer.str();
}

string CDiccionario::agrega_Entidad(char n[20])
{
    long dir_nueva;
    stringstream buffer;
    CEntidad *nueva_entidad, aux_entidad;
    list<CEntidad>::iterator atras_entidad;

    if(this->ptr_Archivo != NULL)
    {
        /*Se pone el apuntador al final de archivo*/
        fseek(this->ptr_Archivo, 0, SEEK_END);
        dir_nueva = ftell(this->ptr_Archivo);
        nueva_entidad = new CEntidad(n, dir_nueva);
        fwrite(nueva_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);

        //Si el diccionario esta vacio agrega y actualiza la cabecera
        if(this->lista_entidades.empty())
        {
            this->cabecera = dir_nueva;
            //Se actualiza la cabecera en el archivo
            fseek(this->ptr_Archivo, 0, SEEK_SET);
            fwrite(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
            //Se agrega la nueva entidad a la lista
            this->lista_entidades.push_back(*nueva_entidad);
            buffer << "Se agrego la entidad " << n << " al diccionario" << endl;

        }
        else
        {
            //recorre la lista y busca el ultimo elemento
            atras_entidad = this->lista_entidades.begin();
            while(atras_entidad != this->lista_entidades.end()
                  && atras_entidad->dameDir_Siguiente() != -1)
            {
                atras_entidad++;
            }
            //Verifica si encontro la ultima entidad al final
            if(atras_entidad->dameDir_Siguiente() == -1)
            {
                fseek(this->ptr_Archivo, atras_entidad->dameDir_Entidad(), SEEK_SET);
                atras_entidad->ponDir_Siguiente(dir_nueva);
                aux_entidad = *atras_entidad;
                fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
                this->lista_entidades.push_back(*nueva_entidad);
                buffer << "Se agrego la entidad " << n << " al diccionario" << endl;
            }
        }
        //Ordena a lista por nombres, esto es posible por la sobrecarga del operador ==
        this->lista_entidades.sort();
    }
    //Retorna un buffer que despues es mostrado al inicio del menu.
    return buffer.str();
}

string CDiccionario::edita_Entidad(int index, char n[20])
{
    stringstream buffer;
    CEntidad aux_entidad;
    char aux_nombre[20];
    list<CEntidad>::iterator iterador;

    if(this->ptr_Archivo != NULL)
    {
        iterador = this->lista_entidades.begin();
        /*Se verifica si la lista no esta vacia comparado el interador con el final de la
        lista*/
        if(iterador != this->lista_entidades.end())
        {
            /*El metodo advance el iterador n veces si seleccione el elemento 2
             * advance recorre el iterador 2 veces
             */
            advance(iterador, index-1);
            strcpy(aux_nombre, iterador->dame_Nombre());
            iterador->pon_Nombre(n);
            aux_entidad = *iterador;
            fseek(this->ptr_Archivo, iterador->dameDir_Entidad(), SEEK_SET);
            fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
            buffer << "Se modifico la Entidad "
                   << aux_nombre << " > " << aux_entidad.dame_Nombre()
                   << endl;
            this->lista_entidades.sort();
        }
        else
        {
            cout << "El diccionario esta vacio" << endl;
        }
    }
    return buffer.str();
}

string CDiccionario::elimina_Entidad(int n)
{
    stringstream buffer;
    CEntidad aux_entidad;
    list<CEntidad>::iterator iterador;
    if(this->ptr_Archivo != NULL)
    {
        iterador = this->lista_entidades.begin();
        if(iterador != this->lista_entidades.end())
        {
            advance(iterador, n);
            aux_entidad = *iterador;
            if(aux_entidad.dameDir_Dato() == -1)
            {
                iterador = this->lista_entidades.begin();
                while((iterador->dameDir_Siguiente() != aux_entidad.dameDir_Entidad())
                      && iterador != this->lista_entidades.end())
                {
                    iterador++;
                }
                if(iterador->dameDir_Siguiente() == aux_entidad.dameDir_Entidad())
                {
                    buffer << "Encontro el anterior " << iterador->dame_Nombre() << endl;
                    iterador->ponDir_Siguiente(aux_entidad.dameDir_Siguiente());
                    fseek(this->ptr_Archivo, iterador->dameDir_Entidad(), SEEK_SET);
                    aux_entidad = *iterador;
                    fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
                    /*Elimino la entidad de la lista*/
                    iterador = this->lista_entidades.begin();
                    advance(iterador, n);
                    this->lista_entidades.remove(*iterador);
                }
                else if(this->cabecera == aux_entidad.dameDir_Entidad())
                {
                    this->cabecera = aux_entidad.dameDir_Siguiente();
                    fseek(this->ptr_Archivo, 0, SEEK_SET);
                    fwrite(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
                    buffer << "Es es el primero" << iterador->dame_Nombre() << endl;
                    /*Elimino la entidad de la lista*/
                    iterador = this->lista_entidades.begin();
                    advance(iterador, n);
                    this->lista_entidades.remove(*iterador);
                }
            }
            else
            {
                buffer << "No se puede eliminar la entidad, tiene datos" << endl;
            }
        }
        else
        {
            cout << "El diccionario esta vacio" << endl;
        }
    }
    return buffer.str();
}

string CDiccionario::imprime_Entidades()
{
    stringstream buffer;
    list<CEntidad>::iterator iterador;

    iterador = this->lista_entidades.begin();

    while(iterador != this->lista_entidades.end())
    {
        buffer << "  Nombre  "
                  << iterador->dame_Nombre() << endl
                  << "\tdir_entidad   "
                  << iterador->dameDir_Entidad() << endl
                  << "\tdir_atributo  "
                  << iterador->dameDir_Atributo() << endl
                  << "\tdir_dato      "
                  << iterador->dameDir_Dato() << endl
                  << "\tdir_siguiente "
                  << iterador->dameDir_Siguiente() << endl
                  << endl;
        iterador++;
    }
    if(this->cabecera == -1)
    {
        buffer << "El diccionario esta vacio " << endl;
    }
    return buffer.str();
}

int CDiccionario::lista_EntidadesMenu()
{
    //Menu que se encarga de imprimir las entidades de la lista de forma numerica
    list<CEntidad>::iterator iterador;
    int contador;
    int res = -1;

    if(!this->lista_entidades.empty())
    {
        cout << "Selecciona Entidad" << endl << endl;
        iterador = this->lista_entidades.begin();
        contador = 1;
        while(iterador != this->lista_entidades.end())
        {
            cout << contador
                      << " - "
                      << iterador->dame_Nombre()
                      << endl;
            contador++;
            iterador++;
        }
        cout << "0 - Regresar" << endl;
        cout << "> ";
        cin >> res;
    }
    return res;
}

string CDiccionario::selecciona_Entidad(int index)
{
    /*Menu principal donde esta seleecionado una entidad
     * para agregarle sus atributos*/
    int opcion, seleccion, tipo, tam = 1;
    char na[20];
    string log;
    stringstream buffer;
    list<CEntidad>::iterator iterator;
    CEntidad aux_entidad;

    if(this->ptr_Archivo != NULL)
    {
        iterator = this->lista_entidades.begin();
        if(iterator != this->lista_entidades.end())
        {
            advance(iterator, index);
            do
            {
                CLEAR
                tam = 1;
                opcion = this->menu_Entidad(iterator->dame_Nombre(), log);

                switch (opcion)
                {
                    case 1:
                        cout << "Nombre Atributo:" << endl
                                  << "> ";
                        cin >> na;
                        cout << "Tipo del Atributo" << endl
                                  << "1 - CHAR\n" << "2 - INT\n" << "3 - FLOAT\n"
                                  << "4 - STRING";
                        do
                        {
                            cout << "\n> ";
                            cin >> tipo;
                        }while(tipo >= 4 && tipo <= 1);
                        if(tipo == 4)
                        {
                            cout << "Tamaño de la cadena:" << endl << "> ";
                            cin >> tam;
                        }
                        log = iterator->agregar_Atributo(na, tipo, tam, this->ptr_Archivo);
                        break;
                    case 2:
                        seleccion = iterator->lista_Atributos();
                        if(seleccion >= 1)
                        {
                            log = iterator->elimina_Atributo(seleccion, this->ptr_Archivo);
                        }
                        else
                        {
                            log = "No hay Atributos\n";
                        }
                        break;
                    case 3:
                        seleccion = iterator->lista_Atributos();
                        if(seleccion >= 1)
                        {
                            cout << "Nuevo nombre Atributo:" << endl
                                      << "> ";
                            cin >> na;
                            cout << "Nuevo tipo del Atributo" << endl
                                      << "1 - CHAR\n" << "2 - INT\n" << "3 - FLOAT\n"
                                      << "4 - STRING";
                            do
                            {
                                cout << "\n> ";
                                cin >> tipo;
                            }while(tipo >= 4 && tipo <= 1);
                            if(tipo == 4)
                            {
                                cout << "Tamaño de la cadena:" << endl << "> ";
                                cin >> tam;
                            }
                            log = iterator->mofica_Atributos(na, tipo, tam, this->ptr_Archivo, seleccion);
                        }
                        else
                        {
                            log = "No hay Atributos\n";
                        }
                        break;
                    case 4:
                        log = iterator->imprime_Atributos();
                        break;
                    case 5:
                        log = iterator->agrega_dato(this->ptr_Archivo);
                        break;
                    case 6:
                        iterator->lista_datos(this->ptr_Archivo);
                        break;
                    case 7:
                        seleccion = iterator->lista_datos(this->ptr_Archivo);
                        if(seleccion > 0)
                        {
                            log = iterator->elimina_dato(seleccion, this->ptr_Archivo);
                        }
                        break;
                    case 8:
                        seleccion = iterator->lista_datos(this->ptr_Archivo);
                        if(seleccion > 0)
                        {
                            log = iterator->edita_dato(seleccion, this->ptr_Archivo);
                        }
                    default:
                        break;
                }
            }while(opcion != 0);
            /*Cuando se terminan de hacer todos los cambios se actualiza la entidad del archivo*/
            fseek(this->ptr_Archivo, iterator->dameDir_Entidad(), SEEK_SET);
            aux_entidad = *iterator;
            fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
        }
    }
    return buffer.str();
}

int CDiccionario::menu_Entidad(char n[20], string buf)
{
    int opcion;
    cout << "Entidad: " << n << endl
              << buf << endl
              << "1 - Agregar Atributo" << endl
              << "2 - Elimina Atributo" << endl
              << "3 - Edita Atributo" << endl
              << "4 - Imprime Atributos" << endl
              << "-----------------------------" << endl
              << "5 - Insertar Dato" << endl
              << "6 - Imprime Datos" << endl
              << "7 - Elimina Dato" << endl
              << "8 - Modifica Dato" << endl
              << "0 - Regresar" << endl
              << "> ";
    cin >> opcion;
    return opcion;
}

bool CDiccionario::existe_Diccionario()
{
    if(this->ptr_Archivo != NULL)
    {
        return true;
    }
    return false;
}
