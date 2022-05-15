#ifndef MESH_H
#define MESH_H

#include <memory>
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
	void Draw(Shader& shader, Camera& camera, glm::mat4& model, glm::vec3 color, bool drawWireframed = false);
	void Draw(Shader& shader, Camera& camera, glm::mat4 &model, bool drawWireframed = false);
	void Draw(Shader &shader, Camera &camera, bool drawWireframed = false);
	void AttachTexture(Texture &texture);
	void AttachTextures(std::vector<Texture>& texture);
	void ReloadData();
	std::vector<Vertex> getVertices() { return  vertices; }
	std::vector<GLuint> getIndices() { return indices; }
	std::vector<Texture> getTextures() { return textures; }
	void setVertices(std::vector<Vertex>& v) { 
		vertices = v;
		ReloadData(); // Требуется обновить буфер вершин
	}
	void setIndices(std::vector<GLuint>& i) {indices = i;} // Не требуется обновлять буфер вершин, см. реализацию Draw
	void setTextures(std::vector<Texture>& t) { textures = t; }
	
protected:

private:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	
	std::shared_ptr<VAO> vao = std::make_shared<VAO>();
};


#endif // !MESH_H

