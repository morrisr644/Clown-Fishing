// ----------------------------------------------------------------
// Written by Will Briggs, Greg Kopack, and whoever wrote the Assimp library tester program I'm using
//   see if I can track down a name!
// 2023-3-8
// This loads a model using the assimp library for use with Madhav's code. 
// It does NOT load the texture -- that has to be already done. 2023-03-16 WSB alter this comment
// ----------------------------------------------------------------

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL/SDL_log.h> //for SDL_Log, which reports errors
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "AssimpLoad.h"
#include "Renderer.h"

#include <string>


//WSB: this is imported from LearnOpenGL
// checks all material textures of a given type and loads the textures if they're not loaded yet.
 // the required info is returned as a Texture struct.
//2023-03-16 It is not currently used.
std::vector<Texture*> loadMaterialTextures(Game* game, const std::string& directory, aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		std::string filename = directory + '/' + std::string(path.C_Str());

		Texture* t = game->GetRenderer()->GetTexture(filename);
		if (t) textures.emplace_back(t); //If loading didn't work, don't use this texture.
	}

	return textures;
}

//Change the specular value for a mesh (or for many)
void setSpecular(std::vector<Mesh*>& meshes, float newSpecular)
{
	for (auto mesh : meshes) mesh->setSpecular(newSpecular);
}

