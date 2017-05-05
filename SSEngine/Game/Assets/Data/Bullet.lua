Actor = 
{
	name = "Bullet1",
	type = "Bullet",
	
	mobility = "Movable",
	
	boundingBox = {
		center = {0.0, 0.0, 0.0},
		extend = {1.5, 1.5, 1.0}
	},
	
	renderObject = {
		path = "Assets\\Texture\\Bullet.dds",
		priority = 0.0
	},
	
	physicsObject = {
		mass = 0.05,
		drag = 0.0001
	},
	
	collisionObject = {
		collideType = "Block",
		active = true
	},
}