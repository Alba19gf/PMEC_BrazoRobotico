#include "Robot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "Kinematics.h"

#define num_pasos 50
#define z_amplitud 80

void Robot::Tlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin, float L1, float L2, float L3) {
    std::vector<float> x_pasos(num_pasos), y_pasos(num_pasos), z_pasos(num_pasos);
    std::vector<float> angs_salida(3 * num_pasos);

    for (int i = 0; i < num_pasos; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);
        z_pasos[i] = z_inicio + (z_fin - z_inicio) * i / (num_pasos - 1);

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i], L1, L2, L3);

        angs_salida[3 * i] = angles.uno;
        angs_salida[3 * i + 1] = angles.dos;
        angs_salida[3 * i + 2] = angles.tres;
    }

    // Aquí puedes agregar código para usar angs_salida según sea necesario
}

void Robot::Telipse(float x_inicio, float y_inicio, float x_fin, float y_fin, float L1, float L2, float L3) {
    std::vector<float> x_pasos(num_pasos), y_pasos(num_pasos), z_pasos(num_pasos);
    std::vector<float> angs_salida(3 * num_pasos);

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

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i], L1, L2, L3);

        angs_salida[3 * i] = angles.uno;
        angs_salida[3 * i + 1] = angles.dos;
        angs_salida[3 * i + 2] = angles.tres;
    }

    // Aquí puedes agregar código para usar angs_salida según sea necesario
}

float** Robot::arrayTlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin, float L1, float L2, float L3) {
    float x_pasos[num_pasos];
    float y_pasos[num_pasos];
    float z_pasos[num_pasos];

    float** angulos = new float*[num_pasos];
    for (int i = 0; i < num_pasos; ++i) {
        angulos[i] = new float[3];
    }

    for (int i = 0; i < num_pasos; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);
        z_pasos[i] = z_inicio + (z_fin - z_inicio) * i / (num_pasos - 1);

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i], L1, L2, L3);

        angulos[i][0] = angles.uno;
        angulos[i][1] = angles.dos;
        angulos[i][2] = angles.tres;
    }
    return angulos;
}

float** Robot::arrayTelipse(float x_inicio, float y_inicio, float x_fin, float y_fin, float L1, float L2, float L3) {
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

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i], L1, L2, L3);

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