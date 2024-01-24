#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <string>

class Texture
{
public:
	Texture();
	Texture(const char* _path);
	~Texture();

	void LoadFromFile(const char* _path);
	void Refresh();

	unsigned int id;
	std::string name;
private:
	int width;
	int height;
	int nrChannels;
	std::string path;
};

#endif // !TEXTURE_H