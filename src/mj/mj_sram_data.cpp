#include "mj_sram_data.h"

#include "bn_sram.h"
#include "bn_string_view.h"

namespace mj
{

namespace
{
    constexpr bn::string_view valid_label = "mj002";
}

void sram_data::init()
{
    sram_data copy;
    bn::sram::read(copy);

    if(bn::string_view(copy._label) == valid_label)
    {
        *this = copy;
    }
    else
    {
        bn::istring_base label_istring(_label);
        bn::ostringstream label_stream(label_istring);
        label_stream.append(valid_label);

        bn::sram::clear(bn::sram::size());
        write();
    }
}

void sram_data::add_high_score(int high_score)
{
    BN_ASSERT(high_score >= 0, "Invalid high score: ", high_score);

    for(int& old_high_score : _high_scores)
    {
        if(high_score > old_high_score)
        {
            bn::swap(high_score, old_high_score);
        }
        else if(high_score == old_high_score)
        {
            return;
        }
    }
}

void sram_data::write()
{
    bn::sram::write(*this);
}

}
