# microjam23

GBA Microjam '23


## Microgame rules

Microgames must follow these rules in order to play along with the animations shown between them and with the pause menu:

* All sprites and backgrounds must have a priority greater than 0 (by default, sprites and backgrounds priority is 3).
* The system sprite color palette (used for rendering text, among other things) must not be changed.
* The pumpkin is rendered using a regular background, so microgames can't show two affine backgrounds or four regular backgrounds when the pumpkin is visible (it is visible when a microgame is created and when the `mj::game::fade_in` and `mj::game::fade_out` methods are called). However, you can use all available backgrounds when `mj::game::play` is called.
* Global grayscale functions like [bn::bg_palettes::set_grayscale_intensity](https://gvaliente.github.io/butano/namespacebn_1_1bg__palettes.html#ac18a981b7cf0220b1ca15b2f9fb5e019) and [bn::sprite_palettes::set_grayscale_intensity](https://gvaliente.github.io/butano/namespacebn_1_1sprite__palettes.html#ad793f7d303292c99e754d2c9715b7fe9) must be avoided, since they're used for the pause menu. You can apply grayscale effects to individual color palettes though.
