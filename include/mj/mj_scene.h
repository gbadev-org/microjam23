#ifndef MJ_SCENE_H
#define MJ_SCENE_H

#include "bn_optional_fwd.h"

namespace mj
{

enum class scene_type : uint8_t;

class scene
{

public:
    virtual ~scene() = default;

    [[nodiscard]] virtual bn::optional<scene_type> update() = 0;

protected:
    scene() = default;
};

}

#endif
