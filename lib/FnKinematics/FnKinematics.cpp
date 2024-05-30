#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "FnKinematics.h"

#define PI 3.141592

// FUNCIÓN QUE REALIZA LA CINEMÁTICA DIRECTA DEL ROBOT
//
// dados los ángulos del robot en grados y las dimensiones principales de los eslabones en mm calcula la cinemática directa
// devolviendo la posición en el espacio XYZ del TCP
struct nums FnDirKinem(float ang_b, float ang_s, float ang_e, float L1, float L2, float L3)
{
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

    return TCP;
}

// FUNCIÓN QUE REALIZA LA CINEMÁTICA INVERSA DEL ROBOT
//
// dada la posición XYZ en el espacio de la punta de la herramienta y las dimensiones principales del robot en mm calcula
// la cinemática inversa devolviendo los ángulos del robot correspondientes a dicha posición
struct nums FnInvKinem(float x, float y, float z, float L1, float L2, float L3)
{
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
    ang_b = ang_b*360/(2*PI);
    ang_s = ang_s*360/(2*PI);
    ang_e = ang_e*360/(2*PI);

    return angulos;
}