#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "FnKinematics.h"

#define L1 275
#define L2 255
#define L3 280

#define PI 3.141592
#define num_pasos 50
#define z_amplitud 80

// FUNCIÓN QUE REALIZA LA CINEMÁTICA DIRECTA DEL ROBOT
//
// dados los ángulos del robot en grados y las dimensiones principales de los eslabones en mm calcula la cinemática directa
// devolviendo la posición en el espacio XYZ del TCP
struct nums FnDirKinem(float ang_b, float ang_s, float ang_e) {
    // Se inicializan las variables a utilizar
    float P1[2], P2[2], P3[2], x, y, z, w;
    struct nums TCP;

    // Inicialmente se pasan los ángulos a radianes pues son las unidades en las que trabajan las funciones senoidales
    ang_b = ang_b*2*PI/360;
    ang_s = ang_s*2*PI/360;
    ang_e = ang_e*2*PI/360;

    // Se obtienen por trigonometría las posiciones en el espacio de las articulaciones del robot
    P1[0] = L1*sin(ang_s);
    P1[1] = L1*cos(ang_s);
    P2[0] = P1[0]+L2*cos(PI/2-(ang_s+ang_e));
    P2[1] = P1[1]+L2*sin(PI/2-(ang_s+ang_e));
    P3[0] = P2[0];
    P3[1] = P2[1]-L3;

    // Se define como w la posición en el eje x del plano del robot de la punta de la herramienta
    w = P3[0];
    // Se define como z la altura respecto al tablero de juego de la punta de la herramienta
    z = P3[1];

    // Conocidas las posiciones en el plano del robot las coordenadas X e Y simplemente son función de este y del ángulo de la base
    x = w*sin(ang_b);
    y = w*cos(ang_b);

    TCP.uno = x;
    TCP.dos = y;
    TCP.tres = z;

    return TCP;
}

// FUNCIÓN QUE REALIZA LA CINEMÁTICA INVERSA DEL ROBOT
//
// dada la posición XYZ en el espacio de la punta de la herramienta y las dimensiones principales del robot en mm calcula
// la cinemática inversa devolviendo los ángulos del robot correspondientes a dicha posición
struct nums FnInvKinem(float x, float y, float z) {
    // Se inicializan las variables a utilizar
    float P1[2], P2[2], P3[2], t[2], ang_b, ang_s, ang_e, w, a, b, c, d;
    struct nums angulos;

    // Si el punto indicado por la generación de trayectorias estña fuera del espacio de trabajo se indica en la ventana de comandos
    if ((x < -95) || (x > 95) ||(y < 220) || (y > 440) || (z < -150) || (z > 40)) {
        printf("\nEl punto deseado se encuentra de fuera de la zona de juego \n");
    }

    // El plano y el ángulo de la base se definen simplemente en base a las coordenadas X e Y
    w = sqrt(pow(x,2)+pow(y,2));
    ang_b = atan2(x, y); // Hay que incluir math.h

    // Conocido que la pinza siempre formará 90 grados con el tablero por la configuración del robot resulta directo definir la
    // posición de la muñeca
    P3[0] = w;
    P3[1] = z;
    P2[0] = w;
    P2[1] = z+L3;

    // El cálculo de la posición de la articulación 1 (codo) se realiza mediante la intersección de dos circunferencias, una de centro
    // el hombro y radio L1 y otra de centro la muñeca y radio L2 (ambas posiciones, codo y muleca son a esta altura ya conocidas)
    a = -4*pow(P2[1],2)-4*pow(P2[0],2);
    b = -4*P2[1]*(pow(L2,2)-pow(L1,2)-pow(P2[0],2)-pow(P2[1],2));
    c = 4*pow(P2[0],2)*pow(L1,2)-pow((pow(L2,2)-pow(L1,2)-pow(P2[0],2)-pow(P2[1],2)),2);
    d = sqrt(pow(b,2)-4*a*c);
    if ((pow(b,2)) < (4*a*c))
    {
        printf("\nERROR: La trayectoria demandada esta fuera del area de trabajo \n");
        // En el caso de que la raíz sea imaginaria evidentemente la ecuación no tiene solución real, el punto exigido no es accesible
        // al brazo robótico y se indica por pantalla
    }
    t[0] = (-b+d)/(2*a);
    t[1] = (-b-d)/(2*a);
    
    // Hay dos posibles soluciones, se toma siempre la de mayor valor, que se corresponderá con una altura Z superior, en caso contrario 
    // se tomaría probablemente una configuración en la que el primer brazo estaría por debajo del tablero
    if (t[0] > t[1])
    {
        P1[1] = t[0];
    }
    else
    {
        P1[1] = t[1];
    }

    // Se obtiene la coordenada W del codo en función de la Z obtenida
    P1[0] = sqrt(pow(L1,2)-pow(P1[1],2));

    ang_s = acos(P1[1]/L1);
    ang_e = -(asin((P2[1]-P1[1])/L2)-PI/2+ang_s);

    // Se realiza la conversión para dar el resultado en grados
    ang_b = ang_b*360/(2*PI);   // Realizar aquí el ajuste de la calibración, lo que ca,biemos se refleja en la salida de las trayectorias
    ang_s = ang_s*360/(2*PI);
    ang_e = ang_e*360/(2*PI);

    angulos.uno = ang_b;
    angulos.dos = ang_s;
    angulos.tres = ang_e;

    return angulos;
}

