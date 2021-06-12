#include "Camera2D.h"

int Camera2D::s_scrollX;
int Camera2D::s_scrollY;
glm::mat4 Camera2D::s_viewMatrix;
AABB Camera2D::s_screenAABB;

void Camera2D::AdjustProjection()
{
	s_scrollX = std::max((int)SCR_WIDTH / 2, s_scrollX);
	s_scrollY = std::max((int)SCR_HEIGHT / 2, s_scrollY);

	float x = (s_scrollX / SCR_WIDTH) * 2 - 1;;
	float y = ((SCR_HEIGHT - s_scrollY) / SCR_HEIGHT) * 2 - 1;

	Transform transform;
	transform.position.x = x;
	transform.position.y = y;

	s_viewMatrix = glm::inverse(transform.to_mat4());
}

AABB Camera2D::GetSCreenAABB()
{
	s_screenAABB.lowerX = Camera2D::s_scrollX - (SCR_WIDTH / 2);
	s_screenAABB.upperX = Camera2D::s_scrollX + (SCR_WIDTH / 2);
	s_screenAABB.lowerY = Camera2D::s_scrollY - (SCR_HEIGHT / 2);
	s_screenAABB.upperY = Camera2D::s_scrollY + (SCR_HEIGHT / 2);

	int threshold = 0;
	s_screenAABB.lowerX += threshold;
	s_screenAABB.upperX -= threshold;
	s_screenAABB.lowerY += threshold;
	s_screenAABB.upperY -= threshold;

//	aabb.lowerX = std::max(aabb.lowerX, (int)SCR_WIDTH);
	//if (s_screenAABB.upperY < SCR_HEIGHT)
	//	s_screenAABB.upperY = SCR_HEIGHT;

	return s_screenAABB;
}
