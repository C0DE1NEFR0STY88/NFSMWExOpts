﻿#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <math.h>

#include "InGameFunctions.h"

float heatLevel, gameSpeed, FallingRainSize, RainAmount, RoadReflection, RainIntensity, RainXing, RainFallSpeed, RainGravity, SplashScreenTimeLimit, CarSelectTireSteerAngle, MaxHeatLevel, MinHeatLevel, WorldAnimationSpeed, CarScale, VTRed, VTBlue, VTGreen, VTBlackBloom, VTColorBloom, VTSaturation, DebugCameraTurboSpeed, DebugCameraSuperTurboSpeed, SBRechargeTime, SBRechargeSpeedLimit, SBMassMultiplier, SpeedingLimit, ExcessiveSpeedingLimit, RecklessDrivingLimit;
int hotkeyToggleForceHeat, hotkeyForceHeatLevel, hotkeyToggleCopLights, hotkeyToggleHeadlights, hotkeyCarHack, hotkeyUnlockAllThings, hotkeyAutoDrive, randomizeCount, hotkeyToggleCops, hotkeyFreezeCamera, NosTrailRepeatCount, UG2SaveMoney;
unsigned char raceType, raceMode, minLaps, maxLaps, minOpponents, maxOpponents, maxLapsRandomQR, maxOpponentsRandomQR, maxBlacklist, csBlacklist, headlightsMode, lowTraffic, medTraffic, highTraffic, ShowHiddenTracks, MaxUniqueOpponentCars, ShowAllCarsInFE, WindowedMode, SelectableMarkerCount, PurchasedCarLimit;
bool copLightsEnabled, HideOnline, ShowOnlineOpts, removeSceneryGroupDoor, removePlayerBarriers, unfreezeKO, EnablePresetAndDebugCars, AlwaysRain, SkipMovies, EnableSound, EnableMusic, EnableCameras, ExOptsTeamTakeOver, ShowSubs, EnableHeatLevelOverride, CarbonStyleRaceProgress, moreVinyls, eatSomeBurgers, UnlockAllThings, GarageRotate, GarageZoom, GarageShowcase, EnableSaveLoadHotPos, EnableMaxPerfOnShop, EnableVTOverride, EnableDebugWorldCamera, DebugWorldCamera, DebugWatchCarCamera, ForceBlackEdition, HelicopterFix, X10Fix, WheelFix, ExperimentalSplitScreenFix, ShowDebugCarCustomize, CarbonStyleBustedScreen, ShowMessage, ReplayBlacklistRaces, PursuitActionMode, MoreCarsForOpponents, VisualFixesAndTweaks, UncensoredBustedScreen, ShowPursuitCops, ShowNonPursuitCops, ShowDebugEventID, CarbonStyleRandomCars, SkipCareerIntro, ShowTimeOfDay, BetterRandomRaces, AllowMultipleInstances, TimeBugFix, NoCatchUp, CarSkinFix, ImmobileColFix, NFSU2StyleLookBackCamera, NoRevLimiter, SkipNISs, ExpandMemoryPools, ShowPresetCarsInFE, AllowLongerProfileNames, DDayFix, BustedNISFix, ShowLanguageSelectScreen, DoScreenPrintf, WorldMapAnywhere;
DWORD selectedCar, careerCar, raceOptions, Strings, HeatLevelAddr, VTecx, StartingCashDWORD, GameState, ThreadDelay;
HWND windowHandle;

void Thing();

bool forceHeatLevel = 0, carHackEnabled = 0, once1 = 0, once2 = 0, once3 = 0, IsOnFocus, AutoDrive, ToggleCops = 1;
DWORD ButtonResult = 0;

#include "DialogInterfaceHook.h"
#include "ExtraOptionsStuff.h"
#include "LanguageSelectScreen.h"
#include "ScreenPrintf.h"

