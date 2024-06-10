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

float paso_x = 2.5;
float paso_y = 2.5;
float paso_z = 2.5;

float* tLineal_x(float x1, float y1, float z1, float x2, float y2, float z2, int n_steps) {
    struct nums angles1 = FnInvKinem(x1, y1, z1);
    struct nums angles2 = FnInvKinem(x2, y2, z2);
    int num_pasos_x;

    //Serial.printf("ángulos x1[%f]: %f, ángulos x2[%f]: %f\n", x1, angles1.uno, x2, angles2.uno);

    num_pasos_x = ceil(abs(angles1.uno - angles2.uno)/paso_x);

    if (num_pasos_x == 0) {
        num_pasos_x = 1;
    }

    if(n_steps > 0)
    {
        num_pasos_x = n_steps;
    }
    else
    {
        n_steps = num_pasos_x;
    }

    float* angulos = new float[num_pasos_x];
    
    float x_pasos[(int)num_pasos_x];

    for (int i = 0; i < num_pasos_x; ++i) {
        angulos[i] = num_pasos_x == 1 ? angles1.uno : angles1.uno + (angles2.uno - angles1.uno) * i / (num_pasos_x - 1);
        //Serial.printf("Angulos[x][%i]: %f\n", i, angulos[i]);
    }
    return angulos;
}

float* tLineal_y(float x1, float y1, float z1, float x2, float y2, float z2, int n_steps) {
    struct nums angles1 = FnInvKinem(x1, y1, z1);
    struct nums angles2 = FnInvKinem(x2, y2, z2);

    //Serial.printf("ángulos y1[%f]: %f, ángulos y2[%f]: %f\n", y1, angles1.dos, y2, angles2.dos);

    int num_pasos_y = ceil(abs(angles1.dos - angles2.dos)/paso_y);

    if (num_pasos_y == 0) {
        num_pasos_y = 1;
    }

    if(n_steps > 0)
    {
        num_pasos_y = n_steps;
    }
    else
    {
        n_steps = num_pasos_y;
    }


    float* angulos = new float[num_pasos_y];
    
    float y_pasos[(int)num_pasos_y];

    for (int i = 0; i < num_pasos_y; ++i) {
        angulos[i] = num_pasos_y == 1 ? angles1.dos : angles1.dos + (angles2.dos - angles1.dos) * i / (num_pasos_y - 1);
        //Serial.printf("Angulos[y][%i]: %f\n", i, angulos[i]);
    }
    return angulos;
}

float *tLineal_z(float x1, float y1, float z1, float x2, float y2, float z2, int n_steps) {
    struct nums angles1 = FnInvKinem(x1, y1, z1);
    struct nums angles2 = FnInvKinem(x2, y2, z2);
    int num_pasos_z;

    //Serial.printf("ángulos z1[%f]: %f, ángulos z2[%f]: %f\n", z1, angles1.tres, z2, angles2.tres);

    
    num_pasos_z = ceil(abs(angles1.tres - angles2.tres)/paso_z);

    if (num_pasos_z == 0) {
        num_pasos_z = 1;
    }

    if(n_steps > 0)
    {
        num_pasos_z = n_steps;
    }
    else
    {
        n_steps = num_pasos_z;
    }

    float* angulos = new float[num_pasos_z];
    
    float z_pasos[(int)num_pasos_z];

    for (int i = 0; i < num_pasos_z; ++i) {
        angulos[i] = num_pasos_z == 1 ? angles1.tres : angles1.tres + (angles2.tres - angles1.tres) * i / (num_pasos_z - 1);
        //Serial.printf("Angulos[z][%i]: %f\n", i, angulos[i]);
    }
    return angulos;
}

float** arrayTLinealv2(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin) {
    struct nums angles_inicio = FnInvKinem(x_inicio, y_inicio, z_inicio);
    struct nums angles_final = FnInvKinem(x_fin, y_fin, z_fin);

    int num_pasos_x = ceil(abs(angles_inicio.uno - angles_final.uno)/1.5);
    int num_pasos_y = ceil(abs(angles_inicio.dos - angles_final.dos)/1.5);
    int num_pasos_z = ceil(abs(angles_inicio.tres - angles_final.tres)/1.5);
    
    int num_pasosv2 = MAX(num_pasos_x, num_pasos_y);
    num_pasosv2 = MAX(num_pasosv2, num_pasos_z);

    if (num_pasosv2 == 0) {
        num_pasosv2 = 1;
    }

    float** angulos = new float*[3];
    for (int i = 0; i < 3; ++i) {
        angulos[i] = new float[num_pasosv2];
    }
    
    float x_pasos[(int)num_pasosv2];
    float y_pasos[(int)num_pasosv2];
    float z_pasos[(int)num_pasosv2];

    for (int i = 0; i < num_pasosv2; ++i) {
        angulos[0][i] = num_pasosv2 == 1 ? angles_inicio.uno : angles_inicio.uno + (angles_final.uno - angles_inicio.uno) * i / (num_pasosv2 - 1);
        angulos[0][i] = num_pasosv2 == 1 ? angles_inicio.dos : angles_inicio.dos + (angles_final.dos - angles_inicio.dos) * i / (num_pasosv2 - 1);
        angulos[0][i] = num_pasosv2 == 1 ? angles_inicio.tres : angles_inicio.tres + (angles_final.tres - angles_inicio.tres) * i / (num_pasosv2 - 1);
        //Serial.printf("Angulos[0][%i]: %f\n", i, angulos[0][i]);
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