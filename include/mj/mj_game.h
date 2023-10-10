#ifndef MJ_GAME_H
#define MJ_GAME_H

#include "bn_string.h"
#include "bn_music_item.h"

#include "mj_game_data.h"
#include "mj_game_jingle_type.h"
#include "mj_game_result.h"

namespace mj
{

/**
 * @brief Micro-games base interface.
 */
class game
{

public:
    static constexpr int minimum_frames = 3 * 60; //!< Minimum number of frames per game.
    static constexpr int maximum_frames = 10 * 60; //!< Maximum number of frames per game.

    /**
     * @brief Returns the recommended total frames for the given completed games.
     * @param base_total_frames Base (maximum) number of frames that this game can last.
     * @param completed_games Number of completed games.
     * @param data Shared data between all games.
     * @return Recommended number of frames that this game can last for the given completed games.
     */
    [[nodiscard]] static int recommended_total_frames(
            int base_total_frames, int completed_games, const game_data& data);

    /**
     * @brief Returns the recommended music tempo for the given completed games.
     * @param completed_games Number of completed games.
     * @param data Shared data between all games.
     * @return Recommended music tempo.
     */
    [[nodiscard]] static bn::fixed recommended_music_tempo(int completed_games, const game_data& data);

    /**
     * @brief Plays the given music item with the recommended tempo.
     * @param music_item Specifies the music to play.
     * @param completed_games Number of completed games.
     * @param data Shared data between all games.
     */
    static void play_music(bn::music_item music_item, int completed_games, const game_data& data);

    /**
     * @brief Plays the given common jingle with the recommended tempo
     * and returns the total frames the game should last.
     * @param jingle Common game jingle to play.
     * @param completed_games Number of completed games.
     * @param data Shared data between all games.
     * @return Recommended total frames (maximum number of frames that this game should last).
     */
    [[nodiscard]] static int play_jingle(game_jingle_type jingle, int completed_games, const game_data& data);

    /**
     * @brief Default virtual destructor.
     */
    virtual ~game() = default;

    /**
     * @brief Returns the title of the game (not empty).
     */
    [[nodiscard]] virtual bn::string<16> title() const = 0;

    /**
     * @brief Returns the maximum number of frames that this game can last.
     *
     * It must be in the range [minimum_frames, maximum_frames].
     */
    [[nodiscard]] virtual int total_frames() const = 0;

    /**
     * @brief Updates this game before playing it.
     *
     * Keypad should not be read when this method is called.
     *
     * @param data Shared data between all games.
     */
    virtual void fade_in(const game_data& data) = 0;

    /**
     * @brief Updates this game when the user is playing it.
     *
     * Keypad should be read only when this method is called.
     *
     * Start key is used for pause, so the game shouldn't read it.
     *
     * @param data Shared data between all games.
     * @return The result of playing this game for a frame.
     */
    [[nodiscard]] virtual game_result play(const game_data& data) = 0;

    /**
     * @brief Indicates if the player beat this game after playing it or not.
     */
    [[nodiscard]] virtual bool victory() const = 0;

    /**
     * @brief Updates this game after playing it.
     *
     * Keypad should not be read when this method is called.
     *
     * @param data Shared data between all games.
     */
    virtual void fade_out(const game_data& data) = 0;

protected:
    game() = default;
};

}

#endif
