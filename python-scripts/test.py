import concept_forge as cf
from math import sin

from concept_forge import Entity
from concept_forge import Vec3

forge = cf.ConceptForge()

pos = Vec3(0.5, 1.5, 0.5)
rot = Vec3(45, 45, 45)
scale = Vec3(1, 1, 1)

entityGen = Entity()

forge.entities.clear()

entityGen.add_cube(forge, pos, rot, scale)

print(forge.entities)

cube = forge.entities[0]

dx = 0
dy = 200
dz = 0

# print(forge.entities)

while not forge.window_should_close():
    forge.calc_delta_time()
    forge.calc_projection()

    forge.gui_management()
    forge.process_input()
    forge.render()
