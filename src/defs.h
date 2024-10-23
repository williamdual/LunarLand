#ifndef DEFS_H
#define DEFS_H

// easily toggle off the debug messages in the entire library
#define DEBUG true

// add some of your own constants here :)
enum EntityType
{
    game_obj,
    player,
    enemy,
    powerup_s,
    beacon,
    bullet
};
enum GameState
{
    ongoing,
    won,
    lost
};
#endif // DEFS_H