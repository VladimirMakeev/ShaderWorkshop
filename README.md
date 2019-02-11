# ShaderWorkshop [![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)

![ShaderWorkshop UI](https://github.com/VladimirMakeev/ShaderWorkshop-examples/blob/master/DefaultShader/screenshot.png)

Inspired by [ShaderToy](https://www.shadertoy.com), made with [Qt](https://www.qt.io).

## Requirements
ShaderWorkshop requires Qt 5.7+ for build.
At runtime, any graphics card capable of running OpenGL 3.3 core profile context
is enough.

## Usage
Open and save '.frag' or '.txt' GLSL fragment shader files using File menu.
As in ShaderToy, you can add/remove buffers and change connections and sampling
parameters between them. Shaders in each buffer must be recompiled separately
for changes to take effect.

## Examples
[Soft shadows](https://github.com/VladimirMakeev/ShaderWorkshop-examples/blob/master/SoftShadowTest/soft_shadow.frag):

![Soft shadow screenshot](https://github.com/VladimirMakeev/ShaderWorkshop-examples/blob/master/SoftShadowTest/screenshot.png)
