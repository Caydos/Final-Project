#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <string>

class Texture
{
public:
	void LoadFromFile(const char* _path);
	Texture();
	Texture(const char* _path);
	~Texture();

	unsigned int id;
	std::string name;
private:
	int width;
	int height;
	int nrChannels;
};

#endif // !TEXTURE_H