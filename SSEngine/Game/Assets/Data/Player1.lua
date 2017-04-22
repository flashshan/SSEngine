Pawn = 
{
	name = "Player1",
	type = "Player",
	
	transform = {
		 location = {0.0, 0.0, 0.0},
		 rotation = {0.0, 0.0, 0},
		 scale = {1.0, 1.0, 1.0}
	},
	
	static = false,
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {2.0, 2.0, 0.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\Player.dds",
		priority = 3000
	},
	
	physicsObject = {
		mass = 0.1,
		drag = 0.001
	},
	
	collisionObject = {
		collideType = "Overlap",
		active = true
	},
}