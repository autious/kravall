
#include "BGnomeImporter.hpp"

BGnomeImporter::Header header;
BGnomeImporter::Mesh mesh;


BGnomeImporter::BGnomeImporter(void)
{	
	std::cout << "Importer is initilized" << std::endl;
}

/*
The function Go will get you a mesh for now.

the function will add it to the float vector you've sent in, pushing it back

it is sorted by

position - normal - uv- tangent - binormal

You also need to send in a path for the file, as a string

It used fstream and ascii, so the function will be optimized in the future :)

I'll be here if you need to ask something :)

//Alice

*/
int BGnomeImporter::Go(std::string filePath, std::vector<float> &getMesh)
{

	std::cout << "reading file" << std::endl;

	std::fstream m_file;
	
	m_file.open(filePath, std::ios::in);
	if (m_file)
	{
		ReadASCII(m_file);
		m_file.close();

		for (int i = 0; i < header.numberOfVertices; i++)
		{
			for( int j = 0; j < 3; j++)
				getMesh.push_back(mesh.vertices[i].position[j]);
			for( int j = 0; j < 3; j++)
				getMesh.push_back(mesh.vertices[i].normal[j]);
			for( int j = 0; j < 2; j++)
				getMesh.push_back(mesh.vertices[i].uv[j]);
			for( int j = 0; j < 3; j++)
				getMesh.push_back(mesh.vertices[i].tangent[j]);
			for( int j = 0; j < 3; j++)
				getMesh.push_back(mesh.vertices[i].binormal[j]);
		}

		return 0;

	}
	m_file.close();
	std::cout << "ERROR" << std::endl;
	return 1;
}

int BGnomeImporter::Go(std::string filePath, GFX::StaticVertex*& getMesh, int& size)
{

	std::cout << "reading file" << std::endl;

	std::fstream m_file;

	m_file.open(filePath, std::ios::in);
	if (m_file)
	{
		ReadASCII(m_file);
		m_file.close();

		getMesh = new GFX::StaticVertex[header.numberOfVertices];
		size = header.numberOfVertices;
		for (int i = 0; i < header.numberOfVertices; i++)
		{
			GFX::StaticVertex v;
			for (int j = 0; j < 3; j++)
				getMesh[i].position[j] = mesh.vertices[i].position[j];
			for (int j = 0; j < 3; j++)
				getMesh[i].normal[j] = mesh.vertices[i].normal[j];
			for (int j = 0; j < 2; j++)
			{
				//if (j == 1)
				//	getMesh[i].uv[j] = 1 - mesh.vertices[i].uv[j];
				//else
					getMesh[i].uv[j] = mesh.vertices[i].uv[j];
			}
			for (int j = 0; j < 3; j++)
				getMesh[i].tangent[j] = mesh.vertices[i].tangent[j];
			for (int j = 0; j < 3; j++)
				getMesh[i].binormal[j] = mesh.vertices[i].binormal[j];

			getMesh[i].position[3] = 1.0f;
			getMesh[i].normal[3] = 0.0f;
			getMesh[i].tangent[3] = 0.0f;
			getMesh[i].tangent[3] = 0.0f;
		}

		return 0;

	}
	m_file.close();
	std::cout << "ERROR" << std::endl;
	return 1;
}


int BGnomeImporter::ReadASCII(std::fstream &file)
{
	
	std::string m_line;
	HeaderASCII(file);
	mesh.materials	= new Material[header.numberOfMaterials];
	mesh.vertices	= new Vertex[header.numberOfVertices]; 
	bones			= new Bone[header.numberOfBones];
	animations		= new Animation[header.numberOfAnimations];
	MaterialASCII(file);
	VertexASCII(file);
	BoneASCII(file);
	AnimationASCII(file);

	return 0;
}

int BGnomeImporter::VertexASCII(std::fstream &file)
{
	std::cout << "Vertices " << header.numberOfVertices << " " << header.numberOfTriangles << std::endl;
	std::string m_line;
	int m_materialId;

	Vertex vertex;

	file >> m_line;
	file >> m_line >> m_materialId; //todo: make dynamic solution for mutiple materials
	vertex.materialId = m_materialId;
	for (int i = 0; i < header.numberOfVertices; ++i)
	{
		file >> m_line;
		file >> vertex.position[0]	 >> vertex.position[1]	 >> vertex.position[2];
		file >> m_line;
		file >> vertex.normal[0]	 >> vertex.normal[1]	 >> vertex.normal[2];
		file >> m_line;
		file >> vertex.tangent[0]	 >> vertex.tangent[1]	 >> vertex.tangent[2];
		file >> m_line;
		file >> vertex.binormal[0]	 >> vertex.binormal[1]	 >> vertex.binormal[2];
		file >> m_line;
		file >> vertex.uv[0]		 >> vertex.uv[1];
		file >> m_line;
		file >> vertex.boneWeight[0] >> vertex.boneWeight[1] >> vertex.boneWeight[2] >> vertex.boneWeight[3];
		file >> m_line;
		file >> vertex.boneIndex[0]	 >> vertex.boneIndex[1]	 >> vertex.boneIndex[2]	 >> vertex.boneIndex[3];

		mesh.vertices[i] = vertex;
	}
	//std::cout << mesh.vertices[0].boneIndex[0] << std::endl;
	return 0;
}

