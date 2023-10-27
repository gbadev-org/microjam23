#ifndef EAB_ROBBERY_H
#define EAB_ROBBERY_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_fixed_rect.h"
#include "bn_keypad.h"


#include "bn_sprite_items_ppick_tileset.h"
#include "mj/mj_game.h"



namespace eab
{



class robbery : public mj::game
{

public:
    robbery(int completed_games, const mj::game_data& data);

    [[nodiscard]] bn::string<16> title() const final
    {
        return "pumpkin robbery!";
    }

    [[nodiscard]] int total_frames() const final
    {
        return total_frames_;
    }

    void fade_in(const mj::game_data& data) final;

    [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

    [[nodiscard]] bool victory() const final
    {
        return victory_;
    }

    void fade_out(const mj::game_data& data) final;

private:
    bn::regular_bg_ptr bg_;

    struct Pumpkin
    {
        bn::sprite_ptr sprite_;
        unsigned slotx_;
        unsigned sloty_;
    };

    bn::vector<Pumpkin, 64> pumpkins_;
    bn::vector<bn::sprite_ptr, 100> decorations_;

    void make_pumpkin(unsigned slotx, unsigned sloty)
    {
        auto spr = make_spr(1);
        spr.set_x(16 + -120 + slotx * 16 + 8);
        spr.set_y(16 + -80 + sloty * 16 + 8);
        pumpkins_.push_back({spr, slotx, sloty});
    }

    void load_map(const unsigned char map[7][12])
    {
        for (int x = 0; x < 12; ++x) {
            for (int y = 0; y < 7; ++y) {
                switch (map[y][x]) {
                default:
                case 0:
                    break;

                case 1:
                    make_pumpkin(x, y);
                    break;

                case 2:
                    start_x_ = 16 + -120 + x * 16 + 8;
                    start_y_ = 16 + -80 + y * 16 + 8;
                    break;
                }
            }
        }
    }

    static bn::sprite_ptr make_spr(int index)
    {
        bn::sprite_builder bld(bn::sprite_items::ppick_tileset, index);
        return bld.build();
    }

    struct Theif
    {
        int run_keyframe_ = 0;
        int keyframe_counter_ = 0;
        int stress_counter_ = 0;

        Theif() : shadow_(make_spr(13)),
                  bag_(make_spr(19)),
                  top_half_(make_spr(3)),
                  legs_(make_spr(12)),
                  stress_(make_spr(20))
        {
            shadow_.put_below();
            stress_.set_visible(false);
        }

        void update(bool can_move)
        {
            bool running = true;

            bn::fixed base_speed = 2.4;
            base_speed -= pumpkin_count_ * bn::fixed(0.07);
            bn::fixed diagonal_speed = base_speed / bn::fixed(1.41);

            if (not can_move) {
                running = false;
            } else if (bn::keypad::up_held()) {
                if (bn::keypad::left_held()) {
                    move(-diagonal_speed, -diagonal_speed);
                } else if (bn::keypad::right_held()) {
                    move(diagonal_speed, -diagonal_speed);
                } else {
                    move(0, -base_speed);
                }
            } else if (bn::keypad::down_held()) {
                if (bn::keypad::left_held()) {
                    move(-diagonal_speed, diagonal_speed);
                } else if (bn::keypad::right_held()) {
                    move(diagonal_speed, diagonal_speed);
                } else {
                    move(0, base_speed);
                }
            } else if (bn::keypad::left_held()) {
                if (bn::keypad::up_held()) {
                    move(-diagonal_speed, -diagonal_speed);
                } else if (bn::keypad::down_held()) {
                    move(-diagonal_speed, diagonal_speed);
                } else {
                    move(-base_speed, 0);
                }
            } else if (bn::keypad::right_held()) {
                if (bn::keypad::up_held()) {
                    move(diagonal_speed, -diagonal_speed);
                } else if (bn::keypad::down_held()) {
                    move(diagonal_speed, diagonal_speed);
                } else {
                    move(base_speed, 0);
                }
            } else {
                running = false;
            }

            if (running) {
                if (++keyframe_counter_ == 3) {
                    keyframe_counter_ = 0;
                    ++run_keyframe_;
                    run_keyframe_ %= 4;
                    top_half_.set_item(bn::sprite_items::ppick_tileset,
                                       4 + run_keyframe_);
                    legs_.set_item(bn::sprite_items::ppick_tileset,
                                   8 + run_keyframe_);
                }
            } else {
                run_keyframe_ = 0;
                top_half_.set_item(bn::sprite_items::ppick_tileset, 3);
                legs_.set_item(bn::sprite_items::ppick_tileset, 12);
                run_keyframe_ = 0;
            }

            if (not running) {
                stress_.set_visible(false);
            } else if (pumpkin_count_ >= 7) {
                if (++stress_counter_ == 20) {
                    stress_counter_ = 0;
                    if (stress_.visible()) {
                        stress_.set_visible(false);
                    } else {
                        stress_.set_visible(true);
                    }
                }
            }
        }

        void move(bn::fixed dx, bn::fixed dy)
        {
            auto new_x = get_x() + dx;
            auto new_y = get_y() + dy;

            if (dx > 0) {
                top_half_.set_horizontal_flip(true);
                legs_.set_horizontal_flip(true);
                shadow_.set_horizontal_flip(true);
                bag_.set_horizontal_flip(true);
                stress_.set_horizontal_flip(true);
            } else if (dx < 0) {
                top_half_.set_horizontal_flip(false);
                legs_.set_horizontal_flip(false);
                shadow_.set_horizontal_flip(false);
                bag_.set_horizontal_flip(false);
                stress_.set_horizontal_flip(false);
            }

            new_x = bn::clamp(new_x, bn::fixed(-120 + 16), bn::fixed(120 - 16));
            new_y = bn::clamp(new_y, bn::fixed(-80 + 16), bn::fixed(80 - 48));
            set_position(new_x, new_y);
        }

        void set_position(bn::fixed x, bn::fixed y)
        {
            top_half_.set_x(x);
            top_half_.set_y(y);
            legs_.set_x(x);
            legs_.set_y(y + 16);
            shadow_.set_x(x);
            shadow_.set_y(y + 16);

            if (bag_.horizontal_flip()) {
                bag_.set_x(x - 10);
                bag_.set_y(y - 3);
                stress_.set_x(x + 4);
                stress_.set_y(y - 3);
            } else {
                bag_.set_x(x + 10);
                bag_.set_y(y - 3);
                stress_.set_x(x - 4);
                stress_.set_y(y - 3);
            }
        }

        bn::fixed get_x() const
        {
            return top_half_.x();
        }

        bn::fixed get_y() const
        {
            return top_half_.y();
        }

        void add_pumpkin()
        {
            ++pumpkin_count_;
            auto growth = pumpkin_count_;
            switch (growth) {
            case 1:
                bag_.set_item(bn::sprite_items::ppick_tileset, 14);
                break;

            case 2:
                bag_.set_item(bn::sprite_items::ppick_tileset, 15);
                break;

            case 3:
                bag_.set_item(bn::sprite_items::ppick_tileset, 16);
                break;

            case 4:
                bag_.set_item(bn::sprite_items::ppick_tileset, 17);
                break;

            case 5:
                bag_.set_item(bn::sprite_items::ppick_tileset, 18);
                break;

            default:
                bag_.set_horizontal_scale(1 + 0.03 * growth);
                bag_.set_vertical_scale(1 + 0.03 * growth);
                break;
            }
        }

        bn::sprite_ptr shadow_;
        bn::sprite_ptr bag_;
        bn::sprite_ptr top_half_;
        bn::sprite_ptr legs_;
        bn::sprite_ptr stress_;
        int pumpkin_count_ = 0;
    };

    static bool has_intersection(const Pumpkin& p, const Theif& t)
    {
        bn::fixed_rect r1(p.sprite_.position(), {16, 16});
        bn::fixed_rect r2(t.top_half_.position(), {16, 28});

        return r1.intersects(r2);
    }

    bn::optional<Theif> theif_;

    int total_frames_;
    int show_result_frames_ = 60;
    bool victory_ = false;
    bool defeat_ = false;

    bn::fixed start_x_;
    bn::fixed start_y_;
};



} // namespace eab



#endif
