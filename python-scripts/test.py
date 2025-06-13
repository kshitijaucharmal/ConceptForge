import concept_forge as cf
from math import sin, cos, pi
import time

from concept_forge import Entity, Vec3

forge = cf.ConceptForge()

entityGen = Entity()

forge.entities.clear()

pos = Vec3(0, 0, 0)
rot = Vec3(0, 0, 0)
scale = Vec3(1, 1, 1)
cube = forge.add_cube(pos, rot, scale)

pos = Vec3(0, 2, 0)
rot = Vec3(0, 0, 0)
scale = Vec3(1, 1, 1)
sphere = forge.add_uvsphere(pos, rot, scale)

while not forge.window_should_close():
    forge.calc_delta_time()
    forge.calc_projection()

    sphere.rotate(200 * forge.deltaTime, Vec3(0, 1, 0))
    cube.rotate(-200 * forge.deltaTime, Vec3(0, 1, 0))

    forge.gui_management()
    forge.process_input()
    forge.render()