void Init()
{
	CIniReader iniReader("NFSMWExtraOptionsSettings.ini");

	// Hotkeys
	hotkeyToggleForceHeat = iniReader.ReadInteger("Hotkeys", "HeatLevelHack", 118); //F7
	hotkeyForceHeatLevel = iniReader.ReadInteger("Hotkeys", "ChangeHeatLevel", 33); //PageUp
	hotkeyToggleCops = iniReader.ReadInteger("Hotkeys", "ToggleCops", 145); // Scroll Lock
	hotkeyToggleCopLights = iniReader.ReadInteger("Hotkeys", "CopLights", 79); // O
	hotkeyToggleHeadlights = iniReader.ReadInteger("Hotkeys", "Headlights", 72); // H
	hotkeyCarHack = iniReader.ReadInteger("Hotkeys", "FreezeCar", 115); //F4
	hotkeyFreezeCamera = iniReader.ReadInteger("Hotkeys", "FreezeCamera", 19); // Pause/Break
	hotkeyUnlockAllThings = iniReader.ReadInteger("Hotkeys", "UnlockAllThings", 116); //F5
	hotkeyAutoDrive = iniReader.ReadInteger("Hotkeys", "AutoDrive", 117); //F6
	EnableSaveLoadHotPos = iniReader.ReadInteger("Hotkeys", "EnableSaveLoadHotPos", 0) == 1;

	// LapControllers
	minLaps = iniReader.ReadInteger("LapControllers", "Minimum", 0);
	maxLaps = iniReader.ReadInteger("LapControllers", "Maximum", 127);
	maxLapsRandomQR = iniReader.ReadInteger("LapControllers", "RandomRace", 127);
	unfreezeKO = iniReader.ReadInteger("LapControllers", "UnfreezeKO", 1) == 1;

	// OpponentControllers
	minOpponents = iniReader.ReadInteger("OpponentControllers", "Minimum", 0);
	maxOpponents = iniReader.ReadInteger("OpponentControllers", "Maximum", 6);
	maxOpponentsRandomQR = iniReader.ReadInteger("OpponentControllers", "RandomRace", 6);
	MaxUniqueOpponentCars = iniReader.ReadInteger("OpponentControllers", "MaxUniqueOpponentCars", 3);
	MoreCarsForOpponents = iniReader.ReadInteger("OpponentControllers", "MoreCarsForOpponents", 0) == 1;
	CarbonStyleRandomCars = iniReader.ReadInteger("OpponentControllers", "CarbonStyleRandomCars", 0) == 1;

	// TrafficControllers
	lowTraffic = iniReader.ReadInteger("TrafficControllers", "Low", 10);
	medTraffic = iniReader.ReadInteger("TrafficControllers", "Medium", 30);
	highTraffic = iniReader.ReadInteger("TrafficControllers", "High", 50);

	// Menu
	HideOnline = iniReader.ReadInteger("Menu", "ShowOnline", 1) == 0;
	ShowOnlineOpts = iniReader.ReadInteger("Menu", "ShowOnlineOpts", 0) == 1;
	ShowSubs = iniReader.ReadInteger("Menu", "ShowSubs", 0) == 1;
	EnablePresetAndDebugCars = iniReader.ReadInteger("Menu", "ShowMoreCarCategories", 0) == 1;
	ShowLanguageSelectScreen = iniReader.ReadInteger("Menu", "ShowLanguageSelectScreen", 1) == 1;
	moreVinyls = iniReader.ReadInteger("Menu", "ShowSpecialVinyls", 1) == 1;
	ShowDebugCarCustomize = iniReader.ReadInteger("Menu", "ShowDebugCarCustomize", 0) == 1;
	ShowDebugEventID = iniReader.ReadInteger("Menu", "ShowDebugEventID", 0) == 1;
	ShowTimeOfDay = iniReader.ReadInteger("Menu", "ShowTimeOfDay", 0) == 1;
	ShowHiddenTracks = iniReader.ReadInteger("Menu", "ShowHiddenTracks", 0);
	BetterRandomRaces = iniReader.ReadInteger("Menu", "BetterRandomRaces", 0) == 1;
	maxBlacklist = iniReader.ReadInteger("Menu", "MaximumBlacklistRival", 15);
	ReplayBlacklistRaces = iniReader.ReadInteger("Menu", "ReplayBlacklistRaces", 0) == 1;
	csBlacklist = iniReader.ReadInteger("Menu", "ChallengeSeriesBlacklist", 19);
	randomizeCount = iniReader.ReadInteger("Menu", "RandomizeCount", 30);
	SplashScreenTimeLimit = iniReader.ReadFloat("Menu", "SplashScreenTimeLimit", 30.0f);
	CarSelectTireSteerAngle = iniReader.ReadFloat("Menu", "CarSelectTireSteerAngle", 21.6723f);
	GarageZoom = iniReader.ReadInteger("Menu", "ShowcaseCamInfiniteZoom", 0) == 1;
	GarageRotate = iniReader.ReadInteger("Menu", "ShowcaseCamInfiniteRotation", 0) == 1;
	GarageShowcase = iniReader.ReadInteger("Menu", "ShowcaseCamAlwaysEnable", 0) == 1;
	AllowLongerProfileNames = iniReader.ReadInteger("Menu", "AllowLongerProfileNames", 1) == 1;
	ExOptsTeamTakeOver = iniReader.ReadInteger("Menu", "DisableTakeover", 0) == 0;

	// Gameplay
	EnableCameras = iniReader.ReadInteger("Gameplay", "EnableHiddenCameraModes", 0) == 1;
	EnableDebugWorldCamera = iniReader.ReadInteger("Gameplay", "EnableDebugCamera", 0) == 1;
	NFSU2StyleLookBackCamera = iniReader.ReadInteger("Gameplay", "ShowHUDOnLookBack", 0) == 1;
	gameSpeed = iniReader.ReadFloat("Gameplay", "GameSpeed", 1.0f);
	WorldAnimationSpeed = iniReader.ReadFloat("Gameplay", "WorldAnimationSpeed", 45.0f);
	DebugCameraTurboSpeed = iniReader.ReadFloat("Gameplay", "DebugCamTurboSpeed", 3.06f);
	DebugCameraSuperTurboSpeed = iniReader.ReadFloat("Gameplay", "DebugCamSuperTurboSpeed", 7.16f);
	CarScale = iniReader.ReadFloat("Gameplay", "CarScale", 1.0f);
	removeSceneryGroupDoor = iniReader.ReadInteger("Gameplay", "RemoveOldBridgeBarrier", 0) == 1;
	removePlayerBarriers = iniReader.ReadInteger("Gameplay", "RemoveNeonBarriers", 0) == 1;
	CarbonStyleRaceProgress = iniReader.ReadInteger("Gameplay", "ShowPercentOn1LapRaces", 1) == 1;
	StartingCashDWORD = iniReader.ReadInteger("Gameplay", "StartingCash", 0);
	UG2SaveMoney = iniReader.ReadInteger("Gameplay", "AwardedCash", 10000);
	WorldMapAnywhere = iniReader.ReadInteger("Gameplay", "WorldMapAnywhere", 1) == 1;
	UnlockAllThings = iniReader.ReadInteger("Gameplay", "UnlockAllThings", 0) == 1;
	ForceBlackEdition = iniReader.ReadInteger("Gameplay", "ForceBlackEdition", 1) == 1;
	eatSomeBurgers = iniReader.ReadInteger("Gameplay", "UnlockBurgerKingChallenge", 0) == 1;
	EnableMaxPerfOnShop = iniReader.ReadInteger("Gameplay", "EnableMaxPerformanceOnShop", 1) == 1;
	ShowAllCarsInFE = iniReader.ReadInteger("Gameplay", "ShowAllCarsInFE", 1);
	// ShowPresetCarsInFE = iniReader.ReadInteger("Gameplay", "ShowPresetCarsInFE", 0);
	NoCatchUp = iniReader.ReadInteger("Gameplay", "NoCatchUp", 0) == 1;
	NoRevLimiter = iniReader.ReadInteger("Gameplay", "NoRevLimiter", 0) == 1;
	NosTrailRepeatCount = iniReader.ReadInteger("Gameplay", "NosTrailRepeatCount", 8);
	SelectableMarkerCount = iniReader.ReadInteger("Gameplay", "SelectableMarkerCount", 2);
	PurchasedCarLimit = iniReader.ReadInteger("Gameplay", "PurchasedCarLimit", 10);
	SBRechargeTime = iniReader.ReadFloat("Gameplay", "SBRechargeTime", 25.0f);
	SBRechargeSpeedLimit = iniReader.ReadFloat("Gameplay", "SBRechargeSpeedLimit", 100.0f);
	SBMassMultiplier = iniReader.ReadFloat("Gameplay", "SBMassMultiplier", 2.0f);

	// Pursuit
	EnableHeatLevelOverride = iniReader.ReadInteger("Pursuit", "HeatLevelOverride", 0) == 1;
	MinHeatLevel = iniReader.ReadFloat("Pursuit", "MinimumHeatLevel", 1.00f);
	MaxHeatLevel = iniReader.ReadFloat("Pursuit", "MaximumHeatLevel", 10.00f);
	PursuitActionMode = iniReader.ReadInteger("Pursuit", "PursuitActionMode", 0) == 1;
	ShowPursuitCops = iniReader.ReadInteger("Pursuit", "ShowPursuitCops", 1) == 1;
	ShowNonPursuitCops = iniReader.ReadInteger("Pursuit", "ShowNonPursuitCops", 0) == 1;
	CarbonStyleBustedScreen = iniReader.ReadInteger("Pursuit", "ShowBustedScreenFrame", 1) == 0;
	UncensoredBustedScreen = iniReader.ReadInteger("Pursuit", "UncensoredBustedScreen", 0) == 1;
	SpeedingLimit = iniReader.ReadFloat("Pursuit", "SpeedingLimit", 67.00f);
	ExcessiveSpeedingLimit = iniReader.ReadFloat("Pursuit", "ExcessiveSpeedingLimit", 80.50f);
	RecklessDrivingLimit = iniReader.ReadFloat("Pursuit", "RecklessDrivingLimit", 89.40f);

	// VisualTreatment
	EnableVTOverride = iniReader.ReadInteger("VisualTreatment", "EnableVisualTreatmentOverride", 0) == 1;
	VTRed = iniReader.ReadFloat("VisualTreatment", "Red", 0.88f);
	VTGreen = iniReader.ReadFloat("VisualTreatment", "Green", 0.8f);
	VTBlue = iniReader.ReadFloat("VisualTreatment", "Blue", 0.44f);
	VTColorBloom = iniReader.ReadFloat("VisualTreatment", "ColorBloomIntensity", 1.75f);
	VTSaturation = iniReader.ReadFloat("VisualTreatment", "Saturation", 0.5f);
	VTBlackBloom = iniReader.ReadFloat("VisualTreatment", "BlackBloomIntensity", 0.9f);

	// Weather
	AlwaysRain = iniReader.ReadInteger("Weather", "AlwaysRain", 0) == 1;
	RainAmount = iniReader.ReadFloat("Weather", "GeneralRainAmount", 1.0f);
	RoadReflection = iniReader.ReadFloat("Weather", "RoadReflectionAmount", 1.0f);
	FallingRainSize = iniReader.ReadFloat("Weather", "RainSize", 0.01f);
	RainIntensity = iniReader.ReadFloat("Weather", "RainIntensity", 0.45f);
	RainXing = iniReader.ReadFloat("Weather", "RainCrossing", 0.02f);
	RainFallSpeed = iniReader.ReadFloat("Weather", "RainSpeed", 0.03f);
	RainGravity = iniReader.ReadFloat("Weather", "RainGravity", 0.35f);

	// Fixes
	WheelFix = iniReader.ReadInteger("Fixes", "DisappearingWheelsFix", 1) == 1;
	ExperimentalSplitScreenFix = iniReader.ReadInteger("Fixes", "ExperimentalSplitScreenFix", 0) == 1;
	HelicopterFix = iniReader.ReadInteger("Fixes", "HelicopterBountyFix", 1) == 1;
	X10Fix = iniReader.ReadInteger("Fixes", "ZeroBountyFix", 1) == 1;
	//TimeBugFix = iniReader.ReadInteger("Fixes", "TimeBugFix", 0) == 1;
	CarSkinFix = iniReader.ReadInteger("Fixes", "CarSkinFix", 0) == 1;
	ImmobileColFix = iniReader.ReadInteger("Fixes", "ImmobileCollisionsFix", 0) == 1;
	DDayFix = iniReader.ReadInteger("Fixes", "DDayFix", 1) == 1;
	BustedNISFix = iniReader.ReadInteger("Fixes", "BustedNISFix", 1) == 1;

	// Misc
	WindowedMode = iniReader.ReadInteger("Misc", "WindowedMode", 0);
	SkipMovies = iniReader.ReadInteger("Misc", "SkipMovies", 0) == 1;
	SkipNISs = iniReader.ReadInteger("Misc", "SkipNISs", 0) == 1;
	EnableSound = iniReader.ReadInteger("Misc", "EnableSound", 1) == 1;
	EnableMusic = iniReader.ReadInteger("Misc", "EnableMusic", 1) == 1;
	ShowMessage = iniReader.ReadInteger("Misc", "ShowMessage", 1) == 1;
	SkipCareerIntro = iniReader.ReadInteger("Misc", "SkipCareerIntro", 0) == 1;
	AllowMultipleInstances = iniReader.ReadInteger("Misc", "AllowMultipleInstances", 0) == 1;
	ThreadDelay = iniReader.ReadInteger("Misc", "ThreadDelay", 5);
	ExpandMemoryPools = iniReader.ReadInteger("Misc", "ExpandMemoryPools", 0) == 1;
	DoScreenPrintf = iniReader.ReadInteger("Misc", "DoScreenPrintf", 1) == 1;

	// Limit values to fix increment & decrement behaviour breaking

	// loop around values
	minLaps %= 128; 
	maxLaps %= 128;
	maxLapsRandomQR %= 128;

	minOpponents %= 16;
	maxOpponents %= 16;
	maxOpponentsRandomQR %= 16;

	headlightsMode = 1;

	randomizeCount %= 501;

	lowTraffic %= 101;
	medTraffic %= 101;
	highTraffic %= 101;

	SelectableMarkerCount--;
	SelectableMarkerCount %= 6;
	SelectableMarkerCount++;

	PurchasedCarLimit %= 26;
	PurchasedCarLimit--;

	// Allow Multiple Instances
	if (AllowMultipleInstances)
	{
		injector::WriteMemory<unsigned char>(0x6665A1, 0xEB, true);
	}

	// Expand Memory Pools (ty Berkay)
	if (ExpandMemoryPools)
	{
		injector::WriteMemory<short>(0x5F7396, 0x2C80, true); // GManager::PreloadTransientVaults (0x2C8000)
		injector::WriteMemory<short>(0x5F73B2, 0x2C80, true);

		injector::WriteMemory<unsigned char>(0x665FDC, 0x80, true); // InitializeEverything (0x8000)
		
		injector::WriteMemory<DWORD>(0x8F5790, 0x0BE6E0, true); // FEngMemoryPoolSize (InitFEngMemoryPool)
		injector::WriteMemory<DWORD>(0x8F7EF0, 0x01CC00, true); // CarLoaderPoolSizes

		// To prevent some crashes
		if (MaxUniqueOpponentCars > 7) MaxUniqueOpponentCars = 7;
	}

	// Remove 1-8 Lap Restriction
	injector::WriteMemory<unsigned char>(0x7AC3EC, minLaps, true); // Decrease lap count min lap controller
	injector::WriteMemory<unsigned char>(0x7AC3F0, maxLaps, true); // Decrease lap count max lap controller
	injector::WriteMemory<unsigned char>(0x7AC405, minLaps, true); // Increase lap count min lap controller
	injector::WriteMemory<unsigned char>(0x7AC401, maxLaps, true); // Increase lap count max lap controller
	// LAN Lobby
	injector::WriteMemory<unsigned char>(0x556bb5, maxLaps, true);
	injector::WriteMemory<unsigned char>(0x556bde, maxLaps, true);
	injector::WriteMemory<unsigned char>(0x556bc4, minLaps, true);
	injector::WriteMemory<unsigned char>(0x556bcf, minLaps, true);

	// Number of bots - Note: 4th and beyond opponents gets bugged cars.
	injector::WriteMemory<unsigned char>(0x7abff9, minOpponents, true); // Decrease opponents count min opponent controller
	injector::WriteMemory<unsigned char>(0x7abffd, maxOpponents + 0x01, true); // Decrease opponents count max opponent controller
	injector::WriteMemory<unsigned char>(0x7ac01a, minOpponents, true); // Increase opponents count min opponent controller
	injector::WriteMemory<unsigned char>(0x7ac00e, maxOpponents + 0x01, true); // Increase opponents count max opponent controller

	// Maximum Unique Opponent Cars (Increasing this value also increases opponent cars' variety)
	injector::WriteMemory<unsigned char>(0x61DE27, MaxUniqueOpponentCars, true);

	// Random Race Laps and Opponents
	injector::WriteMemory<unsigned char>(0x56DCC1, maxOpponentsRandomQR, true); // Random Race Max Opponent Controller
	injector::WriteMemory<unsigned char>(0x56DCE9, maxLapsRandomQR, true); // Random Race Max Lap Controller

	// Traffic Density Controllers (0-100)
	injector::WriteMemory<unsigned char>(0x56dd9E, lowTraffic, true); // Low (10)
	injector::WriteMemory<unsigned char>(0x56dd9A, medTraffic, true); // Medium (30)
	injector::WriteMemory<unsigned char>(0x56dd96, highTraffic, true); // High (50)

	// Blacklist Max Rival Controllers
	if (maxBlacklist == 0) // Unlimited blacklists
	{
		// Enable BL 0 for decrement
		injector::MakeNOP(0x551455, 7, true);
		injector::WriteMemory<unsigned char>(0x551455, 0x31, true);
		injector::WriteMemory<unsigned char>(0x551456, 0xD2, true);

		// Enable BL 0 for increment
		injector::MakeNOP(0x55a571, 7, true);
		injector::WriteMemory<unsigned char>(0x55a571, 0x31, true);
		injector::WriteMemory<unsigned char>(0x55a572, 0xC0, true);

		// Remove Maximum Restriction
		injector::WriteMemory<unsigned char>(0x55a56c, 0xeb, true);

		// Remove Minimum Restriction
		injector::MakeNOP(0x551460, 5, true);
	}

	else // Normal behaviour
	{
		injector::WriteMemory<unsigned char>(0x551461, maxBlacklist, true);
		injector::WriteMemory<unsigned char>(0x551473, maxBlacklist, true);
		injector::WriteMemory<unsigned char>(0x55a555, maxBlacklist, true);
		injector::WriteMemory<unsigned char>(0x55a566, maxBlacklist, true);
	}

	// Remove restrictions for new save games
	if (maxBlacklist != 15)
	{
		injector::MakeNOP(0x55a556, 6, true);
		injector::MakeNOP(0x551433, 6, true);
	}

	// Replay Blacklist Races
	if (ReplayBlacklistRaces)
	{
		injector::MakeNOP(0x5FD1B9, 2, true); //allow challenging defeated rival
		injector::WriteMemory<unsigned char>(0x52F202, 0x00, true); // remove defeated flag
		
		injector::WriteMemory<unsigned char>(0x603269, 0xEB, true); // fix boss race stats getting reset

		injector::MakeJMP(0x6243EF, ReplayBlacklistFixCodeCave, true);
		injector::MakeNOP(0x6243C8, 6, true); // visuals fix

		injector::WriteMemory<DWORD>(0x60075C, 0x91CA3C, true);
		injector::MakeRangedNOP(0x600760, 0x60076B, true); // Graphics and name fixes
	}

	// Challenge Series Bin
	injector::WriteMemory<unsigned char>(0x7ae969, csBlacklist, true);

	// Fix Unexistent Bin Crash?? (EXPERIMENTAL)
	if (maxBlacklist != 15 || csBlacklist != 19)
	{
		injector::WriteMemory<unsigned char>(0x5e39c6, 0x01, true);
		injector::WriteMemory<unsigned char>(0x5e39c7, 0xC8, true);
	}

	// Race Speed Multiplier
	injector::WriteMemory<float>(0x901B1C, gameSpeed, true);

	// Randomize Count
	injector::WriteMemory<int>(0x7b4648, randomizeCount, true);

	// Menu Front Tire Angle
	injector::WriteMemory<float>(0x905E20, CarSelectTireSteerAngle, true);

	// NOS Trail Repeat Count NosTrailRepeatCount
	injector::WriteMemory<int>(0x903694, NosTrailRepeatCount, true);

	// Unfreeze Lap Count for Lap Knockout
	if (unfreezeKO)
	{
		injector::MakeNOP(0x7acf83, 3, true); // Enable editing of lap count for KO
		injector::MakeNOP(0x7ac04b, 3, true); // Don't change laps when opponent count changed
	}

	// Remove Race Barriers and Locked Areas
	if (removePlayerBarriers)
	{
		injector::WriteMemory<unsigned char>(0x8B282B, 0x4A, true);
		injector::WriteMemory<unsigned char>(0x8B2838, 0x4A, true);
		injector::WriteMemory<unsigned char>(0x8B2844, 0x4A, true);

		// Always show whole map
		injector::WriteMemory<unsigned char>(0x89B15D, 0, true);
		injector::WriteMemory<unsigned char>(0x57bf01, 0xEB, true);
		injector::WriteMemory<unsigned char>(0x57bf19, 0xEB, true);
		injector::WriteMemory<unsigned char>(0x57bf5f, 0xEB, true);
		injector::WriteMemory<unsigned char>(0x57bf7c, 0xEB, true);
	}

	// Remove Old Bridge Area Barrier
	if (removeSceneryGroupDoor)
	{
		injector::WriteMemory<unsigned char>(0x8B2817, 0x20, true);
		injector::WriteMemory<unsigned char>(0x8B2810, 0x54, true);
	}

	// Show Hidden Tracks in QR Track select
	if (ShowHiddenTracks)
	{
		injector::MakeJMP(0x7BA650, ShowHiddenTracksCodeCave, true);
		injector::MakeJMP(0x5FAA2A, GetRaceTypeCodeCave, true);
		injector::MakeRangedNOP(0x7AAC7F, 0x7AAC9F, true);
		injector::MakeRangedNOP(0x7AACB5, 0x7AACC4, true);
	}

	// Better Random Races (more cars and tracks to choose)
	if (BetterRandomRaces)
	{
		// Make more tracks available
		injector::MakeRangedNOP(0x7BA2EE, 0x7BA354, true);

		// Make more cars available
		injector::MakeRangedNOP(0x7BA24F, 0x7BA258, true);
	}

	// Don't hide HUD on Look Back Cam
	if (NFSU2StyleLookBackCamera)
	{
		injector::WriteMemory<int>(0x4741DB, 0, true);
	}

	// Remove online option from main menu
	if (HideOnline)
		injector::WriteMemory<unsigned char>(0x5450f9, 0x46, true);

	if (ShowOnlineOpts) injector::MakeNOP(0x5290B2, 2, true); // Show Online Options menu

	// Fix headlights force on
	injector::WriteMemory<float>(0x742AB7, 0, true);
	injector::WriteMemory<float>(0x742ABF, 0, true);

	// Add Debug and Preset Categories to Car Select screen
	if (EnablePresetAndDebugCars)
	{
		injector::WriteMemory<unsigned char>(0x7BF9E9, 0x20, true);
		injector::WriteMemory<unsigned char>(0x7BF9FA, 0x20, true);
		// LAN Lobby
		injector::WriteMemory<unsigned char>(0x556787, 0x20, true);
		injector::WriteMemory<unsigned char>(0x5567bd, 0x20, true);
		injector::WriteMemory<unsigned char>(0x5567e7, 0x20, true);
		injector::WriteMemory<unsigned char>(0x556821, 0x20, true);
	}

	// Rain Options
	if (AlwaysRain)
	{
		injector::WriteMemory<unsigned char>(0x9B0A30, 0x01, true);
		injector::WriteMemory<unsigned char>(0x758293, 0xEB, true); // Road Reflection Fix??
	}
	injector::WriteMemory<float>(0x904B38, RoadReflection, true);
	injector::WriteMemory<float>(0x904B38, RoadReflection, true);
	injector::WriteMemory<float>(0x904A90, FallingRainSize, true);
	injector::WriteMemory<float>(0x904A14, RainAmount, true);
	injector::WriteMemory<float>(0x904A94, RainIntensity, true);
	injector::WriteMemory<float>(0x904A24, RainXing, true);
	injector::WriteMemory<float>(0x904A28, RainFallSpeed, true);
	injector::WriteMemory<float>(0x904A2C, RainGravity, true);

	// Options from ModLoader
	if (WindowedMode != 0)
	{
		injector::WriteMemory<unsigned char>(0x982BF0, 1, true);
	}

	if (SkipMovies)
	{
		injector::WriteMemory<unsigned char>(0x926144, SkipMovies, true);
	}

	if (SkipNISs)
	{
		injector::WriteMemory<unsigned char>(0x9260A4, SkipNISs, true);
	}

	if (!EnableSound)
	{
		injector::WriteMemory<unsigned char>(0x8F86F8, EnableSound, true);
	}

	if (!EnableMusic)
	{
		injector::WriteMemory<unsigned char>(0x8F86FC, EnableMusic, true);
	}
	if (ShowMessage)
	{
		injector::MakeCALL(0x0058F710, DialogBoxReturnValue, true);
	}
	if (SkipCareerIntro)
	{
		injector::WriteMemory<unsigned char>(0x926125, 1, true); // SkipDDayRaces
		injector::WriteMemory<unsigned char>(0x926126, 1, true); // SkipCareerIntro
	}

	// Enable hidden camera modes
	if (EnableCameras)
	{
		injector::MakeNOP(0x5A38AA, 2, true); // For C key
		injector::MakeNOP(0x51C8D9, 2, true); // Player Options Menu - Increment
		injector::MakeNOP(0x51C8AF, 2, true); // Player Options Menu - Decrement
		injector::MakeRangedNOP(0x51C95F, 0x0051C98C, true); // Clean the unused code
		injector::MakeJMP(0x51C95F, CameraNamesCodeCave, true); // Fix Names

		// Rearview Mirror Fix Addition for Widescreen Fix
		injector::MakeNOP(0x595DDD, 2, true);
		injector::MakeNOP(0x595DE2, 2, true);
		injector::MakeNOP(0x595DE7, 2, true);
		injector::MakeNOP(0x595DEC, 2, true);
		injector::WriteMemory<unsigned char>(0x595DF0, 0xFF, true);
	}

	// Splash Screen Time Limit
	injector::WriteMemory<float>(0x8F3CD0, SplashScreenTimeLimit, true);

	// Force Show Subtitles
	if (ShowSubs)
	{
		injector::MakeNOP(0x542dbc, 2, true);
		injector::MakeNOP(0x54ee78, 2, true);
		injector::MakeNOP(0x54f219, 2, true);
	}

	// Heat Level Overrides
	injector::MakeRangedNOP(0x443dc3, HeatLevelsCodeCaveExit, true); // Clean the unused code
	injector::MakeJMP(0x443dc3, HeatLevelsCodeCave, true); // Prepare the game for Ultimate force heat level hack

	// Show COMPLETED %x instead of LAP 1/1
	if (CarbonStyleRaceProgress)
	{
		injector::MakeRangedNOP(0x57AA2F, 0x57AA46, true);
		// call 0x5FAA20 (Just moved this call up)
		injector::MakeCALL(0x57AA2F, 0x5FAA20, true);
		// cmp dword ptr ds: [esi+0x3C],0x01 (if lap count = 1)
		injector::WriteMemory<unsigned int>(0x57AA34, 0x13C7E83, true);
		//je 0x57AA73 (If true, use COMPLETED %)
		injector::WriteMemory<unsigned short int>(0x57AA38, 0x3974, true);
	}

	// Burger King
	if (eatSomeBurgers)
		injector::WriteMemory<unsigned char>(0x576A8D, 0xEB, true);

	// Special Vinyl Category
	if (moreVinyls)
	{
		injector::MakeRangedNOP(0x7BC8FF, VinylMenuCodeCaveExit, true); // CustomizeSub::SetupVinylGroups
		injector::MakeRangedNOP(0x7BD26F, VinylMenuCodeCave2Exit, true); // CustomizeParts::Setup

		injector::MakeJMP(0x7BC8FF, VinylMenuCodeCave, true);
		injector::MakeJMP(0x7BD26F, VinylMenuCodeCave2, true);

		// Category Fixes
		//injector::WriteMemory<unsigned char>(0x7A5208, 0x0A, true); shows new icon
		injector::WriteMemory<unsigned char>(0x7A5823, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7AFE90, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7B600C, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7B7436, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7B75A2, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7BAEC3, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7BAF10, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7bca3c, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7c0ae7, 0x0A, true);
	}

	// Fix (ignore) starting heat level when a challenge executed from quick race screen
	injector::WriteMemory<unsigned char>(0x56DD55, 0xEB, true);

	// Unlock all things
	if (UnlockAllThings)
	{
		injector::WriteMemory<unsigned char>(0x926124, 0x01, true);
	}

	if (WheelFix) // Fix Invisible Wheels
		injector::WriteMemory<unsigned char>(0x74251D, 0x01, true);

	// Garage Hacks
	if (GarageZoom)
	{
		injector::MakeNOP(0x476B04, 5, true);
		injector::WriteMemory<unsigned char>(0x476B04, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x476B05, 0x01, true);
	}
	if (GarageRotate)
	{
		injector::MakeNOP(0x476AC3, 5, true);
		injector::WriteMemory<unsigned char>(0x476AC3, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x476AC4, 0x01, true);
	}
	if (GarageShowcase)
	{
		injector::MakeNOP(0x7A201B, 5, true);
		injector::WriteMemory<unsigned char>(0x7A201B, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x7A201C, 0x01, true);
	}

	// Force Black Edition
	if (ForceBlackEdition)
	{
		injector::MakeNOP(0x5994E6, 3, true);
		injector::WriteMemory<unsigned char>(0x5994E6, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x5994E7, 0x01, true);
	}

	// World Animation Speed
	injector::WriteMemory<float>(0x904AEC, WorldAnimationSpeed, true);

	// Split-Screen Fix (Experimental)
	if (ExperimentalSplitScreenFix)
	{
		injector::WriteMemory<unsigned char>(0x7a3f68, 0x0D, true); // Fix Player 2 Ready control
		injector::WriteMemory<unsigned char>(0x666eb3, 0xEB, true); // Fix loading screen crash
		injector::MakeNOP(0x6cfc03, 2, true); // Fix black screen??

		// Add split screen to qr menu
		injector::MakeJMP(0x7AA821, SplitScreenCodeCave, true);
		// SplitScreen_vtbl
		injector::WriteMemory<DWORD>(0x901400, 0x7B4980, true);
		injector::WriteMemory(0x901404, &SplitScreen_React, true);
	}
	
	// String Replacement
	if (ExOptsTeamTakeOver)
		injector::MakeCALL(0x005A3260, GetGameVersionNumberStringHook, true);

	// Starting Cash
	injector::WriteMemory<unsigned char>(0x56D7D3, 0x81, true); // add
	injector::WriteMemory<unsigned char>(0x56D7D4, 0x41, true); // to [ecx]
	injector::WriteMemory<unsigned char>(0x56D7D5, 0x0C, true); // + 0x0c
	injector::WriteMemory<DWORD>(0x56D7D6, StartingCashDWORD, true); // Starting Cash
	injector::WriteMemory<unsigned char>(0x56D7DA, 0xC2, true); // ret 0004
	injector::WriteMemory<unsigned char>(0x56D7DB, 0x04, true);
	injector::WriteMemory<unsigned char>(0x56D7DC, 0x00, true);
	
	// Cash for Underground 2 save
	if (UG2SaveMoney < -29999) UG2SaveMoney = -29999; // prevents a potential softlock (you'll still need to edit the cost of a car though)
	if (UG2SaveMoney < 0)
	{
		injector::WriteMemory<std::uint8_t>(0x0056D7CC + 1, 0x69, true); // sub instead of add
		injector::WriteMemory<std::uint32_t>(0x0056D7CC + 3, abs(UG2SaveMoney), true); // positive value because uint
	}
	else
	{
		injector::WriteMemory<std::uint32_t>(0x0056D7CC + 3, UG2SaveMoney, true);
	}

	// Car size hack
	injector::WriteMemory<float>(0x737870, CarScale, true); // Length
	injector::WriteMemory<float>(0x7378A2, CarScale, true); // Width
	injector::WriteMemory<float>(0x7378D4, CarScale, true); // Height

	// Helicopter Bounty Fix
	if (HelicopterFix)
	{
		// Fix Bounty Announcement
		injector::MakeRangedNOP(0x595BED, HeliBountyFixCodeCaveExit, true);
		injector::MakeJMP(0x595BED, HeliBountyFixCodeCave, true);

		// Fix Bounty Amount
		injector::MakeRangedNOP(0x418F86, HeliBountyFixCodeCave2Exit, true);
		injector::MakeJMP(0x418F86, HeliBountyFixCodeCave2, true);
	}
	
	// X10 Bounty Fix
	if (X10Fix)
	{ 
		// Fix 0 Bounty
		injector::MakeRangedNOP(0x418F5B, X10FixCodeCaveExit, true);
		injector::MakeJMP(0x418F5B, X10FixCodeCave, true);

		// Fix combo timer
		injector::MakeRangedNOP(0x419019, X10FixCodeCave2Exit, true);
		injector::MakeJMP(0x419019, X10FixCodeCave2, true);
	}

	// Enable unused Busted NISs
	if (BustedNISFix)
	{
		// ChooseArrestAnimation
		injector::MakeJMP(0x44D8AE, 0x44D968, true); // Heat level 2
		injector::MakeJMP(0x44D8C9, 0x44D968, true); // Heat level 3 - ArrestM07
		injector::MakeJMP(0x44D8DA, 0x44D968, true); // Heat level 3 - ArrestM14
		injector::MakeJMP(0x44D8F3, 0x44D968, true); // Heat level 3 - ArrestM07b
		injector::MakeJMP(0x44D904, 0x44D968, true); // Heat level 3 - ArrestM14b
		injector::MakeJMP(0x44D915, 0x44D968, true); // Heat level 3 - ArrestF14/b
	}
	
	// Load Any Save Game
	// injector::WriteMemory<unsigned char>(0x5ACD4F, 0xEB, true); // Skip hash check, breaks compatibility for non-exopts users
	injector::MakeNOP(0x5ACBFA, 6, true); // Skip modded game check

	// Max Performance on Shop
	if (EnableMaxPerfOnShop)
	{
		// Make button active
		injector::MakeRangedNOP(0x7BBC4F, 0x7BBC6E, true);

		// Make button visible and give it a more proper position
		injector::MakeRangedNOP(0x7B11AB, 0x7B11C2, true);
		injector::MakeJMP(0x7B11CE, MaxPerfButtonCodeCave, true);
	}
		
	// Heat Level Override - Initialize
	if (EnableHeatLevelOverride)
	{
		// Keep heat level >x5 on car when used marker or evaded

		// CustomizeMeter::Init
		injector::WriteMemory<float>(0x7bb502, MaxHeatLevel, true); // CustomizeCategoryScreen
		injector::WriteMemory<float>(0x7b1387, MaxHeatLevel, true); // CustomizationScreenHelper
		injector::WriteMemory<float>(0x7b0c89, MaxHeatLevel, true); // CustomizeShoppingCart::Setup
		injector::WriteMemory<float>(0x7B4D7C, MaxHeatLevel, true); // UIQRCarSelect::InitStatsSliders

		// SetHeatLevel
		injector::WriteMemory<float*>(0x435079, &MaxHeatLevel, true); // AIVehicleHuman::~AIVehicleHuman
		injector::WriteMemory<float>(0x435088, MaxHeatLevel, true);

		// Safehouse car select icon stuff (HEAT_X%.0f)
		injector::WriteMemory<float*>(0x7A5B03, &MaxHeatLevel, true);
		injector::WriteMemory<float*>(0x7A5B12, &MaxHeatLevel, true);
	}

	// Visual Treatment Override
	if (EnableVTOverride)
	{
		injector::MakeRangedNOP(0x73E358, VTCodeCaveExit, true);
		injector::MakeJMP(0x73E358, VTCodeCave, true);
	}

	// Remove Busted Screen Frame
	if (CarbonStyleBustedScreen)
		injector::WriteMemory<unsigned char>(0x8948F8, 0x00, true);

	// Show Debug Car Customize
	if (ShowDebugCarCustomize)
	{
		injector::WriteMemory<DWORD>(0x544F80, 0x1AFD5BE6, true); // "My Cars"
		injector::WriteMemory<DWORD>(0x544F85, 0x74CE8C0B, true); // UI_ICON_DEBUG
		injector::MakeNOP(0x544FA6, 7, true); // UAT Fix
		injector::MakeJMP(0x5451D7, 0x544F5D, true); // Jump to unused?? duplicate menu option
		injector::WriteMemory<DWORD>(0x544F92, 0x89CADC, true); // Menu Code To Execute
		injector::WriteMemory<DWORD>(0x53CED7, 0x89FAD4, true); // "UI_DebugCarCustomize.fng"
	}

	// Show Debug Event ID
	if (ShowDebugEventID)
	{
		// Quick race EventID enable
		injector::MakeCALL(0x007AB0F4, FEngFindObjectHook, true);

		// Challenge Series EventID enable
		injector::MakeCALL(0x007A42AB, FEngFindObjectHook2, true);

		// SafeHouse EventID enable
		injector::MakeCALL(0x00547995, FindPackageHook, true);
	}

	// Pursuit Action Game Mode - Harder Pursuits in Quick Races
	if (PursuitActionMode)
	{
		// Replace Race Table (0x2283ECAF) with Heat Table (0xD4B0CC11)
		injector::WriteMemory<DWORD>(0x403476, 0xD4B0CC11, true);
		injector::WriteMemory<DWORD>(0x409176, 0xD4B0CC11, true);

		// Replace Race Support (0xE5332008) with Heat Support (0xF3918F68)
		injector::WriteMemory<DWORD>(0x403496, 0xF3918F68, true);
		injector::WriteMemory<DWORD>(0x4091D9, 0xF3918F68, true);

		// Can Spawn Roadblocks in Quick Race - AIPursuit::RequestRoadBlock
		injector::MakeNOP(0x419519, 6, true);

		// Cops Can Call Support in Quick Race - AIPursuit::RequestGroundSupport
		injector::WriteMemory<unsigned char>(0x4196FF, 0xEB, true);
		injector::MakeNOP(0x41974B, 2, true);

		// Helicopter - AIPursuit::CopRequest
		injector::MakeNOP(0x42BAD6, 6, true);
	}

	// More Stock Cars for Newly Created Save Games
	if (ShowAllCarsInFE != 0)
	{
		injector::WriteMemory<unsigned char>(0x926127, 0x01, true);
		injector::WriteMemory<unsigned char>(0x581663, ShowAllCarsInFE, true); // Add them to desired category
	}

	// Preset Mode (Only for experts)
	/*if (ShowPresetCarsInFE)
	{
		injector::WriteMemory<unsigned char>(0x926129, 0x01, true);

		// Disable bonus cars creation to save tuning slots
		injector::MakeRangedNOP(0x590E5E, 0x590E68, true);
		injector::WriteMemory<unsigned char>(0x590E68, 0xEB, true);

		// Only use Stock, My Cars and Preset categories in Car Select.
		injector::MakeRangedNOP(0x7BF9B4, 0x7BF9B6, true);
	}*/

	// More Opponent Cars
	if (MoreCarsForOpponents)
	{
		injector::MakeRangedNOP(0x61DA2B, 0x61DA30, true); // force RandomOpponent = 1
		injector::WriteMemory<unsigned char>(0x61DA2B, 0xB0, true); // Game_FindPerformanceCandidates
		injector::WriteMemory<unsigned char>(0x61DA2C, 0x01, true); // Opponents can use bonus cars and cop cars
	}

	// Uncensored Busted Screen
	if (UncensoredBustedScreen)
	{
		injector::WriteMemory<unsigned char>(0x6231D6, 0x00, true); // FacePixelation::mPixelationOn = 0
		injector::WriteMemory<unsigned char>(0x623227, 0x00, true);
	}

	// Show Pursuit Cops
	injector::WriteMemory<unsigned char>(0x8F3AAC, ShowPursuitCops, true);

	// Show Non-Pursuit Cops
	injector::WriteMemory<unsigned char>(0x91cf00, ShowNonPursuitCops, true);

	// Carbon-Style Random Cars (no Vinyls on Random Cars)
	if (CarbonStyleRandomCars)
	{
		injector::WriteMemory<unsigned char>(0x75B229, 0, true);
	}

	// Time Of Day (Replaces Level of Detail)
	if (ShowTimeOfDay)
	{
		// VOTimeOfDay::Act(char const *,uint)
		injector::WriteMemory<DWORD>(0x89BB44, 0x50F7A0, true); // change code location
		injector::MakeRangedNOP(0x50F7AE, 0x50F7B6, true); // cleanup

		char LoadToEcx[] = { 0x8B, 0x0D, 0x2C, 0x39, 0x9B, 0x00 }; // mov ecx,[009B392C]
		injector::WriteMemoryRaw(0x50F7AE, LoadToEcx, sizeof(LoadToEcx), true);
		injector::WriteMemory<unsigned char>(0x50F7BE, 0x08, true); // mov [ecx+8], edx


		// VOTimeOfDay::Draw(void)
		injector::WriteMemory<DWORD>(0x89BB4C, 0x51B5E0, true);
		injector::WriteMemory<DWORD>(0x51B5F5, 0x999D1383, true); // "Time of Day"
		char LoadToEcx2[] = { 0x8B, 0x0D, 0x2C, 0x39, 0x9B, 0x00, 0x90, 0x90, 0x90, 0x8B, 0x49, 0x08 }; // mov ecx,[009B392C]; mov ecx,[ecx+08]
		injector::WriteMemoryRaw(0x51B617, LoadToEcx2, sizeof(LoadToEcx2), true);


		// VOTimeOfDay::SetInitialValues(void)
		char LoadTimeOfDay[] = { 0x8B, 0x15, 0x2C, 0x39, 0x9B, 0x00, 0x8B, 0x52, 0x08 }; // mov edx,[009B392C]; mov edx,[edx+08]
		injector::WriteMemoryRaw(0x50F9E0, LoadTimeOfDay, sizeof(LoadTimeOfDay), true);

		injector::MakeRangedNOP(0x50F9F5, 0x50F9F8, true); // nop fstp
		injector::WriteMemory<unsigned char>(0x0050F9F4, 0x52, true); // push edx

		injector::WriteMemory<float>(0x50F9F9, 0.01f, true); // Step Size
		injector::WriteMemory<float>(0x50F9FE, 0.95f, true); // Max Value
		injector::WriteMemory<float>(0x50FA03, 0.05f, true); // Min Value
	}

	// No Catch Up
	if (NoCatchUp)
	{
		// AIPerpVehicle::GetCatchupCheat
		injector::WriteMemory(0x892240, &Return0fHook, true); // AIPerpVehicle::ICheater::`vtbl'
		injector::WriteMemory(0x8925C8, &Return0fHook, true); // AIVehicleRacecar::ICheater::`vtbl'

		// Make Catch Up Controller always 0
		injector::MakeRangedNOP(0x7AC203, 0x7AC206, true); // nop setz bl
		injector::WriteMemory<short>(0x7AC203, 0xDB30, true); // xor bl, bl
		injector::WriteMemory<unsigned char>(0x7AC245, 0xEB, true);
		
		// Freeze it in menu
		injector::WriteMemory<unsigned char>(0x7ACA68, 0, true); // Circuit
		injector::WriteMemory<unsigned char>(0x7ACC4B, 0, true); // Sprint
		injector::WriteMemory<unsigned char>(0x7ACDEE, 0, true); // Drag
		injector::WriteMemory<unsigned char>(0x7AD022, 0, true); // LAP KO
		injector::WriteMemory<unsigned char>(0x7AD26B, 0, true); // Speedtrap
	}

	// No rev limiter
	if (NoRevLimiter)
	{
		injector::WriteMemory(0x8AB78C, &Return0Hook, true); // EngineRacer::UseRevLimiter(void)
		injector::WriteMemory(0x8AC034, &Return0Hook, true); // EngineDragster::UseRevLimiter(void)
	}

	// Marker Count
	injector::WriteMemory<unsigned char>(0x7B3D7D, SelectableMarkerCount, true);
	injector::WriteMemory<unsigned char>(0x7B3E19, SelectableMarkerCount, true);
	injector::WriteMemory<unsigned char>(0x7A7A3C, SelectableMarkerCount, true);
	injector::WriteMemory<unsigned char>(0x7A7ABE, SelectableMarkerCount, true);

	// Garage Car Slot Limit
	injector::WriteMemory<unsigned char>(0x7C1A77, PurchasedCarLimit, true);

	// Debug Cam Speed
	injector::WriteMemory<float>(0x8EDEA4, DebugCameraTurboSpeed, true);
	injector::WriteMemory<float>(0x8EDEA8, DebugCameraSuperTurboSpeed, true);

	// Speedbreaker
	injector::WriteMemory<float>(0x901AE4, SBRechargeTime, true);
	injector::WriteMemory<float>(0x901AE8, SBRechargeSpeedLimit, true);
	injector::WriteMemory<float>(0x901AEC, SBMassMultiplier, true);

	// Infractions
	injector::WriteMemory<float>(0x8A3810, SpeedingLimit, true);
	injector::WriteMemory<float>(0x8A380C, ExcessiveSpeedingLimit, true);
	injector::WriteMemory<float>(0x8A3814, RecklessDrivingLimit, true);

	// Allow Longer Save Game Names
	if (AllowLongerProfileNames)
	{
		injector::WriteMemory<unsigned char>(0x527570, 15, true); // UIProfileMenu::ShowKeyboard - Input Limit on Save Game Creation
		injector::WriteMemory<unsigned char>(0x52758E, 0, true); // UIProfileMenu::ShowKeyboard - Allow every character
		injector::WriteMemory<unsigned char>(0x563A7A, 16, true); // UIMemcardList::NotificationMessage - Include Longer Names in the Load Save list
		injector::WriteMemory<unsigned char>(0x563A7E, '/', true); // UIMemcardList::NotificationMessage - Ignore saves with this character
	}

	// Car Skin Fix
	if (CarSkinFix)
	{
		injector::WriteMemory<unsigned char>(0x75D2B6, 16, true); 
		// injector::WriteMemory<unsigned char>(0x747F22, 12, true); // CARS\TEXTURES.BIN has 12 dummy skins

		injector::MakeJMP(0x747F19, CarSkinFixCodeCave, true);
		injector::MakeRangedNOP(0x73B324, 0x73B328, true); //crash fix??
	}

	// Immobile collisions fix
	if (ImmobileColFix)
	{
		injector::MakeRangedNOP(0x6982C5, 0x6982CB, true);
		injector::WriteMemory<short>(0x6982C5, 0x00B1, true);

		injector::WriteMemory<unsigned char>(0x69828F, 0, true); // RigidBody::IsImmobile
	}

	// DDay fix for save games with progress
	if (DDayFix)
	{
		injector::MakeJMP(0x57F5A9, DDayFixCodeCaveResumeCareer, true);
		injector::MakeJMP(0x531FCA, DDayFixCodeCaveCareerCribNotificationMessage, true);
	}

	// Fix and enable language select screen
	if (ShowLanguageSelectScreen)
	{
		injector::WriteMemory(0x8F3C74, 0x89F7EC, true); // Replace DiscErrorPC.fng with LS_LangSelect.fng
		injector::WriteMemory(0x8F3C94, 0x89F7EC, true); // Replace DiscErrorPC.fng with LS_LangSelect.fng
		injector::WriteMemory(0x8F3CB0, 0x89F7EC, true); // Replace DiscErrorPC.fng with LS_LangSelect.fng
		injector::MakeJMP(0x58E9FC, 0x58EAB4, true); // BootFlowManager::BootFlowManager (Force PAL for lang select)
		injector::WriteMemory<short>(0x64A140, 0x01B0, true); // BuildRegion::ShowLanguageSelect - mov al,1
		injector::MakeCALL(0x5A47F9, LanguageSelectScreen_LanguageSelectScreen, true); // CreateLanguageSelectScreen
	}

	// Screen Printf
	if (DoScreenPrintf)
	{
		injector::WriteMemory(0x926140, 1, true); // Vanilla variable
		injector::MakeJMP(0x666110, InitScreenPrintfCodeCave, true);
		injector::MakeJMP(0x64A80F, DisplayDebugScreenPrints, true);
		injector::WriteMemory(0x89B45C, ResetScreenPrintf, true); // cFEngGameInterface::EndPackageRendering

		// Code caves for functions which had screen printfs in Alpha 124
		injector::MakeJMP(0x542A42, MoviePlayer_Play_ScreenPrintf, true);
		injector::MakeJMP(0x564471, FEManager_Update_ScreenPrintf, true);
		injector::MakeJMP(0x7A8D3B, FindScreenInfo_ScreenPrintf, true); // my cars crash
		injector::MakeJMP(0x7A8EBD, FindScreenInfo_ScreenPrintf2, true);
	}

	// World Map Anywhere
	if (WorldMapAnywhere)
	{
		injector::MakeRangedNOP(0x57C934, 0x57C93A, true);
	}

	// Other Things
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Thing, NULL, 0, NULL);
	injector::MakeCALL(0x663EE8, Thing, true); // Hook the hotkey detection to the main loop
}

