// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "SoundEvent.h"
#include <SDL/SDL_types.h>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddBobber(class BobberActor* bobber); // Rebecca Morris
	void RemoveBobber(class BobberActor* bobber); // Rebecca Morris

	void AddBasicFish(class BasicFish* fish); // Rebecca Morris
	void RemoveBasicFish(class BasicFish* fish); // Rebecca Morris

	class Renderer* GetRenderer() { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }
	class HUD* GetHUD() { return mHUD; }
	
	// Manage UI stack
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	void PushUI(class UIScreen* screen);
	
	class FPSActor* GetPlayer() { return mFPSActor; }
	class BobberActor* GetBobber() {
		return mSingleBobber;
		/*if (mBobbers.size() == 1) // If there is already a bobber, remove it - Rebecca Morris
		{
			return mBobbers[0];
		}*/
	}
	class Hook* GetHook() { return currentHook; }
	class BasicFish* GetBasicFish() {
		return mBasicFish;
	}
	int GetCaughtFishType() {
		return mCaughtFishType;
	}
	class YellowFish* GetYellowFish() {
		return mYellowFish;
	}
	class RedFish* GetRedFish() {
		return mRedFish;
	}
	std::vector<class BasicFish*> GetBasicFishes()
	{
		return mBasicFishes;
	}
	/*std::vector<class YellowFish*> GetYellowFishes()
	{
		return mYellowFishes;
	}*/
	
	std::vector<class BobberActor*> GetBobberCount()
	{
		return mBobbers;
	}
	
	enum GameState
	{
		EGameplay,
		EPaused,
		EQuit
	};
	
	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }

	bool GetReelState() { return isReelingIn; }
	void StartReeling() { isReelingIn = true; }
	void StopReeling() { isReelingIn = false; }
	float GetFishHookDistance() { return fishHookDistance; }
	void SetFishHookDistance(float newDistance) { fishHookDistance = newDistance; }

	float GetTime() { return mCurrentTime; }
	
	class Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);
	// Game-specific
	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }

	void AddGrassPlane(class GrassPlaneActor* grass);
	void RemoveGrassPlane(class GrassPlaneActor* grass);
	std::vector<class GrassPlaneActor*>& GetGrassPlanes() { return mGrassPlanes; }

	void AddShorePlane(class ShorePlaneActor* shore);
	void RemoveShorePlane(class ShorePlaneActor* shore);
	std::vector<class ShorePlaneActor*>& GetShorePlanes() { return mShorePlanes; }

	void AddWoodPlane(class WoodPlaneActor* wood);
	void RemoveWoodPlane(class WoodPlaneActor* wood);
	std::vector<class WoodPlaneActor*>& GetWoodPlanes() { return mWoodPlanes; }

	void AddWaterPlane(class WaterPlaneActor* water);
	void RemoveWaterPlane(class WaterPlaneActor* water);
	std::vector<class WaterPlaneActor*>& GetWaterPlanes() { return mWaterPlanes; }

	void AddUnderPlane(class UnderPlaneActor* under);
	void RemoveUnderPlane(class UnderPlaneActor* under);
	std::vector<class UnderPlaneActor*>& GetUnderPlanes() { return mUnderPlanes; }

	void AddInvisiblePlane(class InvisiblePlaneActor* invis);
	void RemoveInvisiblePlane(class InvisiblePlaneActor* invis);
	std::vector<class InvisiblePlaneActor*>& GetInvisiblePlanes() { return mInvisiblePlanes; }

	void TurnFishOnScreenOn(){ isFishOnScreenOn = true; }

	void TurnFishOffScreenOn() { isFishOffScreenOn = true; }

	bool GetAllCaughtFish(int index);
	// Track if the fish is being reeled in
	bool isReelingIn;

	//int GetAmountOfFishTypes() { return 2; } //Increase as new types are added 
	
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	// All the actors in the game
	std::vector<class BasicFish*> mBasicFishes;
	//std::vector<class YellowFish*> mYellowFishes;
	std::vector<class Actor*> mActors;
	std::vector<class BobberActor*> mBobbers; //Rebecca Morris
	std::vector<class UIScreen*> mUIStack;
	std::unordered_map<std::string, class Font*> mFonts;

	// Map for text localization
	std::unordered_map<std::string, std::string> mText;
	// Any pending actors
	std::vector<class Actor*> mPendingActors;
	std::vector<class BobberActor*> mPendingBobbers; //Rebecca Morris
	std::vector<class BasicFish*> mPendingBasicFish; //Rebecca Morris

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;
	class AudioComponent* mAudioComp;
	class PhysWorld* mPhysWorld;
	class HUD* mHUD;

	Uint32 mTicksCount;
	GameState mGameState;
	// Track if we're updating actors right now
	bool mUpdatingActors;

	

	// Game-specific code
	std::vector<class PlaneActor*> mPlanes;
	std::vector<class WoodPlaneActor*> mWoodPlanes;
	std::vector<class GrassPlaneActor*> mGrassPlanes;
	std::vector<class WaterPlaneActor*> mWaterPlanes;
	std::vector<class ShorePlaneActor*> mShorePlanes;
	std::vector<class UnderPlaneActor*> mUnderPlanes;
	std::vector<class InvisiblePlaneActor*> mInvisiblePlanes;
	class BasicFish* mBasicFish;
	int mCaughtFishType;
	bool mAllCaughtFish[2]{}; //The 2 is the number of all different types of fish, it must be updated as more are added
	float mCurrentTime;
	class FPSActor* mFPSActor;
	class RodActor* mRodActor;
	class BobberActor* mSingleBobber;
	class SpriteComponent* mCrosshair;
	SoundEvent mMusicEvent;
	SoundEvent mReeling;
	class YellowFish* mYellowFish;
	class RedFish* mRedFish;
	class SkyBox* mSkybox;
	bool isFishOnScreenOn;
	bool isFishOffScreenOn;
	bool didFishGetAway;
	float fishHookDistance;
	class Hook* currentHook;
};
