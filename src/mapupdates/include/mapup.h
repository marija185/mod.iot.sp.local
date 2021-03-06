
#include <sys/time.h>

#include "ros/ros.h"

#include "nav_msgs/GetMap.h"

#include <visualization_msgs/Marker.h>
#include <mapupdates/NewObstacles.h>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iostream>
#include <string>
#include <tf/transform_listener.h>
#include <sensor_msgs/LaserScan.h>

struct I_point
{
	int x, y, th;
	bool operator==(I_point a)
	{
		if (x == a.x && y == a.y)
			return 1;
		else
			return 0;
	}

	int operator-(I_point a)
	{
		return std::max(abs(x - a.x), abs(y - a.y));
	}
};

struct I_edge
{
	int xs, ys, xg, yg;
};

class GMcell
{

public:
	int occupancy;
	double x, y; //coordinates of the cell centre
	bool staticcell;
	int visited;
	std::string uuid;
	std::string name;

	GMcell()
	{
		occupancy = 0;
		x = 0;
		y = 0;
		staticcell = false;
		visited = -1;
	};
};

class GridMapCell
{

private:
	int MapSizeX, MapSizeY;
	double size_cell;

	GMcell **map;
	void Free();

public:
	std::vector<I_point> spanningpath;
	std::vector<I_edge> edges;

	int GetMapSizeX() { return MapSizeX; };
	int GetMapSizeY() { return MapSizeY; };
	int GetSizeCell() { return size_cell; };
	GMcell **GetMap() { return map; };
	void spanningTree(int r, int c);
	void createEdges();

	GridMapCell(int size_x, int size_y, int cellsize)
	{
		if (size_x > 0 && size_y > 0)
		{
			size_cell = cellsize;
			MapSizeX = size_x;
			MapSizeY = size_y;
			map = (GMcell **)malloc(size_x * sizeof(GMcell *));
			for (int i = 0; i < size_x; i++)
				map[i] = new GMcell[size_y];
		}
		else
		{
			printf("GridMapCell> Invalid map size");
			exit(1);
		}
		spanningpath.reserve(100);
		spanningpath.clear();
		edges.reserve(100);
		edges.clear();
	};

	~GridMapCell()
	{
		for (int i = 0; i < MapSizeX; i++)
		{
			free(map[i]);
		}
		free(map);
		Free();
		printf("GridMapCell destroyed");
	};
};
