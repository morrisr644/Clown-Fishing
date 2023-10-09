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
	class BasicFish* GetBasicFish() {
		return mBasicFish;
	}
	std::vector<class BasicFish*> GetBasicFishes()
	{
		return mBasicFishes;
	}
	
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
	
	class Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);
	// Game-specific
	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }

	void AddWaterPlane(class WaterPlaneActor* water);
	void RemoveWaterPlane(class WaterPlaneActor* water);
	std::vector<class WaterPlaneActor*>& GetWaterPlanes() { return mWaterPlanes; }
	
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	// All the actors in the game
	std::vector<class BasicFish*> mBasicFishes;
	std::vector<class Actor*> mActors;
	std::vector<class BobberActor*> mBobbers; //Rebecca Morris
	std::vector<class UIScreen*> mUIStack;
	std::unordered_map<std::string, class Font*> mFonts;

	// Map for text localization
	std::unordered_map<std::string, std::string> mText;
	// Any pending actors
	std::vector<class Actor*> mPendingActors;
	std::vector<class BobberActor*> mPendingBobbers; //Rebecca Morris

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;
	class PhysWorld* mPhysWorld;
	class HUD* mHUD;

	Uint32 mTicksCount;
	GameState mGameState;
	// Track if we're updating actors right now
	bool mUpdatingActors;

	// Game-specific code
	std::vector<class PlaneActor*> mPlanes;
	std::vector<class WaterPlaneActor*> mWaterPlanes;
	class BasicFish* mBasicFish;
	class FPSActor* mFPSActor;
	class BobberActor* mSingleBobber;
	class SpriteComponent* mCrosshair;
	SoundEvent mMusicEvent;
};
