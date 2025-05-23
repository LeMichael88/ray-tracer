-- A more macho version of macho_cows.py, in which the view isn't
-- axis aligned.


-- We'll need an extra function that knows how to read Wavefront .OBJ
-- files.

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

-- ##############################################
-- the arch
-- ##############################################

inst = gr.node('inst')

arc = gr.node('arc')
inst:add_child(arc)
arc:translate(0, 0, -10)

p1 = gr.nh_box('p1', {0, 0, 0}, 1)
arc:add_child(p1)
p1:set_material(stone)
p1:scale(0.8, 4, 0.8)
p1:translate(-2.4, 0, -0.4)

p2 = gr.nh_box('p2', {0, 0, 0}, 1)
arc:add_child(p2)
p2:set_material(stone)
p2:scale(0.8, 4, 0.8)
p2:translate(1.6, 0, -0.4)

s = gr.nh_sphere('s', {0, 0, 0}, 1)
arc:add_child(s)
s:set_material(stone)
s:scale(4, 0.6, 0.6)
s:translate(0, 4, 0)


-- #############################################
-- Read in the cow model from a separate file.
-- #############################################

cow_poly = gr.mesh('cow', 'Assets/cow.obj')
factor = 2.0/(2.76+3.637)

cow_poly:set_material(hide)

cow_poly:translate(0.0, 3.637, 0.0)
cow_poly:scale(factor, factor, factor)
cow_poly:translate(0.0, -1.0, 0.0)

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 23)

-- the floor

plane = gr.mesh('plane', 'assets/plane.obj' )
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

-- Construct a central altar in the shape of a buckyball.  The
-- buckyball at the centre of the real Stonehenge was destroyed
-- in the great fire of 733 AD.

buckyball = gr.mesh( 'buckyball', 'assets/buckyball.obj' )
scene:add_child(buckyball)
buckyball:set_material(stone)
buckyball:scale(1.5, 1.5, 1.5)

-- Use the instanced cow model to place some actual cows in the scene.
-- For convenience, do this in a loop.

cow_number = 1

for _, pt in pairs({
		      {{1,1.3,14}, 20},
		      {{5,1.3,-11}, 180},
		      {{-5.5,1.3,-3}, -60}}) do
   cow_instance = gr.node('cow' .. tostring(cow_number))
   scene:add_child(cow_instance)
   cow_instance:add_child(cow_poly)
   cow_instance:scale(1.4, 1.4, 1.4)
   cow_instance:rotate('Y', pt[2])
   cow_instance:translate(table.unpack(pt[1]))
   
   cow_number = cow_number + 1
end

-- Place a ring of arches.

for i = 1, 6 do
   an_arc = gr.node('arc' .. tostring(i))
   an_arc:rotate('Y', (i-1) * 60)
   scene:add_child(an_arc)
   an_arc:add_child(arc)
end

lights = {gr.light({10, 5, 25}, {0.8, 0.8, 0.8}, {1, 0, 0.0005})}
--{0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
imSize = 512
--{200, 202, 430}
gr.render(scene,
	  'tests/mucho-macho-cows', imSize, imSize, 1,
	  {20.8, 2, 21}, {-1.1, -0.1, -1}, {0, 1, 0.5}, 50,
	  {0.4, 0.4, 0.4}, lights, {})
