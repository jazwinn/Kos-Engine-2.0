#include "Config/pch.h"
#include "Resources/R_Animation.h"

glm::mat4 R_Animation::Bone::Interpolate(float time) const {
    glm::mat4 T = InterpolatePosition(time);
    glm::mat4 R = InterpolateRotation(time);
    glm::mat4 S = InterpolateScale(time);
    return T * R * S;
}
const std::string& R_Animation::Bone::GetName() const {
    return m_Name;
}
int R_Animation::Bone::GetID() const {
    return m_ID;
}
int R_Animation::Bone::FindIndex(const std::vector<float>& times, float animTime) const {
    for (size_t i = 0; i < times.size() - 1; i++)
    {
        if (animTime < times[i + 1])
        {
            return static_cast<int>(i);
        }
    }
    return static_cast<int>(times.size() - 2);
}
float R_Animation::Bone::GetFactor(float start, float end, float time) const {
    float diff = end - start;
    ///Might division by zero
    return (time - start) / diff;
}
glm::mat4 R_Animation::Bone::InterpolatePosition(float time) const {
    if (m_Positions.size() == 1)
    {
        return glm::translate(glm::mat4(1.0f), m_Positions[0]);
    }

    int index = FindIndex(m_PosTimes, time);
    float factor = GetFactor(m_PosTimes[index], m_PosTimes[index + 1], time);
    glm::vec3 finalPos = glm::mix(m_Positions[index], m_Positions[index + 1], factor);
    return glm::translate(glm::mat4(1.0f), finalPos);
}
glm::mat4 R_Animation::Bone::InterpolateRotation(float time) const {
    if (m_Rotations.size() == 1)
    {
        return glm::mat4_cast(m_Rotations[0]);
    }

    int index = FindIndex(m_RotTimes, time);
    float factor = GetFactor(m_RotTimes[index], m_RotTimes[index + 1], time);
    glm::quat finalRot = glm::slerp(m_Rotations[index], m_Rotations[index + 1], factor);
    return glm::mat4_cast(finalRot);
}
glm::mat4 R_Animation::Bone::InterpolateScale(float time) const {
    if (m_Scales.size() == 1)
    {
        return glm::scale(glm::mat4(1.0f), m_Scales[0]);
    }

    int index = FindIndex(m_ScaleTimes, time);
    float factor = GetFactor(m_ScaleTimes[index], m_ScaleTimes[index + 1], time);
    glm::vec3 finalScale = glm::mix(m_Scales[index], m_Scales[index + 1], factor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}
template<typename T>
inline T R_Animation::DecodeBinary(std::string& bin, int& offset)
{

    // Check if we have enough bytes remaining
    if (offset + sizeof(T) > bin.size()) {
        LOGGING_DEBUG("File overflow");;
        return T{};;
        // return ;
    }
    T value;
    // Copy the raw bytes directly from the string
    std::memcpy(&value, bin.data() + offset, sizeof(T));
    offset += sizeof(T);

    return value;
}

R_Animation::NodeData R_Animation::NodeDataParser(std::string& serialized, int& offset) {
    unsigned int nameSize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
    NodeData node;
    for (unsigned int i{ 0 }; i < nameSize; i++) {
        node.name += DecodeBinary<char>(serialized, offset);
    }
    std::cout << "ADDING NODE DATA NAME " << node.name;
    node.transformation= DecodeBinary<glm::mat4>(serialized, offset);
    nameSize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
    //Add childrean kill me pls hi Sean
    for (unsigned int i{ 0 }; i < nameSize; i++) {
        node.children.push_back(NodeDataParser(serialized, offset));
    }
    return node;
}
void R_Animation::Load()  {

    //Load from file 
    std::ifstream inputFile(this->m_filePath.string().c_str(), std::ios::binary);
    if (!inputFile) {
        LOGGING_ASSERT("Failed to open mesh file");
    }
    std::string serialized((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    int offset = 0;

    this->m_Duration = DecodeBinary<float>(serialized, offset);
    this->m_TicksPerSecond = DecodeBinary<float>(serialized, offset);

    unsigned int nameSize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
    this->m_Name.clear();
    for (unsigned int i{ 0 }; i < nameSize; i++) {
        this->m_Name += DecodeBinary<char>(serialized, offset);
        //std::cout << this->m_Name << '\n';
    }
    std::cout << "ANBIMATION DETAILKS\n";
    std::cout << this->m_Duration <<'\n';
    std::cout << this->m_TicksPerSecond << '\n';
    std::cout << nameSize;
    std::cout << this->m_Name<<'\n';

    unsigned int boneSize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
    std::cout << "BONE SIZE" << boneSize << '\n';
    for (unsigned int i{ 0 }; i < boneSize; i++) {
        //deserialize key
        unsigned int keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        std::string key;
        for (unsigned int i{ 0 }; i < keySize; i++) {
            key += DecodeBinary<char>(serialized, offset);
        }

        //Add a bone based on the key
        m_Bones[key] = Bone{};
        std::cout<<"KEY NAME" << key << '\n';

        //Get bone name
        keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        key.clear();
        for (unsigned int i{ 0 }; i < keySize; i++) {
            key += DecodeBinary<char>(serialized, offset);
        }
        std::cout << "NAME" << key << '\n';
        m_Bones[key].m_Name = key;

        //Serialize ID
        m_Bones[key].m_ID = DecodeBinary<int>(serialized, offset);

        //Get positions
        keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        for (unsigned int i{ 0 }; i < keySize; i++) {
            glm::vec3 pos;
            pos.x= DecodeBinary<float>(serialized, offset);
            pos.y = DecodeBinary<float>(serialized, offset);
            pos.z = DecodeBinary<float>(serialized, offset);
            std::cout <<"POSITON DATA" << pos.x << ' ' << pos.y << ' ' << pos.z << '\n';
            m_Bones[key].m_Positions.push_back(pos);
        }

        //Get position time
        keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        for (unsigned int i{ 0 }; i < keySize; i++) {
            m_Bones[key].m_PosTimes.push_back(DecodeBinary<float>(serialized, offset));
            std::cout << "POS TIME" << m_Bones[key].m_PosTimes.back() << '\n';
        }


        //Get rotations
        keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        for (unsigned int i{ 0 }; i < keySize; i++) {
            glm::quat rot;
            rot.x = DecodeBinary<float>(serialized, offset);
            rot.y = DecodeBinary<float>(serialized, offset);
            rot.z = DecodeBinary<float>(serialized, offset);
            rot.w = DecodeBinary<float>(serialized, offset);
            std::cout << "Rotation DATA" << rot.x << ' ' << rot.y << ' ' << rot.z <<rot.w<< '\n';
            m_Bones[key].m_Rotations.push_back(rot);
        }

        //Get rotation time
        keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        for (unsigned int i{ 0 }; i < keySize; i++) {
            m_Bones[key].m_RotTimes.push_back(DecodeBinary<float>(serialized, offset));
            std::cout << "ROT TIME" << m_Bones[key].m_RotTimes.back() << '\n';
        }

        //Get scales 
        keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        for (unsigned int i{ 0 }; i < keySize; i++) {
            glm::vec3 scale;
            scale.x = DecodeBinary<float>(serialized, offset);
            scale.y = DecodeBinary<float>(serialized, offset);
            scale.z = DecodeBinary<float>(serialized, offset);
            std::cout << "Rotation DATA" << scale.x << ' ' << scale.y << ' ' << scale.z << '\n';
            m_Bones[key].m_Scales.push_back(scale);
        }

        //Get rotation time
        keySize = static_cast<unsigned int>(DecodeBinary<size_t>(serialized, offset));
        for (unsigned int i{ 0 }; i < keySize; i++) {
            m_Bones[key].m_ScaleTimes.push_back(DecodeBinary<float>(serialized, offset));
            std::cout << "POS TIME" << m_Bones[key].m_ScaleTimes.back() << '\n';
        }
    }
    this->m_RootNode=NodeDataParser(serialized, offset);
    


}

void R_Animation::Unload() {

}

