#pragma once
#include "Resources/Resource.h"
#include <glm/glm.hpp>
#include "GUI.h"

namespace neu {
	class Program;
	class Texture;

	class Material : public Resource, GUI {
	public:
        enum class Parameters : uint32_t {
           None = 0,
           BaseMap = (1 << 0),
           SpecularMap = (1 << 1),
           EmissiveMap = (1 << 2),
           NormalMap = (1 << 3),
           CubeMap = (1 << 4)
        };

		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();

		void UpdateGui() override;

	public:

		res_t<Program> program;
		res_t<Texture> baseMap;
		glm::vec3 baseColor{ 1 };
		res_t<Texture> spcularMap;
        res_t<Texture> emissiveMap;
        res_t<Texture> cubeMap;
        glm::vec3 emissiveColor{ 0 };
		
		float shininess{ 2 };
		glm::vec2 tiling{ 1, 1 };
		glm::vec2 offset{ 0, 0 };

        Parameters parameters = Parameters::None;
	};
}