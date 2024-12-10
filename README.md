# LunarLand
COMP 3501 - FinalProject
William Graham, 101228346
Mason McLeod, 101229241
Jacob Bonner, 101234823

How to Build:
1. Navigate to the LunarLand directory (this should be the main directory)
2. Command: scons platform=windows

Game Architecture:
- The game is third person, with cameras strategically placed around the map to view the player. They
  either pan across the screen to follow the player down long corridors, track the player so they
  remain in the center of the screen, or are stationary with the player in frame. Whenever the player
  enters a new area, the camera will change.
- The object of the game is to find three items. One item can be found in each of the three areas that
  branches out from the center room (the room that is reached by entering the building when the game 
  starts). The puzzles or methods to easily find the items are further detailed in the project report.
- Parts or the whole of an item that is meant to be interacted with glow blue when the player comes
  within a certain radius of them.

Controls:
- W: Move Forward
- A: Move Left
- S: Move Right
- D: Move Backwards
- Left Click: Interact
- Esc: Pause/Unpause

Bugs:
- There are certain areas in the geometry of the map where the player can get stuck. In order to
  prevent this, the player may randomly "hop" to get out of these holes.