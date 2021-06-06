#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/hash.hpp"
#include <iostream>
#include <vector>
#include "Renderer/Shader.h"
#include "Core/keycodes.h"
#include <stb_image.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Renderer/Transform.h"
#include <map>
#include <algorithm>

#include "rapidjson/document.h"
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#define HELL_RED  glm::vec3(1, 0, 0)
#define HELL_WHITE  glm::vec3(1, 1, 1)
#define HELL_YELLOW  glm::vec3(1, 1, 0)
#define HELL_BLUE  glm::vec3(0, 0, 1)

//#define SCR_WIDTH 1280//1920//2880 //1920//1280
//#define SCR_HEIGHT 720//1080//1620//1080//720
#define SCR_WIDTH 960.0f// 1920.0f//1920//2880 //1920//1280
#define SCR_HEIGHT 540.0f//1080.0f//1080//1620//1080//720
#define NEAR_PLANE 0.01f
#define FAR_PLANE 50.0f
#define HELL_PI	(float)3.14159265358979323846

#define GRID_SIZE (int)32
#define MAP_HEIGHT (int)36
#define MAP_WIDTH (int)60

enum FileType { FBX, OBJ, PNG, JPG, UNKNOWN };

struct Vertex2D {
	glm::vec2 Position;
	glm::vec2 TexCoords;
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	unsigned int BlendingIndex[4];
	glm::vec4 BlendingWeight;
	unsigned int MaterialID;

	bool operator==(const Vertex& other) const {
		return Position == other.Position && Normal == other.Normal && TexCoords == other.TexCoords;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.Position) ^ (hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.TexCoords) << 1);
		}
	};
}

struct Line {
	glm::vec3 start_pos;
	glm::vec3 end_pos;
	glm::vec3 start_color;
	glm::vec3 end_color;
};


#define ToRadian(x) (float)(((x) * HELL_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / HELL_PI)) 

#define POSITION_LOCATION    0
#define NORMAL_LOCATION		 1
#define TEX_COORD_LOCATION   2
#define TANGENT_LOCATION     3
#define BITANGENT_LOCATION   4
#define BONE_ID_LOCATION     5
#define BONE_WEIGHT_LOCATION 6

enum VB_TYPES {
	POS_VB,
	TEXCOORD_VB,
};

enum Tile {
	DIRT,
	WALL,
};

struct Cell {
	Tile tile;
	int edge_id[4];
	bool edge_exists[4];

	bool IsObstacle() {
		return (tile == Tile::WALL);
	}
};

using JSONDocument = rapidjson::GenericDocument<rapidjson::UTF8<>>;
using JSONValue = rapidjson::GenericValue<rapidjson::UTF8<>>;

struct Edge {
	int sX, sY;
	int eX, eY;
};

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

#define RENDER_MODE_UNLIT 0
#define RENDER_MODE_LIT 1
#define RENDER_MODE_LIT_WITH_LINE_OF_SIGHT 2
#define RENDER_MODE_FUSION_WORLD_ONLY 3
#define RENDER_MODE_FUSION_WORLD_PLUS_LIGHTING 3

/*
bool operator==(const glm::vec2& other) const {
	return x == other.x && y == other.y;
}*/

struct AABB {
	int lowerX;
	int upperX;
	int lowerY;
	int upperY;
};

struct LineIntersectionResult {
	bool intersectionFound = false;
	int x = 0;
	int y = 0;
};

#define ROOM_LIGHT 0
#define SPOT_LIGHT 1
#define GRATE_LIGHT 2
#define LIGHT_TYPE_COUNT 3