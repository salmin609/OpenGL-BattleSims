//
// Created by pushpak on 10/5/18.
//

#ifndef SIMPLE_OBJ_SCENE_MESH_H
#define SIMPLE_OBJ_SCENE_MESH_H

#include <vector>

#include <glm/glm.hpp>


class Mesh
{
public:
    friend class OBJReader;

    // Get attribute values buffer
    GLfloat *getVertexBuffer();             // attribute 0
    GLfloat *getVertexNormals();            // attribute 1
    GLfloat *getVertexUVs();                // attribute 2

    GLfloat *getVertexNormalsForDisplay();  // attribute 0

    unsigned int getVertexBufferSize();
    unsigned int getVertexCount();
    unsigned int getVertexNormalCount();

    // Get vertex index buffer
    GLuint *getIndexBuffer();
    unsigned int getIndexBufferSize();
    unsigned int getTriangleCount();

    glm::vec3   getModelScale();
    glm::vec3   getModelCentroid();
    glm::vec3   getCentroidVector( glm::vec3 vVertex );


    GLfloat  &  getNormalLength();
    void setNormalLength( GLfloat nLength );

    // initialize the data members
    void initData();

    // calculate vertex normals
    int calcVertexNormals(GLboolean bFlipNormals = false);

    // calculate the "display" normals
    void calcVertexNormalsForDisplay(GLboolean bFlipNormals = false);

    // calculate texture coordinates
    enum UVType { PLANAR_UV = 0,
                  CYLINDRICAL_UV,
                  SPHERICAL_UV,
                  CUBE_MAPPED_UV};

    int         calcUVs( Mesh::UVType uvType = Mesh::PLANAR_UV );
    glm::vec2   calcCubeMap( glm::vec3 vEntity );

private:
    std::vector<glm::vec3>    vertexBuffer;
    std::vector<GLuint>       vertexIndices;
    std::vector<glm::vec2>    vertexUVs;
    std::vector<glm::vec3>    vertexNormals, vertexNormalDisplay;

    glm::vec3               boundingBox[2];
    GLfloat                 normalLength;

};


#endif //SIMPLE_OBJ_SCENE_MESH_H
