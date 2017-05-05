Actor = 
{
	name = "BottomWall",
	type = "Wall",
	
	mobility = "Static",
	
	boundingBox = {
		center = {0.0, 10, 0.0},
		extend = {1200, 20, 1.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\HorizonWall.dds",
		priority = 0.0
	},
	
	collisionObject = {
		collideType = "Block",
		active = true
	},
}