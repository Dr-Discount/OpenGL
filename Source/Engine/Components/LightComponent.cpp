#include "LightComponent.h"

namespace neu {
	FACTORY_REGISTER(LightComponent)

	void LightComponent::Update(float dt) {

	}

	void LightComponent::SetProgram(Program& program, const std::string& name, const glm::mat4& view) {
		// view-space position (existing)
		glm::vec3 position_view = glm::vec3(view * glm::vec4(owner->transform.position, 1));

        program.SetUniform(name + ".type", (int)lightType);
		program.SetUniform(name + ".color", color);
		program.SetUniform(name + ".position", position_view);      
        program.SetUniform(name + ".intensity", intensity);
        program.SetUniform(name + ".range", range);

	}

	void LightComponent::Read(const serial_data_t& value) {
		SERIAL_READ(value, color);
        SERIAL_READ(value, intensity);
        SERIAL_READ(value, range);
	}

	void LightComponent::UpdateGui() {
        const char* types[] = { "Point", "directional", "Spot" };
        std::array<const char*, 3> t;

        //ImGui::Combo{ "Type", (int*)&lightType, types };
		ImGui::ColorEdit3("Color", glm::value_ptr(color));
        ImGui::DragFloat("intensity", &intensity, 0.1f, 0.1f);
		ImGui::DragFloat("range", &range, 0.1f, 0.1f);
        ImGui::DragFloat("outerCutoff", &outerCutoff, 0.1f, 0.1f);
	}
}