#include <iostream>
#include "cdiccionario.h"
#include "limpiapantalla.h"

int menu(std::string log)
{
    int opcion;
    std::cout << log << std::endl
              << "1 - Crear/Abrir diccionario" << std::endl
              << "2 - Agregar Entidad" << std::endl
              << "3 - Elimina entidad" << std::endl
              << "4 - Edita entidad" << std::endl
              << "5 - Imprime Entidades" << std::endl
              << "6 - Agrega Atributos" << std::endl
              << "0 - Salir" << std::endl
              << "> ";
    std::cin >> opcion;
    return opcion;
}

int main()
{
    std::string log;
    CDiccionario d;
    int opcion, seleccion;
    char na[20];
    do
    {
        CLEAR
        opcion = menu(log);
        switch (opcion) {
        case 1:
            std::cout << "Nombre del archivo:" << std::endl
                      << "> ";
            std::cin >> na;
            log = d.abrir_Diccionario(na);
            break;
        case 2:
            if(d.existe_Diccionario())
            {
                std::cout << "Nombre de la entidad:" << std::endl
                          << "> ";
                std::cin >> na;
                log = d.agrega_Entidad(na);
            }
            else
            {
                log = "No hay ningun archivo abierto\n";
            }
            break;
        case 3:
            if(d.existe_Diccionario())
            {
                CLEAR
                seleccion = d.lista_EntidadesMenu();
                if(seleccion >= 1)
                {
                    log = d.elimina_Entidad(seleccion-1);
                }
            }
            else
            {
                log = "No hay ningun archivo abierto\n";
            }
            break;
        case 4:
            if(d.existe_Diccionario())
            {
                CLEAR
                seleccion = d.lista_EntidadesMenu();
                if(seleccion >= 1)
                {
                    std::cout << "Nuevo nombre de la entidad:" << std::endl
                              << "> ";
                    std::cin >> na;
                    log = d.edita_Entidad(seleccion, na);
                }
                else
                {
                    log = "El diccionario esta vacio\n";
                }
            }
            else
            {
                log = "No hay ningun archivo abierto\n";
            }
            break;
        case 5:
            if(d.existe_Diccionario())
            {
                log = d.imprime_Entidades();
            }
            else
            {
                log = "No hay ningun archivo abierto\n";
            }
            break;
        case 6:
            if(d.existe_Diccionario())
            {
                CLEAR
                seleccion = d.lista_EntidadesMenu();
                if(seleccion >= 0)
                {
                    if(seleccion >= 1)
                    {
                        log = d.selecciona_Entidad(seleccion-1);
                    }
                }
                else
                {
                    log = "El diccionario esta vacio\n";
                }
            }
            else
            {
                log = "No hay ningun archivo abierto\n";
            }
            break;
        default:
            log = "Opción incorrecta\n";
            break;
        }
    }while(opcion != 0);

    return 0;
}

