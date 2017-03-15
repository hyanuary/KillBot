#include "bot.h"
#include "time.h"

extern "C"
{
	BotInterface27 BOT_API *CreateBot27()
	{
		return new Blank();
	}
}

Blank::Blank()
{
	m_rand(time(0)+(int)this);
}

Blank::~Blank()
{
}

void Blank::init(const BotInitialData &initialData, BotAttributes &attrib)
{
	m_initialData = initialData;
	attrib.health= 1.0;
	attrib.motor= 1.0;
	attrib.weaponSpeed= 1.0;
	attrib.weaponStrength= 1.0;
	dir.set(3, 3);
	scanAngle = 0;
	randomLocation1 = m_rand() % (30 - 1 + 1) + 1;
	randomLocation2 = m_rand() % (30 - 1 + 1) + 1;
	randomLocation3 = m_rand() % (30 - 1 + 1) + 1;
	randomLocation4 = m_rand() % (30 - 1 + 1) + 1;
	timer = 1;
	m_map.init(initialData.mapData.width, initialData.mapData.height);
	//loking at the tile if it's wall or not
	for (int y = 0; y < initialData.mapData.height; ++y) {
		for (int x = 0; x < initialData.mapData.width; ++x)
		{
			Node& node = m_map.getNode(NodePos(x, y));
			if (initialData.mapData.data[x + y * initialData.mapData.width].wall)
				node.wall = true;
			else
			{
				node.wall = false;
			}
		}
	}
}

void Blank::update(const BotInput &input, BotOutput27 &output)
{
	bool nowShooting = false;
	timer += 1;

	//finding the enemy
	if (input.scanResult.size() > 0)
	{
		for (int i = 0;i < input.scanResult.size();++i)
		{
			if (input.scanResult[i].type == VisibleThing::e_robot)
			{
				m_currentEnemyPos = input.scanResult[i].position;
				nowShooting = true;
				

				break;
			}
		}
	}

	//looking for the parents of each nodes
	NodePos parents;
	NodePos current(input.position.x, input.position.y);
	parents = m_map.getNode(current).parent;

	//moving the bot
	output.moveDirection.set(parents.x - input.position.x + 0.5, parents.y - input.position.y + 0.5);
	output.motor = 0.1;


	//Shooting
	if (nowShooting)
	{
		output.lookDirection = m_currentEnemyPos - input.position;
		output.moveDirection = m_currentEnemyPos;
		output.action = BotOutput::shoot;
		scanAngle -= m_initialData.scanFOV * 3;
	}
	else
	//Scanning
	{
		scanAngle += m_initialData.scanFOV * 2;
		if (input.health < 200)
		{
			scanAngle += m_initialData.scanFOV ;
		}
		output.lookDirection.set(sin(scanAngle), cos(scanAngle));
		output.action = BotOutput::scan;
	}


	if (timer > 1000)
	{
		randomLocation1 = m_rand() % (30 - 1 + 1) + 1;
		randomLocation2 = m_rand() % (30 - 1 + 1) + 1;
		randomLocation3 = m_rand() % (30 - 1 + 1) + 1;
		randomLocation4 = m_rand() % (30 - 1 + 1) + 1;
		timer = 0;
	}
	
	//rendering text
	output.text.clear();
	char buf[50];
	sprintf(buf, "%d", input.health);
	output.text.push_back(TextMsg(buf, input.position - kf::Vector2(0.0f, 1.0f), 0.0f, 0.7f, 1.0f, 80));

	output.lines.clear();
	

	//calling pathfinding
	pathFinding(NodePos (1, 1), NodePos(30, 29));
	
	//drawing
	for (int y = 1; y < m_initialData.mapData.height - 1; ++y) 
	{
		for (int x = 1; x < m_initialData.mapData.width - 1; ++x)
		{
			Node& node = m_map.getNode(NodePos(x, y));
			Line l;
			l.start.set(x + 0.5, y + 0.5);
			l.end.set(node.parent.x + 0.5, node.parent.y + 0.5);
			if (node.parent.x != -1 && node.parent.y != -1)
			{
				output.lines.push_back(l);
			}
		}
	}
	//for (int y = 0; y < m_initialData.mapData.height; ++y) 
	//{
	//	for (int x = 0; x < m_initialData.mapData.width; ++x)
	//	{
	//		Node& node = m_map.getNode(NodePos(x, y));
	//		Line l;
	//		l.start.set(x + 0.2, y + 0.2);
	//		l.end.set(x+0.8,y+0.8);
	//		if (node.state == Node::StateClosed)
	//		{
	//			output.lines.push_back(l);
	//		}
	//	}
	//}


}

void Blank::result(bool won)
{
}

void Blank::bulletResult(bool hit)
{

}

void Blank::pathFinding(const NodePos &startNode, NodePos &endNode)
{
	m_map.clear();
	openList.push_back(startNode);
	pathFound = false;
	while (openList.size() > 0 /*&& pathFound == false*/)
	{
		NodePos currNode;
		currNode = openList.front();
		int j = 0;
		for (int i = 0;i < openList.size();++i)
		{
			if (m_map.getNode(openList[i]).f < m_map.getNode(currNode).f)
			{
				currNode = openList[i];
				j = i;
			}
		}
		openList[j] = openList.back();
		openList.pop_back(); //change it to make it move the other way
		m_map.getNode(currNode).state = Node::StateClosed;
			for (int oy = -1; oy < 2; oy++)
			{
				for (int ox = -1; ox < 2; ox++)
				{
					if (ox == 0 && oy == 0 || ox != 0 && oy != 0)
					{
						continue;
					}
					NodePos adj;
					adj = currNode + NodePos(ox, oy);
					Node &adj1 = m_map.getNode(adj);
					int dx = abs(startNode.x - endNode.x);
					int dy = abs(startNode.y - endNode.y);
					int D = 1;
					int H = D *(dx + dy);
					int G = currNode.g + adj1.c;
					if (adj1.state == Node::StateClosed)
					{

					}
					else if (adj1.state == Node::StateOpen && G < adj1.g)
					{
						adj1.g = G;
						adj1.h = H;
						adj1.parent = currNode;
						adj1.f = adj1.g + adj1.h;
					}
					else if(adj1.state == Node::StateNone)
					{
						adj1.g = G;
						adj1.h = H;
						adj1.parent = currNode;
						adj1.f = adj1.g + adj1.h;
						adj1.state = Node::StateOpen;
						openList.push_back(adj);
					}
					if (adj.x == endNode.x && adj.y == endNode.y)
						pathFound = true;
				}
		
		}
		
	}
}