//#include "hellpch.h"
#include "Util.h"
#include <Windows.h>
#include <random>

int Util::StringToInt(std::string str)
{
	int num;
	std::stringstream ss;
	ss << str;
	ss >> num;
	return num;
}

void Util::Log(std::string text)
{
	std::ofstream outfile;
	outfile.open("log.txt", std::ios_base::app); // append instead of overwrite
	outfile << text << "\n";
}


bool Util::GetRayPlaneIntersecion(const glm::vec3& origin, const glm::vec3& dir, const glm::vec3& plane_point, const glm::vec3& normal, float& out)
{
	float d = dot(dir, normal);
	if (d == 0) {
		return false;
	}
	d = dot(plane_point - origin, normal) / d;
	out = d;
	return true;
}


glm::vec3 Util::GetMouseRay(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, int mouseX, int mouseY, int screenWidth, int screenHeight)
{
	float x = (2.0f * mouseX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / screenHeight;
	glm::vec3 ray_nds = glm::vec3(x, y, 1.0f);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = inverse(projectionMatrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec3 ray_wor = glm::normalize(glm::vec3(inverse(viewMatrix) * ray_eye));
	return ray_wor;
}

void Util::PrintVec3(glm::vec3 v)
{
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}

void Util::PrintMat4(glm::mat4 m)
{
	std::cout << "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ")\n";
	std::cout << "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ")\n";
	std::cout << "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ")\n";
	std::cout << "(" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ")\n";
}

std::string Util::FileNameFromPath(std::string filepath)
{
	// Remove directory if present.
	// Do this before extension removal incase directory has a period character.
	const size_t last_slash_idx = filepath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filepath.erase(0, last_slash_idx + 1);
	}

	// Remove extension if present.
	const size_t period_idx = filepath.rfind('.');
	if (std::string::npos != period_idx)
	{
		filepath.erase(period_idx);
	}
	return filepath;
}
FileType Util::FileTypeFromPath(std::string filepath)
{
	int index = filepath.find_last_of('.') + 1;
	std::string extension = filepath.substr(index);
	//std::cout << "'" << extension << "'\n";

	if (extension == "PNG" || extension == "png")
		return FileType::PNG;
	if (extension == "JPG" || extension == "jpg")
		return FileType::JPG;
	if (extension == "FBX" || extension == "fbx")
		return FileType::FBX;
	if (extension == "OBJ" || extension == "obj")
		return FileType::OBJ;
	else
		return FileType::UNKNOWN;
}

void Util::SetNormalsAndTangentsFromVertices(Vertex* vert0, Vertex* vert1, Vertex* vert2)
{
	// above can be used to replace the shit below here. its the same.

	// Shortcuts for UVs
	glm::vec3& v0 = vert0->Position;
	glm::vec3& v1 = vert1->Position;
	glm::vec3& v2 = vert2->Position;
	glm::vec2& uv0 = vert0->TexCoords;
	glm::vec2& uv1 = vert1->TexCoords;
	glm::vec2& uv2 = vert2->TexCoords;

	// Edges of the triangle : postion delta. UV delta
	glm::vec3 deltaPos1 = v1 - v0;
	glm::vec3 deltaPos2 = v2 - v0;
	glm::vec2 deltaUV1 = uv1 - uv0;
	glm::vec2 deltaUV2 = uv2 - uv0;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
	glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

	glm::vec3 normal = Util::NormalFromTriangle(vert0->Position, vert1->Position, vert2->Position);

	vert0->Normal = normal;
	vert1->Normal = normal;
	vert2->Normal = normal;

	vert0->Tangent = tangent;
	vert1->Tangent = tangent;
	vert2->Tangent = tangent;

	vert0->Bitangent = bitangent;
	vert1->Bitangent = bitangent;
	vert2->Bitangent = bitangent;
}

glm::vec3 Util::NormalFromTriangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2)
{
	return glm::normalize(glm::cross(pos1 - pos0, pos2 - pos0));
}


