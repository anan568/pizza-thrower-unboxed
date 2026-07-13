# C++ Setup Guide
## After cloning this repository, make sure you have the following installed:
1. MSVC <- to compile c++ (for windows)
(google it!!! you may already have it installed!!!)
2. python <- to use scons
(also google it)
3. Scons <- to use the compiler scripts that godot uses...
`pip install scons`

## Then, go into the `convenience files` directory...
and run `update_godot_cpp.bat`,
- (this SHOULD update the `godot-cpp` folder),

then `compile_godot_bindings_debug.bat`,
- (this will compile .cpp files in the `godot-cpp` folder)

then `build_cpp_debug.bat` <- run this file again if you make changes to any cpp files!!
- (this will compile .cpp files in the `cpp` folder and place the output files in `/bin`.)

is this like our todo list?

todo:
- come up with arcade games (fun fun)
- need a falling animation for pj surf
- polish (AS IN POLAND) animations (fix the cap)


does this mean i have to comment my code now :(

yes

bob: figure out how to write a pixel shader
option1 : vertex shader hijack: utilise "uwidth" and "uheight" uniform variables and quantise vertices to nearest nice integer. before passing to rasterisation stage. this will probably not work, but will be very funny to see the result of.

Space invader thing but we shoot enemy doges in ufos w

Game where u control a mace-like (but pizza-like) ball and u can throw it and shit with the mouse button i had this idea quite long ago https://youtu.be/OVxBh8vCGeE

game... make pizza... ingredients.... absurdity in steps needed to acquire ingredients.... hmm... idea generator 4000 needed...

pizza delivery

precision platformer where you have a surf attack and you have to time your surfs mid air to surf into enemies for movement boost paint paint paint. gauntlet attack launches you upward when you hit an enemy. make the game really hard in later levels with chaining surfs and gauntlets
- levels start out with pj on foot then he jumps on a surfboard
- jump to dismount surfboard (but the board will keep going and can be remounted when hopped into -> allows for levels with multiple surfboards)
- air dash action in the air to reverse horizontal velocity (this is your only way to change direction in the game)
- make gauntlet a chargeable attack with like 3 levels of power indicating how high he will jump (tap for shortest jump)
- sidescroller course with a moving paint wave
- camera zooms out the faster u go