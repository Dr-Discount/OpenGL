int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;

	//load model
    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/sphere.obj");

    //material
    auto material = neu::Resources().Get<neu::Material>("materials/hornet.mat");
	material->Bind();

	GLint uniform = glGetUniformLocation(material->program->m_program, "u_time");

    //lights
    material->program->SetUniform("u_ambient_light", glm::vec3{ 0.2, 0.2, 0.2 });
	neu::Transform light{ { 0, 5,  5} };
	glm::vec3 lightColor{ 1 };

    //transformation
    neu::Transform transform{ {1,0,0} };
    neu::Transform camera{ { 0, 0, 5 } };

    //projection matrix
    float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f);
    material->program->SetUniform("u_projection", projection);

	GLint tex_Uniform = glGetUniformLocation(material->program->m_program, "u_texture");
	glUniform1d(tex_Uniform, 0);

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
		    ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
		glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());

        //Camera
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * neu::GetEngine().GetTime().GetDeltaTime();
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * neu::GetEngine().GetTime().GetDeltaTime();
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.y -= speed * neu::GetEngine().GetTime().GetDeltaTime();
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.y += speed * neu::GetEngine().GetTime().GetDeltaTime();

		//model transform
        material->program->SetUniform("u_model", transform.GetMatrix());

		//view matrix
		glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{0, 0, -1}, glm::vec3{0, 1, 0});
		material->program->SetUniform("u_view", view);

		material->program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));
		light.position.x = neu::math::sin(neu::GetEngine().GetTime().GetTime() * 4) * 5.0f;
        material->program->SetUniform("u_light.color", lightColor);

        // draw
        neu::GetEngine().GetRenderer().Clear();

        // start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // set ImGui
        ImGui::Begin("Editor");
		ImGui::DragFloat3("position", glm::value_ptr(light.position), 0.1f);
        ImGui::ColorEdit3("color", glm::value_ptr(lightColor));
		transform.updateGUI();
		material->updateGUI();
        ImGui::End();

        model3d->Draw(GL_TRIANGLES);

        // draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//vb->Draw(GL_TRIANGLES);
        model3d->Draw(GL_TRIANGLES);

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}