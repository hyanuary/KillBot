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
	attrib.health=1.0;
	attrib.motor=10.0;
	attrib.weaponSpeed=10.0;
	attrib.weaponStrength=10.0;
	
	dir.set(0, 0);
	scanAngle = 0;
	m_map.init(initialData.mapData.width, initialData.mapData.height);
}

void Blank::update(const BotInput &input, BotOutput27 &output)
{
	bool nowShooting = false;

	if (input.scanResult.size() > 0)
	{
		for (int i = 0;i < input.scanResult.size();++i)
		{
			if (input.scanResult[i].type == VisibleThing::e_robot)
			{
				m_currentEnemyPos = input.scanResult[i].position;
				nowShooting = true;
				timer += 1;

				break;
			}
		}
	}

	random1 += m_rand() / 2.0;
	dir.set(random1,random1);
	output.moveDirection = dir - input.position;
	if (output.moveDirection.length() < 2)
	{
		//dir.set(m_rand.norm()*3.0 - 1.0, m_rand.norm()*4.0 - 1.0);
		
	}
	//dir.set(m_rand() % (m_initialData.mapData.width - 2) + 1.5, m_rand() % (m_initialData.mapData.width - 2) + 1.5);
	//int go;
	//go += 1;
	//if (go > 10)
	//{
	//	//dir.set(m_rand.norm()*3.0 - 1.0, m_rand.norm()*4.0 - 1.0);
	//	dir.set(m_rand() % (m_initialData.mapData.width - 2) + 1.5, m_rand() % (m_initialData.mapData.width - 2) + 1.5);
	//}

	output.motor = 10.0;
	output.lookDirection.set(m_rand.norm()*3.0, m_rand.norm()*4.0);
	//output.action = BotOutput::scan;


	//Shooting
	if (nowShooting)
	{
		output.lookDirection = m_currentEnemyPos - input.position;
		output.moveDirection = output.lookDirection;
		dir -= m_currentEnemyPos;
		output.motor = 0;
		output.action = BotOutput::shoot;
		scanAngle -= m_initialData.scanFOV * 2;
	}
	else
	//Scanning
	{
		scanAngle += m_initialData.scanFOV * 2;
		output.lookDirection.set(cos(scanAngle), sin(scanAngle));
		output.action = BotOutput::scan;
	}
	
}

void Blank::result(bool won)
{
}

void Blank::bulletResult(bool hit)
{

}
