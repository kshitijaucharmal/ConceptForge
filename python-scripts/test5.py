import concept_forge as cf
from math import sin, cos, pi, sqrt, atan2
import time
import random

from concept_forge import Entity, Vec3
from concept_forge.primitives import UVSphere

forge = cf.ConceptForge()
forge.hierarchy.entities.clear()

# Boids parameters
NUM_BOIDS = 50  # Reduced number for better visibility
WORLD_SIZE = 10.0
MAX_SPEED = 3.0  # Reduced for more controlled movement
MAX_FORCE = 0.1  # Reduced for smoother steering

# Flocking parameters - adjusted for more cohesive behavior
SEPARATION_RADIUS = 2.0  # Increased to maintain more space
ALIGNMENT_RADIUS = 4.0   # Increased for better group alignment
COHESION_RADIUS = 5.0    # Increased for tighter groups

# Adjusted weights for more flocking behavior
SEPARATION_WEIGHT = 1.0   # Reduced to allow closer grouping
ALIGNMENT_WEIGHT = 1.5    # Increased for better alignment
COHESION_WEIGHT = 1.5     # Increased for tighter groups

# Create boids
boids = []
velocities = []
positions = []

for _ in range(NUM_BOIDS):
    # Random initial position within world bounds
    pos = Vec3(
        random.uniform(-WORLD_SIZE/2, WORLD_SIZE/2),
        random.uniform(-WORLD_SIZE/2, WORLD_SIZE/2),
        random.uniform(-WORLD_SIZE/2, WORLD_SIZE/2)
    )
    
    # Random initial velocity with higher initial speed
    vel = Vec3(
        random.uniform(-1, 1),
        random.uniform(-1, 1),
        random.uniform(-1, 1)
    )
    # Normalize and scale initial velocity
    length = sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z)
    if length > 0:
        vel = Vec3(
            vel.x/length * MAX_SPEED * 0.5,
            vel.y/length * MAX_SPEED * 0.5,
            vel.z/length * MAX_SPEED * 0.5
        )
    
    # Create sphere for boid with larger size
    sphere = UVSphere.new(forge, pos, Vec3(0, 0, 0), Vec3(0.3, 0.3, 0.3))  # Increased size
    boids.append(sphere)
    velocities.append(vel)
    positions.append(pos)

def limit_force(force, max_force):
    length = sqrt(force.x * force.x + force.y * force.y + force.z * force.z)
    if length > max_force:
        scale = max_force / length
        return Vec3(force.x * scale, force.y * scale, force.z * scale)
    return force

def limit_velocity(vel, max_speed):
    length = sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z)
    if length > max_speed:
        scale = max_speed / length
        return Vec3(vel.x * scale, vel.y * scale, vel.z * scale)
    return vel

def get_separation_force(boid_pos, boid_vel, other_positions):
    force = Vec3(0, 0, 0)
    count = 0
    
    for other_pos in other_positions:
        dx = boid_pos.x - other_pos.x
        dy = boid_pos.y - other_pos.y
        dz = boid_pos.z - other_pos.z
        dist = sqrt(dx*dx + dy*dy + dz*dz)
        
        if 0 < dist < SEPARATION_RADIUS:
            # Stronger force when closer
            strength = 1.0 - (dist / SEPARATION_RADIUS)
            force = Vec3(
                force.x + dx/dist * strength,
                force.y + dy/dist * strength,
                force.z + dz/dist * strength
            )
            count += 1
    
    if count > 0:
        force = Vec3(force.x/count, force.y/count, force.z/count)
        return limit_force(force, MAX_FORCE)
    return force

def get_alignment_force(boid_pos, boid_vel, other_positions, other_velocities):
    force = Vec3(0, 0, 0)
    count = 0
    
    for i, other_pos in enumerate(other_positions):
        dx = boid_pos.x - other_pos.x
        dy = boid_pos.y - other_pos.y
        dz = boid_pos.z - other_pos.z
        dist = sqrt(dx*dx + dy*dy + dz*dz)
        
        if 0 < dist < ALIGNMENT_RADIUS:
            # Weight by distance
            weight = 1.0 - (dist / ALIGNMENT_RADIUS)
            force = Vec3(
                force.x + other_velocities[i].x * weight,
                force.y + other_velocities[i].y * weight,
                force.z + other_velocities[i].z * weight
            )
            count += 1
    
    if count > 0:
        force = Vec3(force.x/count, force.y/count, force.z/count)
        return limit_force(force, MAX_FORCE)
    return force

