Actor = 
{
	name = "LeftWall",
	type = "Actor",
	
	transform = {
		 location = {-300, -200, 0.0},
		 rotation = {0.0, 0.0, 0.0},
		 scale = {1.0, 1.0, 1.0}
	},
	
	canCollide = true,
	static = true,
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {10, 50, 1.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\Wall.dds",
		priority = 0.0
	},
}