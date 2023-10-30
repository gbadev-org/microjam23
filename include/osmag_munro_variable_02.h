/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef MUNRO_VARIABLE_02_H
#define MUNRO_VARIABLE_02_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_osmag_munro_variable_02.h"

namespace osmag{
namespace Police{

constexpr bn::utf8_character munro_variable_02_utf8_characters[] = {
	"é", "è", "à", "ê", "ô", "ç", "û", "î", "ù", "€"
};

constexpr int8_t munro_variable_02_character_widths[] = {
    2,  // 32
    1,  // 33 !
    3,  // 34 "
    5,  // 35 #
    3,  // 36 $
    7,  // 37 %
    6,  // 38 &
    1,  // 39 '
    2,  // 40 (
    2,  // 41 )
    3,  // 42 *
    5,  // 43 +
    1,  // 44 ,
    3,  // 45 -
    1,  // 46 .
    3,  // 47 /
    4,  // 48 0
    2,  // 49 1
    3,  // 50 2
    3,  // 51 3
    4,  // 52 4
    3,  // 53 5
    4,  // 54 6
    3,  // 55 7
    4,  // 56 8
    4,  // 57 9
    1,  // 58 :
    1,  // 59 ;
    5,  // 60 <
    5,  // 61 =
    5,  // 62 >
    3,  // 63 ?
    7,  // 64 @
    4,  // 65 A
    4,  // 66 B
    3,  // 67 C
    4,  // 68 D
    3,  // 69 E
    3,  // 70 F
    4,  // 71 G
    4,  // 72 H
    1,  // 73 I
    2,  // 74 J
    4,  // 75 K
    3,  // 76 L
    5,  // 77 M
    4,  // 78 N
    4,  // 79 O
    4,  // 80 P
    4,  // 81 Q
    4,  // 82 R
    3,  // 83 S
    3,  // 84 T
    4,  // 85 U
    5,  // 86 V
    5,  // 87 W
    5,  // 88 X
    5,  // 89 Y
    3,  // 90 Z
    2,  // 91 [
    3,  // 92 
    2,  // 93 ]
    3,  // 94 ^
    5,  // 95 _
    3,  // 96 `
    4,  // 97 a
    4,  // 98 b
    3,  // 99 c
    4,  // 100 d
    4,  // 101 e
    3,  // 102 f
    4,  // 103 g
    4,  // 104 h
    1,  // 105 i
    3,  // 106 j
    4,  // 107 k
    1,  // 108 l
    7,  // 109 m
    4,  // 110 n
    4,  // 111 o
    4,  // 112 p
    5,  // 113 q
    3,  // 114 r
    3,  // 115 s
    3,  // 116 t
    4,  // 117 u
    5,  // 118 v
    7,  // 119 w
    5,  // 120 x
    4,  // 121 y
    3,  // 122 z
    3,  // 123 {
    2,  // 124 |
    3,  // 125 }
    5,  // 126 ~  
	4,  // é
	4,  // è
	4,  // à
    4,  // ê
    4,  // ô
    3,  // ç	
	4,  // û
	3,  // î
	4,  // ù
	7,  // €
};

constexpr bn::span<const bn::utf8_character> munro_variable_02_utf8_characters_span(
	munro_variable_02_utf8_characters
);

constexpr auto munro_variable_02_utf8_characters_map =
        bn::utf8_characters_map<munro_variable_02_utf8_characters_span>();

constexpr bn::sprite_font munro_variable_02(
        bn::sprite_items::osmag_munro_variable_02,
		munro_variable_02_utf8_characters_map.reference(),
        munro_variable_02_character_widths,
		1
);

}
}

#endif
