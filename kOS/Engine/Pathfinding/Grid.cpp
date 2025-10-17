///*****************************************************************
//FILENAME:   Grid.cpp
//AUTHOR(S):  Raymond (100%)
//BRIEF:      Grid contains the node class and also contains the
//			functions needed to create the grid outline
//
//All content � 2024 DigiPen Institute of Technology Singapore. All
//rights reserved.
//******************************************************************/
//
//#include "Grid.h"
//
//// Default constructor
//Node::Node() : gCost{ 0 }, hCost{ 0 }, traversable{ false }, parentNode{ nullptr }, worldPos{ 0.f, 0.f },
//gridPos{ 0, 0 }, currentObject{ 0 } {
//}
//
//// Copy constructor
//Node::Node(Node const& other) : gCost{ other.gCost }, hCost{ other.hCost }, traversable{ other.traversable },
//neighbours{ other.neighbours }, parentNode{ other.parentNode }, worldPos{ other.worldPos },
//gridPos{ other.gridPos }, currentObject{ other.currentObject } {
//}
//
//Node& Node::operator=(Node const& other) {
//	gCost = other.gCost;
//	hCost = other.hCost;
//	traversable = other.traversable;
//	neighbours = other.neighbours;
//	parentNode = other.parentNode;
//	worldPos = other.worldPos;
//	gridPos = other.gridPos;
//	currentObject = other.currentObject;
//
//	return *this;
//}
//
//// Accessors
//float Node::GetFCost() const {
//	return gCost + hCost;
//}
//
//float Node::GetGCost() const {
//	return gCost;
//}
//
//float Node::GetHCost() const {
//	return hCost;
//}
//
//void Node::SetGCost(float newCost) {
//	gCost = newCost;
//}
//
//void Node::SetHCost(float newCost) {
//	hCost = newCost;
//}
//
//bool Node::GetTraversable() const {
//	return traversable;
//}
//
//std::vector<Node*> Node::GetNeighbours() const {
//	return neighbours;
//}
//
//Node* Node::GetParent() const {
//	return parentNode;
//}
//
//std::pair<int, int> Node::GetNodeGridPos() const {
//	return gridPos;
//}
//
//glm::vec3 Node::GetNodeWorldPos() const {
//	return worldPos;
//}
//
//int Node::GetObject() const {
//	return currentObject;
//}
//
//void Node::SetTraversable(bool isTraversable) {
//	traversable = isTraversable;
//}
//
//void Node::AddNeighbour(Node* neighbour) {
//	neighbours.push_back(neighbour);
//}
//
//void Node::SetParent(Node* newParentNode) {
//	parentNode = newParentNode;
//}
//
//void Node::SetNodeGridPos(std::pair<int, int> pos) {
//	gridPos = pos;
//}
//
//void Node::SetNodeWorldPos(glm::vec3 pos) {
//	worldPos = pos;
//}
//
//void Node::SetObject(int object) {
//	currentObject = object;
//}