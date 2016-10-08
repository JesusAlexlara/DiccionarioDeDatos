#ifndef LIMPIAPANTALLA_H
#define LIMPIAPANTALLA_H

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__) && defined(__MACH__)
#define CLEAR system("clear");
#else
#define CLEAR system("cls");
#endif

#endif // LIMPIAPANTALLA_H
