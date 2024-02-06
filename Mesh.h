// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <string>
#include "Collision.h"

//WSB moved from Mesh.cpp 2023-03-07 This has to be here so LoadAssimpMeshes can see and use it.
//I can't just make LoadAssimpMeshes a member of Mesh, as it loads *multiple* Meshes. I *could*
//put LoadAssimpMeshes in Mesh.cpp. But it makes sense to put this here I think.
union Vertex
{
	float f;
	uint8_t b[4];
};

class Mesh
{
	//WSB added 2023-03-10. Better to avoid "friend," but I'm trying to avoid altering Mesh much as I can
	friend void LoadAssimpMeshes(std::vector<Mesh*>& meshes, class Game* game, const char* fName, class Texture* t, const char* nameForDebugging);
	friend class IcosphereActor;
public:
	Mesh();
	~Mesh();
	// Load/unload mesh
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	// Get the vertex array associated with this mesh
	class VertexArray* GetVertexArray() { return mVertexArray; }
	// Get a texture from specified index
	class Texture* GetTexture(size_t index);
	// Get name of shader
	const std::string& GetShaderName() const { return mShaderName; }
	// Get object space bounding sphere radius
	float GetRadius() const { return mRadius; }
	// Get object space bounding box
	const AABB& GetBox() const { return mBox; }
	// Get specular power of mesh
	float GetSpecPower() const { return mSpecPower; }

	void setSpecular(float newSpecular) { mSpecPower = newSpecular;	} //WSB added 2023-03-10

//private:  //2023-03-16 WSB: made this public for now so I can get to mTextures
	// AABB collision
	AABB mBox;
	// Textures associated with this mesh
	std::vector<class Texture*> mTextures;
	// Vertex array associated with this mesh
	class VertexArray* mVertexArray;
	// Name of shader specified by mesh
	std::string mShaderName;
	// Stores object space bounding sphere radius
	float mRadius;
	// Specular power of surface
	float mSpecPower;
};