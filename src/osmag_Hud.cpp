#include "osmag_Hud.h"

#include "osmag_munro_variable_02.h"
#include "osmag_munro_variable_04.h"


namespace osmag{

Hud::Hud()
    : plume(Police::munro_variable_02)
    , plume_terre(Police::munro_variable_04)
{
}

void Hud::init(const Physique& phys){
    plume.set_bg_priority(1);
    plume_terre.set_bg_priority(1);
    init_noms();
    plume.set_right_alignment();
    update(phys);
}

void Hud::init_noms(){
    plume.set_left_alignment();

    plume.generate(pos_noms(0, 0), "Power: ", txt_noms);
    plume.generate(pos_noms(0, 1), "Angle: ", txt_noms);
    plume.generate(pos_noms(1, 0), "X Pos: ", txt_noms);
    plume.generate(pos_noms(1, 1), "Y Pos: ", txt_noms);
    plume.generate(pos_noms(2, 0), "Wind: ", txt_noms);
    plume.generate(pos_noms(2, 1), "Gravity: ", txt_noms);

    plume_terre.generate(pos_noms(0, 11), "POWER: A+", txt_noms);
    plume_terre.generate(pos_noms(0, 12), "ANGLE: Down-, Up+", txt_noms);
    plume_terre.generate(pos_noms(0, 13), "X POS: Left-, Right+", txt_noms);

    for(bn::sprite_ptr& spt : txt_noms){
        spt.set_blending_enabled(true);
    }
    bn::blending::set_transparency_alpha(0.85);

}

bn::point Hud::pos_noms(int col, int ligne){
    constexpr const int x_col0 = -120 + 3;
    constexpr const int y_ligne0 = -80 + 8;//80 - 17;
    constexpr const int dec_x = 68;
    constexpr const int dec_y = 10;

    return bn::point(x_col0 + dec_x * col, y_ligne0 + dec_y * ligne);
}

bn::point Hud::pos_valeurs(int col, int ligne){
    bn::point pos = pos_noms(col, ligne);
    pos.set_x(pos.x() + 49);

    return pos;
}



void Hud::update(const Physique& phys){
    txt_valeurs.clear();


    int v0 = (phys.get_v0() / 3).floor_integer();
    plume.generate(pos_valeurs(0, 0), bn::to_string<5>(v0), txt_valeurs);
//    plume.generate(pos_valeurs(0, 0), bn::to_string<5>(phys.get_v0()), txt_valeurs);

    int angle = phys.get_angle();
    if(angle >= 270 && angle < 360){
        angle -= 360;
    }
    plume.generate(pos_valeurs(0, 1),  bn::to_string<5>(angle), txt_valeurs);

    plume.generate(pos_valeurs(1, 0),  bn::to_string<5>(phys.get_pos0().x()), txt_valeurs);
    plume.generate(pos_valeurs(1, 1),  bn::to_string<5>(phys.get_pos0().y()), txt_valeurs);
    plume.generate(pos_valeurs(2, 0),  bn::to_string<5>(phys.get_awx()), txt_valeurs);
    plume.generate(pos_valeurs(2, 1),  bn::to_string<5>(phys.get_g()), txt_valeurs);

    for(bn::sprite_ptr& spt : txt_valeurs){
        spt.set_blending_enabled(true);
    }
}

void Hud::clear(){
    txt_noms.clear();
    txt_valeurs.clear();
}


}



