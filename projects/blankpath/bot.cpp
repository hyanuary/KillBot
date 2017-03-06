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
	timer = 50.0f;
	dir.set(2, 28);
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
				

				break;
			}
		}
	}

	random1 = m_rand() % 2;
	
	output.moveDirection = dir - input.position;
	//dir.set(m_rand() % (m_initialData.mapData.width - 2) + 1.5, m_rand() % (m_initialData.mapData.width - 2) + 1.5);

	if (output.moveDirection.length() < 2)
	{
		//dir.set(2, 2);
	}
	output.motor = 10.0;
	//output.lookDirection.set(m_rand.norm()*3.0, m_rand.norm()*4.0);


	//Shooting
	if (nowShooting)
	{
		output.lookDirection = m_currentEnemyPos - input.position;
		output.moveDirection = m_currentEnemyPos;
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

	if (input.health / 350 == 1)
		dir.set(28, 2);
	if (input.health / 200 == 1)
		dir.set(28, 28);
	if (input.health / 150 == 1)
		dir.set(2, 28);
	if (input.health / 50 == 1)
	{
		dir.set(2, 2);
	}
		
	//rendering text
	output.text.clear();
	char buf[100];
	sprintf(buf, "%d", input.health);
	output.text.push_back(TextMsg(buf, input.position - kf::Vector2(0.0f, 1.0f), 0.0f, 0.7f, 1.0f, 80));
	
}

void Blank::result(bool won)
{
}

void Blank::bulletResult(bool hit)
{

}
