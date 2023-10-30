#include "bn_memory.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_sprite_ptr.h"
#include "bn_size.h"
#include "bn_tile.h"
#include "bn_keypad.h"
#include "bn_hdma.h"
#include "bn_blending.h"
#include "bn_sound_items.h"
#include "bn_music_items.h"

#include "mj/mj_game_list.h"

#include "term_terminal_game.h"
#include "term_fixedsys_excelsior_font.h"

#include "bn_regular_bg_items_term_black_notransparency.h"

namespace{
    constexpr bn::string_view code_credits[] = {"tiredsasha"};
    constexpr bn::string_view graphics_credits[] = {"Darien Valentine", "Kirill Pertsev"};
    constexpr bn::string_view sfx_credits[] = {"phoenix1291", "janbezouska", "tempest"};
}

// MJ_GAME_LIST_ADD(terminal_game::game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)


//declaring this here to cut down on the files needed for my microgame
//it's every text prompt. option 0 is always the correct one but it gets
//shuffled during generation
constexpr char term_text_LUT[TERM_MONSTERCOUNT][9][30] = {
{
"A werewolf howls off in the",
"distance... Then he dashes at",
"you, fangs bare, eyes red!",
"  Pull out your revolver and",
"shoot a silver bullet",
"  Pull out your sunglasses",
"and shoot a charming look",
"  Pull out your basketball",
"and shoot a three-pointer"
},{
"Frankenstein's monster stands",
"before you, hideous in form,",
"giant in stature.",
"  Look past his terrible",
"appearance and offer kindness",
"  Try beating up a creature",
"that's twice your size",
"  Try shooting him?",
""
},{
"A vampire materializes before",
"your very eyes! He thirsts",
"for blood on this night.",
"  Drive a wooden stake into",
"his heart, before he attacks!",
"  Warn about the health risks",
"linked to drinking blood",
"  Tell him that you're",
"deathly allergic to bats"
},{
"A horde of zombies lurch",
"towards you, slow and stiff,",
"arms extended, eyes lifeless.",
"  Grab your 12 gauge shotgun",
"and start shooting",
"  Group hug! :D",
"",
"  Convince them to try out a",
"brain-free diet"
},{
"A witch stirs her cauldron",
"with feverish intent, the pox",
"mixture bubbling, boiling.",
"  Uncap your water flask, and",
"then throw the water at her",
"  Ask her if you could have a",
"taste; it smells delicious!",
"  Tell her that you could",
"make better pox in your sleep"
},{
"A ghost floats towards you, a",
"dim white glow emanating from",
"within. Its soul is restless.",
"  Help the ghost sort through",
"whatever it left unfinished",
"  Pretend that you're also a",
"ghost, make an \"oooo\" sound",
"  Open up the Yellow Pages",
"and look for someone to call"
},{
"An ancient, decayed mummy",
"looms before you, eyes bright",
"yellow. He groans in anger.",
"  Throw your burning torch at",
"him and then run!",
"  Yell \"we don't want any!\"",
"and then slam the door",
"  Tell him you think mummy",
"jokes get a bad wrap",
},{
"A skeleton rushes toward you,",
"longsword drawn. Its eyes are",
"flames of burning green.",
"  Hurl your vial of holy",
"water at the animated bones!",
"  Try scaring it! Skeletons",
"scare easily; no guts",
"  Challenge it to a dance-",
"off of epic proportions! >:D"
},{
"A ghoul limps forth, wild",
"anger in its awful face. Its",
"fetid odor makes you sick.",
"  Fling a metal lance through",
"the air, piercing its chest",
"  Give it some makeup, it'll",
"look 5 years less undead",
"  Try selling it some life",
"insurance"
},{
"A hideous merman emerges from",
"the bubbling water, its scaly",
"body covered in slime.",
"  Draw out your sword imbued",
"with electricity, and attack!",
"  Loudly yell at it to go",
"take a shower",
"  Turn on the dehumidifier",
""
},{
"A dark tree towers above you,",
"bent and deformed. It looks",
"at you with piercing hatred.",
"  Light the base of the tree",
"on fire, using your torch",
"  Start decorating it with",
"Christmas lights",
"  Tell it to make like a tree",
"and leave"
},{
"An extraterrestrial descends",
"from its flying saucer. Its",
"large eyes stare down at you.",
"  Pull out your plasma gun",
"and shoot at it!",
"  Offer a friendly hand and",
"take it to your leader",
"  Ask it to move its flying",
"saucer, it's in a bus zone"
},{
"A clown of darkness rides his",
"unicycle, juggling six bloody",
"knives. His suit is rainbow.",
"  He's just a human; use your",
"pistol to shoot him",
"  Tell him to stop clowning",
"around",
"  Walk up to him and honk his",
"nose a few times"
},{
"A demon of unbelievable power",
"bursts forth into your realm,",
"from beyond an evil portal.",
"  Set up a talisman by",
"drawing a pentacle with salt",
"  \"Did it hurt when you fell",
"from heaven?... Wait sorry\"",
"  Jump into the portal",
""
}};

