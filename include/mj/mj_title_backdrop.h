#ifndef MJ_TITLE_BACKDROP_H
#define MJ_TITLE_BACKDROP_H

#include "bn_fixed_fwd.h"
#include "bn_optional_fwd.h"

namespace mj
{

class title_backdrop
{

public:
    static constexpr int frames_per_bg_update = 3;
    static constexpr int fade_frames = 22 * frames_per_bg_update;

    virtual ~title_backdrop() = default;

    [[nodiscard]] virtual bn::optional<bn::fixed> update() = 0;

protected:
    title_backdrop() =  default;
};

}

#endif
