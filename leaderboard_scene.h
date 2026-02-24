#pragma once

#include "scene.h"
#include "game_scene.h"
#include "config.h"

class LeaderboardScene : public Scene
{
public:
	LeaderboardScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox);
	~LeaderboardScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	//All items on the leaderboard
	std::vector<GUIObject*> leaderboard_items;
	float time=0;
};