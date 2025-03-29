-- Animation Script for A5 Project

-- Animation Settings
startFrame = 0
frames = 307
output_path = 'tests/animation'

if frames > 1 or startFrame ~= 0 then output_path = 'renders/frames/animation' end

-- Materials
green = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
brown = gr.material({0.1, 0.032, 0.005}, {0.0, 0.0, 0.0}, 0)
light_brown = gr.material({0.13, 0.067, 0.032}, {0.0, 0.0, 0.0}, 0)
dark_green = gr.material({0.05, 0.3, 0.034}, {0.0, 0.0, 0.0}, 0)
grey = gr.material({0.4, 0.4, 0.4}, {0.0, 0.0, 0.0}, 0)
black = gr.material({0.1, 0.1, 0.1}, {0.0, 0.0, 0.0}, 0)
clouds = gr.material({0.9, 0.9, 0.9}, {0.1, 0.1, 0.1}, 10)

blue = gr.reflective_material({0.2, 0.3, 0.4}, {0.6, 0.7, 0.8}, {0.3, 0.3, 0.3}, 25)
white = gr.reflective_material({0.8, 0.8, 0.8}, {0.95, 0.95, 0.95},  {0.1, 0.1, 0.1}, 25)
yellow = gr.reflective_material({0.4, 0.4, 0}, {1, 1, 0.0},  {0.0, 0.0, 0.0}, 25)

grass = gr.textured_material({1.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}, 25, "assets/rock.png")

-- Root node to hold all items
scene = gr.node('scene')

-- Main ground
snow = gr.mesh('snow', 'assets/Snow.obj')
scene:add_child(snow)
snow:set_material(white)

-- Additional texture of main ground
shore = gr.mesh('shore', 'assets/Shore.obj')
scene:add_child(shore)
shore:set_material(grey)

-- River and ocean
water = gr.mesh('water', 'assets/Water.obj')
scene:add_child(water)
water:set_material(blue)
water:set_displacement_map(0, frames, 'waves')

-- Main Trees
TreeLeavesM_001 = gr.mesh('TreeLeavesM_001', 'assets/TreeLeavesM_001.obj')
scene:add_child(TreeLeavesM_001)
TreeLeavesM_001:set_material(green)

TreeStumpM_001 = gr.mesh('TreeStumpM_001', 'assets/TreeStumpM_001.obj')
scene:add_child(TreeStumpM_001)
TreeStumpM_001:set_material(brown)

TreeLeavesM_002 = gr.mesh('TreeLeavesM_002', 'assets/TreeLeavesM_002.obj')
scene:add_child(TreeLeavesM_002)
TreeLeavesM_002:set_material(green)

TreeStumpM_002 = gr.mesh('TreeStumpM_002', 'assets/TreeStumpM_002.obj')
scene:add_child(TreeStumpM_002)
TreeStumpM_002:set_material(brown)

TreeLeavesM_003 = gr.mesh('TreeLeavesM_003', 'assets/TreeLeavesM_003.obj')
scene:add_child(TreeLeavesM_003)
TreeLeavesM_003:set_material(green)

TreeStumpM_003 = gr.mesh('TreeStumpM_003', 'assets/TreeStumpM_003.obj')
scene:add_child(TreeStumpM_003)
TreeStumpM_003:set_material(brown)

TreeLeavesM_004 = gr.mesh('TreeLeavesM_004', 'assets/TreeLeavesM_004.obj')
scene:add_child(TreeLeavesM_004)
TreeLeavesM_004:set_material(green)

TreeStumpM_004 = gr.mesh('TreeStumpM_004', 'assets/TreeStumpM_004.obj')
scene:add_child(TreeStumpM_004)
TreeStumpM_004:set_material(brown)

TreeLeavesM_005 = gr.mesh('TreeLeavesM_005', 'assets/TreeLeavesM_005.obj')
scene:add_child(TreeLeavesM_005)
TreeLeavesM_005:set_material(green)

TreeStumpM_005 = gr.mesh('TreeStumpM_005', 'assets/TreeStumpM_005.obj')
scene:add_child(TreeStumpM_005)
TreeStumpM_005:set_material(brown)

TreeLeavesM_006 = gr.mesh('TreeLeavesM_006', 'assets/TreeLeavesM_006.obj')
scene:add_child(TreeLeavesM_006)
TreeLeavesM_006:set_material(green)

