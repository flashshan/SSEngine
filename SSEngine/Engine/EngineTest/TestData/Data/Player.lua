
Player = 
{
	name = "Joe",
	class = "Player",
	controller = "InputController",
	initial_position = { 1.0, 2.0, 3.0 },
	
	bounding_box = { 
			offset	= { -1.1, 1.2, 3.0 },
			size	= { 10.0, 4.0, 2.0 }
	},

	physics_settings = 
	{
		mass = 10.0,
		drag = 0.02
	},

	render_settings = 
	{
		sprite = { 
			offset	= { 0.0, 0.0 },
					-- left, top, right, bottom
			extents	= { -1.0, -1.0, 1.0, 1.0 }
		},
		sprite_color = { 255, 255, 255, 255 }
	}
}