# Computer Graphics

Coursework for Computer Graphics course DVA304 at Mälardalens Högskola.

Requires OpenGL 4.3+, freeGLUT and GLEW.

## About

The assignments are about rasterization. They include: perspective and
orthogonal projections, interactive camera, view frustum culling, Gouraud
shading, Phong shading and cartoon shading.

The final assignment is about ray tracing. It includes: spheres, planes,
many light sources with Phong lighting, reflections and glossy reflections,
shadows and soft shadows, transparent materials, anti-aliasing, 2D texture
mapping from BMP images for the spheres and multi-threaded rendering process
using POSIX threads.

## Use

For the assignments:

```
$ cd assignments
$ make
$ ./computergraphics
```

For the final assignment

```
$ cd finalassignment
$ make
$ ./raytracer
```

To run it on my Arch Linux installation with Bumblebee, I do

```
$ optirun ./computergraphics
```
