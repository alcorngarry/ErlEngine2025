#ifndef MODEL_H
#define MODEL_H
#include<glm/glm.hpp>
#include<glm/ext/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<stb_image/stb_image.h>
#include<map>
#include<string>
#include"animation/BoneInfo.h"
#include"animation/Bone.h"
#include"Mesh.h"

class Model
{
	public:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh*> meshes;
		std::string directory;
		std::string fileName;
		bool gammaCorrection;

		Model() = default;
		Model(char* path);

		void draw(Shader& shader);
		glm::vec3 getMinAABB() const;
		glm::vec3 getMaxAABB() const;
		std::map<std::string, BoneInfo> get_bone_info_map() const;
		int& get_bone_count();

	private:
		glm::vec3 minAABB;
		glm::vec3 maxAABB;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		int m_BoneCounter = 0;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
		void extract_bone_weight_for_vertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
		void set_vertex_bone_data(Vertex& vertex, int boneID, float weight);
		void set_vertex_bone_data_to_default(Vertex& vertex);
		glm::mat4 convert_mat4_to_glm(const aiMatrix4x4& from);
};

#endif 