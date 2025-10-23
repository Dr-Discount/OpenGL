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

    //shaders     
    /*auto vs = neu::Resources().Get<neu::Shader>("Shaders/basic_lit.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("Shaders/basic_lit.frag", GL_FRAGMENT_SHADER);*/

    //Program
    auto program = neu::Resources().Get<neu::Program>("shaders/basic_lit.prog");
    program->Use();

    int success;
    glGetProgramiv(program->m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(program->m_program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("program linked failed: {}", infoLog);
    } else {
		LOG_INFO("Program linked succeeded");
    }

	glUseProgram(program->m_program);

    //texture
	neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/hornet.png");

	GLint uniform = glGetUniformLocation(program->m_program, "u_time");

    //lights
    program->SetUniform("u_ambient_light", glm::vec3{ 0.2, 0.2, 0.2 });
	neu::Transform light{ { 0, 5,  5} };

    //transformation
    neu::Transform transform{ {1,0,0} };
    neu::Transform camera{ { 0, 0, 5 } };

    //projection matrix
    float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f);
    program->SetUniform("u_projection", projection);

	GLint tex_Uniform = glGetUniformLocation(program->m_program, "u_texture");
	glUniform1d(tex_Uniform, 0);

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
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
        program->SetUniform("u_model", transform.GetMatrix());

		//view matrix
		glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{0, 0, -1}, glm::vec3{0, 1, 0});
		program->SetUniform("u_view", view);

		program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));
		light.position.x = neu::math::sin(neu::GetEngine().GetTime().GetTime() * 4) * 5.0f;
        program->SetUniform("u_light.color", glm::vec3{ 1, 1, 1 });

        // draw
        neu::GetEngine().GetRenderer().Clear();

		//vb->Draw(GL_TRIANGLES);
        model3d->Draw(GL_TRIANGLES);

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}