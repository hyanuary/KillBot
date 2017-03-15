#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"
#include "kf/kf_random.h"
#include "pathfind.h"

#ifdef BOT_EXPORTS
#define BOT_API __declspec(dllexport)
#else
#define BOT_API __declspec(dllimport)
#endif

class Blank:public BotInterface27
{
public:
	Blank();
	virtual ~Blank();
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib);
	virtual void update(const BotInput &input, BotOutput27 &output);
	virtual void result(bool won);
	virtual void bulletResult(bool hit);
	virtual void pathFinding(const NodePos &startNode, NodePos &endNode);
	

	kf::Xor128 m_rand;
	BotInitialData m_initialData;
	kf::Vector2 dir;
	kf::Vector2 currDir;
	kf::Vector2 m_currentEnemyPos;
	float scanAngle;
	int randomLocation1;
	int randomLocation2;
	int randomLocation3;
	int randomLocation4;

	int timer;

	//finding path
	std::vector<NodePos> openList;
	bool pathFound;
	

	Map m_map;
};


#endif