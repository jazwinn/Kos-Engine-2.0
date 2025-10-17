//#pragma once
//
//#ifndef PATHFINDING_H
//#define PATHFINDING_H
//
//#include "ECS/ECS.h"
//#include "Config/pch.h"
//#include "Grid.h"
//
//std::list<Node*> EvaluatePath(Node* startNode, Node* endNode);
//
//bool IsNodeInContainer(Node* nodeToCheck, std::vector<Node*> const& container);
//
//Node* FindObjectNode(GameObject* go);
//
//struct PathfinderPlayAreas {
//	static std::shared_ptr<std::map<std::string, Grid>> pathfinderPlayAreas;
//	static std::shared_ptr <std::string> currentPlayAreaName;
//};
//
//void CreatePlayArea(std::string playAreaName, glm::vec3 midPoint, size_t row, size_t col, float nodeSizeWidth, float nodeSizeLength);
//
//void DebugDrawPlayArea(std::string playAreaName);
//
//#endif