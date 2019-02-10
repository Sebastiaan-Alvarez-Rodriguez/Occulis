# Occulis
I had a dream once where I was sitting on the slope of a green mountain, and was looking at grain-ish grass waving in the wind,
so I decided to make my "dream come true" in C++ with modern OpenGL. 

A nice toy to play around with, look at, and calm down. My first attempt at modern OpenGL programming.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 
This project has been made for Linux systems.
See usage to see all the options of this project.

### Prerequisites
The following is required:

* OpenGL 3.3 or later
* Any C++ compiler capable of compiling C++11, e.g. G++ 7 (part of gcc)
* Any builder tool which understands a simple Makefile, e.g. GNU make
* GLEW
* SDL version 1 or up (2 or higher recommended)

### Installing

1. Clone the master branch of this repo
2. Get all prequisites (per example with APT)
3. Move to the cloned repo
4. Type ```make``` to build the project

After following these steps, an application named "Occulis" should be constructed

## Usage
Two things are described here: startup (commandline) commands and in-application controls

### Commands and defaults
```./Occulis ...```
* ```--fps <number>```/```-f <number>```
  Set maximum FPS rate to ```<number>```. Default is ```60,0```
* ```--grass <number>```/```-g```
  Amount of grass blades per patch. Default is ```500.000,0```
* ```--limit```/```-l```
  Toggle of fps limit and counter
* ```--help```/```-h```
  Show these options


### Controls
* Camera
  * Rotations
    * I - up
    * K - down
    * J - left
    * L - right
  * Movements
    * R - up
    * F - down
    * W - forward
    * S - backward
    * A - left
    * D - right
* Atmosphere
  * Y - increase time
  * H - decrease time
* Wind
  * O - increase wind
  * P - decrease wind
* Toggles
  * T - wireframe
  * G - grass
  * Z - daycycles

## Authors

* **Sebastiaan Alvarez Rodriguez** - [Sebastiaan Alvarez Rodriguez](https://github.com/Sebastiaan-Alvarez-Rodriguez)

## Acknowledgments

* Thanks to [Snektron](https://github.com/Snektron) for helping out when needed
* Thanks to [g-truc](https://github.com/g-truc) for the creation of GLM
* Thanks to [nigels-com](https://github.com/nigels-com/) for the creation of GLEW
