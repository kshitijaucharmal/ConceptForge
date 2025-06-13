# Import the necessary libraries
import concept_forge as cf
from math import sin

from concept_forge import Entity
from concept_forge import Vec3
from concept_forge import Cube
from concept_forge import Vec3

# Initialize the ConceptForge instance
forge = cf.ConceptForge()
forge.entities.clear()


def create_static_bug_cube():
    forge.add_cube(position=Vec3(0, 0, 0), rotation=Vec3(0, 0, 0), scale=Vec3(1, 1, 1))
    return forge.entities[-1]


def create_small_cubes():
    for i in range(10):
        cube = forge.add_cube(
            position=Vec3(i * 2, sin(i), i * 2),
            rotation=Vec3(0, 0, 0),
            scale=Vec3(0.1, 0.1, 0.1),
        )
    return forge.entities[-10:]


def update():
    # Update small cubes
    global static_bug_cube, small_cubes
    for i, cube in enumerate(small_cubes):
        rotation = cube.get_rotation()

        if rotation.x < -0.5:
            small_cubes[i] = forge.add_cube(
                position=Vec3(i * 5, 10 * sin(i), i * 5),
                rotation=Vec3(0, 0, 0),
                scale=Vec3(0.1, 0.1, 0.1),
            )
        elif rotation.x > 0.5:
            small_cubes[i] = forge.add_cube(
                position=Vec3(i * 5, -10 * sin(i), i * 5),
                rotation=Vec3(0, 0, 0),
                scale=Vec3(0.1, 0.1, 0.1),
            )

    # Update static bug cube
    static_bug_cube.rotate(delta=Vec3(0, sin(forge.deltaTime), 0))


static_bug_cube = create_static_bug_cube()
small_cubes = create_small_cubes()

while not forge.window_should_close():
    forge.calc_delta_time()
    forge.calc_projection()

    update()

    forge.gui_management()
    forge.process_input()
    forge.render()
