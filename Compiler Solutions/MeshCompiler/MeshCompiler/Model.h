#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>
#include <iostream>
#define MAX_BONE_INFLUENCE 4

//Forward Declarations
class Model;
class Bone;
class Animation;
class Animator;

struct NodeData
{
    std::string name;
    glm::mat4 transformation;
    std::vector<NodeData> children;
};

//Possibility to optimize this?
struct BoneInfo
{
    glm::mat4 offsetMatrix;      // From ASSIMP during loading
    glm::mat4 finalTransformation{ 1.f }; // Calculated each frame
};

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};

class Animation
{
public:
    
    Animation(const aiAnimation* anim, const aiScene* scene, std::unordered_map<std::string, int>& boneMap);

    const Bone* FindBone(const std::string& name) const;
    float GetDuration() const;
    float GetTicksPerSecond() const;
    const NodeData& GetRootNode() const;
    std::string m_Name;
    std::unordered_map<std::string, Bone> m_Bones;
    NodeData m_RootNode;

private:
    float m_Duration;
    float m_TicksPerSecond;

   
    NodeData CopyNodeHierarchy(const aiNode* src);

    glm::mat4 ConvertToGLMMat4(const aiMatrix4x4& original);
};

class Animator
{
public:
    Animator(const Animation* animation, const std::vector<BoneInfo>& boneInfo, const std::unordered_map<std::string, int>& boneMap, const glm::mat4& globalInverse);

    void Update(float dt, glm::mat4 parentTransform);
    const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

private:
    const Animation* m_CurrentAnimation;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::vector<BoneInfo> m_BoneInfo;
    std::unordered_map<std::string, int> m_BoneMap;
    glm::mat4 m_GlobalInverse{ 1.f };
    float m_CurrentTime = 0.0f;
    
    void CalculateBoneTransform(const NodeData& node, const glm::mat4& parentTransform);
    glm::mat4 ConvertToGLMMat4(const aiMatrix4x4& original);
};

class Bone
{
public:
    Bone() = default;
    Bone(const std::string& name, int id, const aiNodeAnim* channel);

    glm::mat4 Interpolate(float time) const;

    const std::string& GetName() const;
    int GetID() const;
    std::string m_Name;
    int m_ID;

    std::vector<glm::vec3> m_Positions;
    std::vector<float> m_PosTimes;

    std::vector<glm::quat> m_Rotations;
    std::vector<float> m_RotTimes;

    std::vector<glm::vec3> m_Scales;
    std::vector<float> m_ScaleTimes;

    int FindIndex(const std::vector<float>& times, float animTime) const;
    float GetFactor(float start, float end, float time) const;

    glm::mat4 InterpolatePosition(float time) const;
    glm::mat4 InterpolateRotation(float time) const;
    glm::mat4 InterpolateScale(float time) const;
};

class Model
{
public:
    Model(const char* path)
    {
        LoadModel(path);
    }
    Model(const char* path, int i) {
        LoadAnimations(path);
    }

    const std::vector<Animation>& GetAnimations() const { return animations; }
    const std::vector<BoneInfo>& GetBoneInfo() const { return bone_info; }
    const std::unordered_map<std::string, int>& GetBoneMap() const { return bones_loaded; }
    glm::mat4 GetGlobalInverse() const { return globalInverseTransform; }

    /// <summary>
    /// TEMPORARY HERE
    /// </summary>
    std::vector<Animation> animations;

   // void RetrieveAnimationIndex(unsigned int index) const;
    std::vector<Mesh> meshes;

private:
    std::unordered_map<std::string, int> bones_loaded{};
    std::vector<BoneInfo> bone_info{}; // Only contains the matrices of the bones not the bone itself
    // model data    
    std::string directory;

    //For animation purposes
    glm::mat4 globalInverseTransform{ 1.f };
    

    void LoadModel(std::string path);
    void LoadAnimations(std::string path);

    void ExtractBoneWeights(aiMesh* mesh, std::vector<Vertex>& vertices);
    
    void ProcessNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& transform);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const aiMatrix4x4& transform);

    //Utility
    glm::mat4 ConvertToGLMMat4(const aiMatrix4x4& original);
};

