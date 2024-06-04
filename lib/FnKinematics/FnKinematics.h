#ifndef FUNCIONES_H
#define FUNCIONES_H

struct nums{
    float uno;
    float dos;
    float tres;
};

struct nums FnDirKinem(float ang_b, float ang_s, float ang_e);
std::vector<float> Tlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin);
std::vector<float> Telipse(float x_inicio, float y_inicio, float x_fin, float y_fin);
float** arrayTLineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin);
float** arrayTelipse(float x_inicio, float y_inicio, float x_fin, float y_fin);
//std::vector<float> Rest(float x_inicio, float y_inicio);
//std::vector<float> Approach(float x, float y);
//std::vector<float> MovRobot(float x_inicio, float y_inicio, float x_fin, float y_fin);

#endif