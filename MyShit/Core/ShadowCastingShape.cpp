#include "ShadowCastingShape.h"

ShadowCastingShape::ShadowCastingShape()
{
}

ShadowCastingShape::ShadowCastingShape(const char* name, int x, int y, int width, int height, int angle)
{
	m_name = name;
	m_position = glm::vec2(x,y);
	m_width = width;
	m_height = height;
	m_angle = angle;
	CalculateCorners();
}

ShadowCastingShape::ShadowCastingShape(int fixedValue, int width, int height, int angle)
{
	m_name = "fixed_value_shape";
	m_fixedVale = fixedValue;
	m_position = glm::vec2(0, 0);
	m_width = width;
	m_height = height;
	m_angle = angle;
}

bool ShadowCastingShape::HasFixedValuePair()
{
	if (m_fixedVale == -1)
		return false;
	else return true;
}

void ShadowCastingShape::CalculateCorners()
{
	float lowerX = -m_width / 2;
	float upperX = m_width / 2;
	float lowerY = -m_height / 2;
	float upperY = m_height / 2;

	float angle = m_angle * HELL_PI / 180;
	glm::mat3 rotMatrix;
	rotMatrix[0][0] = cos(angle);
	rotMatrix[0][1] = -sin(angle);
	rotMatrix[0][2] = 0;
	rotMatrix[1][0] = sin(angle);
	rotMatrix[1][1] = cos(angle);
	rotMatrix[1][2] = 0;
	rotMatrix[2][0] = 0;
	rotMatrix[2][1] = 0;
	rotMatrix[2][2] = 1;

	glm::vec3 position(m_position.x, m_position.y, 0);
	glm::vec3 cornerA = rotMatrix * glm::vec3(lowerX, lowerY, 0) + position;
	glm::vec3 cornerB = rotMatrix * glm::vec3(lowerX, upperY, 0) + position;
	glm::vec3 cornerC = rotMatrix * glm::vec3(upperX, upperY, 0) + position;
	glm::vec3 cornerD = rotMatrix * glm::vec3(upperX, lowerY, 0) + position;

	m_corners[0] = Coord2D(cornerA.x, cornerA.y);
	m_corners[1] = Coord2D(cornerB.x, cornerB.y);
	m_corners[2] = Coord2D(cornerC.x, cornerC.y);
	m_corners[3] = Coord2D(cornerD.x, cornerD.y);
}

void ShadowCastingShape::SetAngle(int angle)
{
	if (angle == m_angle)
		return;

	CalculateCorners();
	m_angle = angle;
	m_modified = true;
}

void ShadowCastingShape::SetPosition(int x, int y)
{
	if ((int)m_position.x == x && (int)m_position.y == y)
		return;

	CalculateCorners();
	m_position = glm::vec2(x, y);
	m_modified = true;
}

void ShadowCastingShape::SetWidth(int width)
{
	if (width == m_width)
		return;

	CalculateCorners();
	m_width = width;
	m_modified = true;
}

void ShadowCastingShape::SetHeight(int height)
{
	if (height == m_height)
		return;

	CalculateCorners();
	m_height = height;
	m_modified = true;
}

int ShadowCastingShape::GetAngle()
{
	CalculateCorners();
	return m_angle;
}

int ShadowCastingShape::GetX()
{
	CalculateCorners();
	return m_position.x;
}

int ShadowCastingShape::GetY()
{
	CalculateCorners();
	return m_position.y;
}

int ShadowCastingShape::GetWidth()
{
	CalculateCorners();
	return m_width;
}

int ShadowCastingShape::GetHeight()
{
	CalculateCorners();
	return m_height;
}

bool ShadowCastingShape::WasModified()
{
	CalculateCorners();
	return m_modified;
}

void ShadowCastingShape::ResetModifiedFlag()
{
	m_modified = false;
}

Coord2D ShadowCastingShape::GetCornerA()
{
	return m_corners[0];
}

Coord2D ShadowCastingShape::GetCornerB()
{
	return m_corners[1];
}

Coord2D ShadowCastingShape::GetCornerC()
{
	return m_corners[2];
}

Coord2D ShadowCastingShape::GetCornerD()
{
	return m_corners[3];
}