std::vector<float> Tlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin) {
    // Conocida la posición incial y final se realiza un bucle para dividir la recta que las une en num_pasos
    std::vector<float> x_pasos(num_pasos), y_pasos(num_pasos), z_pasos(num_pasos);
    std::vector<float> angs_salida(3*num_pasos);

        for (int i = 0; i < num_pasos; ++i) {
            x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
            y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);
            z_pasos[i] = z_inicio + (z_fin - z_inicio) * i / (num_pasos - 1);

            struct nums angles = FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

            angs_salida[3*i] = angles.uno;
            angs_salida[3*i+1] = angles.dos;
            angs_salida[3*i+2] = angles.tres;
        }
    return angs_salida;
}

float** arrayTLineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin) {
 // Conocida la posición incial y final se realiza un bucle para dividir la recta que las une en num_pasos
    float x_pasos[num_pasos];
    float y_pasos[num_pasos];
    float z_pasos[num_pasos];

    // Array de salida
    float** angulos = new float*[num_pasos];
    // Inicializamos el array de salida
    for (int i = 0; i < num_pasos; ++i) {
        angulos[i] = new float[3];
    }

    // Asignamos los valores del array de salida
    for (int i = 0; i < num_pasos; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);
        z_pasos[i] = z_inicio + (z_fin - z_inicio) * i / (num_pasos - 1);

        struct nums angles = FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

        angulos[i][0] = angles.uno;
        angulos[i][1] = angles.dos;
        angulos[i][2] = angles.tres;
    }
    return angulos;
}

std::vector<float> Telipse(float x_inicio, float y_inicio, float x_fin, float y_fin) {
    // Se divide la trayectoria elíptica en num_pasos y se calcula la cinemática inversa para cada uno de ellos
    std::vector<float> x_pasos(num_pasos), y_pasos(num_pasos), z_pasos(num_pasos);
    std::vector<float> angs_salida(3*num_pasos);

    for (int i = 0; i < num_pasos; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);

        float t = static_cast<float>(i) / (num_pasos - 1) * 2;
        if ((0 < t) && (t <= 2)) {
            float angle = (t / 2) * PI;
            z_pasos[i] = z_amplitud * sin(angle);
        } else {
            z_pasos[i] = 0;
        }

        // Cálculo de los ángulos para las posiciones intermedias
        struct nums angles = FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

        angs_salida[3*i] = angles.uno;
        angs_salida[3*i+1] = angles.dos;
        angs_salida[3*i+2] = angles.tres;
    }   
    return angs_salida;
}

float** arrayTelipse(float x_inicio, float y_inicio, float x_fin, float y_fin) {
    // Conocida la posición incial y final se realiza un bucle para realizar una trayectoria elípitca de num_pasos entre ellas
    float x_pasos[num_pasos];
    float y_pasos[num_pasos];
    float z_pasos[num_pasos];

    // Array de salida
    float** angulos = new float*[num_pasos];
    // Inicializamos el array de salida
    for (int i = 0; i < num_pasos; ++i) {
        angulos[i] = new float[3];
    }

    // Asignamos valores al array de salida
    for (int i = 0; i < num_pasos; i++) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);

        float t = static_cast<float>(i) / (num_pasos - 1) * 2;
        if ((0 < t) && (t <= 2)) {
            float angle = (t / 2) * PI;
            z_pasos[i] = z_amplitud * sin(angle);
        } 
        else {
            z_pasos[i] = 0;
        }

        struct nums angles = FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

        angulos[i][0] = angles.uno;
        angulos[i][1] = angles.dos;
        angulos[i][2] = angles.tres;
    }
    return angulos;
}

// HAY QUE VER CÓMO SE INTEGRAN FINALMENTE ESTAS FUNCIONES, SI SERÍAN PARTE DE LA CINEMÁTICA O HABRÍA QUE INTEGRARLAS DENTRO DEL BUCLE PRINCIPAL

// std::vector<float> Rest(float x_inicio, float y_inicio) {
//     // LLAMADA A LOS ENCÓDERS PARA CONOCER LOS ÁNGULOS ACTUALES
//     float ang_b = 20;   // AQUÍ PONDRÍAMOS LAS LECTURAS DE LOS ENCÓDERS
//     float ang_s = 10;
//     float ang_e = 80;
//     struct nums posi = FnDirKinem(ang_b, ang_s, ang_e);
//     float xTCP = posi.uno;
//     float yTCP = posi.dos;
//     float zTCP = posi.tres;
//     Tlineal(xTCP, yTCP, zTCP, xTCP, yTCP, zTCP+20);
//     Tlineal(xTCP, yTCP, zTCP+20, 0, 250, 40);
// }

// std::vector<float> Approach(float x, float y) {
//     // Función que aproxima la herramienta al tablero en la posición incial deseada para el movimiento
//     // Se trazaría una líne recta desde la posición de reposo a una posición superior a la posición deseada
//     Tlineal(0, 250, 40, x, y, -120);
//     Tlineal(x, y, -120, x, y, -140);
// }

// std::vector<float> MovRobot(float x_inicio, float y_inicio, float x_fin, float y_fin) {
//     Approach(x_inicio, y_inicio);
//     // CIERRE DE LA PINZA
//     Tlineal(x_inicio, y_inicio, -140, x_inicio, y_fin, -120);
//     // Trayectoria elípitica (ver si se puede utilizar para una altura que no sea la del tablero)ç
//     Tlineal(x_fin, y_fin, -120, x_fin, y_fin, -140);
//     // APERTURA DE LA PINZA 
//     Rest(x_fin, y_fin);
// }