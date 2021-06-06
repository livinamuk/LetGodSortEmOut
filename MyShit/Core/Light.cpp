#include "Light.h"
#include "Camera2D.h"
#include "WorldMap.h"
#include "CoreGL.h"

Light::Light()
{
}

Light::Light(std::string name, glm::vec2 position, glm::vec3 lightColor, float scale, int type, float strength, int rotate)
{
    m_name = name;
    m_position = position;
    m_color = lightColor;
    m_scale = scale;
    m_type = type;
    m_strength = strength;
    m_rotate = rotate;

    SetTextureFromType();
}

void Light::DrawSolidPolygon(Shader* shader)
{
    /*AABB aabb;
    aabb.lowerX = m_position.x - (m_texture->width / 2) * m_scale;
    aabb.upperX = m_position.x + (m_texture->width / 2) * m_scale;
    aabb.lowerY = m_position.y - (m_texture->height / 2) * m_scale;
    aabb.upperY = m_position.y + (m_texture->height / 2) * m_scale;

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_fboID);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glClearTexImage(s_polygonTextureID, 0, GL_RGBA, GL_FLOAT, 0);

    // Light
    float srcX = m_position.x;
    float srcY = m_position.y;

    float testX = srcX + Camera2D::s_scrollX - SCR_WIDTH / 2;
    float testY = srcY + Camera2D::s_scrollY - SCR_HEIGHT / 2;

    WorldMap::BuildEdgeMapFromWorldMap(&aabb);
    m_lightVisibilityPolygonPoints = WorldMap::CalculateVisibilityPolygon(srcX, srcY, 1.0f);;

    if (Input::s_keyDown[HELL_KEY_U])
        return;

    std::vector<glm::vec2> vertices;
    glDisable(GL_CULL_FACE);

    shader->use();
    shader->setVec3("color", glm::vec3(1));

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
    float x1 = std::get<1>(m_lightVisibilityPolygonPoints[0]);
    float y1 = std::get<2>(m_lightVisibilityPolygonPoints[0]);
    float ndc_x = x1 / SCR_WIDTH * 2 - 1;
    float ndc_y = (SCR_HEIGHT - y1) / SCR_HEIGHT * 2 - 1;
    vertices.push_back(glm::vec2(ndc_x, ndc_y));


    unsigned int VAO;
    unsigned int VBO;


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    glPointSize(5);
    glBindVertexArray(VAO);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);*/
}

void Light::DrawOutline(Shader* shader)
{
    /*int sourceFBO = s_fboID;
    int sourceTextureID = s_polygonTextureID;
    int destinationFBO = s_fboID;
    int sourceColorAttachment = GL_COLOR_ATTACHMENT0;
    int destinationColorAttachment = GL_COLOR_ATTACHMENT1;
    int outlineSize = -1;

    shader->use();
    glDisable(GL_BLEND);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO);
    glReadBuffer(sourceColorAttachment);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sourceTextureID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationColorAttachment);
    glDrawBuffer(destinationColorAttachment);

    Quad2D::RenderFullScreenQuad(shader);    */
}

void Light::DrawShadowCastingLight(Shader* shader, int gBuffgerID)
{
    AABB aabb;
    aabb.lowerX = m_position.x - (m_texture->width / 2) * m_scale;
    aabb.upperX = m_position.x + (m_texture->width / 2) * m_scale;
    aabb.lowerY = m_position.y - (m_texture->height / 2) * m_scale;
    aabb.upperY = m_position.y + (m_texture->height / 2) * m_scale;

    // Light
    float srcX = m_position.x;
    float srcY = m_position.y;

    float testX = srcX + Camera2D::s_scrollX - SCR_WIDTH / 2;
    float testY = srcY + Camera2D::s_scrollY - SCR_HEIGHT / 2;

    WorldMap::BuildEdgeMapFromWorldMap(&aabb);
    m_lightVisibilityPolygonPoints = WorldMap::CalculateVisibilityPolygon(srcX, srcY, 1.0f);;

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
    float x1 = std::get<1>(m_lightVisibilityPolygonPoints[0]);
    float y1 = std::get<2>(m_lightVisibilityPolygonPoints[0]);
    float ndc_x = x1 / SCR_WIDTH * 2 - 1;
    float ndc_y = (SCR_HEIGHT - y1) / SCR_HEIGHT * 2 - 1;
    vertices.push_back(glm::vec2(ndc_x, ndc_y));

    shader->use();
    shader->setVec3("color", m_color);
    shader->setFloat("strength", m_strength);
    float x = m_position.x - Camera2D::s_scrollX + SCR_WIDTH / 2;
    float y = m_position.y - Camera2D::s_scrollY + SCR_HEIGHT / 2;
    shader->setVec2("lightScreenSpacePixelPosition", glm::vec2(x, y));
    shader->setFloat("spriteWidth", m_texture->width);
    shader->setFloat("spriteHeight", m_texture->height);
    shader->setFloat("lightScale", m_scale);
    shader->setInt("rotate", m_rotate);

    glBindTexture(GL_TEXTURE_2D, m_texture->ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}


void Light::NextType()
{
    if (++m_type == LIGHT_TYPE_COUNT)
        m_type = 0;
    SetTextureFromType();
}

void Light::RotateLight()
{
    if (++m_rotate == 4)
        m_rotate = 0;
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
    return (m_type == ROOM_LIGHT ||
        m_type == SPOT_LIGHT);
}

bool Light::IsPairedToObject()
{
    return m_pairedObjectData.isPaired;
}