int BGnomeImporter::MaterialASCII(std::fstream & file)
{
	std::cout << "Materials " << header.numberOfMaterials << std::endl;
	std::string m_line;

	Material material;	

	file >> m_line;
	for (int i = 0; i < header.numberOfMaterials; ++i)
	{
		file >> material.name;
		file >> m_line;
		file >> material.ambient[0]		>> material.ambient[1]		>> material.ambient[2];
		file >> m_line;
		file >> material.diffuse[0]		>> material.diffuse[1]		>> material.diffuse[2];
		file >> m_line;
		file >> material.specularity[0] >> material.specularity[1]	>> material.specularity[2];
		file >> m_line;
		file >> material.specularityPower;
		file >> m_line;
		file >> material.reflectivity;
		file >> m_line;
		file >> material.transparency;
		file >> m_line;
		file >> material.alphaClip;
		file >> m_line;
		file >> material.diffuseTexture;
		file >> m_line;
		file >> material.normalMap;
		file >> m_line;
		file >> material.alphaMap;

		mesh.materials[i] = material;
		std::cout << mesh.materials[i].name << std::endl;
		std::cout << mesh.materials[i].diffuseTexture << std::endl;
	}

	return 0;
}

int BGnomeImporter::HeaderASCII(std::fstream &file)
{
	std::string m_line;
	file >> m_line;

	file >> m_line;
	file >> header.numberOfMaterials;
	file >> m_line;
	file >> header.numberOfVertices;
	file >> m_line;
	file >> header.numberOfTriangles;
	file >> m_line;
	file >> header.numberOfBones;
	file >> m_line;
	file >> header.numberOfAnimations;
	
	
	
	std::cout << header.numberOfMaterials << std::endl;
	std::cout << header.numberOfVertices << std::endl;
	std::cout << header.numberOfTriangles << std::endl;
	std::cout << header.numberOfBones << std::endl;
	std::cout << header.numberOfAnimations << std::endl;

	return 0;
}

int BGnomeImporter::BoneASCII(std::fstream &file)
{
	std::string m_line;
	file >> m_line;

	Bone bone;

	for (int k = 0; k < header.numberOfBones; k++)
	{
		file >> bone.Name;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				file >> bone.offsetMatrix[i][j];
				if (k == 1)
					std::cout << bone.offsetMatrix[i][j] << " ";
			}
		bones[k] = bone;
		if (k == 1)
			std::cout << std::endl;
	}
	
	file >> m_line;
	for (int i = 0; i < header.numberOfBones; i++)
	{
		file >> m_line;
		file >> bones[i].parentID;
		bones[i].id = i;
	}
	return 1;
}

int BGnomeImporter::AnimationASCII(std::fstream &file)
{
	std::string m_line;
	file >> m_line;
	file >> m_line;
	Animation animation;
	

	for (int i = 0; i < header.numberOfAnimations; ++i)
	{
		file >> m_line;
		file >> animation.name;
		file >> m_line;

		for (int j = 0; j < header.numberOfBones; ++j)
		{
			int m_noKeys;
			file >> m_line >> m_line >> m_noKeys;
			file >> m_line;
			
			std::vector<Keyframe> keys; //TODO: unvectorize //c:\users\alice\downloads\flag.gnome
			for (int k = 0; k < m_noKeys; k++)
			{
				Keyframe keyframe;
				file >> m_line;
				file >> keyframe.time;
				file >> m_line;
				file >> keyframe.position[0] >> keyframe.position[1] >> keyframe.position[2];
				file >> m_line;
				file >> keyframe.scale[0]	 >> keyframe.scale[1]	 >> keyframe.scale[2]; 
				file >> m_line;
				file >> keyframe.rotation[0] >> keyframe.rotation[1] >> keyframe.rotation[2] >> keyframe.rotation[3];


				keys.push_back(keyframe);
			}
			file >> m_line;

			animations[i].keyframes.push_back(keys);
			animations[i].namedKeyframes[bones[j].Name] = keys;
		}
	}

	return 1;
}

BGnomeImporter::~BGnomeImporter(void)
{

}