TreeStumpM_006 = gr.mesh('TreeStumpM_006', 'assets/TreeStumpM_006.obj')
scene:add_child(TreeStumpM_006)
TreeStumpM_006:set_material(brown)

TreeLeavesM_007 = gr.mesh('TreeLeavesM_007', 'assets/TreeLeavesM_007.obj')
scene:add_child(TreeLeavesM_007)
TreeLeavesM_007:set_material(green)

TreeStumpM_007 = gr.mesh('TreeStumpM_007', 'assets/TreeStumpM_007.obj')
scene:add_child(TreeStumpM_007)
TreeStumpM_007:set_material(brown)

TreeLeavesM_008 = gr.mesh('TreeLeavesM_008', 'assets/TreeLeavesM_008.obj')
scene:add_child(TreeLeavesM_008)
TreeLeavesM_008:set_material(green)

TreeStumpM_008 = gr.mesh('TreeStumpM_008', 'assets/TreeStumpM_008.obj')
scene:add_child(TreeStumpM_008)
TreeStumpM_008:set_material(brown)

TreeLeavesM_009 = gr.mesh('TreeLeavesM_009', 'assets/TreeLeavesM_009.obj')
scene:add_child(TreeLeavesM_009)
TreeLeavesM_009:set_material(green)

TreeStumpM_009 = gr.mesh('TreeStumpM_009', 'assets/TreeStumpM_009.obj')
scene:add_child(TreeStumpM_009)
TreeStumpM_009:set_material(brown)

TreeLeavesM_010 = gr.mesh('TreeLeavesM_010', 'assets/TreeLeavesM_010.obj')
scene:add_child(TreeLeavesM_010)
TreeLeavesM_010:set_material(green)

TreeStumpM_010 = gr.mesh('TreeStumpM_010', 'assets/TreeStumpM_010.obj')
scene:add_child(TreeStumpM_010)
TreeStumpM_010:set_material(brown)

TreeLeavesM_011 = gr.mesh('TreeLeavesM_011', 'assets/TreeLeavesM_011.obj')
scene:add_child(TreeLeavesM_011)
TreeLeavesM_011:set_material(green)

TreeStumpM_011 = gr.mesh('TreeStumpM_011', 'assets/TreeStumpM_011.obj')
scene:add_child(TreeStumpM_011)
TreeStumpM_011:set_material(brown)

-- Smaller Trees
TreeLeavesS_001 = gr.mesh('TreeLeavesS_001', 'assets/TreeLeavesS_001.obj')
scene:add_child(TreeLeavesS_001)
TreeLeavesS_001:set_material(green)

TreeStumpS_001 = gr.mesh('TreeStumpS_001', 'assets/TreeStumpS_001.obj')
scene:add_child(TreeStumpS_001)
TreeStumpS_001:set_material(brown)

TreeLeavesS_002 = gr.mesh('TreeLeavesS_002', 'assets/TreeLeavesS_002.obj')
scene:add_child(TreeLeavesS_002)
TreeLeavesS_002:set_material(green)

TreeStumpS_002 = gr.mesh('TreeStumpS_002', 'assets/TreeStumpS_002.obj')
scene:add_child(TreeStumpS_002)
TreeStumpS_002:set_material(brown)

-- Bushes
BushL_001 = gr.mesh('BushL_001', 'assets/BushL_001.obj')
scene:add_child(BushL_001)
BushL_001:set_material(dark_green)

BushL_002 = gr.mesh('BushL_002', 'assets/BushL_002.obj')
scene:add_child(BushL_002)
BushL_002:set_material(dark_green)

BushL_003 = gr.mesh('BushL_003', 'assets/BushL_003.obj')
scene:add_child(BushL_003)
BushL_003:set_material(dark_green)

BushS_001 = gr.mesh('BushS_001', 'assets/BushS_001.obj')
scene:add_child(BushS_001)
BushS_001:set_material(dark_green)

BushS_002 = gr.mesh('BushS_002', 'assets/BushS_002.obj')
scene:add_child(BushS_002)
BushS_002:set_material(dark_green)

-- Rocks
RockM_001 = gr.mesh('RockM_001', 'assets/RockM_001.obj')
scene:add_child(RockM_001)
RockM_001:set_material(grey)

