#pragma once

#include "glm.hpp"
#include <string>
#include <vector>
//#include "Texture.h"

namespace aie {

// a simple triangle mesh wrapper
class OBJMesh {
public:

	// a vertex structure for obj files
	struct Vertex {
		glm::vec3 position; // added to attrib location 0
		glm::vec3 normal;	// added to attrib location 1
		glm::vec2 texcoord;	// added to attrib location 2

							// normal-mapping data
		glm::vec4 tangent;	// added to attrib location 3
	};

	// a basic material
	class Material {
	public:

		Material() : ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1) {}
		~Material() {}

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 emissive;

		float specularPower;
		float opacity;
	};

	OBJMesh() {}
	~OBJMesh();

	// will fail if a mesh has already been loaded in to this instance
	bool load(const char* filename, bool loadTextures = true, bool flipTextureV = false);

	//Loads a texture and binds it
	void LoadDiffuse(const char* texturePath);
	void LoadNormal(const char* texturePath);

	// allow option to draw as patches for tessellation
	void draw(bool usePatches = false);

	// access to the filename that was loaded
	const std::string& getFilename() const { return m_filename; }

	// material access
	size_t getMaterialCount() const { return m_materials.size();  }
	Material& getMaterial(size_t index) { return m_materials[index];  }

	glm::vec3 GetPosition() { return (glm::vec3)m_localTransform[3]; }
	void SetPosition(glm::vec3 position) { m_localTransform[3] = glm::vec4(position, 1.0f); }
	float GetScale() { return m_scale; }
	void SetScale(float scale);

	std::vector<Material>	m_materials;	//Moved to public for Application access

private:

	void calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	struct MeshChunk {
		unsigned int	vao, vbo, ibo;
		unsigned int	indexCount;
		int				materialID;
	};

	std::string				m_filename;
	std::vector<MeshChunk>	m_meshChunks;

	unsigned int m_diffuseTexture = 0;
	unsigned int m_normalTexture = 0;

	glm::mat4 m_localTransform;
	float m_scale = 1.0f;
};

} // namespace aie