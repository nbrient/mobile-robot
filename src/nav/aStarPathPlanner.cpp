/**
 * @file aStarPathPlanner.cpp
 *
 * @brief A star path planner implementation.
 *
 * @details  This file implements an A* planner on an occupancy grid. The planner supports 8-connected movement:
 * horizontal, vertical and diagonal moves.
 *
 * @version 1.0
 */

/* System includes */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

/* Project includes */
#include "nav/aStarPathPlanner.hpp"

/* Declarations */
namespace {

/**
 * @brief Cost to move from one grid cell to a horizontal or vertical neighbor.
 */
static constexpr float ASTAR_STRAIGHT_MOVE_COST = 1.0f;

/**
 * @brief Cost to move from one grid cell to a diagonal neighbor sqrt(2).
 */
static constexpr float ASTAR_DIAGONAL_MOVE_COST = 1.41421356237f;

/**
 * @brief Estimate the cost between two cells using octile distance.
 *
 * @details This heuristic is used for an 8-connected grid.
 *
 * @param current Current cell.
 * @param target Target cell.
 *
 * @return Estimated cost from current to target.
 */
float estimateCost(const GridCell& current, const GridCell& target) {
    const int dx = abs(current.x - target.x);
    const int dy = abs(current.y - target.y);

    const int diagonalMoveCount = min(dx, dy);
    const int straightMoveCount = max(dx, dy) - diagonalMoveCount;

    return (static_cast<float>(diagonalMoveCount) * ASTAR_DIAGONAL_MOVE_COST) +
           (static_cast<float>(straightMoveCount) * ASTAR_STRAIGHT_MOVE_COST);
}

/**
 * @brief Node stored in the A* open list.
 */
struct AStarNode {
    /**
     * @brief Grid cell represented by this node.
     */
    GridCell m_gridCell;

    /**
     * @brief Estimated total cost from start to target through this node.
     *
     * @details fCost = gCost + hCost.
     */
    float m_fCost;

    /**
     * @brief Estimated cost from this node to target.
     *
     * @details hCost is only an estimation. It is used to guide the search.
     */
    float m_hCost;
};

}  // namespace

