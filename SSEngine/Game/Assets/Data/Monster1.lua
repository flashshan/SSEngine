Pawn = 
{
	name = "Monster1",
	type = "FocusMoveMonster",
	
	mobility = "Movable",
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {3, 3, 0.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\FocusMonster.dds",
		priority = 2000
	},
	
	collisionObject = {
		collideType = "Block",
		active = true
	},
	
	HP = 2,
}