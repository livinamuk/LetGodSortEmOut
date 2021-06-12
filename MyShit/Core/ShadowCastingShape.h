#pragma once
#include "Header.h"

class ShadowCastingShape
{
public: // fields
	std::string m_name;
	int m_fixedVale = -1;

private:
	Coord2D m_corners[4];	
	glm::vec2 m_position = glm::vec2(0, 0);
	int m_width = 0;
	int m_height = 0;
	int m_angle = 1;
	bool m_modified = true;


public: // methods
	ShadowCastingShape();
	ShadowCastingShape(const char* name, int x, int y, int width, int height, int angle);
	ShadowCastingShape(int fixedValue, int width, int height, int angle);
	bool HasFixedValuePair();
	void CalculateCorners();
	void SetAngle(int angle);
	void SetPosition(int x, int y);
	void SetWidth(int width);
	void SetHeight(int height);
	int GetAngle();
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	bool WasModified();
	void ResetModifiedFlag();
	Coord2D GetCornerA();
	Coord2D GetCornerB();
	Coord2D GetCornerC();
	Coord2D GetCornerD();
};

// next optimisation: only recalculate lights if this shape has moved or rotated