void LoadAssimpMeshes(std::vector<Mesh*>& meshes, Game* game, const char* fName, Texture* t, const char* nameForDebugging)
{
	meshes.clear(); //just to be sure it's empty

	//2023-03-24 WSB
	//Madhav has Renderer::mMeshes, an unordered map of Mesh* keyed on filename. Each key in an unordered map is unique.
	// Each Assimp file may have multiple meshes per file, so I can't use this scheme.
	// If I just ignore it, I don't have a way of ensuring a model isn't loaded multiple times, which can be very slow.
	// If I just use Madhav's Render::mMeshes, I can only note one Mesh* per file (as keys are unique), so I still must reload.
	// If I make my own, I can do that, but it seems clunky.
	// My thought at the moment is to make mMeshes into a multimap so it can have repeated keys
	// of Madhav's code), have my own list of meshes, or I can just put an entry in mMeshes for each Assimp file (NOT
	// each mesh in it). This presumes that the Mesh* in the map isn't actually used for anything, which seems to be true.
	// So I will add an entry into Renderer::mMeshes for each Assimp file, a pair <filename, nullptr> (see end of this function)
	// Then I can check, as Madhav does, to be sure I don't load the same file twice, right here:

	auto& existingMeshes = game->GetRenderer()->mMeshes;
	if (existingMeshes.find(fName) != existingMeshes.end()) //If we've already loaded this file
	{
		for (auto i = existingMeshes.lower_bound(fName); i != existingMeshes.upper_bound(fName); ++i)
			meshes.emplace_back(i->second);

		return;                                            //don't reload it; get already loaded meshes and quit
	}

	const aiScene* scene = aiImportFile(fName, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
		SDL_Log("Problem loading %s: %s", fName, aiGetErrorString());
	else
	{
		// Load all meshes and whatnot
		meshes.reserve(scene->mNumMeshes);
		for (std::uint32_t meshIdx = 0u; meshIdx < scene->mNumMeshes; meshIdx++)
		{
			aiMesh* mesh = scene->mMeshes[meshIdx];

			Mesh* mMesh = new Mesh; //2023-2-7 WSB be sure somebody deletes this eventually

			mMesh->mShaderName = "BasicMesh";

			// Set the vertex layout/size based on the format in the file
			VertexArray::Layout layout = VertexArray::PosNormTex;
			size_t vertSize = 8;
			mMesh->mSpecPower = 100.0;

			// Load in the vertices
			std::vector<Vertex> vertices;
			vertices.reserve(mesh->mNumVertices); //((_countof(verts) / 3) * 8); //verts/3 is # of vertices. For each vertex, 8 floats: 3 for vert, 3 for index, 2 for text coords

			mMesh->mRadius = 0.0f;
			for (std::uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				aiVector3D vert = mesh->mVertices[i];
				aiVector3D norm = mesh->mNormals[i];
				aiVector3D uv(0.f, 0.f, 0.f);
				if (mesh->mTextureCoords[0]) //if there ARE texture coords
					uv = mesh->mTextureCoords[0][i]; //WSB 2023-02-21 Is there possibly a z component? Why use a 3d vector?

				//Keep mRadius and mBox updated: for each new vertex, if this one's further out, remember it
				Vector3 pos(vert[0], vert[1], vert[2]);
				mMesh->mRadius = Math::Max(mMesh->mRadius, pos.LengthSq());
				mMesh->mBox.UpdateMinMax(pos);

				Vertex v;

				// Add the floats
				for (int j = 0; j < 3; j++) { v.f = vert[j]; vertices.emplace_back(v); }
				for (int j = 0; j < 3; j++) { v.f = norm[j]; vertices.emplace_back(v); }
				v.f = uv[0];  vertices.emplace_back(v);
				v.f = -uv[1];  vertices.emplace_back(v);
			}

			// We were computing length squared earlier
			mMesh->mRadius = Math::Sqrt(mMesh->mRadius);

			std::vector<unsigned int> indices;
			indices.reserve(mesh->mNumFaces * 3u);
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				indices.emplace_back(mesh->mFaces[i].mIndices[0u]);
				indices.emplace_back(mesh->mFaces[i].mIndices[1u]);
				indices.emplace_back(mesh->mFaces[i].mIndices[2u]);
			}

			std::string path(fName); //2023-03-16 WSB the next line is from LearnOpenGL
			std::string directory = path.substr(0, path.find_last_of('/'));

			//Associate the loaded texture
			if (t) mMesh->mTextures.emplace_back(t); //If one was sent in, just use it.
			else
			{
				//This is where i will put stuff about loading textures. 2023-03-16 WSB
				//It is from LearnOpenGL

				//// process materials
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
				// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
				// Same applies to other texture as the following list summarizes:
				// diffuse: texture_diffuseN
				// specular: texture_specularN
				// normal: texture_normalN

				//// 1. diffuse maps
				std::vector<Texture*> diffuseMaps = loadMaterialTextures(game, directory, material, aiTextureType_DIFFUSE, "texture_diffuse");
				mMesh->mTextures.insert(mMesh->mTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
				// 2. specular maps
				std::vector<Texture*> specularMaps = loadMaterialTextures(game, directory, material, aiTextureType_SPECULAR, "texture_specular");
				mMesh->mTextures.insert(mMesh->mTextures.end(), specularMaps.begin(), specularMaps.end());
				// 3. normal maps
				std::vector<Texture*> normalMaps = loadMaterialTextures(game, directory, material, aiTextureType_HEIGHT, "texture_normal");
				mMesh->mTextures.insert(mMesh->mTextures.end(), normalMaps.begin(), normalMaps.end());
				// 4. height maps
				std::vector<Texture*> heightMaps = loadMaterialTextures(game, directory, material, aiTextureType_AMBIENT, "texture_height");
				mMesh->mTextures.insert(mMesh->mTextures.end(), heightMaps.begin(), heightMaps.end());

				//If no texture was sent in AND the file doesn't know about any, use the default.
				if (diffuseMaps.size() + specularMaps.size() + normalMaps.size() + heightMaps.size() == 0)
				{
					t = game->GetRenderer()->GetTexture("Assets/Default.png");
					mMesh->mTextures.emplace_back(t);
				}

				assert(mMesh->mTextures.size());
			}
			//WSB 2023-02-21: aren't we supposed to have an mBox?

			// Now create a vertex array
			mMesh->mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
				layout, indices.data(), static_cast<unsigned>(indices.size()));

			meshes.push_back(mMesh);
			game->GetRenderer()->mMeshes.emplace(fName, mMesh);
		}
	}
}