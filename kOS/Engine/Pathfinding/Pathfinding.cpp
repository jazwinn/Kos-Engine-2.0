//#include "Pathfinding.h"
//
//std::shared_ptr < std::map<std::string, Grid>> PathfinderPlayAreas::pathfinderPlayAreas{ std::make_shared < std::map<std::string, Grid>>(std::map<std::string, Grid>{}) };
//std::shared_ptr < std::string> PathfinderPlayAreas::currentPlayAreaName{ std::make_shared<std::string>(std::string{}) };
//
//// Main function to evaluate the path between start node and end node
//std::list<Node*> EvaluatePath(Node* startNode, Node* endNode) {
//	// Create a container to store open and closed nodes, adding
//	// the starting node into the open nodes
//	std::vector<Node*> openNodes{ startNode }, closedNodes;
//	std::list<Node*> evaluatedPath;
//
//	if (!startNode || !endNode) {
//		return evaluatedPath;
//	}
//
//	// Run a loop while the number of open nodes is more than 0
//	while (openNodes.size() > 0) {
//		// Set current node as the first one inside open nodes
//		Node* currentNode{ openNodes[0] };
//
//		openNodes[0]->SetGCost(startNode->GetNodeWorldPos().Distance(currentNode->GetNodeWorldPos()));
//		openNodes[0]->SetHCost(currentNode->GetNodeWorldPos().Distance(endNode->GetNodeWorldPos()));
//
//		// Current node will be the node in open nodes with the
//		// lowest f-cost
//		std::vector<Node*>::iterator lowestFCostIt = openNodes.begin();
//
//		for (size_t i = 1; i < openNodes.size(); ++i) {
//			if (openNodes[i]->GetFCost() < currentNode->GetFCost() ||
//				(openNodes[i]->GetFCost() == currentNode->GetFCost() && openNodes[i]->GetHCost() < currentNode->GetHCost())) {
//				currentNode = openNodes[i];
//				lowestFCostIt = openNodes.begin() + i;
//			}
//		}
//
//		// Remove current node from open nodes
//		openNodes.erase(lowestFCostIt);
//
//		// Add current node to closed nodes
//		closedNodes.push_back(currentNode);
//
//		// Check if current node is the target node
//		if (currentNode == endNode) {
//			// If it is, path has been found
//			Node* nodeToEvaluate = endNode;
//
//			// Take each node's parent as the shortest path
//			while (nodeToEvaluate != startNode) {
//				evaluatedPath.push_front(nodeToEvaluate);
//				nodeToEvaluate = nodeToEvaluate->GetParent();
//			}
//
//			//Clear neighbours
//			currentNode->GetNeighbours().clear();
//
//			return evaluatedPath;
//		}
//
//		// Check each neighbours of the current node
//		Node* neighbourNode;
//
//		if (currentNode->GetNodeGridPos().first - 1 >= 0) {
//			neighbourNode = &(*PathfinderPlayAreas::pathfinderPlayAreas)[*PathfinderPlayAreas::currentPlayAreaName][currentNode->GetNodeGridPos().second][currentNode->GetNodeGridPos().first - 1];
//			currentNode->AddNeighbour(neighbourNode);
//		}
//
//		if (currentNode->GetNodeGridPos().first + 1 < (*PathfinderPlayAreas::pathfinderPlayAreas)[*PathfinderPlayAreas::currentPlayAreaName].GetColSize()) {
//			neighbourNode = &(*PathfinderPlayAreas::pathfinderPlayAreas)[*PathfinderPlayAreas::currentPlayAreaName][currentNode->GetNodeGridPos().second][currentNode->GetNodeGridPos().first + 1];
//			currentNode->AddNeighbour(neighbourNode);
//		}
//
//		if (currentNode->GetNodeGridPos().second - 1 >= 0) {
//			neighbourNode = &(*PathfinderPlayAreas::pathfinderPlayAreas)[*PathfinderPlayAreas::currentPlayAreaName][currentNode->GetNodeGridPos().second - 1][currentNode->GetNodeGridPos().first];
//			currentNode->AddNeighbour(neighbourNode);
//		}
//
//		if (currentNode->GetNodeGridPos().second + 1 < (*PathfinderPlayAreas::pathfinderPlayAreas)[*PathfinderPlayAreas::currentPlayAreaName].GetRowSize()) {
//			neighbourNode = &(*PathfinderPlayAreas::pathfinderPlayAreas)[*PathfinderPlayAreas::currentPlayAreaName][currentNode->GetNodeGridPos().second + 1][currentNode->GetNodeGridPos().first];
//			currentNode->AddNeighbour(neighbourNode);
//		}
//
//		for (Node* node : currentNode->GetNeighbours()) {
//			if (!node->GetTraversable() || IsNodeInContainer(node, closedNodes)) {
//				continue;
//			}
//
//			node->SetGCost(startNode->GetNodeWorldPos().Distance(node->GetNodeWorldPos()));
//			node->SetHCost(node->GetNodeWorldPos().Distance(endNode->GetNodeWorldPos()));
//
//			float newMovementCostToNeighbour = currentNode->GetGCost() + currentNode->GetNodeWorldPos().Distance(node->GetNodeWorldPos());
//
//			if (newMovementCostToNeighbour < node->GetGCost() || !IsNodeInContainer(node, openNodes)) {
//				node->SetGCost(newMovementCostToNeighbour);
//				node->SetHCost(node->GetNodeWorldPos().Distance(endNode->GetNodeWorldPos()));
//				node->SetParent(currentNode);
//
//				if (!IsNodeInContainer(node, openNodes)) {
//					openNodes.push_back(node);
//				}
//			}
//		}
//	}
//
//	return evaluatedPath;
//}
//
//bool IsNodeInContainer(Node* nodeToCheck, std::vector<Node*> const& container) {
//	for (Node* node : container) {
//		if (node == nodeToCheck) {
//			return true;
//		}
//	}
//
//	return false;
//}
//
//Node* FindObjectNode(GameObject* go) {
//	Grid* currentGrid = &(*PathfinderPlayAreas::pathfinderPlayAreas)[*PathfinderPlayAreas::currentPlayAreaName];
//	Vector2D displacedPos = (go->FindComponent<Transform>()->RetrieveWorldPosition() + go->FindComponent<Transform>()->RetrievePosition()) - currentGrid->GetMidPoint();
//	float newDisplacedPosX = displacedPos.x + ((currentGrid->GetRowSize() * currentGrid->GetNodeSizeWidth()) / 2);
//	float newDisplacedPosY = ((currentGrid->GetColSize() * currentGrid->GetNodeSizeHeight()) / 2) - displacedPos.y;
//	int nodeFirst = static_cast<int>(newDisplacedPosX / currentGrid->GetNodeSizeWidth());
//	int nodeSecond = static_cast<int>(newDisplacedPosY / currentGrid->GetNodeSizeHeight());
//
//	if (nodeFirst < 0 || nodeFirst >= currentGrid->GetRowSize() || nodeSecond < 0 || nodeSecond >= currentGrid->GetColSize()) {
//		return NULL;
//	}
//
//	Node* node = &(*currentGrid)[nodeSecond][nodeFirst];
//
//	return node;
//}
//
//void CreatePlayArea(std::string playAreaName, Vector2D midPoint, size_t row, size_t col, float nodeSizeWidth, float nodeSizeHeight) {
//	if ((*PathfinderPlayAreas::pathfinderPlayAreas).find(playAreaName) == (*PathfinderPlayAreas::pathfinderPlayAreas).end())
//	{
//		Grid grid{ row, col, nodeSizeWidth, nodeSizeHeight, midPoint };
//
//		float rowOffset = (grid.GetColSize() * nodeSizeWidth) / 2 - nodeSizeWidth / 2;
//		float colOffset = (grid.GetRowSize() * nodeSizeHeight) / 2 - nodeSizeHeight / 2;
//
//		for (int i = 0; i < grid.GetRowSize(); ++i) {
//			for (int j = 0; j < grid.GetColSize(); ++j) {
//				grid[i][j].SetNodeGridPos({ j, i });
//				grid[i][j].SetNodeWorldPos({ (j * nodeSizeWidth) - rowOffset + midPoint.x, colOffset - (i * nodeSizeHeight) + midPoint.y });
//
//				grid[i][j].SetObject(0);
//				grid[i][j].SetTraversable(true);
//			}
//		}
//
//		std::map<std::string, GameObject>::iterator it;
//		for (it = ObjectList::goList->begin(); it != ObjectList::goList->end(); ++it) {
//
//			if (*(it->second.GetTag()) == "Solid" && it->second.FindComponent<BoxCollider2D>()) {
//
//				Vector2D topRightBound = it->second.FindComponent<BoxCollider2D>()->RetrieveCollider()->GetAABB().max;
//				Vector2D bottomLeftBound = it->second.FindComponent<BoxCollider2D>()->RetrieveCollider()->GetAABB().min;
//				//float colliderWidth = it->second.FindComponent<BoxCollider2D>()->RetrieveCollider()->GetAABB().Width();
//				//float colliderHeight = it->second.FindComponent<BoxCollider2D>()->RetrieveCollider()->GetAABB().Height();
//
//				float bottomLeftBoundDisplacedPosX = bottomLeftBound.x - midPoint.x + ((row * nodeSizeWidth) / 2);
//				float bottomLeftBoundDisplacedPosY = ((col * nodeSizeHeight) / 2) - (bottomLeftBound.y - midPoint.y);
//				float topRightBoundDisplacedPosX = topRightBound.x - midPoint.x + ((row * nodeSizeWidth) / 2);
//				float topRightBoundDisplacedPosY = ((col * nodeSizeHeight) / 2) - (topRightBound.y - midPoint.y);
//				int bottomLeftBoundNodePosX = static_cast<int>(bottomLeftBoundDisplacedPosX / nodeSizeWidth);
//				int bottomLeftBoundNodePosY = static_cast<int>(bottomLeftBoundDisplacedPosY / nodeSizeHeight);
//				int topRightBoundNodePosX = static_cast<int>(topRightBoundDisplacedPosX / nodeSizeWidth);
//				int topRightBoundNodePosY = static_cast<int>(topRightBoundDisplacedPosY / nodeSizeHeight);
//
//				for (int i = bottomLeftBoundNodePosX; i <= topRightBoundNodePosX; ++i) {
//					if (i < 0 || i >= row) {
//						continue;
//					}
//
//					for (int j = topRightBoundNodePosY; j <= bottomLeftBoundNodePosY; ++j) {
//						//std::cout << "NODE: " << i << "," << j << std::endl;
//						if (j < 0 || j >= col) {
//							continue;
//						}
//
//						grid[j][i].SetTraversable(false);
//					}
//				}
//			}
//		}
//
//		(*PathfinderPlayAreas::pathfinderPlayAreas).insert({ playAreaName, std::move(grid) });
//	}
//	else {
//		std::cout << "Stage of same name already exists!!!\n";
//	}
//}
//
//void DebugDrawPlayArea(std::string stage) {
//	if ((*PathfinderPlayAreas::pathfinderPlayAreas).find(stage) != (*PathfinderPlayAreas::pathfinderPlayAreas).end()) {
//		Grid grid{ (*PathfinderPlayAreas::pathfinderPlayAreas)[stage] };
//
//		for (int i = 0; i < grid.GetRowSize(); ++i) {
//			for (int j = 0; j < grid.GetColSize(); ++j) {
//				DebugColor color{ 255.f, 255.f, 255.f, 0.25f };
//
//				if (grid[i][j].GetTraversable()) {
//					color = { 255.f, 255.f, 255.f, 0.25f };
//				}
//				else if (!grid[i][j].GetTraversable()) {
//					color = { 255.f, 0.f, 0.f, 0.25f };
//				}
//
//				DebugDrawSquare(grid[i][j].GetNodeWorldPos().x, grid[i][j].GetNodeWorldPos().y, 100, 100, color);
//			}
//		}
//	}
//}