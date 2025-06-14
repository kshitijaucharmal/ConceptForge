import concept_forge as cf
from math import sin, cos, pi
import time
import random

from concept_forge import Entity, Vec3
from concept_forge.primitives import Cube, UVSphere

forge = cf.ConceptForge()
forge.entities.clear()

# Create a larger grid of cubes for terrain
cubes = []
grid_size = 20  # Much bigger grid
spacing = .6  # Space between cubes

for i in range(grid_size):
    for j in range(grid_size):
        # Calculate position for grid
        x = (i - grid_size/2 + 0.5) * spacing
        z = (j - grid_size/2 + 0.5) * spacing
        
        pos = Vec3(x, 0, z)
        rot = Vec3(0, 0, 0)
        scale = Vec3(0.5, 0.5, 0.5)  # Make cubes smaller
        cube = Cube.new(forge, pos, rot, scale)
        cubes.append(cube)

# Create bee-like spheres
spheres = []
velocities = []  # Separate list to store velocities
num_spheres = 12  # More spheres
for i in range(num_spheres):
    # Create sphere with random initial position
    x = random.uniform(-5, 5)
    y = random.uniform(1, 3)
    z = random.uniform(-5, 5)
    pos = Vec3(x, y, z)
    rot = Vec3(0, 0, 0)
    scale = Vec3(0.2, 0.2, 0.2)  # Even smaller spheres
    sphere = UVSphere.new(forge, pos, rot, scale)
    spheres.append(sphere)
    
    # Store initial velocity
    velocities.append(Vec3(
        random.uniform(-0.5, 0.5),
        random.uniform(-0.2, 0.2),
        random.uniform(-0.5, 0.5)
    ))

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
        y_pos = (wave1 + wave2) * 0.5
        
        # Keep original x and z positions
        cube.set_position(Vec3(cube.get_position().x, y_pos, cube.get_position().z))

    # Update sphere positions with bee-like movement
    t = time.time()
    for i, sphere in enumerate(spheres):
        # Get current position and velocity
        pos = sphere.get_position()
        vel = velocities[i]
        
        # Add some random movement to velocity
        vel.x += random.uniform(-0.1, 0.1)
        vel.y += random.uniform(-0.05, 0.05)
        vel.z += random.uniform(-0.1, 0.1)
        
        # Dampen velocity to prevent too much acceleration
        vel.x *= 0.98
        vel.y *= 0.98
        vel.z *= 0.98
        
        # Update position
        new_pos = Vec3(
            pos.x + vel.x,
            pos.y + vel.y,
            pos.z + vel.z
        )
        
        # Keep spheres within bounds
        if new_pos.x < -6: new_pos.x = -6; vel.x *= -0.5
        if new_pos.x > 6: new_pos.x = 6; vel.x *= -0.5
        if new_pos.y < 0.5: new_pos.y = 0.5; vel.y *= -0.5
        if new_pos.y > 4: new_pos.y = 4; vel.y *= -0.5
        if new_pos.z < -6: new_pos.z = -6; vel.z *= -0.5
        if new_pos.z > 6: new_pos.z = 6; vel.z *= -0.5
        
        sphere.set_position(new_pos)
        
        # Add some wobble rotation
        sphere.rotate(100 * dt, Vec3(
            sin(t * 5) * 0.5,
            cos(t * 5) * 0.5,
            sin(t * 3) * 0.5
        ))

    forge.gui_management()
    forge.process_input()
    forge.render() 