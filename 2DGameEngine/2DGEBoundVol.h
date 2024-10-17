#pragma once
#include <SDL.h>
#include <vector>
#include "2DGEBody.h"//�Ժ���ܻ�Ƕ��bodymanager
#include "2DGEDraw.h"
struct AABB
{
	std::vector<SDL_FPoint> vertices_ = {{0.0f, 0.0f},
										 {0.0f, 0.0f},
										 {0.0f, 0.0f},
										 {0.0f, 0.0f} };
	
	void GetAABB(std::vector<Body>::iterator body);
	void RenderAABB(Brush& brush);
};