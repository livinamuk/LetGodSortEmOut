#include "Light.h"
#include "Camera2D.h"
#include "WorldMap.h"
#include "CoreGL.h"

int Light::s_lightsDrawn;

Light::Light()
{
}

Light::Light(std::string name, glm::vec2 position, glm::vec3 color, float scale, const char* textureName, float brightness, float angle, bool castShadows)
{
    m_name = name;
    m_position = position;
    m_color = color;
    m_scale = scale;
    m_brightness = brightness;
    m_angle = angle;
    m_castShadows = castShadows;

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    SetTexture(textureName);
}

void Light::DrawShadowCastingLight(Shader* shader)
{
    // If light range is smaller than 1 pixel, skip
    if (m_texture->height * m_scale < 1 || m_texture->width * m_scale < 1)
        return;

    // Light position
    float srcX = m_position.x;
    float srcY = m_position.y;

    // Bail if light source is inside obstacle.
    int gridX = srcX / GRID_SIZE;
    int gridY = srcY / GRID_SIZE;
    if (WorldMap::IsTileObstacle(WorldMap::s_map[gridX][gridY].tile))
        return;

    AABB aabb = this->GetAABB();

    // Check if any Shadow Casting Shapes moved within this lights range
    for (auto& it : Scene::s_shadowCastingShape)
    {
        ShadowCastingShape& shape = it.second;

        if (m_visibilityPolygonNeedsUpdate == false && shape.WasModified()) {
            if (aabb.ContainsPoint(shape.GetCornerA()) ||
                aabb.ContainsPoint(shape.GetCornerB()) ||
                aabb.ContainsPoint(shape.GetCornerC()) ||
                aabb.ContainsPoint(shape.GetCornerD())) {

                m_visibilityPolygonNeedsUpdate = true;
                Renderer::s_testCounter++;
            }
        }
    }

    if (m_visibilityPolygonNeedsUpdate)
    {
        WorldMap::BuildEdgeMapFromWorldMap(&aabb, Renderer::s_wallEdgeInset);
        m_lightVisibilityPolygonPoints = WorldMap::CalculateVisibilityPolygon(srcX, srcY, 1.0f);
        m_visibilityPolygonNeedsUpdate = false;

        std::vector<glm::vec2> vertices;

        float ndc_srcx = srcX / SCR_WIDTH * 2 - 1;
        float ndc_srcy = (SCR_HEIGHT - srcY) / SCR_HEIGHT * 2 - 1;
        vertices.push_back(glm::vec2(ndc_srcx, ndc_srcy));

        for (int i = 0; i < m_lightVisibilityPolygonPoints.size(); i++) {
            float x1 = std::get<1>(m_lightVisibilityPolygonPoints[i]);
            float y1 = std::get<2>(m_lightVisibilityPolygonPoints[i]);
            float ndc_x = x1 / SCR_WIDTH * 2 - 1;
            float ndc_y = (SCR_HEIGHT - y1) / SCR_HEIGHT * 2 - 1;
            vertices.push_back(glm::vec2(ndc_x, ndc_y));
        }
        if (m_lightVisibilityPolygonPoints.size() > 0) {
            float x1 = std::get<1>(m_lightVisibilityPolygonPoints[0]);
            float y1 = std::get<2>(m_lightVisibilityPolygonPoints[0]);
            float ndc_x = x1 / SCR_WIDTH * 2 - 1;
            float ndc_y = (SCR_HEIGHT - y1) / SCR_HEIGHT * 2 - 1;
            vertices.push_back(glm::vec2(ndc_x, ndc_y));
            m_vertexCount = vertices.size();
        }

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    }

    float x = m_position.x - Camera2D::s_scrollX + SCR_WIDTH / 2;
    float y = m_position.y - Camera2D::s_scrollY + SCR_HEIGHT / 2;
    shader->setVec2("lightScreenSpacePixelPosition", glm::vec2(x, y));
    shader->setVec3("color", m_color);
    shader->setFloat("brightness", m_brightness);
    shader->setFloat("spriteWidth", m_texture->width);
    shader->setFloat("spriteHeight", m_texture->height);
    shader->setFloat("lightScale", m_scale);
    shader->setFloat("angle", m_angle * HELL_PI / 180);

    glBindTexture(GL_TEXTURE_2D, m_texture->ID);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertexCount);
    s_lightsDrawn++;
}


void Light::NextType()
{
    if (++m_type == LIGHT_TYPE_COUNT)
        m_type = 0;
    SetTextureFromType();
}


void Light::SetTextureFromType()
{
    if (m_type == ROOM_LIGHT)
        m_texture = Texture::GetTexByName("Light_Room.png");
    else if (m_type == SPOT_LIGHT)
        m_texture = Texture::GetTexByName("Light_Spot.png");
    else if (m_type == GRATE_LIGHT)
        m_texture = Texture::GetTexByName("Light_Grate.png");
}

bool Light::IsInScreenBounds()
{
    AABB aabb;
    aabb.lowerX = m_position.x - (m_texture->width / 2) * m_scale;
    aabb.upperX = m_position.x + (m_texture->width / 2) * m_scale;
    aabb.lowerY = m_position.y - (m_texture->height / 2) * m_scale;
    aabb.upperY = m_position.y + (m_texture->height / 2) * m_scale;

    // Return false if outside camera bounds duuuh dikkead
   /* AABB aabbCamera = Camera2D::GetSCreenAABB();
    if (aabb.upperX < aabbCamera.lowerX ||
        aabb.lowerX > aabbCamera.upperX ||
        aabb.upperY < aabbCamera.lowerY ||
        aabb.lowerY > aabbCamera.upperY)
        return false;*/
    AABB aabbCamera = Camera2D::GetSCreenAABB();
    if (aabb.upperX < aabbCamera.lowerX ||
        aabb.lowerX > aabbCamera.upperX ||
        aabb.upperY < aabbCamera.lowerY ||
        aabb.lowerY > aabbCamera.upperY)
        return false;
    // or return true pussyy
    else
        return true;
}

bool Light::IsShadowCasting()
{
    return m_castShadows;
}

void Light::SetCastShadows(bool value)
{
    m_castShadows = true;
}

bool Light::IsPairedToObject()
{
    return m_pairedObjectData.isPaired;
}

void Light::SetPosition(float x, float y)
{
    m_position = glm::vec2(x, y);
    m_visibilityPolygonNeedsUpdate = true;
}

void Light::SetScale(float scale)
{
    m_scale = scale;
    if (m_scale < 0)
        m_scale = 0;
}

float Light::GetScale()
{
    return m_scale;
}

float Light::GetX()
{
    return m_position.x;
}

float Light::GetY()
{
    return m_position.y;
}

AABB Light::GetAABB()
{
    m_aabb.lowerX = m_position.x - (m_texture->width / 2) * m_scale;
    m_aabb.upperX = m_position.x + (m_texture->width / 2) * m_scale;
    m_aabb.lowerY = m_position.y - (m_texture->height / 2) * m_scale;
    m_aabb.upperY = m_position.y + (m_texture->height / 2) * m_scale;
    return m_aabb;
}

void Light::SetTexture(std::string textureName)
{
    m_texture = Texture::GetTexByName(textureName);
}

const char* Light::GetTextureName()
{
    if (m_texture)
        return m_texture->name.c_str();
    else
        return "undefined";
}