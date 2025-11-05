#pragma once
#include "Framework/Component.h"

namespace neu {
	class LightComponent : public Component, GUI {
	public:
        enum class LightType {
            Point, 
            Directionl,
            Spot
        };

		CLASS_PROTOTYPE(LightComponent)

			void Update(float dt) override;
		void SetProgram(Program& program, const std::string& name, const glm::mat4& view);

		void Read(const serial_data_t& value) override;
		void UpdateGui() override;

	public:
		glm::vec3 color{ 1, 1, 1 };
        float intensity{ 1 };
        float range{ 20 };
        LightType lightType = LightType::Point;

        float outerCutoff{ 20 };
	};
}