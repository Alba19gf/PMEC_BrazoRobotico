#ifndef FUNCIONES_H
#define FUNCIONES_H

struct nums{
    float uno;
    float dos;
    float tres;
};
struct nums FnDirKinem(float ang_b, float ang_s, float ang_e, float L1, float L2, float L3);
struct nums FnInvKinem(float x, float y, float z, float L1, float L2, float L3);

#endif