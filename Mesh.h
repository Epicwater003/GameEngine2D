#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include "Buffer.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"


class Mesh
{
public:
	
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
	//Mesh();
	Mesh(const Mesh& refMesh);
	void Draw(Shader& shader, Camera& camera, glm::mat4 &model);
	void Draw(Shader &shader, Camera &camera);
	void AttachTexture(Texture &texture);
	void AttachTextures(std::vector<Texture>& texture);
	void ReloadData();
	std::vector<Vertex> getVertices() { return  vertices; }
private:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	
	VAO * vao = new VAO();
};


#endif // !MESH_H

