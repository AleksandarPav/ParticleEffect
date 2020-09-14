# ParticleEffect
 
The project simulates a particle fire effect. Run from main.cpp.
Created 3 classes: Particle, Swarm and Screen.
Used SDL API for the graphics output.
Initialized every pixel with random direction and speed, which I updated in every frame.
After random intervals, pixels are initialized again and again.
Color changing done with sinusoidal function.
Implemented blurring filter, called every frame to blur pixels from previous frames.