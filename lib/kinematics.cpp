#include "Kinematics.h"

Nums Kinematics::FnDirKinem(float ang_b, float ang_s, float ang_e, float L1, float L2, float L3) {
    float P1[2], P2[2], P3[2], x, y, z, w;
    Nums TCP;

    ang_b = ang_b * 2 * PI / 360;
    ang_s = ang_s * 2 * PI / 360;
    ang_e = ang_e * 2 * PI / 360;

    P1[0] = L1 * sin(ang_s);
    P1[1] = L1 * cos(ang_s);
    P2[0] = P1[0] + L2 * cos(PI / 2 - (ang_s + ang_e));
    P2[1] = P1[1] + L2 * sin(PI / 2 - (ang_s + ang_e));
    P3[0] = P2[0];
    P3[1] = P2[1] - L3;

    w = P3[0];
    z = P3[1];

    x = w * sin(ang_b);
    y = w * cos(ang_b);

    TCP.uno = x;
    TCP.dos = y;
    TCP.tres = z;

    return TCP;
}

Nums Kinematics::FnInvKinem(float x, float y, float z, float L1, float L2, float L3) {
    float P1[2], P2[2], P3[2], t[2], ang_b, ang_s, ang_e, w, a, b, c, d;
    Nums angulos;

    if ((x < -95) || (x > 95) || (y < 220) || (y > 440) || (z < -150) || (z > 40)) {
        std::cout << "\nEl punto deseado se encuentra fuera de la zona de juego \n";
    }

    w = sqrt(pow(x, 2) + pow(y, 2));
    ang_b = atan2(x, y);

    P3[0] = w;
    P3[1] = z;
    P2[0] = w;
    P2[1] = z + L3;

    a = -4 * pow(P2[1], 2) - 4 * pow(P2[0], 2);
    b = -4 * P2[1] * (pow(L2, 2) - pow(L1, 2) - pow(P2[0], 2) - pow(P2[1], 2));
    c = 4 * pow(P2[0], 2) * pow(L1, 2) - pow((pow(L2, 2) - pow(L1, 2) - pow(P2[0], 2) - pow(P2[1], 2)), 2);
    d = sqrt(pow(b, 2) - 4 * a * c);
    if ((pow(b, 2)) < (4 * a * c)) {
        std::cout << "\nERROR: La trayectoria demandada está fuera del área de trabajo \n";
    }
    t[0] = (-b + d) / (2 * a);
    t[1] = (-b - d) / (2 * a);

    if (t[0] > t[1]) {
        P1[1] = t[0];
    } else {
        P1[1] = t[1];
    }

    P1[0] = sqrt(pow(L1, 2) - pow(P1[1], 2));

    ang_s = acos(P1[1] / L1);
    ang_e = -(asin((P2[1] - P1[1]) / L2) - PI / 2 + ang_s);

    ang_b = ang_b * 360 / (2 * PI);
    ang_s = ang_s * 360 / (2 * PI);
    ang_e = ang_e * 360 / (2 * PI);

    angulos.uno = ang_b;
    angulos.dos = ang_s;
    angulos.tres = ang_e;

    return angulos;
}
