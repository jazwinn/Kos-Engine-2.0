///*****************************************************************
//FILENAME:   Grid.h
//AUTHOR(S):  Raymond (100%)
//BRIEF:      Grid contains the node class and also contains the
//			functions needed to create the grid outline
//
//All content � 2024 DigiPen Institute of Technology Singapore. All
//rights reserved.
//******************************************************************/
//
//#pragma once
//
//#ifndef GRID_H
//#define GRID_H
//
//#include <map>
//#include <iostream>
//#include <string>
//#include "Config/pch.h"
//#include "ECS/ECS.h"
//
//#define NODESIZE 100.f
//#define NODEDRAWSIZE 45.f
//#define NODE 50.f
//
//class Node {
//private:
//	// F Cost -> Total cost of the node
//	// G Cost -> Distance between current node and start node
//	// H Cost -> Estimated distance from the current node and end node
//	float gCost, hCost;
//	bool traversable;
//	std::vector<Node*> neighbours;
//	Node* parentNode;
//	std::pair<int, int> gridPos;
//	glm::vec3 worldPos;
//	int currentObject;
//
//public:
//	//std::vector<Node*> neighbours;
//
//	// Default constructor
//	Node();
//
//	// Copy constructor
//	Node(Node const& other);
//
//	// Copy Assignment operator
//	Node& operator=(Node const& other);
//
//	// Accessors and Setters
//	float GetFCost() const;
//	float GetGCost() const;
//	float GetHCost() const;
//	void SetGCost(float newCost);
//	void SetHCost(float newCost);
//	bool GetTraversable() const;
//	std::vector<Node*> GetNeighbours() const;
//	Node* GetParent() const;
//	std::pair<int, int> GetNodeGridPos() const;
//	glm::vec3 GetNodeWorldPos() const;
//	int GetObject() const;
//	void SetTraversable(bool isTraversable);
//	void SetParent(Node* newParentNode);
//	void SetNodeGridPos(std::pair<int, int> pos);
//	void SetNodeWorldPos(glm::vec3 pos);
//	void SetObject(int object);
//
//	void AddNeighbour(Node* neighbour);
//};
//
//class Grid {
//private:
//	size_t rowSize, colSize;
//	float nodeSizeWidth, nodeSizeHeight;
//	glm::vec3 midPoint;
//	size_t sizeOfData;
//	Node* data;
//
//public:
//	class Proxy {
//	private:
//		Grid& grid;
//		size_t row;
//
//	public:
//		Proxy(Grid& _grid, size_t _row) : grid{ _grid }, row{ _row } {}
//
//		Node& operator[](size_t colIndex) {
//			return grid.data[row * grid.colSize + colIndex];
//		}
//	};
//
//	Grid() : rowSize{ 0 }, colSize{ 0 }, nodeSizeWidth{ 0 }, nodeSizeHeight{ 0 }, midPoint{ 0.f, 0.f, 0.f }, sizeOfData{ 0 }, data{ nullptr } {}
//
//	Grid(size_t _rowSize, size_t _colSize, float _nodeSizeWidth, float _nodeSizeHeight, glm::vec3 _midPoint) : rowSize{ _rowSize }, colSize{ _colSize },
//		nodeSizeWidth{ _nodeSizeWidth }, nodeSizeHeight{ _nodeSizeHeight }, midPoint{ _midPoint }, sizeOfData{ rowSize * colSize }, data{ new Node[sizeOfData] } {
//	}
//
//	Grid(Grid const& otherGrid) : rowSize{ otherGrid.rowSize }, colSize{ otherGrid.colSize }, nodeSizeWidth{ otherGrid.nodeSizeWidth },
//		nodeSizeHeight{ otherGrid.nodeSizeHeight }, midPoint{ otherGrid.midPoint }, sizeOfData{ otherGrid.sizeOfData }, data{ new Node[otherGrid.sizeOfData] } {
//		for (size_t i = 0; i < sizeOfData; ++i) {
//			data[i] = otherGrid.data[i];
//		}
//	}
//
//	Grid(Grid&& otherGrid) noexcept : rowSize{ otherGrid.rowSize }, colSize{ otherGrid.colSize }, nodeSizeWidth{ otherGrid.nodeSizeWidth },
//		nodeSizeHeight{ otherGrid.nodeSizeHeight }, midPoint{ otherGrid.midPoint }, sizeOfData{ otherGrid.sizeOfData }, data{ otherGrid.data } {
//		otherGrid.data = nullptr;
//	}
//
//	Grid& operator=(Grid const& other) = default;
//
//	Grid& operator=(Grid&& other) noexcept = default;
//
//	~Grid() noexcept {
//		delete[] data;
//	}
//
//	Proxy operator[](size_t rowIndex) {
//		return Proxy(*this, rowIndex);
//	}
//
//	size_t GetRowSize() const {
//		return rowSize;
//	}
//
//	size_t GetColSize() const {
//		return colSize;
//	}
//
//	float GetNodeSizeWidth() const {
//		return nodeSizeWidth;
//	}
//
//	float GetNodeSizeHeight() const {
//		return nodeSizeHeight;
//	}
//
//	glm::vec3 GetMidPoint() const {
//		return midPoint;
//	}
//};
//
//#endif