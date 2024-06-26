// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//Header file for the game class - Rebecca Morris

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
	bool Restart();
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
	void PopUI(class UIScreen* screen);
	
	class FPSActor* GetPlayer() { return mFPSActor; }
	class BobberActor* GetBobber() {
		return mSingleBobber;
	}
	class Hook* GetHook() { return currentHook; }
	char GetCaughtFishType() {
		return mCaughtFishType;
	}
	class BasicFish* GetYellowFish() {
		return mYellowFish;
	}
	class BasicFish* GetRedFish() {
		return mRedFish;
	}
	class BasicFish* GetOrangeFish() {
		return mOrangeFish;
	}
	class BasicFish* GetGreenFish() {
		return mGreenFish;
	}
	class BasicFish* GetBlueFish() {
		return mBlueFish;
	}
	class BasicFish* GetPurpleFish() {
		return mPurpleFish;
	}
	class BasicFish* GetPinkFish() {
		return mPinkFish;
	}
	class BasicFish* GetSpottedFish() {
		return mSpottedFish;
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

	bool GetReelState() { return isReelingIn; }
	void StartReeling() { isReelingIn = true; }
	void StopReeling() { isReelingIn = false; }
	float GetFishHookDistance() { return fishHookDistance; }
	void SetFishHookDistance(float newDistance) { fishHookDistance = newDistance; }

	float GetTime() { return mCurrentTime; }
	
	class Font* GetFont(const std::string& fileName);

	class Animation* GetAnimation(const std::string& fileName);
	class Skeleton* GetSkeleton(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);
	// Game-specific
	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }

	void TurnScreenSaysFishOnOn(){ isScreenSaysFishOnOn = true; }

	void TurnScreenSaysFishOffOn() { isScreenSaysFishOffOn = true; }

	bool GetAllCaughtFish(int index);
	// Track if the fish is being reeled in
	bool isReelingIn;

	class BasicFish* GetCurrentFish() { return mCurrentFish; };

	void SetCurrentFish(BasicFish* fish);

	char GetCurrentFishType() { return CurrentFishType; }

	float GetCurrentFishDistance() { return mCurrentFishDistance; }

	void JustCaughtFish() { didJustCatchFish = true; }

	void PauseMusic();

	bool CheckIfAllFishCaught();

	void NewMainMenu();

	int GetWindowWidth() { return mWindowWidth; }
	int GetWindowHeight() { return mWindowHeight; }
	
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
	// Map of loaded skeletons
	std::unordered_map<std::string, class Skeleton*> mSkeletons;
	// Map of loaded animations
	std::unordered_map<std::string, class Animation*> mAnims;

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
	//class BasicFish* mBasicFish;
	char mCaughtFishType;
	bool mAllCaughtFish[8]{}; //8 is the number of all different types of fish - Rebecca Morris
	float mCurrentTime;
	class FPSActor* mFPSActor;
	class RodActor* mRodActor;
	class BobberActor* mSingleBobber;
	class SpriteComponent* mCrosshair;
	SoundEvent mMusicEvent;
	SoundEvent mReeling;
	SoundEvent mWin;
	class BasicFish* mYellowFish;
	class BasicFish* mRedFish;
	class BasicFish* mOrangeFish;
	class BasicFish* mGreenFish;
	class BasicFish* mBlueFish;
	class BasicFish* mPurpleFish;
	class BasicFish* mPinkFish;
	class BasicFish* mSpottedFish;
	class SkyBox* mSkybox;
	bool isScreenSaysFishOnOn;
	bool isScreenSaysFishOffOn;
	bool didFishGetAway;
	float fishHookDistance;
	class Hook* currentHook;
	class BasicFish* mCurrentFish;
	char CurrentFishType;
	float mCurrentFishDistance;
	bool didJustCatchFish;
	int mWindowWidth;
	int mWindowHeight;
};
