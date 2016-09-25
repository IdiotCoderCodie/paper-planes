Paper Planes ACW
----------------
This repository contains the source code and visual studio solution for the "Paper Planes" Coursework.
Completed in my 4th Year. It contributed towards a C++ Programming module and a Real Time Graphics module, this was the module I learned DirectX in. 


About
-----
The application runs a basic 3D real-time scene using DirectX for graphics. 
It showcases a simple "Entity/Component" engine, where everything within the scene is an "Entity" and it's function/behaviour is defined by it's "Components".
The actual scene itself is very simple and was created to the ACW Specification. It is a cube "world" with an open side facing the camera, there are 5 airports on the 5 sides, with numerous paper planes on set flight paths. The flight paths use Bezier curves and are displayed in the scene as white lines.
Some normal mapping can be seen too, a studded couch type normal map was used to better show off the effect.
There are a total of 4 spotlights in the scene which cast shadows using a single shadow map each.

Build
-----
Please build using Microsoft Visual Studio 2015. Build tested with Community Edition. 

Run
---
Run through Visual Studio.
OR
Run from command line:
	- Navigate to the PaperPlanes project directory: paper-planes\PaperPlanes\PaperPlanes
	- Run ..\Debug\PaperPlanes.exe


Window Size / Options
---------------------
Window Height, Width and Fullscreen can be edited in paper-planes/PaperPlanes/PaperPlanes/window_config.txt


Controls
--------
Application starts out on MAIN CAMERA
MAIN CAMERA
Switch to Main Camera ........ F1 Key
Look Up ...................... Up Arrow
Look Down .................... Down Arrow
Look Left .................... Left Arrow
Look Right ................... Right Arrow
Move Forward ................. LCtrl+Up Arrow
Move Back .................... LCtrl+Down Arrow
Move Left .................... LCtrl+Left Arrow
Move Right ................... LCtrl+Right Arrow
Move Up ...................... LCtrl+Page Up
Move Down .................... LCtrl+Page Down

AIRFIELD CAMERA
Switch to Airfield Camera .... F2 Key
(Cannot Move/Look Around)

FOLLOW CAMERA
Switch to Follow Camera ...... F3 Key
Switch planes to Follow ...... F4 Key
(Cannot Move/Look Around)

GENERAL
Pause Animation .............. P
Restart Animation ............ R
Exit Application ............. ESC

Third Party
-----------
- GLM - Used for maths - glm.g-truc.net
- AntTweakBar - UI - anttweakbar.sourceforge.net
- DirectXTK - Used only for loading textures from file - https://directxtk.codeplex.com/


