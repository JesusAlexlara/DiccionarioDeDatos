#include "cdiccionario.h"

CDiccionario::CDiccionario()
{
    this->ptr_Archivo = NULL;
    this->cabecera = -1;
    this->lista_entidades.clear();
}

CDiccionario::~CDiccionario()
{
    std::fclose(this->ptr_Archivo);
}

std::__cxx11::string CDiccionario::abrir_Diccionario(char n[20])
{
    std::stringstream buffer;
    CEntidad aux_entidad;
    long auxDir_siguiente;

    this->lista_entidades.clear();
    //Si el archivo esta abierto se cierra
    if(this->ptr_Archivo != NULL)
    {
        std::fclose(this->ptr_Archivo);
    }
    /*Se abre el archivo considerando que existe*/
    this->ptr_Archivo = std::fopen(n, "r+b");
    /*Si el archivo se pudo abrir se cargan sus datos*/
    if(this->ptr_Archivo != NULL)
    {
        buffer << "Diccionario " << n << " Abierto!!";
        /*Se lee su cabecera en el archivo y sea actualiza la cabecera de la clase*/
        std::fread(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
        buffer << " Cabecera en " << this->cabecera  << std::endl;

        /*Se cargas todos sus Entidades y atributos*/
        std::fseek(this->ptr_Archivo, this->cabecera, SEEK_SET);
        std::fread(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
        aux_entidad.inicia_ListaAtributos();
        aux_entidad.carga_Atributos(this->ptr_Archivo);
        this->lista_entidades.push_back(aux_entidad);
        while(aux_entidad.dameDir_Siguiente() != -1)
        {
            auxDir_siguiente = aux_entidad.dameDir_Siguiente();
            std::fseek(this->ptr_Archivo, auxDir_siguiente, SEEK_SET);
            std::fread(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
            aux_entidad.inicia_ListaAtributos();
            aux_entidad.carga_Atributos(this->ptr_Archivo);
            this->lista_entidades.push_back(aux_entidad);
        }
        /*Cargadas todas las entidades se ordena la lista*/
        this->lista_entidades.sort();
    }
    /*Si el archivo no se pudo abrir se crea automaticamente*/
    else
    {
        this->ptr_Archivo = std::fopen(n, "w+b");
        /*Si el archivo se pudo crear*/
        if(this->ptr_Archivo != NULL)
        {
            buffer << "Diccionario " << n << " Creado!!" << std::endl;
            /*Escribimos la cabecera vacia en el archivo nuevo*/
            std::fwrite(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
        }
        /*Si no se pudo crear el diccionario*/
        else
        {
            buffer << "No se pudo crear el diccionario " << n << std::endl;
        }
    }
    return buffer.str();
}

std::__cxx11::string CDiccionario::agrega_Entidad(char n[20])
{
    long dir_nueva;
    std::stringstream buffer;
    CEntidad *nueva_entidad, aux_entidad;
    std::list<CEntidad>::iterator atras_entidad;

    if(this->ptr_Archivo != NULL)
    {
        /*Se pone el apuntador al final de archivo*/
        std::fseek(this->ptr_Archivo, 0, SEEK_END);
        dir_nueva = std::ftell(this->ptr_Archivo);
        nueva_entidad = new CEntidad(n, dir_nueva);
        std::fwrite(nueva_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);

        //Si el diccionario esta vacio agrega y actualiza la cabecera
        if(this->lista_entidades.empty())
        {
            this->cabecera = dir_nueva;
            //Se actualiza la cabecera en el archivo
            std::fseek(this->ptr_Archivo, 0, SEEK_SET);
            std::fwrite(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
            //Se agrega la nueva entidad a la lista
            this->lista_entidades.push_back(*nueva_entidad);
            buffer << "Se agrego la entidad " << n << " al diccionario" << std::endl;

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
                std::fseek(this->ptr_Archivo, atras_entidad->dameDir_Entidad(), SEEK_SET);
                atras_entidad->ponDir_Siguiente(dir_nueva);
                aux_entidad = *atras_entidad;
                std::fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
                this->lista_entidades.push_back(*nueva_entidad);
                buffer << "Se agrego la entidad " << n << " al diccionario" << std::endl;
            }
        }
        //Ordena a lista por nombres, esto es posible por la sobrecarga del operador ==
        this->lista_entidades.sort();
    }
    //Retorna un buffer que despues es mostrado al inicio del menu.
    return buffer.str();
}

std::__cxx11::string CDiccionario::edita_Entidad(int index, char n[20])
{
    std::stringstream buffer;
    CEntidad aux_entidad;
    char aux_nombre[20];
    std::list<CEntidad>::iterator iterador;

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
            std::advance(iterador, index-1);
            std::strcpy(aux_nombre, iterador->dame_Nombre());
            iterador->pon_Nombre(n);
            aux_entidad = *iterador;
            std::fseek(this->ptr_Archivo, iterador->dameDir_Entidad(), SEEK_SET);
            std::fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
            buffer << "Se modifico la Entidad "
                   << aux_nombre << " > " << aux_entidad.dame_Nombre()
                   << std::endl;
            this->lista_entidades.sort();
        }
        else
        {
            std::cout << "El diccionario esta vacio" << std::endl;
        }
    }
    return buffer.str();
}

std::__cxx11::string CDiccionario::elimina_Entidad(int n)
{
    std::stringstream buffer;
    CEntidad aux_entidad;
    std::list<CEntidad>::iterator iterador;

    if(this->ptr_Archivo != NULL)
    {
        iterador = this->lista_entidades.begin();
        if(iterador != this->lista_entidades.end())
        {
            std::advance(iterador, n);
            aux_entidad = *iterador;

            iterador = this->lista_entidades.begin();
            while((iterador->dameDir_Siguiente() != aux_entidad.dameDir_Entidad())
                  && iterador != this->lista_entidades.end())
            {
                iterador++;
            }
            if(iterador->dameDir_Siguiente() == aux_entidad.dameDir_Entidad())
            {
                buffer << "Encontro el anterior " << iterador->dame_Nombre() << std::endl;
                iterador->ponDir_Siguiente(aux_entidad.dameDir_Siguiente());
                std::fseek(this->ptr_Archivo, iterador->dameDir_Entidad(), SEEK_SET);
                aux_entidad = *iterador;
                std::fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
                /*Elimino la entidad de la lista*/
                iterador = this->lista_entidades.begin();
                std::advance(iterador, n);
                this->lista_entidades.remove(*iterador);
            }
            else if(this->cabecera == aux_entidad.dameDir_Entidad())
            {
                this->cabecera = aux_entidad.dameDir_Siguiente();
                std::fseek(this->ptr_Archivo, 0, SEEK_SET);
                std::fwrite(&this->cabecera, sizeof(long), 1, this->ptr_Archivo);
                buffer << "Es es el primero" << iterador->dame_Nombre() << std::endl;
                /*Elimino la entidad de la lista*/
                iterador = this->lista_entidades.begin();
                std::advance(iterador, n);
                this->lista_entidades.remove(*iterador);
            }
        }
        else
        {
            std::cout << "El diccionario esta vacio" << std::endl;
        }
    }
    return buffer.str();
}

std::__cxx11::string CDiccionario::imprime_Entidades()
{
    std::stringstream buffer;
    std::list<CEntidad>::iterator iterador;

    iterador = this->lista_entidades.begin();

    while(iterador != this->lista_entidades.end())
    {
        buffer << "  Nombre  "
                  << iterador->dame_Nombre() << std::endl
                  << "\tdir_entidad   "
                  << iterador->dameDir_Entidad() << std::endl
                  << "\tdir_atributo  "
                  << iterador->dameDir_Atributo() << std::endl
                  << "\tdir_dato      "
                  << iterador->dameDir_Dato() << std::endl
                  << "\tdir_siguiente "
                  << iterador->dameDir_Siguiente() << std::endl
                  << std::endl;
        iterador++;
    }
    if(this->cabecera == -1)
    {
        buffer << "El diccionario esta vacio " << std::endl;
    }
    return buffer.str();
}

int CDiccionario::lista_EntidadesMenu()
{
    //Menu que se encarga de imprimir las entidades de la lista de forma numerica
    std::list<CEntidad>::iterator iterador;
    int contador;
    int res = -1;

    if(!this->lista_entidades.empty())
    {
        std::cout << "Selecciona Entidad" << std::endl << std::endl;
        iterador = this->lista_entidades.begin();
        contador = 1;
        while(iterador != this->lista_entidades.end())
        {
            std::cout << contador
                      << " - "
                      << iterador->dame_Nombre()
                      << std::endl;
            contador++;
            iterador++;
        }
        std::cout << "0 - Regresar" << std::endl;
        std::cout << "> ";
        std::cin >> res;
    }
    return res;
}

std::__cxx11::string CDiccionario::selecciona_Entidad(int index)
{
    /*Menu principal donde esta seleecionado una entidad
     * para agregarle sus atributos*/
    int opcion, seleccion, tipo, tam = 1;
    char na[20];
    std::string log;
    std::stringstream buffer;
    std::list<CEntidad>::iterator iterator;
    CEntidad aux_entidad;

    if(this->ptr_Archivo != NULL)
    {
        iterator = this->lista_entidades.begin();
        if(iterator != this->lista_entidades.end())
        {
            std::advance(iterator, index);
            do
            {
                CLEAR
                tam = 1;
                opcion = this->menu_Entidad(iterator->dame_Nombre(), log);

                switch (opcion)
                {
                    case 1:
                        std::cout << "Nombre Atributo:" << std::endl
                                  << "> ";
                        std::cin >> na;
                        std::cout << "Tipo del Atributo" << std::endl
                                  << "1 - CHAR\n" << "2 - INT\n" << "3 - FLOAT\n"
                                  << "4 - STRING";
                        do
                        {
                            std::cout << "\n> ";
                            std::cin >> tipo;
                        }while(tipo >= 4 && tipo <= 1);
                        if(tipo == 4)
                        {
                            std::cout << "Tamaño de la cadena:" << std::endl << "> ";
                            std::cin >> tam;
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
                            std::cout << "Nuevo nombre Atributo:" << std::endl
                                      << "> ";
                            std::cin >> na;
                            std::cout << "Nuevo tipo del Atributo" << std::endl
                                      << "1 - CHAR\n" << "2 - INT\n" << "3 - FLOAT\n"
                                      << "4 - STRING";
                            do
                            {
                                std::cout << "\n> ";
                                std::cin >> tipo;
                            }while(tipo >= 4 && tipo <= 1);
                            if(tipo == 4)
                            {
                                std::cout << "Tamaño de la cadena:" << std::endl << "> ";
                                std::cin >> tam;
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
                    default:
                        break;
                }
            }while(opcion != 0);
            /*Cuando se terminan de hacer todos los cambios se actualiza la entidad del archivo*/
            std::fseek(this->ptr_Archivo, iterator->dameDir_Entidad(), SEEK_SET);
            aux_entidad = *iterator;
            std::fwrite(&aux_entidad, sizeof(CEntidad), 1, this->ptr_Archivo);
        }
    }
    return buffer.str();
}

int CDiccionario::menu_Entidad(char n[20], std::string buf)
{
    int opcion;
    std::cout << "Entidad: " << n << std::endl
              << buf << std::endl
              << "1 - Agregar Atributo" << std::endl
              << "2 - Elimina entidad" << std::endl
              << "3 - Edita entidad" << std::endl
              << "4 - Imprime Atributos" << std::endl
              << "5 - Insertar Dato" << std::endl
              << "0 - Regresar" << std::endl
              << "> ";
    std::cin >> opcion;
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
