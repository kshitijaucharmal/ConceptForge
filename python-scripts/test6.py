import concept_forge as cf
from concept_forge import Vec3
from concept_forge.primitives import Cube, UVSphere
import math

class BeesAndBombs:
    def __init__(self, forge):
        self.forge = forge
        self.entities = []
        self.time = 0
        
        # Create a grid of cubes
        grid_size = 5
        spacing = 2.0
        for x in range(-grid_size, grid_size + 1):
            for y in range(-grid_size, grid_size + 1):
                pos = Vec3(x * spacing, 0, y * spacing)
                rot = Vec3(0, 0, 0)
                scale = Vec3(0.5, 0.5, 0.5)
                cube = Cube.new(forge, pos, rot, scale)
                self.entities.append(cube)
        
        # Create a sphere that will follow the pattern
        pos = Vec3(0, 0, 0)
        rot = Vec3(0, 0, 0)
        scale = Vec3(0.3, 0.3, 0.3)
        self.sphere = UVSphere.new(forge, pos, rot, scale)
    
    def update(self, dt):
        self.time += dt
        
        # Update sphere position using Bees and Bombs pattern
        radius = 5.0
        frequency = 2.0
        x = radius * math.cos(self.time * frequency)
        y = radius * math.sin(self.time * frequency)
        z = radius * math.sin(self.time * frequency * 0.5)
        
        self.sphere.set_position(Vec3(x, y, z))
        
        # Rotate all cubes
        for entity in self.entities:
            entity.rotate(45 * dt, Vec3(0, 1, 0))

def main():
    forge = cf.ConceptForge()
    simulation = BeesAndBombs(forge)
    
    while not forge.window_should_close():
        dt = forge.dt()
        forge.calc_projection()
        
        simulation.update(dt)
        
        forge.gui_management()
        forge.process_input()
        forge.render()

if __name__ == "__main__":
    main() 