#pragma once
#include "2DGEBody.h"
//作用力发生器 用于计算生成各种力 如浮力阻力，弹力等等.返回的都是力
class ForceGenerator {

public:
	virtual void updateForce(std::vector<Body>::iterator body);
};