//different colors for us to pull in for use with HDMA. white, gray, green, and red, in that order
constexpr unsigned short term_colorLUT[4][2][512]=
{{{32767,32767,32767,32767,32767,31710,31710,31710,31710,31710,31710,31710,31710,31710,30653,30653,30653,30653,30653,30653,30653,30653,30653,29596,29596,29596,29596,29596,29596,29596,29596,29596,28539,28539,28539,28539,28539,28539,28539,28539,28539,28539,27482,27482,27482,27482,27482,27482,27482,27482,27482,27482,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,27482,27482,27482,27482,27482,27482,27482,27482,27482,27482,28539,28539,28539,28539,28539,28539,28539,28539,28539,28539,29596,29596,29596,29596,29596,29596,29596,29596,29596,30653,30653,30653,30653,30653,30653,30653,30653,30653,31710,31710,31710,31710,31710,31710,31710,31710,31710,32767,32767,32767,32767,32767,32767,32767,32767,32767,31710,31710,31710,31710,31710,31710,31710,31710,31710,30653,30653,30653,30653,30653,30653,30653,30653,30653,29596,29596,29596,29596,29596,29596,29596,29596,29596,28539,28539,28539,28539,28539,28539,28539,28539,28539,28539,27482,27482,27482,27482,27482,27482,27482,27482,27482,27482,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,23254,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,24311,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,25368,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,26425,27482,27482,27482,27482,27482,27482,27482,27482,27482,27482,28539,28539,28539,28539,28539,28539,28539,28539,28539,28539,29596,29596,29596,29596,29596,29596,29596,29596,29596,30653,30653,30653,30653,30653,30653,30653,30653,30653,31710,31710,31710,31710,31710,31710,31710,31710,31710,32767,32767,32767,32767,},{6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,4228,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,5285,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,6342,},},{{15855,15855,15855,15855,15855,15855,15855,15855,15855,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,15855,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,10570,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,11627,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,12684,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,13741,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,14798,15855,15855,15855,15855,15855,15855,15855,15855,},{3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,2114,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,3171,},},{{992,992,992,992,992,960,960,960,960,960,960,960,960,960,928,928,928,928,928,928,928,928,928,896,896,896,896,896,896,896,896,896,864,864,864,864,864,864,864,864,864,864,832,832,832,832,832,832,832,832,832,832,800,800,800,800,800,800,800,800,800,800,800,800,768,768,768,768,768,768,768,768,768,768,768,768,768,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,768,768,768,768,768,768,768,768,768,768,768,768,768,800,800,800,800,800,800,800,800,800,800,800,800,832,832,832,832,832,832,832,832,832,832,864,864,864,864,864,864,864,864,864,864,896,896,896,896,896,896,896,896,896,928,928,928,928,928,928,928,928,928,960,960,960,960,960,960,960,960,960,992,992,992,992,992,992,992,992,992,960,960,960,960,960,960,960,960,960,928,928,928,928,928,928,928,928,928,896,896,896,896,896,896,896,896,896,864,864,864,864,864,864,864,864,864,864,832,832,832,832,832,832,832,832,832,832,800,800,800,800,800,800,800,800,800,800,800,800,768,768,768,768,768,768,768,768,768,768,768,768,768,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,768,768,768,768,768,768,768,768,768,768,768,768,768,800,800,800,800,800,800,800,800,800,800,800,800,832,832,832,832,832,832,832,832,832,832,864,864,864,864,864,864,864,864,864,864,896,896,896,896,896,896,896,896,896,928,928,928,928,928,928,928,928,928,960,960,960,960,960,960,960,960,960,992,992,992,992,},{192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,},},{{31,31,31,31,31,30,30,30,30,30,30,30,30,30,29,29,29,29,29,29,29,29,29,28,28,28,28,28,28,28,28,28,27,27,27,27,27,27,27,27,27,27,26,26,26,26,26,26,26,26,26,26,25,25,25,25,25,25,25,25,25,25,25,25,24,24,24,24,24,24,24,24,24,24,24,24,24,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,30,30,30,30,30,30,30,30,30,29,29,29,29,29,29,29,29,29,28,28,28,28,28,28,28,28,28,27,27,27,27,27,27,27,27,27,27,26,26,26,26,26,26,26,26,26,26,25,25,25,25,25,25,25,25,25,25,25,25,24,24,24,24,24,24,24,24,24,24,24,24,24,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,31,31,31,31,},{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,},},};



