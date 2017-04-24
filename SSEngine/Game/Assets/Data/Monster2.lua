Pawn = 
{
	name = "Monster2",
	type = "RandomMoveMonster",
	
	transform = {
		 location = {100, 150, 0.0},
		 rotation = {0.0, 0.0, 0},
		 scale = {1.0, 1.0, 1.0}
	},
	
	static = false,
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {1.0, 1.0, 0.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\RandomMonster.dds",
		priority = 2000
	},
	
	collisionObject = {
		collideType = "Block",
		active = true
	},
}