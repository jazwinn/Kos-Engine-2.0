/******************************************************************/
/*!
\file      Graph.cpp
\author    Yeo See Kiat Raymond, seekiatraymond.yeo, 2301268
\par       seekiatraymond.yeo@digipen.edu
\date      October 3, 2025
\brief     This file contains the list of nodes and edges to access
			and determine the most optimized path through AStar.


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Graph.h"
#include "OctreeGrid.h"

#include <queue>

namespace Octrees {
	int Node::nextNodeId = 0;

	Node::Node() {
		id = -1;
	}

	Node::Node(OctreeNode _octreeNode) {
		id = nextNodeId++;
		octreeNode = _octreeNode;
		fromID = -1;
	}

	bool Node::operator==(Node const& other) const {
		return id == other.id;
	}

	bool Node::operator==(Node* other) const {
		return id == other->id;
	}

	bool Edge::operator==(Edge const& other) const {
		return ((a == other.a && b == other.b) || (b == other.a && a == other.b));
	}

	int Graph::GetPathLength() {
		return static_cast<int>(pathList.size());
	}

	OctreeNode Graph::GetPathNode(int index) {
		if (pathList.empty()) {
			return OctreeNode();
		}

		if (index < 0 || index >= pathList.size()) {
			return OctreeNode();
		}

		return pathList[index].octreeNode;
	}

	const int maxIterations = 1000;

	bool Graph::AStar(OctreeNode* startNode, OctreeNode* endNode) {
		pathList.clear();

		Node* start = FindNode(startNode);
		Node* end = FindNode(endNode);

		if (!start || !end) {
			std::cout << "NO ID\n";
			return false;
		}

		//std::cout << "START NODE: " << start->octreeNode.bounds.center.x << ", " << start->octreeNode.bounds.center.y << ", " << start->octreeNode.bounds.center.z << std::endl;

		//std::multiset<Node*, CompareNode> openSet;
		//std::vector<Node*> closedSet;
		std::multiset<Node, CompareNode> openSet;
		//std::vector<Node> openSet;
		std::vector<Node> closedSet;

		int iterationCount = 0;

		//std::cout << "START NODE: " << start->octreeNode.bounds.center.x << ", " << start->octreeNode.bounds.center.y << ", " << start->octreeNode.bounds.center.z << std::endl;
		//std::cout << "END NODE: " << end->octreeNode.bounds.center.x << ", " << end->octreeNode.bounds.center.y << ", " << end->octreeNode.bounds.center.z << std::endl;
		start->g = 0;
		start->h = Heuristic(*start, *end);
		start->f = start->g + start->h;
		//start->from = nullptr;
		start->fromID = -1;
		openSet.insert(*start);
		//openSet.push_back(*start);

		Node *neighbour, current;

		while (openSet.size()) {
			//std::cout << "OPEN SET SIZE: " << openSet.size() << std::endl;
			//std::cout << "ITERATION COUNT: " << iterationCount << std::endl;

			if (++iterationCount > maxIterations) {
				std::cout << "NO PATH FOUND1\n";
				return false;
			}

			Node smallestFCostNode = *openSet.begin();
			for (Node nod : openSet) {
				if (nod.f < smallestFCostNode.f) {
					smallestFCostNode = nod;
				}
			}

			current = (*openSet.begin());
			openSet.erase(openSet.begin());
			//current = smallestFCostNode;
			//auto smallestFCostNodeIt = std::find(openSet.begin(), openSet.end(), smallestFCostNode);
			//if (smallestFCostNodeIt != openSet.end()) {
			//	openSet.erase(smallestFCostNodeIt);
			//}

			//std::cout << "CURRENT NODE: " << current->octreeNode.bounds.center.x << ", " << current->octreeNode.bounds.center.y << ", " << current->octreeNode.bounds.center.z << std::endl;
			//std::cout << "CURRENT NODE: " << current.edges.size() << std::endl;
			//std::cout << "END NODE: " << end->octreeNode.bounds.center.x << ", " << end->octreeNode.bounds.center.y << ", " << end->octreeNode.bounds.center.z << std::endl;
			//std::cout << "CURRENT NODE ID: " << current.id << "===" << "END NODE ID: " << end->id << std::endl;
			//std::cout << "CURRENT NODE POS: " << current.octreeNode.bounds.center.x << ", " << current.octreeNode.bounds.center.y << ", " << current.octreeNode.bounds.center.z << std::endl;
			if (current == end) {
				std::cout << "PATH IS FOUND\n";
				//ReconstructPath(current);
				return true;
			}

			closedSet.push_back(current);
			for (Edge edge : current.edges) {
				//std::cout << "CURRENT EDGES CHECK A: " << edge.a->octreeNode.bounds.center.x << ", " << edge.a->octreeNode.bounds.center.y << ", " << edge.a->octreeNode.bounds.center.z << std::endl;
				//std::cout << "CURRENT EDGES CHECK B: " << edge.b->octreeNode.bounds.center.x << ", " << edge.b->octreeNode.bounds.center.y << ", " << edge.b->octreeNode.bounds.center.z << std::endl;
				neighbour = ((edge.a) == current) ? (edge.b) : (edge.a);
				
				auto closedSetIt = std::find(closedSet.begin(), closedSet.end(), neighbour);
				if (closedSetIt != closedSet.end()) {
					continue;
				}

				//std::cout << "CRASH1\n";
				//std::cout << "EDGE CHOSEN: " << neighbour->octreeNode.bounds.center.x << ", " << neighbour->octreeNode.bounds.center.y << ", " << neighbour->octreeNode.bounds.center.z << std::endl;
				//std::cout << Heuristic(current, *neighbour) << std::endl;
				float tentative_gCost = current.g + Heuristic(current, *neighbour);
				//std::cout << "CURRENT G COST: " << current.g << " CURRENT HEURISTIC: " << Heuristic(current, *neighbour) << std::endl;
				auto openSetIt = std::find(openSet.begin(), openSet.end(), neighbour);
				if (tentative_gCost < neighbour->g || openSetIt == openSet.end()) {
					neighbour->g = tentative_gCost;
					neighbour->h = Heuristic(*neighbour, *end);
					neighbour->f = neighbour->g + neighbour->h;
					//neighbour->from = &current;
					neighbour->fromID = current.id;
					openSet.insert(*neighbour);
					//std::cout << "CRASH2\n";
					//Node nodeToInsert = *neighbour;
					//openSet.push_back(nodeToInsert);
					//std::cout << "INSERTING NEIGHBOUR ID: " << neighbour->id << std::endl;
					//std::cout << "NEIGHBOUR BOUNDS: " << neighbour->octreeNode.bounds.center.x << ", " << neighbour->octreeNode.bounds.center.y << ", " << neighbour->octreeNode.bounds.center.z << std::endl;
 				}
			}
		}

		std::cout << "NO PATH FOUND2\n";
		return false;
	}

	void Graph::ReconstructPath(Node* current) {
		while (current->id != -1) {
			pathList.push_back(*current);
			// UNCOMMENT THIS ONCE I GET TO PATH RECONSTRUCT
			//current = current->from;
		}

		std::reverse(pathList.begin(), pathList.end());
	}

	float Graph::Heuristic(Node a, Node b) {
		//std::cout << "CRASH1\n";
		float result = glm::length2(a.octreeNode.bounds.center - b.octreeNode.bounds.center);
		//std::cout << "CRASH2\n" << result << std::endl;
		return result;
	}

	void Graph::AddNode(OctreeNode _octreeNode) {
		for (Node node : nodes) {
			if (node.octreeNode.id == _octreeNode.id) {
				return;
			}
		}

		nodes.push_back(Node(_octreeNode));
	}

	void Graph::AddEdge(OctreeNode* a, OctreeNode* b) {
		Node* nodeA = FindNode(a);
		Node* nodeB = FindNode(b);

		if (!nodeA || !nodeB) {
			return;
		}

		Edge edge(nodeA, nodeB);

		auto it = std::find(edges.begin(), edges.end(), edge);
		if (it != edges.end()) {
			return;
		}

		//std::cout << "EDGE COUNT: " << Edge::testEdgeId++ << std::endl;
		//std::cout << "ADDED EDGE A: " << edge.a->octreeNode.bounds.center.x << ", " << edge.a->octreeNode.bounds.center.y << ", " << edge.a->octreeNode.bounds.center.z << std::endl;
		//std::cout << "ADDED EDGE B: " << edge.b->octreeNode.bounds.center.x << ", " << edge.b->octreeNode.bounds.center.y << ", " << edge.b->octreeNode.bounds.center.z << std::endl;
		edges.push_back(edge);
		nodeA->edges.push_back(edge);
		//auto start2 = std::chrono::high_resolution_clock::now();
		nodeB->edges.push_back(edge);
		//auto end2 = std::chrono::high_resolution_clock::now();
		//auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
		//std::cout << "Elapsed time FOR PUSHING EDGE: " << duration2.count() << " microseconds\n";
		//TIMEELAAPSED += duration2.count();
		//std::cout << "TOTAL TIME Elapsed time FOR PUSHING EDGE: " << TIMEELAAPSED << " microseconds\n";
	}

	Node* Graph::FindNode(OctreeNode* _octreeNode) {
		for (Node& node : nodes) {
			if (node.octreeNode.id == _octreeNode->id) {
				return &node;
			}
		}

		return nullptr;
	}

	void Graph::DrawGraph() {
		for (Node node : nodes) {
			std::shared_ptr<GraphicsManager> gm = GraphicsManager::GetInstance();
			glm::mat4 model{ 1.f };
			model = glm::translate(model, node.octreeNode.bounds.center) * glm::scale(model, { 0.3f, 0.3f, 0.3f });
			BasicDebugData basicDebug;
			basicDebug.worldTransform = model;
			gm->gm_PushCubeDebugData(BasicDebugData{ basicDebug });
		}
	}

}