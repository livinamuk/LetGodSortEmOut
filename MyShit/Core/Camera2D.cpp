#include "Camera2D.h"

int Camera2D::s_scrollX;
int Camera2D::s_scrollY;
glm::mat4 Camera2D::s_viewMatrix;

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
	AABB aabb;
	aabb.lowerX = Camera2D::s_scrollX - (SCR_WIDTH / 2);
	aabb.upperX = Camera2D::s_scrollX + (SCR_WIDTH / 2);
	aabb.lowerY = Camera2D::s_scrollY - (SCR_HEIGHT / 2);
	aabb.upperY = Camera2D::s_scrollY + (SCR_HEIGHT / 2);

	//	aabb.lowerX = std::max(aabb.lowerX, (int)SCR_WIDTH);
	if (aabb.upperY < SCR_HEIGHT)
		aabb.upperY = SCR_HEIGHT;

	return aabb;
}