void Thing()
{
	//Sleep(ThreadDelay);
	_asm pushad;

	raceOptions = *(DWORD*)0x91CF90; // Race Options Pointer (Thanks to samfednik)
	GameState = *(DWORD*)0x0925E90; // 3 = FE, 4&5 = Loading screen, 6 = Gameplay
	windowHandle = *(HWND*)0x982BF4;
	IsOnFocus = !(*(bool*)0x982C50);

	// Windowed Mode Related Fixes (Center and Resize)
	if (WindowedMode && windowHandle && !once2)
	{
		RECT o_cRect, n_cRect, n_wRect;
		GetClientRect(windowHandle, &o_cRect);

		DWORD wStyle = GetWindowLongPtr(windowHandle, GWL_STYLE);

		switch (WindowedMode)
		{
		case 1: wStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU); break;
		case 2: default: wStyle |= (WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW); break;
		}

		SetWindowLongPtr(windowHandle, GWL_STYLE, wStyle);

		// make window change style
		SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);

		GetWindowRect(windowHandle, &n_wRect);
		GetClientRect(windowHandle, &n_cRect);
		int n_wWidth = n_wRect.right - n_wRect.left;
		int n_wHeight = n_wRect.bottom - n_wRect.top;
		int dif_wWidth = o_cRect.right - n_cRect.right;
		int dif_wHeight = o_cRect.bottom - n_cRect.bottom;
		int newWidth = n_wWidth + dif_wWidth;
		int newHeight = n_wHeight + dif_wHeight;

		HWND hDesktop = GetDesktopWindow();
		RECT desktop;
		GetWindowRect(hDesktop, &desktop);

		int newXPos = ((desktop.right - desktop.left) - newWidth) / 2;
		int newYPos = ((desktop.bottom - desktop.top) - newHeight) / 2;

		SetWindowPos(windowHandle, NULL, newXPos, newYPos, newWidth, newHeight, SWP_NOZORDER | SWP_NOACTIVATE);

		once2 = 1;
	}

	// Ultimate Force Heat Level
	if ((GetAsyncKeyState(hotkeyToggleForceHeat) & 1) && (GameState == 6) && IsOnFocus) //When pressed "Toggle Force Heat"
	{
		forceHeatLevel = !forceHeatLevel; // Toggle option
		if (forceHeatLevel)
		{
			EnableHeatLevelOverride = 1;
			heatLevel = MinHeatLevel - 1;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have enabled Ultimate Force Heat Level hack.^You can now press ChangeHeatLevel (PageUp as default) key to increase your heat level.");
			}
		}
		else
		{
			// Re-read the override boolean from config file
			CIniReader iniReader("NFSMWExtraOptionsSettings.ini");
			EnableHeatLevelOverride = iniReader.ReadInteger("Pursuit", "HeatLevelOverride", 0) == 1;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have disabled Ultimate Force Heat Level hack.");
			}
		}

		if (EnableHeatLevelOverride)
		{
			// CustomizeMeter::Init
			injector::WriteMemory<float>(0x7bb502, MaxHeatLevel, true); // CustomizeCategoryScreen
			injector::WriteMemory<float>(0x7b1387, MaxHeatLevel, true); // CustomizationScreenHelper
			injector::WriteMemory<float>(0x7b0c89, MaxHeatLevel, true); // CustomizeShoppingCart::Setup
			injector::WriteMemory<float>(0x7B4D7C, MaxHeatLevel, true); // UIQRCarSelect::InitStatsSliders

			// SetHeatLevel
			injector::WriteMemory<float*>(0x435079, &MaxHeatLevel, true); // AIVehicleHuman::~AIVehicleHuman
			injector::WriteMemory<float>(0x435088, MaxHeatLevel, true);
		}
		else
		{
			// CustomizeMeter::Init
			injector::WriteMemory<float>(0x7bb502, 5.0f, true); // CustomizeCategoryScreen
			injector::WriteMemory<float>(0x7b1387, 5.0f, true); // CustomizationScreenHelper
			injector::WriteMemory<float>(0x7b0c89, 5.0f, true); // CustomizeShoppingCart::Setup
			injector::WriteMemory<float>(0x7B4D7C, 5.0f, true); // UIQRCarSelect::InitStatsSliders

			// SetHeatLevel
			injector::WriteMemory<DWORD>(0x435079, 0x890DA4, true); // AIVehicleHuman::~AIVehicleHuman
			injector::WriteMemory<float>(0x435088, 5.0f, true);
		}
	}

	if ((GetAsyncKeyState(hotkeyForceHeatLevel) & 1) && forceHeatLevel && (GameState == 6) && IsOnFocus) //When pressed "Forced Heat Level"
	{
		heatLevel = (int)(floorf(heatLevel) - floorf(MinHeatLevel) + 1) % (int)(floorf(MaxHeatLevel) - floorf(MinHeatLevel) + 1) + MinHeatLevel;
			
		Game_SetWorldHeat(heatLevel); // Use direct function call to set the heat level
	}

	if ((GetAsyncKeyState(hotkeyToggleCops) & 1) && forceHeatLevel && (GameState == 6) && IsOnFocus) // Toggle Cops
	{
		ToggleCops = !ToggleCops;
		Game_SetCopsEnabled(ToggleCops);

		if (ToggleCops)
		{
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Cops are now enabled.");
			}
		}
		else
		{
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Cops are now disabled.");
			}
		}
	}

	// Cop Lights
	if ((GetAsyncKeyState(hotkeyToggleCopLights) & 1) && (GameState == 6) && IsOnFocus) // When pressed "Toggle Police Lights" key
	{
		copLightsEnabled = !copLightsEnabled;
			
		DWORD* PlayerPVehicle = *(DWORD**)0x0092CD28;

		if (PlayerPVehicle)
		{
			if (copLightsEnabled) // Enable cop lights - more proper method
			{
				if (!PVehicle_IsGlareOn(PlayerPVehicle, 0x7000)) PVehicle_GlareOn(PlayerPVehicle, 0x7000);
			}
			else // Disable cop lights
			{
				if (PVehicle_IsGlareOn(PlayerPVehicle, 0x7000)) PVehicle_GlareOff(PlayerPVehicle, 0x7000);
			}
		}
	}

	// Headlights
	if ((GetAsyncKeyState(hotkeyToggleHeadlights) & 1) && (GameState == 6) && IsOnFocus) // When pressed "Toggle Head Lights" key
	{
		headlightsMode = !headlightsMode;

		DWORD* PlayerPVehicle = *(DWORD**)0x0092CD28;

		if (PlayerPVehicle)
		{
			if (headlightsMode) // Enable headlights - more proper method
			{
				if (!PVehicle_IsGlareOn(PlayerPVehicle, 7))
				{
					injector::WriteMemory<unsigned char>(0x6703D6, 7, true);
					PVehicle_GlareOn(PlayerPVehicle, 7);
						
				}
			}
			else // Disable headlights
			{
				if (PVehicle_IsGlareOn(PlayerPVehicle, 7))
				{
					injector::WriteMemory<unsigned char>(0x6703D6, 0, true);
					PVehicle_GlareOff(PlayerPVehicle, 7);
				}
			}
		}
	}


	// Car Hack
	if ((GetAsyncKeyState(hotkeyCarHack) & 1) && IsOnFocus) // When pressed "Toggle Freeze Car" key
	{
		carHackEnabled = !carHackEnabled;

		if (carHackEnabled)
		{
			raceType = *(unsigned char*)(raceOptions + 0x18); // Get the race type
			raceMode = *(unsigned char*)(raceOptions + 0x12C); // Get the mode. (1 = Career, 4 = Quick Race, 32 = My Cars)
			careerCar = *(DWORD*)(*(DWORD*)(raceOptions + 0x10) + 0xA8);

			switch (raceMode)
			{
			case 1: selectedCar = *(DWORD*)(*(DWORD*)(raceOptions + 0x10) + 0xA8); break; //Freeze career car
			case 4: selectedCar = *(DWORD*)(raceOptions + 0x2C + (raceType * 0x18)); break; // Freeze QR Car
			case 32: selectedCar = *(DWORD*)(raceOptions + 0x2C); break; //Freeze my car
			default: carHackEnabled = 0; break; // Don't enable it while not in a car select screen
			}

			if (carHackEnabled && ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have enabled Freeze Car Hack.^You can now use this car anywhere.");
			}

		}
		else
		{
			// Fix Challenge Series behaviour
			injector::WriteMemory<unsigned char>(0x6f48e7, 0x74, true);

			// Fix Career Car
			unsigned int CareerCarPointer = *(DWORD*)(raceOptions + 0x10) + 0xA8;
			*(unsigned int*)CareerCarPointer = careerCar;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have disabled Freeze Car Hack.^You can now select cars normally.");
			}
		}
	}

	if (carHackEnabled)
	{
		unsigned int PointerToWrite;
		// Freeze values
		PointerToWrite = raceOptions + 0x2C;  // Sprint (0)
		*(unsigned int*)PointerToWrite = selectedCar;
			
		PointerToWrite = raceOptions + 0x2C + (0x18); // Circuit (1)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (2 * 0x18); // Drag (2)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (3 * 0x18); // Lap Knockout (3)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (4 * 0x18); // Tollbooth (4)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (5 * 0x18); // Speedtrap (5)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (8 * 0x18); // Challenge (8)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = *(DWORD*)(raceOptions + 0x10) + 0xA8; // Career
		*(unsigned int*)PointerToWrite = selectedCar;

		injector::WriteMemory<unsigned char>(0x6f48e7, 0xEB, true); // Use selected cars at Challenge Series
	}

	// Drunk Driver
	if ((GetAsyncKeyState(hotkeyAutoDrive) & 1) && (GameState == 6) && IsOnFocus) // When pressed "Drunk Driver" key
	{
		AutoDrive = !AutoDrive;

		if (AutoDrive)
		{
			Game_ForceAIControl(1);
		}
		else
		{
			Game_ClearAIControl(1);
		}
	}

	// Freeze Camera
	if ((GetAsyncKeyState(hotkeyFreezeCamera) & 1) && (GameState == 6) && IsOnFocus)
	{
		*(bool*)0x911020 = !(*(bool*)0x911020);
	}

	// Unlock All Things
	if ((GetAsyncKeyState(hotkeyUnlockAllThings) & 1) && IsOnFocus) // When pressed "Unlock All Things" key
	{
		UnlockAllThings = !UnlockAllThings;
		CIniReader iniReader("NFSMWExtraOptionsSettings.ini");
		iniReader.WriteInteger("Gameplay", "UnlockAllThings", UnlockAllThings);

		if (UnlockAllThings)
		{
			*(unsigned char*)0x926124 = 1;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have enabled UnlockAllThings hack.^Everything is now unlocked. Enjoy!");
			}

		}
		else
		{
			*(unsigned char*)0x926124 = 0;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have disabled UnlockAllThings hack.^Unlock status of everything is returned back to normal.");
			}
		}
	}
		
	// .hot Save And Load
	if (EnableSaveLoadHotPos && (GameState == 6) && IsOnFocus)
	{
		// Save
		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(49) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 1;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 1);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(50) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 2;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 2);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(51) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 3;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 3);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(52) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 4;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 4);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(53) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 5;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 5);
			}
		}

		// Load
		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(49) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 1;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 1);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(50) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 2;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 2);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(51) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 3;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 3);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(52) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 4;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 4);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(53) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 5;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 5);
			}
		}
	}

	// Debug Camera
	if (EnableDebugWorldCamera && (GameState == 6) && IsOnFocus)
	{
		if ((GetAsyncKeyState(VK_BACK) & 1)) // Backspace : Toggle Debug World Camera
		{
			DebugWorldCamera = !DebugWorldCamera;

			if (*(int*)0x00925E90 == 6 && DebugWorldCamera)
			{
				injector::WriteMemory<unsigned char>(0x750F4C, 0xEB, true); // fix crash??
				*(unsigned int*)0x0091102C = 0;
				CameraAI_SetAction(1, "CDActionDebug");
			}

			if (*(int*)0x00925E90 == 6 && !DebugWorldCamera)
			{
				CameraAI_SetAction(1, "CDActionDrive");
				*(unsigned char*)0x911038 = 0;
				injector::WriteMemory<unsigned char>(0x750F4C, 0x75, true);
			}
		}

		if ((GetAsyncKeyState(VK_NEXT) & 1)) // Page Down : Toggle Debug Watch Car Camera
		{
			DebugWatchCarCamera = !DebugWatchCarCamera;
			*(unsigned char*)0x00911058 = 1;

			if (*(int*)0x00925E90 == 6 && DebugWatchCarCamera)
			{
				injector::WriteMemory<unsigned char>(0x750F4C, 0xEB, true); // fix crash??
				*(unsigned char*)0x911038 = 1;
				CameraAI_SetAction(1, "CDActionDebugWatchCar");
			}

			if (*(int*)0x00925E90 == 6 && !DebugWatchCarCamera)
			{
				CameraAI_SetAction(1, "CDActionDrive");
				*(unsigned char*)0x911038 = 0;
				injector::WriteMemory<unsigned char>(0x750F4C, 0x75, true);
			}
		}

		if ((GetAsyncKeyState(VK_ADD) & 1) && DebugWatchCarCamera) // + : Next car
		{
			*(unsigned char*)0x00911058 += 1;
		}

		if ((GetAsyncKeyState(VK_SUBTRACT) & 1) && DebugWatchCarCamera) // - : Previous car
		{
			*(unsigned char*)0x00911058 -= 1;
		}
	}

	// Continue after a dialog is closed
	if ((GameState == 6) && (ButtonResult == DialogBoxReturn))
	{
		ButtonResult = 0;
		UnPause(NULL);
	}

	_asm popad;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x7C4040) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			Init();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.3 English speed.exe (5,75 MB (6.029.312 bytes)).", "NFSMW Extra Options", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}

