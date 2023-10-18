#ifndef MJ_SRAM_DATA_H
#define MJ_SRAM_DATA_H

#include "bn_span_fwd.h"

namespace mj
{

class sram_data
{

public:
    void init();

    [[nodiscard]] bn::span<const int> high_scores() const;

    void add_high_score(int high_score);

    void write();

private:
    char _label[8] = {};
    int _high_scores[3] = {};
    char _padding[1024 - 8 - 12] = {};
};

}

#endif
