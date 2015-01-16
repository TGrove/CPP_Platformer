#pragma once

#include <string>
#include <vector>

class TileLayer;
class BackgroundLayer;
class Cloud;
class Enemy;

class LevelData
{
public:
	LevelData(void);
	~LevelData(void);
	void initialize();
	void Load(std::string levelName);

private:
	void LoadTileLayers();
	void LoadBackgroundLayers();
	void LoadClouds();

private:
	TileLayer *mCollisionLayer;
	TileLayer *mTextureLayer;
	
	std::vector<BackgroundLayer *> mBackgrounds;
	std::vector<Cloud *> mClouds;

	std::vector<Enemy *> mEnemies;
};

