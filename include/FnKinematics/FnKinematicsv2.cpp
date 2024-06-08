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
#define paso 10 // Aquí se define el paso, no hay una correspondencia directa entre este paso y el de los
                // ángulos, pero con 10mm parece funcionar bien al proporcionar diferencias angulares entre
                // pasos de entre 1 y 3 grados aproximadamente
#define z_amplitud 80
#define MAX(a,b) (((a)>=(b)) ? (a) : (b))

float** arrayTLinealv2(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin) {
    // Conocida la posición incial y final se realiza un bucle para dividir la recta que las une en num_pasos
    float num_pasos_x = round(abs(x_fin-x_inicio)/paso);
    float num_pasos_y = round(abs(y_fin-y_inicio)/paso);
    float num_pasos_z = round(abs(z_fin-z_inicio)/paso);
    
    float num_pasosv2 = MAX(num_pasos_x, num_pasos_y);
    num_pasosv2 = MAX(num_pasosv2, num_pasos_z);
    
    float x_pasos[(int)num_pasosv2];
    float y_pasos[(int)num_pasosv2];
    float z_pasos[(int)num_pasosv2];

    // Array de salida
    float** angulos = new float*[(int)num_pasosv2];
    // Inicializamos el array de salida
    for (int i = 0; i < num_pasosv2; ++i) {
        angulos[i] = new float[3];
    }

    // Asignamos los valores del array de salida
    for (int i = 0; i < num_pasosv2; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasosv2 - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasosv2 - 1);
        z_pasos[i] = z_inicio + (z_fin - z_inicio) * i / (num_pasosv2 - 1);

        struct nums angles = FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

        angulos[i][0] = angles.uno;
        angulos[i][1] = angles.dos;
        angulos[i][2] = angles.tres;

        //printf("TLINEAL %d: %2.3f \t %2.3f \t %2.3f \n", i, angulos[i][0], angulos[i][1], angulos[i][2]);
    }
    return angulos;
}

float** arrayTelipsev2(float x_inicio, float y_inicio, float x_fin, float y_fin) {
    // Conocida la posición incial y final se realiza un bucle para realizar una trayectoria elípitca de num_pasos entre ellas
    float num_pasos_x = round(abs(x_fin-x_inicio)/paso);
    float num_pasos_y = round(abs(y_fin-y_inicio)/paso);
    
    float num_pasosv2 = MAX(num_pasos_x, num_pasos_y);
    
    float x_pasos[(int)num_pasosv2];
    float y_pasos[(int)num_pasosv2];
    float z_pasos[(int)num_pasosv2];

    // Array de salida
    float** angulos = new float*[(int)num_pasosv2];
    // Inicializamos el array de salida
    for (int i = 0; i < num_pasosv2; ++i) {
        angulos[i] = new float[3];
    }

    // Asignamos valores al array de salida
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

        struct nums angles = FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i]);

        angulos[i][0] = angles.uno;
        angulos[i][1] = angles.dos;
        angulos[i][2] = angles.tres;

        printf("TELIPSE %d: %2.3f \t %2.3f \t %2.3f \n", i, angulos[i][0], angulos[i][1], angulos[i][2]);
    }
    return angulos;
}