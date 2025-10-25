# 🧩 KosEngine 2.0  

![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![CMake](https://img.shields.io/badge/Build-CMake-green.svg)
![Tests](https://img.shields.io/badge/Testing-GoogleTest-orange.svg)
![Pipeline](https://img.shields.io/badge/CI-Bitbucket%20Pipelines-blue.svg)
![Status](https://img.shields.io/badge/Status-Active-success.svg)

KosEngine 2.0 is a ongoing **modular, high-performance 3D game engine** built with **modern C++**, featuring **compile-time reflection**, a **powerful asset system**, and **robust 3D rendering architecture**.  

This release focuses on **performance, scalability**, and **automated testing** through **Bitbucket Pipelines**.  

---

## 🚀 Features  

### 🧱 Core Architecture  
- **Entity-Component-System (ECS)** framework for modular and efficient gameplay logic  
- **Compile-Time Reflection** for serialization, editor integration, and metadata generation  
- **Field Metadata System** enabling safe introspection and manipulation of class fields  

### 🖥️ Graphics  
- Modern **3D rendering pipeline** (OpenGL/Vulkan-ready)  
- Support for **materials, meshes, lighting, and cameras**  
- Optimized GPU resource and draw call management  

### 📦 Asset & Resource System  
- Centralized **asset manager** with caching and reference counting  
- Handles **textures, shaders, meshes, prefabs, and scenes**  
- **JSON-based asset definitions** for easy editing and tool integration  

### ⚙️ Build System  
- **CMake-based** cross-platform build setup  
- Modular subproject structure for integration and scalability  
- Easy dependency management via external libraries  

### 🧪 Testing  
- Integrated **GoogleTest** framework  
- Automated **Bitbucket Pipeline** build & test validation  
