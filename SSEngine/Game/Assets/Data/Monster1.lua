Pawn = 
{
	name = "Monster1",
	type = "FocusMoveMonster",
	
	transform = {
		 location = {-100, -100, 0.0},
		 rotation = {0.0, 0.0, 0},
		 scale = {1.0, 1.0, 1.0}
	},
	
	static = false,
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {1.5, 1.5, 0.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\FocusMonster.dds",
		priority = 2000
	},
	
	collisionObject = {
		collideType = "Block",
		active = true
	},
}