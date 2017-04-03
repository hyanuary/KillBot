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
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib); //initial value
	virtual void update(const BotInput &input, BotOutput27 &output); //update
	virtual void result(bool won); //not implemented
	virtual void bulletResult(bool hit); //not implemented
	virtual void pathFinding(const NodePos &startNode, NodePos &endNode);
	

	kf::Xor128 m_rand; // random number
	BotInitialData m_initialData; //initial variable pick
	kf::Vector2 dir; //direction -> where the want to go
	kf::Vector2 currDir; //current direction
	kf::Vector2 m_currentEnemyPos; //current enemy location -> to locate the enemy
	float scanAngle; //scannig angle
	
	
	
	
	int randomLocation1; //random location
	int randomLocation2;
	int randomLocation3;
	int randomLocation4;

	int timer; //timer

	//finding path
	std::vector<NodePos> openList; //list for the node
	bool pathFound; //check for if the path is found or not
	

	Map m_map; //map variables
};


#endif