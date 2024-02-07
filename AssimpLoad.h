// ----------------------------------------------------------------
// Written by Will Briggs and whoever wrote the Assimp library tester program I'm using
//   see if I can track down a name!
// 2023-3-8
// This loads a model using the assimp library for use with Madhav's code. 
// It does NOT load the texture -- that has to be already done.
// ----------------------------------------------------------------

#pragma once

#include <vector>

class Texture;
class Game;
class Mesh;

// This loads a model using the assimp library for use with Madhav's code. 
// It does NOT load the texture -- that has to be already done.
void LoadAssimpMeshes(std::vector<Mesh*>& meshes, Game* game, const char* fName, Texture* t, const char* nameForDebugging = "");
void setSpecular(std::vector<Mesh*>& meshes, float newSpecular);
void setSpecular(Mesh* mesh, float newSpecular);



