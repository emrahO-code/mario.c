# Super Mario Bros Clone

A simple recreation of World 1-1 from Super Mario Bros, built in C using Raylib.

## Screenshots

*Basic gameplay showing Mario, enemies, and level elements*

*Mario collecting items and changing forms*

*Reaching the flagpole to win*

## What's Working

- **Mario Movement**: Walk, jump, and basic physics
- **Three Mario Forms**: Small → Super → Fire Mario
- **Enemies**: Goombas that walk around and can be defeated
- **Items**: Mushrooms, stars, and fire flowers spawn from blocks
- **Level**: Recreation of World 1-1 with pipes, blocks, and pits
- **Collision**: Mario can hit blocks, land on platforms, and interact with everything
- **Game Loop**: Die and restart, reach flagpole to win

## Controls

- **A/D**: Move left/right
- **W**: Jump
- **ESC**: Quit

## How It Works

The game is split into different systems:
- `player.c` - Mario's movement and states
- `enemy.c` - Goomba AI and spawning
- `item.c` - Items that come out of blocks
- `level.c` - The level layout and blocks
- `collision.c` - Handles all the collision detection
- `main.c` - Puts it all together

## Building

Need CMake and a C compiler:

```bash
mkdir build && cd build
cmake ..
make
cd ..
./mario
```

## What I Learned

- C programming and managing a larger project
- Game physics and collision detection
- Sprite rendering and animation
- How to structure game code so it doesn't become a mess
- Working with Raylib for graphics

## Future Ideas

- More levels
- Sound effects
- Better animations
- More enemy types
- Actual fireball shooting for Fire Mario

---

Just a fun project to learn C and game development. Mario belongs to Nintendo, this is just for learning!