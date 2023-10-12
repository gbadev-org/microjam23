#ifndef MJ_BIG_SPRITE_FONT_H
#define MJ_BIG_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"

#include "bn_sprite_items_mj_font.h"
#include "bn_sprite_tiles_items_mj_big_font.h"

namespace mj
{

constexpr bn::utf8_character big_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr int8_t big_sprite_font_character_widths[] = {
    10,  // 32
    7,  // 33 !
    7,  // 34 "
    8,  // 35 #
    8,  // 36 $
    7,  // 37 %
    8,  // 38 &
    4,  // 39 '
    8,  // 40 (
    8,  // 41 )
    7,  // 42 *
    7,  // 43 +
    4,  // 44 ,
    7,  // 45 -
    4,  // 46 .
    7,  // 47 /
    11,  // 48 0
    9,  // 49 1
    9,  // 50 2
    10,  // 51 3
    10,  // 52 4
    10,  // 53 5
    10,  // 54 6
    10,  // 55 7
    10,  // 56 8
    10,  // 57 9
    4,  // 58 :
    4,  // 59 ;
    6,  // 60 <
    7,  // 61 =
    6,  // 62 >
    7,  // 63 ?
    8,  // 64 @
    14,  // 65 A
    10,  // 66 B
    12,  // 67 C
    11,  // 68 D
    10,  // 69 E
    10,  // 70 F
    11,  // 71 G
    10,  // 72 H
    6,  // 73 I
    9,  // 74 J
    11,  // 75 K
    9,  // 76 L
    12,  // 77 M
    11,  // 78 N
    13,  // 79 O
    10,  // 80 P
    14,  // 81 Q
    11,  // 82 R
    10,  // 83 S
    12,  // 84 T
    11,  // 85 U
    12,  // 86 V
    16,  // 87 W
    12,  // 88 X
    12,  // 89 Y
    11,  // 90 Z
    5,  // 91 [
    7,  // 92
    5,  // 93 ]
    4,  // 94 ^
    7,  // 95 _
    4,  // 96 `
    14,  // 97 a
    10,  // 98 b
    12,  // 99 c
    11,  // 100 d
    10,  // 101 e
    10,  // 102 f
    11,  // 103 g
    10,  // 104 h
    6,   // 105 i
    9,   // 106 j
    11,  // 107 k
    9,   // 108 l
    12,  // 109 m
    11,  // 110 n
    13,  // 111 o
    10,  // 112 p
    14,  // 113 q
    11,  // 114 r
    10,  // 115 s
    12,  // 116 t
    11,  // 117 u
    12,  // 118 v
    16,  // 119 w
    12,  // 120 x
    12,  // 121 y
    11,  // 122 z
    6,  // 123 {
    3,  // 124 |
    6,  // 125 }
    8,  // 126 ~
    7,  // Á
    7,  // É
    5,  // Í
    7,  // Ó
    7,  // Ú
    7,  // Ü
    7,  // Ñ
    7,  // á
    7,  // é
    4,  // í
    7,  // ó
    7,  // ú
    7,  // ü
    7,  // ñ
    3,  // ¡
    7,  // ¿
};

constexpr bn::span<const bn::utf8_character> big_sprite_font_utf8_characters_span(big_sprite_font_utf8_characters);

constexpr auto big_sprite_font_utf8_characters_map = bn::utf8_characters_map<big_sprite_font_utf8_characters_span>();

constexpr bn::sprite_font big_sprite_font(
        bn::sprite_item(bn::sprite_tiles_items::mj_big_font_shape_size,
                        bn::sprite_tiles_items::mj_big_font, bn::sprite_items::mj_font.palette_item()),
        big_sprite_font_utf8_characters_map.reference(), big_sprite_font_character_widths);

}

#endif
