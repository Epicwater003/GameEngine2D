#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures): vertices(vertices), indices(indices), textures(textures){
	vao->Bind();
	
	vbo = std::make_shared<VBO>(vertices);
	ebo = std::make_shared<EBO>(indices);
	
	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
	vao->LinkAttrib(*vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));
	
	vao->Unbind();
	vbo->Unbind();
	ebo->Unbind();
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices) : vertices(vertices), indices(indices) {
	vao->Bind();

	vbo = std::make_shared<VBO>(vertices);
	ebo = std::make_shared<EBO>(indices);

	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
	vao->LinkAttrib(*vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));

	vao->Unbind();
	vbo->Unbind();
	ebo->Unbind();
}


Mesh::Mesh(const Mesh& refMesh) {
	vertices = refMesh.vertices;
	indices  = refMesh.indices;
	textures = refMesh.textures;
	vao = refMesh.vao;
}


void Mesh::Draw(Shader& shader, Camera &camera, bool drawWireframed){
	shader.Activate();
	shader.setBool("hasTextures", GL_FALSE);
	vao->Bind();

	unsigned int numDiffuse  = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0, s = textures.size(); i < s; i++)
	{
		std::string num;
		std::string type = textures[i].getType();
		if      (type == "Diff")
			num = std::to_string(numDiffuse++);
		else if (type == "Spec")
			num = std::to_string(numSpecular++);
		
		textures[i].texUnit(shader, (type + "Tex" + num).c_str(), i);
		textures[i].Bind();
	}
	
	
	if (numDiffuse + numSpecular)
		shader.setBool("hasTextures", GL_TRUE);
	shader.setVec3("eViewPos"   , camera.position);
	shader.setMat4("eView"      , camera.getViewMatrix());
	shader.setMat4("eProjection", camera.getPerspProjectionMatrix());
	if (drawWireframed)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (const void*)0);

	for (unsigned int i = 0, s = textures.size(); i < s; i++) {
		textures[i].Unbind();
	}
	vao->Unbind();
}
void Mesh::Draw(Shader& shader, Camera& camera, glm::mat4 &model, bool drawWireframed) {
	shader.Activate();
	shader.setBool("hasTextures", GL_FALSE);


	vao->Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0, s = textures.size(); i < s; i++)
	{
		std::string num;
		std::string type = textures[i].getType();
		if (type == "Diff")
			num = std::to_string(numDiffuse++);
		else if (type == "Spec")
			num = std::to_string(numSpecular++);

		textures[i].texUnit(shader, (type + "Tex" + num).c_str(), i);
		textures[i].Bind();
	}
	

	if (numDiffuse + numSpecular)
		shader.setBool("hasTextures", GL_TRUE);
	shader.setVec3("eViewPos", camera.position);
	shader.setMat4("eView", camera.getViewMatrix());
	shader.setMat4("eProjection", camera.getPerspProjectionMatrix());
	shader.setMat4("eModel", model);
	if (drawWireframed)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);;

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (const void*)0);

	for (unsigned int i = 0, s = textures.size(); i < s; i++) {
		textures[i].Unbind();
	}
	vao->Unbind();
}





void Mesh::AttachTexture(Texture &texture) {
	textures.push_back(texture);
}
void Mesh::AttachTextures(std::vector<Texture> &textures) {
	this->textures = textures;
}
void Mesh::ReloadData() {
	vao->Bind();

	vbo = std::make_shared<VBO>(vertices);
	ebo = std::make_shared<EBO>(indices);

	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
	vao->LinkAttrib(*vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));

	vao->Unbind();
	vbo->Unbind();
	ebo->Unbind();
}