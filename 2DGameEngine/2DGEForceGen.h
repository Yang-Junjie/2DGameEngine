#pragma once
#include "2DGEBody.h"
//������������ ���ڼ������ɸ����� �縡�������������ȵ�.���صĶ�����
class ForceGenerator {

public:
	virtual void updateForce(std::vector<Body>::iterator body);
};