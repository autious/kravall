#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <gfx/Vertex.hpp>

class BGnomeImporter
{
public:
	struct Header
	{
		int numberOfMaterials;
		int numberOfVertices;
		int numberOfAnimations;
		int numberOfBones;
		int numberOfTriangles;
	};

	struct Bone
{
	int parentID;
	//std::vector<int> childIDs;
	std::string Name;
	int id;
	float offsetMatrix[4][4];
};



	struct Keyframe
	{
		float rotation[4];
		float position[3];
		float time;
		float scale[3];
	};

	struct Animation
	{
		int fps;
		std::string name;
		std::vector<std::vector<Keyframe>> keyframes; //per joint
		std::map<std::string,std::vector<Keyframe>> namedKeyframes;
	};

	Animation* animations;

	struct Vertex
	{
		//Materials
		int materialId;

		//Ordinary stuff
		float position[3];
		float normal[3];
		float uv[2];

		//Normalmapping
		float tangent[3];
		float binormal[3];

		//Animation
		float boneWeight[4];
		int boneIndex[4];
	};

	struct Material
	{
		//Identification
		std::string name;
		int id;

		//Numbers
		float ambient[3];
		float diffuse[3];
		float specularity[3];
		float specularityPower;
		float reflectivity;
		float transparency;
		bool alphaClip;

		//Textures
		std::string diffuseTexture;
		std::string normalMap;
		std::string alphaMap;
	};

	struct Mesh
	{
		Vertex* vertices;
		Material* materials;
	};

	Bone* bones;
	
	int Go(std::string filePath, std::vector<float> &mesh);
	int Go(std::string filePath, GFX::StaticVertex*& getMesh, int& size);
	int ReadASCII(std::fstream &file);

	int HeaderASCII(std::fstream &file);
	int MaterialASCII(std::fstream &file);
	int VertexASCII(std::fstream &file);
	int BoneASCII(std::fstream &file);
	int AnimationASCII(std::fstream &file);

	BGnomeImporter(void);
	~BGnomeImporter(void);


};

