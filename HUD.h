// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	
	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);
protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);
	void UpdateTensionBar(float deltaTime);
	
	class Texture* mBlankTensionBar;
	class Texture* mTensionBar1;
	class Texture* mTensionBar2;
	class Texture* mTensionBar3;
	class Texture* mTensionBar4;
	class Texture* mTensionBar5;
	class Texture* mTensionBar6;
	class Texture* mTensionBar7;
	class Texture* mTensionBar8;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mRadarArrow;
	
	// All the target components in the game
	std::vector<class TargetComponent*> mTargetComps;
	// 2D offsets of blips relative to radar
	std::vector<Vector2> mBlips;
	// Adjust range of radar and radius
	float mRadarRange;
	float mRadarRadius;
	// Whether the crosshair targets an enemy
	bool mTargetEnemy;

	std::vector<Vector2> mTensionBarBlank; //Tension Bars
	std::vector<Vector2> mTensionBarLevel1;
	std::vector<Vector2> mTensionBarLevel2;
	std::vector<Vector2> mTensionBarLevel3;
	std::vector<Vector2> mTensionBarLevel4;
	std::vector<Vector2> mTensionBarLevel5;
	std::vector<Vector2> mTensionBarLevel6;
	std::vector<Vector2> mTensionBarLevel7;
	std::vector<Vector2> mTensionBarLevel8;
};
