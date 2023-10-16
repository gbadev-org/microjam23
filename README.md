# microjam23

[GBA Microjam '23](https://itch.io/jam/gba-microjam-23)


## How to add a new microgame

You can add a new microgame by duplicating the test microgame code files (`tmg_test_game.h` and `tmg_test_game.cpp`) and changing the namespace to a new one.

For example, if your new microgame is going to use the `xyz` namespace, you can duplicate the mentioned code files with `xyz_game.h` and `xyz_game.cpp` as filenames.

Remember to change the namespace in the `MJ_GAME_LIST_ADD` macro too.


## Microgame rules

Microgames must follow these rules in order to play along with other microgames, with the animations shown between them and with the pause menu:

* All code used by a microgame should be placed in its own namespace. For example, the test microgame uses the `tmg` namespace.
* All code and asset files (graphics, music, etc.) used by a microgame should have the code namespace as filename prefix. For example, the test microgame uses `tmg_` as filename prefix.
* All microgames must include a text file per asset type indicating the license of all used assets. For example, the test microgame uses `tmg_credits.txt` as credits filename.
* The start key is used to show and hide the pause menu, so microgames should not use it.
* Butano core functions such as [bn::core::update](https://gvaliente.github.io/butano/namespacebn_1_1core.html#aa849bfe482eb6964e3a29f9eba78f7b6) are handled outside of the microgames, so they should not call them.
* All sprites and backgrounds must have a priority greater than 0 (by default, sprites and backgrounds priority is 3).
* The system sprite color palette (used for rendering text, among other things) must not be changed.
* The pumpkin is rendered using a regular background, so microgames can't show two affine backgrounds or four regular backgrounds when the pumpkin is visible (it is visible when a microgame is created and when the `mj::game::fade_in` and `mj::game::fade_out` methods are called). However, you can use all available backgrounds when `mj::game::play` is called.
* Global palette functions like [bn::bg_palettes::set_grayscale_intensity](https://gvaliente.github.io/butano/namespacebn_1_1bg__palettes.html#ac18a981b7cf0220b1ca15b2f9fb5e019) and [bn::sprite_palettes::set_grayscale_intensity](https://gvaliente.github.io/butano/namespacebn_1_1sprite__palettes.html#ad793f7d303292c99e754d2c9715b7fe9) must be avoided, since they're used for the transition between microgames and the pause menu. You can apply palette effects to individual color palettes though.
* SRAM will be handled outside of the microgames, so they should not write it.


## Frequently asked questions (FAQ)

### How can I disable the test microgame or other microgames?
You can disable a microgame by commenting out the `MJ_GAME_LIST_ADD` macro call.

### Where's the microgame interface documentation?
For now it's in the header file (`mj_game.h`).
