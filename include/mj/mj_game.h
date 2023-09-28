#ifndef MJ_GAME_H
#define MJ_GAME_H

#include "bn_string.h"

#include "mj_game_data.h"
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