std::string Util::BoolToString(bool boolean)
{
	if (boolean)
		return "TRUE";
	else
		return "FALSE";
}

glm::mat4 Util::Mat4FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr)
{
	glm::mat4 m;

	m[0][0] = arr[0].GetFloat();
	m[0][1] = arr[1].GetFloat();
	m[0][2] = arr[2].GetFloat();
	m[0][3] = arr[3].GetFloat();
	m[1][0] = arr[4].GetFloat();
	m[1][1] = arr[5].GetFloat();
	m[1][2] = arr[6].GetFloat();
	m[1][3] = arr[7].GetFloat();
	m[2][0] = arr[8].GetFloat();
	m[2][1] = arr[9].GetFloat();
	m[2][2] = arr[10].GetFloat();
	m[2][3] = arr[11].GetFloat();
	m[3][0] = arr[12].GetFloat();
	m[3][1] = arr[13].GetFloat();
	m[3][2] = arr[14].GetFloat();
	m[3][3] = arr[15].GetFloat();

	return m;
}

glm::vec3 Util::Vec3FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr)
{
	return glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
}

glm::vec3 Util::GetTranslationFromMatrix(glm::mat4 matrix)
{
	return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
}


void Util::InterpolateQuaternion(glm::quat& Out, const glm::quat& Start, const glm::quat& End, float pFactor)
{
	// calc cosine theta
	float cosom = Start.x * End.x + Start.y * End.y + Start.z * End.z + Start.w * End.w;

	// adjust signs (if necessary)
	glm::quat end = End;
	if (cosom < static_cast<float>(0.0))
	{
		cosom = -cosom;
		end.x = -end.x;   // Reverse all signs
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	// Calculate coefficients
	float sclp, sclq;
	if ((static_cast<float>(1.0) - cosom) > static_cast<float>(0.0001)) // 0.0001 -> some epsillon
	{
		// Standard case (slerp)
		float omega, sinom;
		omega = std::acos(cosom); // extract theta from dot product's cos theta
		sinom = std::sin(omega);
		sclp = std::sin((static_cast<float>(1.0) - pFactor) * omega) / sinom;
		sclq = std::sin(pFactor * omega) / sinom;
	}
	else
	{
		// Very close, do linear interp (because it's faster)
		sclp = static_cast<float>(1.0) - pFactor;
		sclq = pFactor;
	}

	Out.x = sclp * Start.x + sclq * end.x;
	Out.y = sclp * Start.y + sclq * end.y;
	Out.z = sclp * Start.z + sclq * end.z;
	Out.w = sclp * Start.w + sclq * end.w;
}

glm::mat4 Util::Mat4InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
	/*	glm::mat4 m = glm::mat4(1);
		m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
		m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
		m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
		m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
		return m;*/

	return glm::scale(glm::mat4(1.0), glm::vec3(ScaleX, ScaleY, ScaleZ));
}

glm::mat4 Util::Mat4InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	glm::mat4 rx = glm::mat4(1);
	glm::mat4 ry = glm::mat4(1);
	glm::mat4 rz = glm::mat4(1);

	const float x = ToRadian(RotateX);
	const float y = ToRadian(RotateY);
	const float z = ToRadian(RotateZ);

	rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
	rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
	rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
	rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

	ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
	ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
	ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
	ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

	rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
	rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
	rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
	rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

	return rz * ry * rx;
}

glm::mat4 Util::Mat4InitTranslationTransform(float x, float y, float z)
{
	return  glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}



bool Util::StrCmp(const char* queryA, const char* queryB)
{
	if (strcmp(queryA, queryB) == 0)
		return true;
	else
		return false;
}

const char* Util::CopyConstChar(const char* text)
{
	char* b = new char[strlen(text) + 1]{};
	std::copy(text, text + strlen(text), b);
	return b;
}

