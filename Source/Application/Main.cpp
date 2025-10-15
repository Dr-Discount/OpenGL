int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;
    
	//openGL setup
    std::vector<neu::vec3> points { {-0.5f, 0.3f, 0 }, {0.9f, 0.7f, 0 }, {-0.4f, -0.9f, 0 }};
    std::vector<neu::vec3> colors{ {0, 1, 0}, {0, 0, 1}, {0, 1, 0}};
    std::vector<neu::vec2> textcords{{0, 0}, {0.5f, -1}, {1, 1}};

    struct Vertex {
        neu::vec3 position;
        neu::vec3 color;
		neu::vec2 textcord;
    };

    std::vector<Vertex> vertices{
        {{-0.5f, -0.5f, 0 }, {0, 1, 0}, {0, 0} },
        {{-0.5f, 0.5f, 0 }, {0, 0, 1}, {0, 1} },
        {{0.5f, 0.5f, 0 }, {0, 1, 0}, {1, 1}},
        {{0.5f, -0.5f, 0 }, {0, 0, 1}, {1, 0}},
    };

    std::vector<unsigned int> indices{
        0, 1, 2,
        0, 2, 3
	};

    /*
    GLuint vbo[3];
    glGenBuffers(3, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(neu::vec3), points.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(neu::vec3), colors.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, textcords.size() * sizeof(neu::vec2), textcords.data(), GL_STATIC_DRAW);

    GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    //position
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
	//color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//textcord
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    */

	//vertex buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//index buffer
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    //vertex array
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textcord));

    //vertex shader
    std::string vs_source;
	neu::file::ReadTextFile("Shaders/Basic.vert", vs_source);
	const char* vs_cstr = vs_source.c_str();
    
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_cstr, nullptr);
	glCompileShader(vs);

	//check for vetex compile errors
    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    } else {
        LOG_INFO("Shader compilation succeeded");
	}

	//fragment shader
	std::string fs_source;
	neu::file::ReadTextFile("Shaders/Basic.frag", fs_source);
	const char* fs_cstr = fs_source.c_str();

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_cstr, nullptr);
	glCompileShader(fs);

	//check for fragment compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    } else {
        LOG_INFO("Shader compilation succeeded");
    }

    //Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("program linked failed: {}", infoLog);
    } else {
		LOG_INFO("Program linked succeeded");
    }

	glUseProgram(program);

    //texture
	neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/hornet.png");
    
    //uniform
	GLint uniform = glGetUniformLocation(program, "u_time");

	GLint tex_Uniform = glGetUniformLocation(program, "u_texture");
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

        // draw
        //neu::vec3 color{ 0, 0, 0 };
        //neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size());
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}