-- A simple scene with some miscellaneous geometry.

mat1_ref = gr.reflective_material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, {0.15, 0.15, 0.15}, 25)
mat2_ref = gr.reflective_material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, {0.2, 0.2, 0.2}, 25)
mat3_ref = gr.reflective_material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, {0.1, 0.1, 0.1}, 25)
mat4_ref = gr.reflective_material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, {0.1, 0.1, 0.1}, 25)
globe = gr.textured_material({0.7, 0.7, 0.7}, {0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}, 25, "tests/earth.png")
grass = gr.textured_material({1.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}, 25, "tests/grass.png")
stone = gr.textured_material({1.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 25, "tests/stone.png")

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
scene_root:add_child(s1)
s1:set_material(globe)

s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
scene_root:add_child(s2)
s2:set_material(mat1_ref)

s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(mat2_ref)

b1 = gr.nh_box('b1', {-200, -225, 100}, 100)
scene_root:add_child(b1)
b1:set_material(grass)

-- s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
s4 = gr.nh_sphere('s4', {-150, -085, 150}, 30)
scene_root:add_child(s4)
s4:set_material(mat3_ref)

s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat1_ref)

-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'tests/smstdodeca.obj' )
steldodec:set_material(stone)
scene_root:add_child(steldodec)

-- Particles
p1 = gr.particles('p1', {0, -100, 125}, 200, 15, 1, 20, 'y', -3, mat1_ref)
scene_root:add_child(p1)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'tests/nonhier-par', 512, 512, 1,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light}, {p1})
