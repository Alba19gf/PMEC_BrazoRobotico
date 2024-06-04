#include "Robot.h"

#define num_pasos 50
#define z_amplitud 80

void Robot::Tlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin, float L1, float L2, float L3) {
    std::vector<float> x_pasos(num_pasos), y_pasos(num_pasos), z_pasos(num_pasos);
    for (int i = 0; i < num_pasos; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);
        z_pasos[i] = z_inicio + (z_fin - z_inicio) * i / (num_pasos - 1);

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_pasos[i], L1, L2, L3);
        float ang_b_intermedio = angles.uno;
        float ang_s_intermedio = angles.dos;
        float ang_e_intermedio = angles.tres;

        // INCLUIR PAUSA 100ms ENTRE PASOS
    }
}

void Robot::Telipse(float x_inicio, float y_inicio, float x_fin, float y_fin, float L1, float L2, float L3) {
    Nums angles_inicio = Kinematics::FnInvKinem(x_inicio, y_inicio, 0, L1, L2, L3);
    float ang_b_inicio = angles_inicio.uno;
    float ang_s_inicio = angles_inicio.dos;
    float ang_e_inicio = angles_inicio.tres;

    // INCLUIR PAUSA PARA LA POSICIÓN INICIAL

    std::vector<float> x_pasos(num_pasos), y_pasos(num_pasos);
    for (int i = 0; i < num_pasos; ++i) {
        x_pasos[i] = x_inicio + (x_fin - x_inicio) * i / (num_pasos - 1);
        y_pasos[i] = y_inicio + (y_fin - y_inicio) * i / (num_pasos - 1);

        float t = static_cast<float>(i) / (num_pasos - 1) * 2;
        float z_intermedio;
        if ((0 < t) && (t <= 2)) {
            float angle = (t / 2) * PI;
            z_intermedio = z_amplitud * sin(angle);
        } else {
            z_intermedio = 0;
        }

        Nums angles = Kinematics::FnInvKinem(x_pasos[i], y_pasos[i], z_intermedio, L1, L2, L3);
        float ang_b_intermedio = angles.uno;
        float ang_s_intermedio = angles.dos;
        float ang_e_intermedio = angles.tres;

        // INCLUIR PAUSA ENTRE PASOS
    }

    // INCLUIR PAUSA POSICIÓN FINAL
}

void Robot::Rest(float x_inicio, float y_inicio, float L1, float L2, float L3
