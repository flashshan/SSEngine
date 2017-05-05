Actor = 
{
	name = "LeftWall",
	type = "Wall",
	
	mobility = "Static",
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {20, 1200, 1.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\VerticalWall.dds",
		priority = 0.0
	},
	
	collisionObject = {
		collideType = "Block",
		active = true
	},
}