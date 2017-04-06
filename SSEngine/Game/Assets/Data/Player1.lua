Pawn = 
{
	name = "Player1",
	type = "Player",
	
	transform = {
		 location = {0.0, 0.0, 0.0},
		 rotation = {0.0, 0.0, 0.0},
		 scale = {1.0, 1.0, 1.0}
	},
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {1.0, 1.0, 1.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\player.dds",
		priority = 3000
	},
	
	physicsObject = {
		mass = 100.0,
		drag = 0.1
	}
}