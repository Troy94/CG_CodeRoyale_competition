#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <ctime>

using namespace std;

const int MAP_WIDTH_PIXELS = 1920;
const int MAP_HEIGHT_PIXELS = 1000;
const int MAP_WIDTH_TILES = 48;
const int MAP_HEIGHT_TILES = 25;
const float TILE_WIDTH = 40;
const float TILE_HEIGHT = 40;

const int NO_STRUCTURE = -1;
const int GOLDMINE = 0;
const int TOWER = 1;
const int BARRACKS = 2;

const int ME_OR_FREE = -3;
const int ANY = -2;
const int FREE = -1;
const int ME = 0;
const int ENEMY = 1;

const int QUEEN = -1;
const int KNIGHT = 0;
const int ARCHER = 1;
const int GIANT = 2;

enum class MAP_SIDE
{
	LEFT = 0,
	RIGHT = 1
};

//##########################################################
struct Vector2d
{
	Vector2d() {}
	Vector2d(int pX, int pY) : x(pX), y(pY)
	{

	}

	inline bool operator==(const Vector2d& v2)
	{
		if (x == v2.x and x == v2.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline Vector2d operator+(const Vector2d& v2)
	{
		Vector2d result{};
		result.x = x + v2.x;
		result.y = y + v2.y;
		return result;
	}

	inline Vector2d operator-(const Vector2d& v2)
	{
		Vector2d result{};
		result.x = x - v2.x;
		result.y = y - v2.y;
		return result;
	}

	int x;
	int y;
};

int GetTilemapIndex(int pX, int pY)
{
	return (MAP_WIDTH_TILES * pY) + pX;
}

int GetManhattanDistance(Vector2d v1, Vector2d v2)
{
	return abs(v2.x - v1.x) + abs(v2.y - v1.y);
}

int GetDistance(Vector2d v1, Vector2d v2)
{
	return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

Vector2d TileToScreen(Vector2d tileCoords)
{
	return Vector2d(tileCoords.x * TILE_WIDTH, tileCoords.y * TILE_HEIGHT);
}

Vector2d ScreenToTile(Vector2d screenCoords)
{
	return Vector2d(floor(screenCoords.x / TILE_WIDTH), floor(screenCoords.y / TILE_HEIGHT));
}

struct Unit
{
	Unit() {}
	Unit(int pOwner, int pType, int pHealth, Vector2d pLoc)
	{
		owner = pOwner;
		unitType = pType;
		health = pHealth;
		position = pLoc;
	}
	int owner;
	int unitType;
	int health;
	Vector2d position;
};

struct Site
{
	Site() {}
	Site(int pID, Vector2d pLoc, int pRad)
	{
		siteId = pID;
		position = pLoc;
		radius = pRad;
	}
	int siteId;
	Vector2d position;
	int radius;
	int gold;
	int maxMineSize;
	int structureType;
	int owner;
	int param1; // num turns before training
	int param2; // creep type
};

struct Tile
{
	Vector2d position;
	bool hasSite{ false };
};


struct TileMap
{
	TileMap()
	{
		tiles.resize(MAP_HEIGHT_TILES * MAP_WIDTH_TILES);

		for (int rows = 0; rows < MAP_HEIGHT_TILES; ++rows)
		{
			for (int cols = 0; cols < MAP_WIDTH_TILES; ++cols)
			{
				Vector2d pos = Vector2d(cols * TILE_WIDTH, rows * TILE_HEIGHT);
				Tile newTile;
				newTile.position = pos;
				tiles[GetTilemapIndex(cols, rows)] = newTile;
			}
		}
	}
	vector<Tile> tiles;
};

int GetGoldAmount(vector<Site>& sites)
{
	int amt{ 0 };
	for (auto s : sites)
	{
		if (s.structureType == GOLDMINE)
			amt += s.param1;
	}
	return amt;
}

int GetNumberOfUnits(vector<Unit>& units, int type)
{
	int amt{ 0 };
	for (Unit u : units)
	{
		if (u.unitType == type)
			++amt;
	}
	return amt;
}

Vector2d TileContainsEnemyUnits(Vector2d position, vector<Unit> enemies)
{
	Vector2d enemyVector{ 0,0 };

	for (auto e : enemies)
	{
		if (ScreenToTile(position) == ScreenToTile(e.position))
		{
			enemyVector == e.position;
			return enemyVector;
		}
	}
	return enemyVector;
}

Vector2d TileContainsObstacle(Vector2d position, vector<Site> sites)
{
	Vector2d obstacleVector{ 0,0 };

	for (int i = 0; i < sites.size(); ++i)
	{
		if (ScreenToTile(position) == ScreenToTile(sites[i].position))
		{
			obstacleVector == sites[i].position;
			return obstacleVector;
		}
	}
	return obstacleVector;
}

Site GetNearestSite(Vector2d currPos, map<int, Site>& sites, int owner)
{
	map<int, Site> nearestSite;
	for (int i = 0; i < sites.size(); ++i)
	{
		if (owner == ANY)
			nearestSite[GetDistance(currPos, sites[i].position)] = sites[i];
		else if (owner == ME_OR_FREE)
		{
			if (sites[i].owner == ME or sites[i].owner == FREE)
				nearestSite[GetDistance(currPos, sites[i].position)] = sites[i];
		}
		else
		{
			if (sites[i].owner == owner)
				nearestSite[GetDistance(currPos, sites[i].position)] = sites[i];
		}
	}
	return nearestSite.begin()->second;
}

Site GetNearestFreeSite(Vector2d currPos, map<int, Site> freeSites, vector<Site> enemySites)
{
	map<int, Site> furthestFromEnemySite;
	map<int, Site> nearestFreeSite;
	for (map<int, Site>::iterator i = freeSites.begin(); i != freeSites.end(); ++i)
	{
		//furthestFromEnemySite[GetDistance(freeSites)]
	}
}

int main()
{
	//##########################################################
	srand(time(0));
	TileMap theMap;
	map<int, Site> theSites;
	MAP_SIDE myMapSide = MAP_SIDE::LEFT;
	//##########################################################  

	int numSites;
	cin >> numSites; cin.ignore();
	for (int i = 0; i < numSites; i++) {
		int siteId;
		int x;
		int y;
		int radius;
		cin >> siteId >> x >> y >> radius; cin.ignore();

		//##########################################################
		Vector2d location = Vector2d(x, y);
		Site newSite = Site(siteId, location, radius);
		theSites[siteId] = newSite;
		Vector2d tilePos = ScreenToTile(location);
		theMap.tiles[GetTilemapIndex(tilePos.x, tilePos.y)].hasSite = true;
		//##########################################################
	}

	while (1) {
		//##########################################################
		map<int, Site> nearestSite{};
		vector<Unit> myUnits{};
		vector<Unit> enemyUnits{};
		int numBarracks = 0;
		int numTowers = 0;
		int numGoldmines = 0;

		int numEnemyGoldmines{ 0 };
		int numEnemyBarracks{ 0 };
		int numEnemyTowers{ 0 };
		int numEnemyUnits{ 0 };

		vector<Site> mySites{};
		vector<Site> enemySites{};
		vector<Site> freeSites{};

		int numFreeSites{ 0 };
		int numEnemySites{ 0 };
		int numMySites{ 0 };

		Unit myQueen, enemyQueen;

		//##########################################################       
		int gold;
		int touchedSite; // -1 if none        
		cin >> gold >> touchedSite; cin.ignore();
		for (int i = 0; i < numSites; i++) {
			int siteId;
			int gold; // used in future leagues
			int maxMineSize; // used in future leagues
			int structureType; // -1 = No structure, 2 = Barracks
			int owner; // -1 = No structure, 0 = Friendly, 1 = Enemy
			int param1;
			int param2;
			cin >> siteId >> gold >> maxMineSize >> structureType >> owner >> param1 >> param2; cin.ignore();
			//##########################################################           
			theSites[siteId].gold = gold;
			theSites[siteId].maxMineSize = maxMineSize;
			theSites[siteId].structureType = structureType;
			theSites[siteId].owner = owner;
			theSites[siteId].param1 = param1;
			theSites[siteId].param2 = param2;
			if (owner == ME)
			{
				switch (structureType)
				{
				case GOLDMINE:
					++numGoldmines;
					break;
				case BARRACKS:
					++numBarracks;
				case TOWER:
					++numTowers;
				}
				mySites.push_back(theSites[siteId]);
				++numMySites;
			}
			else if (owner == ENEMY)
			{
				switch (structureType)
				{
				case GOLDMINE:
					++numEnemyGoldmines;
					break;
				case BARRACKS:
					++numEnemyBarracks;
				case TOWER:
					++numEnemyTowers;
				}
				enemySites.push_back(theSites[siteId]);
				++numEnemySites;
			}
			else
			{
				freeSites.push_back(theSites[siteId]);
				++numFreeSites;
			}
			//##########################################################
		}
		int numUnits;
		cin >> numUnits; cin.ignore();
		for (int i = 0; i < numUnits; i++) {
			int x;
			int y;
			int owner;
			int unitType; // -1 = QUEEN, 0 = KNIGHT, 1 = ARCHER
			int health;
			cin >> x >> y >> owner >> unitType >> health; cin.ignore();
			//##########################################################
			Vector2d location = Vector2d(x, y);
			Unit newUnit = Unit(owner, unitType, health, location);
			// I am the owner
			if (owner == ME)
			{
				myUnits.push_back(newUnit);
				if (unitType == QUEEN)
					myQueen = newUnit;
			}
			else if (owner == ENEMY) // or not         
			{
				enemyUnits.push_back(newUnit);
				++numEnemyUnits;

				if (unitType == QUEEN)
					enemyQueen = newUnit;
			}
			//##########################################################
		}

		// Determine which side of map we are on
		if (myQueen.position.x < enemyQueen.position.x)
		{
			myMapSide == MAP_SIDE::LEFT;
		}
		else
		{
			myMapSide == MAP_SIDE::RIGHT;
		}



		//##########################################################     
		myQueen.position = myQueen.position + TileContainsEnemyUnits(myQueen.position, enemyUnits); // +TileContainsObstacle(myQueen.position, enemySites);




		if (GetDistance(myQueen.position, enemyQueen.position) < 200)
		{
			Vector2d newPosition = { myQueen.position.x / 2, myQueen.position.y / 2 };
			Site moveToSite = GetNearestSite(newPosition, theSites, FREE);
			cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
		}
		else if (numBarracks < 1)
		{
			if (touchedSite == FREE) // not at a site? Go there!
			{
				Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
				cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
			}
			else
			{
				if (theSites[touchedSite].owner == FREE)
				{
					cout << "BUILD " << touchedSite << " BARRACKS-KNIGHT" << endl;
					++numBarracks;
				}
				else
				{
					Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
					cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
				}
			}
		}
		else
		{
			if (numGoldmines < 1)
			{
				if (touchedSite == FREE) // not at a site? Go there!
				{
					Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
					cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
				}
				else
				{
					if (theSites[touchedSite].owner == FREE and theSites[touchedSite].gold > 0)
					{
						cout << "BUILD " << touchedSite << " MINE" << endl;
						++numGoldmines;
					}
					else
					{
						Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
						cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
					}
				}
			}
			else
			{
				if (touchedSite == FREE)
				{
					Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
					cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
				}
				else
				{
					if (theSites[touchedSite].owner == ME)
					{
						if (theSites[touchedSite].structureType == GOLDMINE)
						{
							if (theSites[touchedSite].param1 < theSites[touchedSite].maxMineSize and theSites[touchedSite].gold > 0)
							{
								cout << "BUILD " << touchedSite << " MINE" << endl;
							}
							else
							{
								Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
								cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
							}
						}
						else if (theSites[touchedSite].structureType == TOWER && theSites[touchedSite].param1 < 750)
						{
							cout << "BUILD " << touchedSite << " TOWER" << endl;

							/* -- CHANGE BACK ON MONDAY IF NOT WORKING, SUNDAY STUFF HERE
							if (theSites[touchedSite].param1 < 10)
							{
							cout << "BUILD " << touchedSite << " TOWER" << endl;
							}
							else
							{
							Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
							cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
							}*/

						}
						else
						{
							Site moveToSite = GetNearestSite(myQueen.position, theSites, FREE);
							cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
						}
					}
					else if (theSites[touchedSite].owner == FREE)
					{
						/*int rando = rand() % 2;
						if (rando == 0)
						{*/
						cout << "BUILD " << touchedSite << " TOWER" << endl;
						++numTowers;
						/*}
						else if (rando != 0 and numBarracksGiant < 2)
						{
						cout << "BUILD " << touchedSite << " BARRACKS-GIANT" << endl;
						++numBarracksGiant;
						}*/

					}
					else
					{
						Site moveToSite = GetNearestSite(myQueen.position, theSites, ME);
						cout << "MOVE " << moveToSite.position.x << " " << moveToSite.position.y << endl;
					}
				}

			}
		}
		// COMMENCE TRAINING
		cout << "TRAIN";
		vector<int> trainingSites{};

		for (int i = 0; i < numSites; ++i)
		{
			if (theSites[i].owner == ME and theSites[i].structureType == BARRACKS)
			{
				trainingSites.push_back(theSites[i].siteId);
			}
		}


		for (auto s : trainingSites)
			cout << " " << s;

		cout << endl;
	}
}