/* Implementation */
AStarPathPlanner::AStarPathPlanner(float cellSize) : PathPlanner(cellSize) {
}
vector<Vector2Dim> AStarPathPlanner::computePath(const Map& map, float robotRadius, const Vector2Dim& start,
                                                 const Vector2Dim& target) const {
    /* Calculate grid size. */
    const int gridWidth = static_cast<int>(map.getWidth() / m_cellSize);
    const int gridHeight = static_cast<int>(map.getHeight() / m_cellSize);

    printf("[A_STAR] start=(%.1f, %.1f) target=(%.1f, %.1f)\n", start.x, start.y, target.x, target.y);

    /* Convert start point and target point to grid cell. */
    const GridCell startCell = worldToGrid(start);
    const GridCell targetCell = worldToGrid(target);

    /* Check start and target are on grid. */
    if ((startCell.x < 0) || (startCell.x >= gridWidth) || (startCell.y < 0) || (startCell.y >= gridHeight)) {
        printf("[A_STAR] ERROR start outside grid\n");
        return {};
    }

    if ((targetCell.x < 0) || (targetCell.x >= gridWidth) || (targetCell.y < 0) || (targetCell.y >= gridHeight)) {
        printf("[A_STAR] ERROR target outside grid\n");
        return {};
    }

    /* Check that start and target are not blocked. */
    const bool isStartBlocked = isCellBlocked(map, robotRadius, startCell.x, startCell.y);
    const bool isTargetBlocked = isCellBlocked(map, robotRadius, targetCell.x, targetCell.y);

    if (isStartBlocked || isTargetBlocked) {
        printf("[A_STAR] ERROR blocked start=%d target=%d startCell=(%d,%d) targetCell=(%d,%d)\n",
               isStartBlocked ? 1 : 0, isTargetBlocked ? 1 : 0, startCell.x, startCell.y, targetCell.x, targetCell.y);
        return {};
    }

    /* Convert a 2D grid into a unique linear index. */
    auto indexCellInLinear = [gridWidth](int x, int y) -> int { return (y * gridWidth) + x; };

    const size_t cellCount = static_cast<size_t>(gridWidth) * static_cast<size_t>(gridHeight);
    const float invalidCost = numeric_limits<float>::infinity();

    vector<float> gCost(cellCount, invalidCost); /* Real cost from the start cell to each grid cell. */
    vector<int> parent(cellCount,
                       -1); /* Previous cell index used to reach each cell. Used later to rebuild the final path. */
    vector<bool> closed(cellCount, false); /* True when a cell has already been processed. */

    /*
     * Priority queue comparator.
     *
     * priority_queue returns the "largest" element first.
     * This comparator gives lower priority to the node with the highest fCost.
     *
     * Result:
     *   - the node with the lowest fCost is processed first
     *   - if fCost is equal, the node with the lowest hCost is processed first
     */
    const auto compareAStarNodes = [](const AStarNode& left, const AStarNode& right) -> bool {
        if (left.m_fCost == right.m_fCost) {
            return left.m_hCost > right.m_hCost;
        }

        return left.m_fCost > right.m_fCost;
    };

    priority_queue<AStarNode,                   /* Data type of the queue */
                   vector<AStarNode>,           /* Internal container type */
                   decltype(compareAStarNodes)> /* Comparator type */
        openList(compareAStarNodes);            /* Name assigned to the priority queue */

    /*
     * Initialize the start cell.
     *
     * gCost = 0 because the cost from start to start is zero.
     * hCost = estimated cost from start to target.
     * fCost = gCost + hCost.
     */
    const int startIndex = indexCellInLinear(startCell.x, startCell.y);
    const float startGCost = 0.0f;
    const float startHCost = estimateCost(startCell, targetCell);
    const float startFCost = startGCost + startHCost;

    gCost[static_cast<size_t>(startIndex)] = startGCost;
    openList.push({startCell, startFCost, startHCost});

    /*
     * 8-connected neighborhood offsets.
     *
     * Direction order:
     *   0: right
     *   1: left
     *   2: down
     *   3: up
     *   4: down-right
     *   5: up-right
     *   6: down-left
     *   7: up-left
     */
    static constexpr size_t DIRECTION_COUNT = 8u;
    const int dx[DIRECTION_COUNT] = {1, -1, 0, 0, 1, 1, -1, -1};
    const int dy[DIRECTION_COUNT] = {0, 0, 1, -1, 1, -1, 1, -1};
    bool found = false;

    /* Explore cells while candidates are available. */
    while (!openList.empty()) {
        /*
         * Get the best candidate cell.
         *
         * The best candidate is the cell with the lowest fCost.
         */
        const AStarNode currentNode =
            openList.top();  // TODO : ici je ne comprends pas le lien avec la map et le current node
        openList.pop();

        const GridCell current = currentNode.m_gridCell;
        const int currentIndex = indexCellInLinear(current.x, current.y);

        /*
         * Ignore already closed cells.
         */
        if (closed[static_cast<size_t>(currentIndex)]) {
            continue;
        }

        /* Mark the current cell as processed. */
        closed[static_cast<size_t>(currentIndex)] = true;

        /* Stop when target is reached. */
        if (areCellsEqual(current, targetCell)) {
            found = true;
            break;
        }

        /* Explore neighboring cells to find target. */
        for (size_t direction = 0; direction < DIRECTION_COUNT; direction++) {
            const int nextX = current.x + dx[direction];
            const int nextY = current.y + dy[direction];

            /* Ignore cells out of grid range. */
            if ((nextX < 0) || (nextX >= gridWidth) || (nextY < 0) || (nextY >= gridHeight)) {
                continue;
            }

            const int nextIndex = indexCellInLinear(nextX, nextY);

            /* Ignore already closed cells. */
            if (closed[static_cast<size_t>(nextIndex)]) {
                continue;
            }

            /* Ignore blocked cells. */
            if (isCellBlocked(map, robotRadius, nextX, nextY)) {
                continue;
            }

            /* Check if the movement is diagonal. */
            const bool isDiagonalMove = (dx[direction] != 0) && (dy[direction] != 0);

            /* Avoid cutting corners. Both adjacent straight cells must be free. */
            if (isDiagonalMove) {
                const int horizontalX = current.x + dx[direction];
                const int horizontalY = current.y;

                const int verticalX = current.x;
                const int verticalY = current.y + dy[direction];

                if (isCellBlocked(map, robotRadius, horizontalX, horizontalY) ||
                    isCellBlocked(map, robotRadius, verticalX, verticalY)) {
                    continue;
                }
            }

            /* Compute the cost to move from current cell to this neighbor. */
            const float moveCost = isDiagonalMove ? ASTAR_DIAGONAL_MOVE_COST : ASTAR_STRAIGHT_MOVE_COST;

            /* Calculate the cost to go from the starting point to the next cell. */
            const float tentativeGCost = gCost[static_cast<size_t>(currentIndex)] + moveCost;

            /* Keep this path only if it improves the previous known cost. */
            if (tentativeGCost >= gCost[static_cast<size_t>(nextIndex)]) {
                continue;
            }

            const GridCell nextCell = {nextX, nextY};

            const float hCost = estimateCost(nextCell, targetCell); /* Estimated cost from next cell to target cell. */
            const float fCost = tentativeGCost + hCost; /* Estimated total cost if the path goes through next cell. */

            /*
             * Save the best known path to this neighbor.
             *
             * parent[nextIndex] = currentIndex means:
             *   to reach nextCell, the best path currently comes from current.
             */
            gCost[static_cast<size_t>(nextIndex)] = tentativeGCost;
            parent[static_cast<size_t>(nextIndex)] = currentIndex;

            /* Add the neighbor to the open list for future exploration. */
            openList.push({nextCell, fCost, hCost});
        }
    }

    /* No path found. */
    if (!found) {
        printf("[A_STAR] No path found\n");
        return {};
    }

    /* Reconstruct path backwards from target to start. */
    vector<GridCell> reversedCells;
    GridCell current = targetCell;

    while (!areCellsEqual(current, startCell)) {
        reversedCells.push_back(current);

        const int currentIndex = indexCellInLinear(current.x, current.y);
        const int parentIndex = parent[static_cast<size_t>(currentIndex)];

        current.x = parentIndex % gridWidth;
        current.y = parentIndex / gridWidth;
    }

    reversedCells.push_back(startCell);
    /* Reverse the path to get the correct order from start to target. */
    reverse(reversedCells.begin(), reversedCells.end());

    /* Convert grid cells back to world-space waypoints. */
    vector<Vector2Dim> path;
    path.reserve(reversedCells.size() + 1U);

    for (GridCell& cell : reversedCells) {
        path.push_back(gridToWorld(cell));
    }

    /* Add target as final waypoint. */
    path.push_back(target);

    return path;
}
