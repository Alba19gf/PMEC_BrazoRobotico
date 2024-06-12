#ifndef KINEMATICS_H
#define KINEMATICS_H

struct Nums {
    float uno;
    float dos;
    float tres;
};

class Kinematics {
public:
    static Nums FnDirKinem(float ang_b, float ang_s, float ang_e);
    static Nums FnInvKinem(float x, float y, float z);
};

#endif // KINEMATICS_H