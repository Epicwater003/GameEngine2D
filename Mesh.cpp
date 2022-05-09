#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures): vertices(vertices), indices(indices), textures(textures){
	vao->Bind();
	
	VBO VBO(vertices);
	EBO EBO(indices);
	
	vao->LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao->LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao->LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	
	vao->Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices) : vertices(vertices), indices(indices) {
	vao->Bind();

	VBO VBO(vertices);
	EBO EBO(indices);

	vao->LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao->LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao->LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	vao->Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


Mesh::Mesh(const Mesh& refMesh) {
	vertices = refMesh.vertices;
	indices  = refMesh.indices;
	textures = refMesh.textures;
	vao = new VAO();
}


void Mesh::Draw(Shader& shader, Camera &camera){
	shader.Activate();
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
	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
void Mesh::Draw(Shader& shader, Camera& camera, glm::mat4 &model) {
	shader.Activate();
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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); TODO: ƒобавить возможность отрисовки лини€ми
	//glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}



void Mesh::AttachTexture(Texture &texture) {
	textures.push_back(texture);
}
void Mesh::AttachTextures(std::vector<Texture> &textures) {
	this->textures = textures;
}
void Mesh::ReloadData() {
	vao->Bind();

	VBO VBO(vertices);
	EBO EBO(indices);

	vao->LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao->LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao->LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	vao->Unbind();
	VBO.Unbind();
	EBO.Unbind();
}