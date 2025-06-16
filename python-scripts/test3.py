# Import the necessary libraries
import concept_forge as cf
from math import sin
import time

from concept_forge import Vec3
from concept_forge.primitives import Cube

# Initialize the ConceptForge instance
forge = cf.ConceptForge()
forge.hierarchy.entities.clear()


def create_static_bug_cube():
    pos = Vec3(0, 0, 0)
    rot = Vec3(0, 0, 0)
    scale = Vec3(1, 1, 1)
    cube = Cube.new(forge, pos, rot, scale)
    return cube


def create_small_cubes():
    cubes = []
    for i in range(10):
        pos = Vec3(i * 2, sin(i), i * 2)
        rot = Vec3(0, 0, 0)
        scale = Vec3(0.1, 0.1, 0.1)
        cube = Cube.new(forge, pos, rot, scale)
        cubes.append(cube)
    return cubes


def update():
    # Update small cubes
    global static_bug_cube, small_cubes
    for i, cube in enumerate(small_cubes):
        rotation = cube.get_rotation()

        if rotation.x < -0.5:
            pos = Vec3(i * 5, 10 * sin(i), i * 5)
            rot = Vec3(0, 0, 0)
            scale = Vec3(0.1, 0.1, 0.1)
            small_cubes[i] = Cube.new(forge, pos, rot, scale)
        elif rotation.x > 0.5:
            pos = Vec3(i * 5, -10 * sin(i), i * 5)
            rot = Vec3(0, 0, 0)
            scale = Vec3(0.1, 0.1, 0.1)
            small_cubes[i] = Cube.new(forge, pos, rot, scale)

    # Update static bug cube
    static_bug_cube.rotate(sin(time.time()) * 45, Vec3(0, 1, 0))


static_bug_cube = create_static_bug_cube()
small_cubes = create_small_cubes()

while not forge.window_should_close():
    dt = forge.dt()
    forge.calc_projection()

    update()

    forge.gui_management()
    forge.process_input()
    forge.render()
