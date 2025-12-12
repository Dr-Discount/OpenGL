#pragma once

namespace neu {
	class PostProcessComponent : public Component {
	public:
		enum class Parameters : uint32_t {
			None		= 0,
            GrayScale = (1 << 0),
            ColorTint = (1 << 1),
            ScanLines = (1 << 2),
		};

	public:
		CLASS_PROTOTYPE(PostProcessComponent)

		void Update(float dt) override;
		void Apply(Program& program);

		void Read(const serial_data_t& value);
		void UpdateGui();

	public:
		Parameters parameters = Parameters::None;
        glm::vec3 colorTint{ 1.0f, 1.0f, 1.0f };
	};
}