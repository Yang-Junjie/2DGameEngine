#pragma once

#include <vector>
#include "SDL.h"
#include "2DGEBody.h"




static bool IntersectAABB(Body& bodyA, Body bodyB)
{
	if (bodyA.vertices_aabb[1].x <= bodyB.vertices_aabb[0].x ||
		bodyB.vertices_aabb[1].x <= bodyA.vertices_aabb[0].x ||
		bodyA.vertices_aabb[0].y <= bodyB.vertices_aabb[2].y ||
		bodyB.vertices_aabb[0].y <= bodyA.vertices_aabb[2].y) {
		return false;
	}
	return true;
}
