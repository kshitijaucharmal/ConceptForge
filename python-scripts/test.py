import concept_forge as cf
from math import sin, cos, pi
import time

from concept_forge import Entity, Vec3
from concept_forge.primitives import Cube, UVSphere

forge = cf.ConceptForge()

entityGen = Entity()
forge.hierarchy.entities.clear()

pos = Vec3(0, 0, 0)
rot = Vec3(0, 0, 0)
scale = Vec3(1, 1, 1)
cube = Cube.new(forge, pos, rot, scale)

pos = Vec3(0, 2, 0)
rot = Vec3(0, 0, 0)
scale = Vec3(1, 1, 1)
sphere = UVSphere.new(forge, pos, rot, scale)

while not forge.window_should_close():
    dt = forge.dt()
    forge.calc_projection()

    # sphere.rotate(200 * dt, Vec3(0, 1, 0))
    # cube.rotate(-200 * dt, Vec3(0, 1, 0))
    cube.set_position(Vec3(0, sin(time.time() * 2) * 2, 0))

    forge.gui_management()
    forge.process_input()
    forge.render()
