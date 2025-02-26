Flare README

Homepage  http://clintbellanger.net/rpg/
Source    http://code.google.com/p/flare-engine/
Forums    http://opengameart.org/forums/projects/flare

=== DEPENDENCIES ===

To build flare you need the 1.2 Development Libraries for SDL, SDL_image, and SDL_mixer.
To run flare you need the equivalent 1.2 Runtime Libraries.

http://www.libsdl.org/download-1.2.php

Here's an example of how to install the dependencies for Ubuntu:

sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev

Additionally, for easy building I recommend using cmake and make.


=== BUILDING WITH CMAKE ===

To build flare, go to the build folder (cd build) and run the following commands:

cmake .
make

Once you're done, return to this folder and you'll see the flare executable.


=== or, BUILDING WITH g++ ===

If you prefer building directly with C++, the command will be something like this.

Windows plus MinGW (depending on where your SDL dev headers are)
g++ -I C:\MinGW\include\SDL src\*.cpp -o flare.exe -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer

Linux (depending on where your SDL includes are)
g++ -I /usr/include/SDL src/*.cpp -o flare -lSDL -lSDL_image -lSDL_mixer


=== RUNNING FLARE ===

Running flare from command line is straightforward in Linux:
./flare

If you're running flare from your operating system's gui file browser (e.g. Windows Explorer or OSX Finder), you'll want to use one of the provided launchers.  This helps the flare executable use its own working directory, so it can see all those data folders.


=== FULLSCREEN ===

If flare works in windowed mode, it should be safe to run fullscreen.  To run flare in fullscreen mode, edit config/settings.txt and set
fullscreen=1

=== SAVE FILES ===

Make sure you have read and write access to the "saves" folder.  Currently the game is automatically saved when you exit.