RockM_002 = gr.mesh('RockM_002', 'assets/RockM_002.obj')
scene:add_child(RockM_002)
RockM_002:set_material(grey)

RockM_003 = gr.mesh('RockM_003', 'assets/RockM_003.obj')
scene:add_child(RockM_003)
RockM_003:set_material(grey)

RockM_004 = gr.mesh('RockM_004', 'assets/RockM_004.obj')
scene:add_child(RockM_004)
RockM_004:set_material(grey)

RockS_001 = gr.mesh('RockS_001', 'assets/RockS_001.obj')
scene:add_child(RockS_001)
RockS_001:set_material(grey)

RockS_002 = gr.mesh('RockS_002', 'assets/RockS_002.obj')
scene:add_child(RockS_002)
RockS_002:set_material(grey)

-- Cliff
cliff = gr.cube('cliff')
scene:add_child(cliff)
cliff:set_material(grass)
cliff:scale(10, 10, 10)
cliff:rotate('Y', 231.3)
cliff:translate(9.18, 6.25, 93.8)

-- Cow
cow = gr.mesh('cow', 'assets/cow.obj')
scene:add_child(cow)
cow:set_material(light_brown)
cow:translate(0, 0.25, 0)

-- Birds
Bird_001 = gr.mesh('Bird_001', 'assets/Bird_001.obj')
scene:add_child(Bird_001)
Bird_001:set_material(black)
Bird_001:set_animation(24, 79, 't', 'right')
Bird_001:set_animation(79, 109, 't', 'leftFall')

Bird_002 = gr.mesh('Bird_002', 'assets/Bird_002.obj')
scene:add_child(Bird_002)
Bird_002:set_material(black)
Bird_002:set_animation(48, 79, 't', 'left')
Bird_002:set_animation(79, 109, 't', 'rightFall')

Bird_003 = gr.mesh('Bird_003', 'assets/Bird_003.obj')
scene:add_child(Bird_003)
Bird_003:set_material(black)
Bird_003:set_animation(72, 290, 't', 'left')

-- Sun
sun = gr.mesh('sun', 'assets/smstellateddodecahedron.obj')
scene:add_child(sun)
sun:set_material(yellow)
sun:set_animation(0, 307, 'r', 'spin')
sun:set_animation(0, 307, 't', 'suntrans')
sun:set_animation(124, 194, 't', 'up')

-- Clouds
Cloud_001 = gr.mesh('Cloud_001', 'assets/Cloud_001.obj')
scene:add_child(Cloud_001)
Cloud_001:set_material(clouds)
Cloud_001:set_animation(124, 194, 't', 'right')

Cloud_002 = gr.mesh('Cloud_002', 'assets/Cloud_002.obj')
scene:add_child(Cloud_002)
Cloud_002:set_material(clouds)
Cloud_002:set_animation(124, 194, 't', 'left2')

-- Snow particles
snow_particles1 = gr.particles('snow_particles1', {-5, 20.5, 80}, 4.5, 2, 0.075, 100, 'y', -0.08, white)
scene:add_child(snow_particles1)
snow_particles1:set_animation(124, 194, 't', 'left2')

snow_particles2 = gr.particles('snow_particles2', {-1.5, 20.5, 80}, 4.5, 2, 0.075, 100, 'y', -0.08, white)
scene:add_child(snow_particles2)
snow_particles2:set_animation(124, 194, 't', 'right')

particles = {snow_particles1, snow_particles2}

-- Lights
grey_light = gr.light({0, 40, 150}, {0.4, 0.4, 0.4}, {1, 0, 0})
yellow_light = gr.light({0, -25, -18}, {0.6, 0.6, 0.4}, {1, 0, 0})
yellow_light2 = gr.light({20, -25, -40}, {0.6, 0.6, 0.4}, {1, 0, 0})
yellow_light3 = gr.light({-20, -25, -40}, {0.6, 0.6, 0.4}, {1, 0, 0})
lights = {grey_light, yellow_light, yellow_light2, yellow_light3}

gr.render(scene, output_path, 512, 512, startFrame, frames,
	  {-0.96, 20, 98.3}, {0, 0, -18}, {0, 1, 0}, 40,
	  {0.4, 0.4, 0.4}, lights, particles)
