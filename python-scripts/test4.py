import concept_forge as cf
from math import sin, cos, pi
import time
import random

from concept_forge import Entity, Vec3, ShaderType
from concept_forge.primitives import Cube, UVSphere

forge = cf.ConceptForge()

# Create a larger grid of cubes for terrain
cubes = []
grid_size = 20  # Much bigger grid
spacing = 0.5  # Space between cubes

for i in range(grid_size):
    for j in range(grid_size):
        # Calculate position for grid
        x = (i - grid_size / 2 + 0.5) * spacing
        z = (j - grid_size / 2 + 0.5) * spacing

        pos = Vec3(x, 0, z)
        rot = Vec3(0, 0, 0)
        scale = Vec3(0.5, 0.5, 0.5)  # Make cubes smaller
        cube = Cube.new(forge, pos, rot, scale)
        cubes.append(cube)

while not forge.window_should_close():
    dt = forge.dt()
    forge.calc_projection()

    # Update each cube's position with a Bees and Bombs style wave
    for idx, cube in enumerate(cubes):
        i = idx // grid_size
        j = idx % grid_size

        # Calculate wave parameters
        t = time.time() * 2.0  # Slower overall speed

        # Create a more complex wave pattern
        wave1 = sin(t + i * 0.5) * cos(t + j * 0.5)
        wave2 = sin(t * 0.7 + j * 0.3) * cos(t * 0.7 + i * 0.3)

        # Combine waves for more organic motion
        y_pos = (wave1 + wave2) * 1.1

        # Keep original x and z positions
        cube.set_position(Vec3(cube.get_position().x, y_pos, cube.get_position().z))

    forge.gui_management()
    forge.process_input()
    forge.render()