def get_cohesion_force(boid_pos, boid_vel, other_positions):
    force = Vec3(0, 0, 0)
    count = 0
    
    for other_pos in other_positions:
        dx = boid_pos.x - other_pos.x
        dy = boid_pos.y - other_pos.y
        dz = boid_pos.z - other_pos.z
        dist = sqrt(dx*dx + dy*dy + dz*dz)
        
        if 0 < dist < COHESION_RADIUS:
            # Weight by distance
            weight = 1.0 - (dist / COHESION_RADIUS)
            force = Vec3(
                force.x + other_pos.x * weight,
                force.y + other_pos.y * weight,
                force.z + other_pos.z * weight
            )
            count += 1
    
    if count > 0:
        force = Vec3(force.x/count, force.y/count, force.z/count)
        force = Vec3(
            force.x - boid_pos.x,
            force.y - boid_pos.y,
            force.z - boid_pos.z
        )
        return limit_force(force, MAX_FORCE)
    return force

def update_boids(dt):
    for i in range(NUM_BOIDS):
        # Get forces
        sep_force = get_separation_force(positions[i], velocities[i], positions)
        align_force = get_alignment_force(positions[i], velocities[i], positions, velocities)
        coh_force = get_cohesion_force(positions[i], velocities[i], positions)
        
        # Apply forces with weights
        velocities[i] = Vec3(
            velocities[i].x + (sep_force.x * SEPARATION_WEIGHT + 
                             align_force.x * ALIGNMENT_WEIGHT + 
                             coh_force.x * COHESION_WEIGHT) * dt,
            velocities[i].y + (sep_force.y * SEPARATION_WEIGHT + 
                             align_force.y * ALIGNMENT_WEIGHT + 
                             coh_force.y * COHESION_WEIGHT) * dt,
            velocities[i].z + (sep_force.z * SEPARATION_WEIGHT + 
                             align_force.z * ALIGNMENT_WEIGHT + 
                             coh_force.z * COHESION_WEIGHT) * dt
        )
        
        # Limit velocity
        velocities[i] = limit_velocity(velocities[i], MAX_SPEED)
        
        # Update position
        positions[i] = Vec3(
            positions[i].x + velocities[i].x * dt,
            positions[i].y + velocities[i].y * dt,
            positions[i].z + velocities[i].z * dt
        )
        
        # Wrap around world bounds
        for axis in ['x', 'y', 'z']:
            pos = getattr(positions[i], axis)
            if pos > WORLD_SIZE/2:
                setattr(positions[i], axis, -WORLD_SIZE/2)
            elif pos < -WORLD_SIZE/2:
                setattr(positions[i], axis, WORLD_SIZE/2)
        
        # Update boid position
        boids[i].set_position(positions[i])
        
        # Update boid rotation to face direction of movement
        if velocities[i].x != 0 or velocities[i].y != 0 or velocities[i].z != 0:
            # Calculate rotation to face velocity direction
            forward = velocities[i]
            up = Vec3(0, 1, 0)
            right = Vec3(
                forward.y * up.z - forward.z * up.y,
                forward.z * up.x - forward.x * up.z,
                forward.x * up.y - forward.y * up.x
            )
            up = Vec3(
                right.y * forward.z - right.z * forward.y,
                right.z * forward.x - right.x * forward.z,
                right.x * forward.y - right.y * forward.x
            )
            
            # Normalize vectors
            length = sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z)
            forward = Vec3(forward.x/length, forward.y/length, forward.z/length)
            
            length = sqrt(right.x * right.x + right.y * right.y + right.z * right.z)
            right = Vec3(right.x/length, right.y/length, right.z/length)
            
            length = sqrt(up.x * up.x + up.y * up.y + up.z * up.z)
            up = Vec3(up.x/length, up.y/length, up.z/length)
            
            # Set rotation
            boids[i].set_rotation(Vec3(
                atan2(forward.y, forward.z) * 180/pi,
                atan2(forward.x, forward.z) * 180/pi,
                atan2(right.y, right.x) * 180/pi
            ))

while not forge.window_should_close():
    dt = forge.dt()
    forge.calc_projection()
    
    # Update boids
    update_boids(dt)
    
    forge.gui_management()
    forge.process_input()
    forge.render() 