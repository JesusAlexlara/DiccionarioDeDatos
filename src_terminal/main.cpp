#include <iostream>
#include "cdiccionario.h"
#include "limpiapantalla.h"

int menu(string log)
{
    int opcion;
    cout << log << endl
              << "1 - Crear/Abrir diccionario" << endl
              << "2 - Agregar Entidad" << endl
              << "3 - Elimina entidad" << endl
              << "4 - Edita entidad" << endl
              << "5 - Imprime Entidades" << endl
              << "6 - Agrega Atributos" << endl
              << "0 - Salir" << endl
              << "> ";
    cin >> opcion;
    return opcion;
}

int main()
{
    string log;
    CDiccionario d;
    int opcion, seleccion;
    char na[20];
    do
    {
        CLEAR
        opcion = menu(log);
        switch (opcion) {
        case 1:
            cout << "Nombre del archivo:" << endl
                      << "> ";
            cin >> na;
            log = d.abrir_Diccionario(na);
            break;
        case 2:
            if(d.existe_Diccionario())
            {
                cout << "Nombre de la entidad:" << endl
                          << "> ";
                cin >> na;
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
                    cout << "Nuevo nombre de la entidad:" << endl
                              << "> ";
                    cin >> na;
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

