
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;

    std::vector<neu::vec3> points {
        {-0.5f, 0.3f, 0 },
        {0.9f, 0.7f, 0 },
        {-0.4f, -0.9f, 0 }
    };
    std::vector<neu::vec3> colors{
        {0, 1, 0},
        {0, 0, 1},
        {0, 1, 0}
	};

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

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

        glLoadIdentity();
        glPushMatrix();
        glTranslatef(0, 0, 0);
		glRotatef(60.0f * neu::GetEngine().GetTime().GetTime(), 0, 0, 1);

        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < points.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
        glPopMatrix();
        glEnd();
        
        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
		glVertex3f(-1, 0, 0);
        glColor3f(0, 0, 1);
		glVertex3f(1, 0, 0);

        glColor3f(1, 0, 0);
        glVertex3f(0, -1, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 1, 0);
        glEnd();



        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}