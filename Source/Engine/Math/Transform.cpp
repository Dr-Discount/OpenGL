#include "Transform.h"

namespace neu {
    void Transform::Read(const serial_data_t& value) {
        SERIAL_READ(value, position);
        SERIAL_READ(value, rotation);
        SERIAL_READ(value, scale);
    }
    void Transform::updateGUI() {
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f);
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
    }
}