namespace terminal_game
{

game::game([[maybe_unused]] int completed_games, [[maybe_unused]] const mj::game_data& data) :
    //_text_sprites(bn::sprite_items::term_sprites.create_sprite(0, 0)), // create the sprite in the initialiser list
	//_sprites(bn::sprite_items::sprite_font()),
	//(bn::sprite_font::sprite_font(_sprites)),
	_text_palette_item(_text_colors, bn::bpp_mode::BPP_4),
	_text_palette(bn::sprite_palette_ptr::create(_text_palette_item)),
	_cursor_text_generator(term_fixedsys_excelsior_sprite_font, _text_palette_item),
	_terminal_text_generator(term_fixedsys_excelsior_sprite_font, _text_palette_item),
	
	_palette_item(_colors, bn::bpp_mode::BPP_8),
    _palette(bn::bg_palette_ptr::create(_palette_item)),
    _tiles(bn::regular_bg_tiles_ptr::allocate(30 * 20 * 2, bn::bpp_mode::BPP_8)),
    _map(bn::regular_bg_map_ptr::allocate(bn::size(32, 32), _tiles, _palette)),
    _bg(bn::regular_bg_ptr::create((256 - 240) / 2, (256 - 160) / 2, _map)),
    _tiles_span(*_tiles.vram()),
    _map_span(*_map.vram()),
	_black_bg(bn::regular_bg_items::term_black_notransparency.create_bg((256 - 240) / 2, (256 - 160) / 2))
	//_total_frames(play_jingle(term_flourescent, completed_games, data))
	//_total_frames(bn::music_item::term_flourescent.it.play_jingle(1))
	//bn::sound_items::term_buzz.play(1);
{
    // Clear all VRAM tiles:
    bn::memory::set_words(0, _tiles_span.size_bytes() / 4, _tiles_span.data());

    //deal with the tilemap
	//here's where we start
    int first_tile_index = _tiles.id() * 32;
	//here's the cell_info we'll be using
    bn::regular_bg_map_cell_info cell_info;
	
	
	//iterate through every tile and make it different
	//for (int i = 0; i < _map_span.size_bytes() / 2; i++) {
	//for (int i = 0; i < _map_span.size_bytes() / 4; i++) {
	for (int y = 0; y < (160 / 8) / TERM_STATIC_H; y++) {
		for (int x = 0; x < 1 + (240 / 8) / TERM_STATIC_W; x++) {
			int counter = 0;
			for (int yy = 0; yy < TERM_STATIC_H; yy++) {
				for (int xx = 0; xx < TERM_STATIC_W; xx++) {
					cell_info.set_tile_index(first_tile_index + counter);
					bn::memory::set_half_words(cell_info.cell(), 1, _map_span.data() + xx + (x * TERM_STATIC_W) + (yy * 32) + (y * TERM_STATIC_H * 32));
					//increment the counter
					counter++;
				}
			}
		}
	}

    // Set the second 8BPP VRAM tile to color 1:
    //bn::memory::set_half_words((1 << 8) + 1, 64 / 2, _tiles_span.data() + 2);

    // Point the second VRAM tilemap cell to the second VRAM tile:
    //cell_info.set_tile_index(first_tile_index + 1);
    //bn::memory::set_half_words(cell_info.cell(), 1, _map_span.data() + 1);

    // You can't get a palette VRAM span, so you have to write the colors in WRAM and commit them to VRAM:	
	//we want a gradient, so we can do a static effect
	//_text_colors.fill(bn::color(0, 0, 31));
	//for (int i = 0; i < 16; i++) {
	//	_text_colors[i] = bn::color(i << 1, i << 1, i << 1);
	//}
	_text_colors[1] = bn::color(31,  0,  0);
	_text_palette.set_colors(_text_palette_item);
	
	//fill in the hdma table with something. this probably won't make
	//any difference because it'll get overwritten in one frame
	for (int i = 0; i < 161; i++) {
		hdma_colors[0][(i<<1)] = 31;
		hdma_colors[1][(i<<1)] = 31;
		hdma_colors[0][(i<<1) + 1] = 6;
		hdma_colors[1][(i<<1) + 1] = 6;
	}	
	
	//set up the background colors
	for (int i = 0; i < TERM_STATIC_COLCOUNT; i++) {
		_colors[i + 1] = bn::color(i, i, i);
	}
	
    _palette.set_colors(_palette_item);
	
	//int p = bn::sprite_palette_ptr::find(_text_palette_item);
	
	palval = reinterpret_cast<uint16_t*>(TERM_TEXT_PAL_MEM);
	
	//terminal_text_generator.set_blending_enabled(true);
	//for (auto it = begin (_text_sprites); it != end (_text_sprites); ++it) {
	//	it.set_blending_enables(true);
	//}
	//turn on blending for the things we want
	//first the sprite text
	for (auto& sprite : _text_sprites)
		sprite.set_blending_enabled(true);
	//also for the cursor text
	for (auto& sprite : _cursor_sprites)
		sprite.set_blending_enabled(true);
	//then the background
    _bg.set_blending_enabled(true);
	_black_bg.set_blending_enabled(true);
	
	//set the priorities
	_bg.set_priority(1);
	for (auto& sprite : _text_sprites)
		sprite.set_bg_priority(2);
	for (auto& sprite : _cursor_sprites)
		sprite.set_bg_priority(2);
	_black_bg.set_priority(3);
	
	//set the alpha
	//it works really nicely if the static is very dark
	//we use weights here and not set_transparency_alpha because we want the
	//text to be at full brightness, the static is just layered on top of that
	bn::blending::set_transparency_weights(0.25, 1);
	
	
	//figure out the order of options
    _current_permutation = data.random.get_int(TERM_PERM_OPTIONS);
	//which is the correct answer?
	for (int i = 0; i < TERM_OPTIONCOUNT; i++) {
		if (_permutations[_current_permutation][i] == 0) {
			_current_correct_answer = i;
			break;
		}
	}
    _current_monster = data.random.get_int(TERM_MONSTERCOUNT);
	
	//set up text!
    _terminal_text_generator.set_left_alignment();
    _cursor_text_generator.set_left_alignment();

    //bn::vector<bn::sprite_ptr, 32> terminal_text_sprites;
	int y_cur = TERM_Y;
	for (int y = 0; y < 3; y++) {
		_terminal_text_generator.generate(TERM_X, y_cur, term_text_LUT[_current_monster][y], _text_sprites);
		y_cur += TERM_Y_SPACING;
	}
	//now go through our three options
	for (int i = 0; i < TERM_OPTIONCOUNT; i++) {
		//what line are we on?
		int cur_line = 3 + (_permutations[_current_permutation][i] * 2);
		
		_terminal_text_generator.generate(TERM_X, y_cur, term_text_LUT[_current_monster][cur_line], _text_sprites);
		y_cur += TERM_Y_SPACING;
		_terminal_text_generator.generate(TERM_X, y_cur, term_text_LUT[_current_monster][cur_line + 1], _text_sprites);
		y_cur += TERM_Y_SPACING;
	}
	
	move_cursor();
	
	//start the "music"
	//bn::sound_items::term_buzz.play(1);
	//bn::sound::play(bn::sound_items::term_buzz);
	play_music(bn::music_items::term_flourescent, 0, data);
	//play(bn::music_items::term_flourescent);
}

void game::fade_in(const mj::game_data& data)
{
	term_anim(data);
	hdma_anim();
}

mj::game_result game::play(const mj::game_data& data)
{	
	//animate!
	term_anim(data);
	
    mj::game_result result;
    result.exit = data.pending_frames == 0;
	
	if(! _victory && ! _defeat)
    {
        if(bn::keypad::a_pressed())
        {
            play_select(data);
			result.remove_title = true;
			if (_current_option == _current_correct_answer) {
				_victory = true;
				bn::sound_items::term_correct.play(TERM_CORRECT_VOLUME);
			} else {
				_defeat = true;
				bn::sound_items::term_incorrect.play(TERM_INCORRECT_VOLUME);
			}
        }

		if (bn::keypad::down_pressed()) {
			_key_delay = 0;
			_key_repeat = 0;
			
			_current_option++;
			if (_current_option >= TERM_OPTIONCOUNT) {
				_current_option = 0;
			}
			move_cursor();
			//bn::sound_items::term_select.play(TERM_SELECT_VOLUME);
            play_option(data);
		} else if (bn::keypad::up_pressed()) {
			_key_delay = 0;
			_key_repeat = 0;
			
			_current_option--;
			if (_current_option < 0) {
				_current_option = TERM_OPTIONCOUNT - 1;
			}
			move_cursor();
			//bn::sound_items::term_select.play(TERM_SELECT_VOLUME);
            play_option(data);
		} else if (bn::keypad::down_held()) {
			if (_key_delay < TERM_KEYDELAY) {
				_key_delay++;
			} else {
				if (_key_repeat == 0) {
					_current_option++;
					if (_current_option >= TERM_OPTIONCOUNT) {
						_current_option = 0;
					}
					move_cursor();
					//bn::sound_items::term_select.play(TERM_SELECT_VOLUME);
                    play_option(data);
				}
				_key_repeat++;
				if (_key_repeat >= TERM_KEYREPEAT) {
					_key_repeat = 0;
				}
			}
		} else if (bn::keypad::up_held()) {
			if (_key_delay < TERM_KEYDELAY) {
				_key_delay++;
			} else {
				if (_key_repeat == 0) {
					_current_option--;
					if (_current_option < 0) {
						_current_option = TERM_OPTIONCOUNT - 1;
					}
					move_cursor();
					//bn::sound_items::term_select.play(TERM_SELECT_VOLUME);
                    play_option(data);
				}
				_key_repeat++;
				if (_key_repeat >= TERM_KEYREPEAT) {
					_key_repeat = 0;
				}
			}
		} 
    }
    else
    {
        if(_show_result_frames)
        {
            --_show_result_frames;
        }
        else
        {
            result.exit = true;
        }
    }
	
	hdma_anim();
    return result;
}

void game::fade_out(const mj::game_data& data)
{
	term_anim(data);
	hdma_anim();
}

//the animation to take place for text and the static
void game::term_anim(const mj::game_data& data) {
	bn::hdma::start(*hdma_colors[current_hdma], 2, *palval);
	
	//do the static effect
	int tile_info[8 * 2];
	const int* s = tile_info;
	int* d = reinterpret_cast<int*>(_tiles_span.data());
	//int icount = (160/32 * 240/32);
	int icount = TERM_STATIC_W * TERM_STATIC_H;
	for (int i = 0; i < icount; i++) {
		for (int j = 0; j < 8 * 2; j += 2) {
            int r = data.random.get();
			char col1 = (r >>  0) & TERM_STATIC_AND;
			char col2 = (r >>  4) & TERM_STATIC_AND;
			char col3 = (r >>  8) & TERM_STATIC_AND;
			char col4 = (r >> 12) & TERM_STATIC_AND;
			tile_info[j] = col1 + (col2 << 8) + (col3 << 16) + (col4 << 24);
			col1 = (r >> 16) & TERM_STATIC_AND;
			col2 = (r >> 20) & TERM_STATIC_AND;
			col3 = (r >> 24) & TERM_STATIC_AND;
			col4 = (r >> 28) & TERM_STATIC_AND;
			tile_info[j + 1] = col1 + (col2 << 8) + (col3 << 16) + (col4 << 24);
		}
		
		bn::memory::copy(*s, 64 / 4, *d);
		
		d += 16;
	}
}

void game::move_cursor() {
	//clear the sprites
	_cursor_sprites.clear();
	//draw it in the new location
	_cursor_text_generator.generate(TERM_X, TERM_Y + ((3 + (_current_option * 2)) * TERM_Y_SPACING), ">", _cursor_sprites);
}

void game::hdma_anim() {
	int track = _hdma_tracker;
	int track_advance = 20;
	int track_wrap = 511;
	int hdma_speed = 2;
	
	//where to start the white
	int options_start = 73 + TERM_Y + (3  * TERM_Y_SPACING);
	int white_start = 73 + TERM_Y + ((3 + (_current_option * 2)) * TERM_Y_SPACING);
	int white_end = white_start + (TERM_Y_SPACING << 1);
	//cycle through each color on the screen, up to white
	for (int i = 0; i < options_start; i++) {
		hdma_colors[current_hdma][(i << 1)] = term_colorLUT[2][0][track];
		hdma_colors[current_hdma][(i << 1) + 1] = term_colorLUT[2][1][track];
		track = (track + track_advance) & track_wrap;
	}
	for (int i = options_start; i < white_start; i++) {
		hdma_colors[current_hdma][(i << 1)] = term_colorLUT[1][0][track];
		hdma_colors[current_hdma][(i << 1) + 1] = term_colorLUT[1][1][track];
		track = (track + track_advance) & track_wrap;
	}
	//do a different color for our current selection, depending on the state
	//if we just won, show green
	if (_victory) {
		for (int i = white_start; i < white_end; i++) {
			hdma_colors[current_hdma][(i << 1)] = term_colorLUT[2][0][track];
			hdma_colors[current_hdma][(i << 1) + 1] = term_colorLUT[2][1][track];
			track = (track + track_advance) & track_wrap;
		}
	//if we just lost, show red
	} else if (_defeat) {
		for (int i = white_start; i < white_end; i++) {
			hdma_colors[current_hdma][(i << 1)] = term_colorLUT[3][0][track];
			hdma_colors[current_hdma][(i << 1) + 1] = term_colorLUT[3][1][track];
			track = (track + track_advance) & track_wrap;
		}
	//otherwise, show white
	} else {
		for (int i = white_start; i < white_end; i++) {
			hdma_colors[current_hdma][(i << 1)] = term_colorLUT[0][0][track];
			hdma_colors[current_hdma][(i << 1) + 1] = term_colorLUT[0][1][track];
			track = (track + track_advance) & track_wrap;
		}
	}
	//then continue with the regular color
	for (int i = white_end; i < 161; i++) {
		hdma_colors[current_hdma][(i << 1)] = term_colorLUT[1][0][track];
		hdma_colors[current_hdma][(i << 1) + 1] = term_colorLUT[1][1][track];
		track = (track + track_advance) & track_wrap;
	}
	//flip the 1st bit
	current_hdma ^= 1;
	
	//advance the hdma tracker, for the next frame
	_hdma_tracker += hdma_speed;
	_hdma_tracker = _hdma_tracker & track_wrap;
}

//make it turn black when we pause the game, so it can't be seen
void game::on_pause_start([[maybe_unused]] const mj::game_data& data) {
	for (int i = 0; i < 160; i++) {
		hdma_colors[current_hdma][(i << 1)] = 0;
		hdma_colors[current_hdma][(i << 1) + 1] = 0;
	}
	bn::hdma::start(*hdma_colors[current_hdma], 2, *palval);
	//flip the 1st bit
	//current_hdma ^= 1;
}

void game::play_option(const mj::game_data& data) {
    int option = data.random.get_int(9);
	switch(option) {
		case 0:
			bn::sound_items::term_keyboard00.play(TERM_CLICK_VOLUME);
			break;
		case 1:
			bn::sound_items::term_keyboard03.play(TERM_CLICK_VOLUME);
			break;
		case 2:
			bn::sound_items::term_keyboard04.play(TERM_CLICK_VOLUME);
			break;
		case 3:
			bn::sound_items::term_keyboard05.play(TERM_CLICK_VOLUME);
			break;
		case 4:
			bn::sound_items::term_keyboard06.play(TERM_CLICK_VOLUME);
			break;
		case 5:
			bn::sound_items::term_keyboard07.play(TERM_CLICK_VOLUME);
			break;
		case 6:
			bn::sound_items::term_keyboard08.play(TERM_CLICK_VOLUME);
			break;
		case 7:
			bn::sound_items::term_keyboard09.play(TERM_CLICK_VOLUME);
			break;
		case 8:
			bn::sound_items::term_keyboard10.play(TERM_CLICK_VOLUME);
			break;
		default:
			break;
	}
}

void game::play_select(const mj::game_data& data) {
    int option = data.random.get_int(2);
	switch(option) {
		case 0:
			bn::sound_items::term_keyboard01.play(TERM_CLICK_VOLUME);
			break;
		case 1:
			bn::sound_items::term_keyboard02.play(TERM_CLICK_VOLUME);
			break;
		default:
			break;
	}
}

}
