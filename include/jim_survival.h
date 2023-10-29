#ifndef JIM_SURVIVAL_H
#define JIM_SURVIVAL_H

#include "jim_forest_scene.h"
#include "mj/mj_game.h"

namespace jim
{
    class survival : public mj::game
    {
        public:
            
            survival(int completed_games, const mj::game_data& data);

            [[nodiscard]] bn::string<16> title() const final
            {
                return "Survive";
            }

            [[nodiscard]] int total_frames() const final
            {
                return _total_frames;
            }

            void fade_in(const mj::game_data& data) final;

            [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

            [[nodiscard]] bool victory() const final
            {
                return _victory;
            }
            
            void fade_out(const mj::game_data& data) final;

        private:
            jim::forest_scene scene;

            int _total_frames;
            int _show_result_frames = 60;
            
            bool _victory = false;
            bool _defeat = false;
    };
}

#endif
