#ifndef __XLI_ASTAR_H__
#define __XLI_ASTAR_H__

#include <Xli/Array.h>
#include <Xli/HashMap.h>

namespace Xli
{
	template <typename TNode, typename TScore> class AStar
	{
		virtual void findNeighbours(const TNode& v, Array<TNode>& n) = 0;
		virtual TScore dist(const TNode& x, const TNode& y) = 0;
		virtual TScore heuristicDist(const TNode& x, const TNode& y) = 0;	

		static TNode findLowest(Array<TNode>& set, HashMap<TNode, TScore>& scores)
		{
			TNode lowest = set[0];
			TScore lowestScore = scores[lowest];
			for (int i = 1; i < set.Length(); i++)
			{
				if (scores[set[i]] < lowestScore)
				{
					lowest = set[i];
					lowestScore = scores[lowest];
				}
			}
			return lowest;
		}

		static void reconstructPath(const TNode& n, HashMap<TNode, TNode>& cameFrom, Array<TNode>& path)
		{
			TNode r;
			if (cameFrom.Get(n, r)) reconstructPath(r, cameFrom, path);
			path.Add(n);
		}

	public:
		bool FindShortestPath(const TNode& start, const TNode& goal, Array<TNode>& resultPath)
		{
			Array<TNode> closed, open;
			open.Add(start);
			HashMap<TNode, TScore> gScore, hScore, fScore;
			HashMap<TNode, TNode> cameFrom;
		
			gScore[start] = 0;
			hScore[start] = heuristicDist(start, goal);
			fScore[start] = hScore[start];
		
			while (open.Length())
			{
				TNode x = findLowest(open, fScore);

				if (x == goal)
				{
					reconstructPath(cameFrom[goal], cameFrom, resultPath);
					resultPath.Add(goal);
					return true;
				}

				open.Remove(x);
				closed.Add(x);
				Array<TNode> n;
				findNeighbours(x, n);

				for (int i = 0; i < n.Length(); i++)
				{
					TNode y = n[i];
					if (closed.Contains(y))
					{
						continue;
					}
				
					TScore g = gScore[x] + dist(x, y);
					bool better;

					if (!open.Contains(y))
					{
						open.Add(y);
						better = true;
					}
					else if (g < gScore[y])
					{
						better = true;
					}
					else
					{
						better = false;
					}
				
					if (better)
					{
						cameFrom[y] = x;
						gScore[y] = g;
						hScore[y] = heuristicDist(y, goal);
						fScore[y] = gScore[y] + hScore[y];
					}
				}
			}
		
			return false;
		}
	};
}

#endif
