Pawn = 
{
	name = "Monster2",
	type = "RandomMoveMonster",
	
	mobility = "Movable",
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {3, 3, 0.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\RandomMonster.dds",
		priority = 2000
	},
	
	collisionObject = {
		collideType = "Block",
		active = true
	},
	
	HP = 2,
}