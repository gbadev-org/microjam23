#include "osmag_Physique.h"

namespace osmag{


Physique::Physique(){
}

void Physique::init(bn::random &rand){
    v0 = 0;
    angle = 15;
    generer_position_iniale(rand);
    g = 100; //160
    generer_vent(rand);
}

void Physique::generer_position_iniale(bn::random &rand){
    pos0 = bn::fixed_point(rand.get_int(-111, -56), 80 - 64 - 5); //80 -24 -5

    // HISTO  (-95,-45) (-85,-50) (-83,- 69)
}

void Physique::generer_vent(bn::random& rand){
    //    // Ancien v0

    //    awx = rand.get_int(-70, 71);
    //    if(awx > 15){
    //        awx = 0;
    //    }

    //    // Nouveau v1
    //    int proba = rand.get_int(1, 101);  // Générer un nombre aléatoire entre 1 et 100

    //    if(proba >= 1 && proba <= 50) {  // 50% de chance
    //        awx = 0;
    //    }
    //    else if(proba >= 51 && proba <= 80) {  // 30% de chance
    //        awx = rand.get_int(-70, 0);  // Générer un nombre aléatoire entre -70 et -1
    //    }
    //    else if(proba >= 81 && proba <= 100) {  // 20% de chance
    //        awx = rand.get_int(1, 16);  // Générer un nombre aléatoire entre 1 et 15
    //    }

    // Nouveau 2
    bn::array<bn::array<int, 3>, 3> probabilites = {{
        {15, 0, 0},
        {15, 1 , 5},   // histo {20, 1 , 20}
        {70, -100 , -1}, // histo {70, -85 , -1} {63, -80 , -1}, {60, -70 , -1},

    }};

    int tirage = rand.get_int(1, 101);

    int sommePourcentage = 0;
    for(const auto& proba : probabilites) {
        sommePourcentage += proba[0];

        if(tirage <= sommePourcentage) {
            if(proba[1] == proba[2]) {
                awx = proba[1];
            } else {
                awx = rand.get_int(proba[1], proba[2] + 1);
            }
            break;
        }
    }
}






bn::fixed Physique::get_v0() const{
    return v0;
}

int Physique::get_angle() const{
    return angle;
}

const bn::fixed_point& Physique::get_pos0() const{
    return pos0;
}

bn::fixed Physique::get_g() const{
    return g;
}

bn::fixed Physique::get_awx() const{
    return awx;
}

bool Physique::augmenter_angle(){
    if(angle != 90){
        angle = (angle + 1) % 360;
        return true;
    }
    return false;
}

bool Physique::diminuer_angle(){
    if(angle != 270){
        angle = (angle - 1 +360) % 360;
        return true;
    }
    return false;
}

bool Physique::augmenter_v0(){
    if(v0 < V0_MAX){
        v0 = bn::min((v0 + 1), V0_MAX); // v0 + 3 // INFO test
        return true;
    }
    return false;
}



void Physique::avancer_x0(){
    //pos0.set_x(bn::min(pos0.x() + 1, x0_dec_max));
    pos0.set_x(bn::min(pos0.x() + 1, bn::fixed(-55)));
}

void Physique::reculer_x0(){
    //pos0.set_x(bn::max(pos0.x() - 1, x0_dec_min));
    pos0.set_x(bn::max(pos0.x() - 1, bn::fixed(-111)));
}


bn::pair<bn::fixed_point, bn::fixed> Physique::calculer_position_et_orientation_projectile(const bn::fixed t) const{
    // Composantes de la vitesse initiale;
    bn::fixed v0x = v0 * bn::degrees_lut_cos(angle);
    bn::fixed v0y = v0 * bn::degrees_lut_sin(angle);

    // Calcul de la position
    bn::fixed x = pos0.x() + v0x * t    +   0.5 * awx * t * t;
    bn::fixed y = pos0.y() - (v0y * t    -   0.5 * g * t * t);


    // Calcul de l'orientation theta du projectile
    // Vitesses à l'instant t
    bn::fixed vx = v0x + awx * t;
    bn::fixed vy = v0y - g * t;
    // Angle de rotation
    bn::fixed theta = bn::degrees_atan2(vy.integer(), vx.integer());   //ceil
    theta = theta - 360.0 * ((theta / 360.0).floor_integer());

    return bn::pair<bn::fixed_point, bn::fixed>(bn::fixed_point(x, y), theta);

}


bn::fixed_point Physique::calculer_position_projectile(const bn::fixed t) const{
    // Composantes de la vitesse initiale;
    bn::fixed v0x = v0 * bn::degrees_lut_cos(angle);
    bn::fixed v0y = v0 * bn::degrees_lut_sin(angle);

    // Calcul de la position
    bn::fixed x = pos0.x() + v0x * t    +   0.5 * awx * t * t;
    bn::fixed y = pos0.y() - (v0y * t    -   0.5 * g * t * t);

    return bn::fixed_point(x, y);
}


}
