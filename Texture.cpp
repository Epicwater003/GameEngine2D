#include "Texture.h"

Texture::Texture(const char* image, const char* texType, GLenum slot, GLenum format, GLenum pixelType){
	type = texType;
	int widthImg, heightImg, numColCh;
	
	stbi_set_flip_vertically_on_load(true);	                                       // Переворачиваем изображение вертикально(так того требует OpenGl)
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);  // Считываем изображение

	glGenTextures(1, &ID);	               // Создаем объект текстуры
	glActiveTexture(GL_TEXTURE0 + slot);   // Активируем текстуру
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);	   // Привязываем текстуру

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	   // Устанавливаем, как будет фильтроваться текстура
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	       // Поведение текстуры при несоответствии размеров
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes); // Загружаем данные
	
	glGenerateMipmap(GL_TEXTURE_2D);   // Создаём мипмапы

	
	stbi_image_free(bytes);	           // Очистка ресурсов
	glBindTexture(GL_TEXTURE_2D, 0);   // Отвязываем текстуру
}

Texture::~Texture() {
	glDeleteTextures(1, &ID);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.getID(), uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
