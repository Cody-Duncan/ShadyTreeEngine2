ShadyTreeEngine 2
===============

Author: Cody Duncan  

A rudimentary game engine for CS 529 course. Much of what was 
learned in my independent study at Northwest Missouri State University (see ShadyTreeEngine)
has been used and greatly expanded upon for this project.

###Engine Features: 

1. Entity-Component Architecture - GameObjects are an aggregation of components.
2. Dynamic Component Allocation - Custom components can be added to the component memory management from outside the engine code.
3. 2D game engine - Can draw sprites, text, primitives.
4. Data Driven Design - Level layout and player/enemy archetypes are defined and loaded from json format files.
5. Basic Physics - Simple physics system to handle objects bumping and standing on level geometry.
6. Input - takes and stores keyboard and mouse state.
7. Math - using the SimpleMath library from the DirectX toolkit. Functions for collision detection between circles/bounding boxes/lines/points.
8. Diagnostics - Mechanisms for outputting errors to console or file, instrumentation for measuring runtime performance, and mechanisms for assertion and error checking during runtime.
9. Resources - Can scan a directory for asset files, and then load them into resource allocators using only the filename (no extension, path, etc). Uses tinydir to support this feature.


###The Game: Super Smash Box

######Description:  
Box fighting arena.  
Try to knock the other boxes off the screen.  

######Controls:  
Move with arrow keys.  
Attack by pressing Q and a direction.  

######Objective:  
Pickup powerups for special abilties or effects.  
Knock the other boxes off to win.  
Get knocked off 3 times to lose.  

