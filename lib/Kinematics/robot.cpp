#include "Robot.h"
#include "Kinematics.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>

#define PI 3.141592
#define paso 10
#define z_amplitud 80
#define MAX(a,b) (((a)>=(b)) ? (a) : (b))

float** Robot::arrayTlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin) {
    float num_pasos_x = round(abs(x_fin-x_inicio)/paso);
    float num_pasos_y = round(abs(y_fin-y_inicio)/paso);
    float num_pasos_z = round(abs(z_fin-z_inicio)/paso);
    
    float num_pasos = MAX(num_pasos_x, num_pasos_y);
    num_pasos = MAX(num_pasos, num_pasos_z);
    
    std::vector<float> x_pasos((int)num_pasos);
    std::vector<float> y_pasos((int)num_pasos);
    std::vector<float> z_pasos((int)num_pasos);

    float** angulos = new float*[(int)num_pasos];
    for (int i = 0; i < num_pasos; ++i) {
        angulos[i] = new float[3];
    }

    for (int i = 0; i < num_pasos; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos- 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);
        z_pasos[i] = z_inicio + (z_fin - z_inicio) * i / (num_pasos - 1);

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

        angulos[i][0] = angles.uno;
        angulos[i][1] = angles.dos;
        angulos[i][2] = angles.tres;

        printf("TLINEAL %d: %2.3f \t %2.3f \t %2.3f \n", i, angulos[i][0], angulos[i][1], angulos[i][2]);
    }
    return angulos;
}

float** Robot::arrayTelipse(float x_inicio, float y_inicio, float x_fin, float y_fin) {
    float num_pasos_x = round(abs(x_fin-x_inicio)/paso);
    float num_pasos_y = round(abs(y_fin-y_inicio)/paso);
    
    float num_pasosv2 = MAX(num_pasos_x, num_pasos_y);
    
    std::vector<float> x_pasos((int)num_pasosv2);
    std::vector<float> y_pasos((int)num_pasosv2);
    std::vector<float> z_pasos((int)num_pasosv2);

    float** angulos = new float*[(int)num_pasosv2];
    for (int i = 0; i < num_pasosv2; ++i) {
        angulos[i] = new float[3];
    }

    for (int i = 0; i < num_pasosv2; i++) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasosv2 - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasosv2 - 1);

        float t = static_cast<float>(i) / (num_pasosv2 - 1) * 2;
        if ((0 < t) && (t <= 2)) {
            float angle = (t / 2) * PI;
            z_pasos[i] = z_amplitud * sin(angle);
        } 
        else {
            z_pasos[i] = 0;
        }

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

        angulos[i][0] = angles.uno;
        angulos[i][1] = angles.dos;
        angulos[i][2] = angles.tres;

        printf("TELIPSE %d: %2.3f \t %2.3f \t %2.3f \n", i, angulos[i][0], angulos[i][1], angulos[i][2]);
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