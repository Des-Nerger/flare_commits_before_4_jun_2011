README for OSARE (Open Source Action Roleplaying Engine) by Clint Bellanger
version 0.04

What you need
- C++ compiler
- Libraries for your operating system: SDL, SDL_image, SDL_mixer


Compile Notes by platform

OSX 
- Compiled using XCode on Leopard. Setup help: http://www.meandmark.com/sdlopenglpart2.html
- SDLMain.h and SDLMain.m used for Cocoa wrappings
- Game maps/art is placed in the .app's Resources folder
- SDL, SDL_image, and SDL_mixer go in the .app's Frameworks folder

Win
- You MUST remove SDLMain.h and SDLMain.m (those SDLMain files are only for OSX)
- SDL.dll, SDL_image.dll, SDL_mixer.dll and dependencies are placed with the .exe file (or a PATH folder)
- As you see in the command below, I put the SDL header files in the MinGW include folder
- I use the following build.bat to compile (run this in the src folder)
g++ -I C:\MinGW\include\SDL *.cpp -o rpgdemo.exe -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer
- Move rpgdemo.exe to the resources folder and run it from there


Linux
- You MUST remove SDLMain.h and SDLMain.m (those SDLMain files are only for OSX)
- SDL, SDL_image, and SDL_mixer libraries are required (probably available through your regular package manager)
- The following command will compile the game on most systems (run this in the src folder)
g++ -I /usr/include/SDL *.cpp -o rpgdemo -lSDL -lSDL_image -lSDL_mixer
- Move rpgdemo to the resources folder and run it from there
