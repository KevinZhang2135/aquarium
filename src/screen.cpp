#include "screen.h"

Screen::Screen(uint width, uint height, bool full_screen) {
    // Seeds random number generator with current time
    srand(time(NULL));

    // Creates SDL window and renderer
    this->width = width;
    this->height = height;

    int flags = (full_screen) ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;
    isRunning = SDL_Init(SDL_INIT_VIDEO) == 0;

    if (isRunning) {
        window = SDL_CreateWindow("Aquarium", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
    }

    if (debug) {
        Fish::MAX_SEGMENTS = 1;
    }

    // Generates optimization hash arrays
    spatial_hash = new SpatialHash(NUM_FISH, GRID_SIZE, Vector2(width, height));
}

Screen::~Screen() {
    delete spatial_hash;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

/// @brief Returns whether the screen is running
/// @return Whether the screen is running
bool Screen::Running() { return isRunning; }

void Screen::HandleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            for (Fish* fish : spatial_hash->GetFish()) {
                fish->SetPosition(Vector2(mouse_x, mouse_y));
            }

            break;

        default:
            break;
    }
}

/// @brief Renders the specified fish onto the renderer
/// @param fish The fish to be drawn
void Screen::DrawFish(const Fish* fish) const {
    Anchor* anchor = fish->head;
    Anchor* temp = nullptr;

    if (debug) {
        // Debug triangle
        float base_ratio = 0.666; // Ratio of the base to height

        // Vector representing the height and base of the triangle
        Vector2 height = fish->velocity.ScaleToLength(anchor->radius) * 4;
        Vector2 base = height.Rotate(M_PI_2) * base_ratio;

        // Triangle points
        Vector2 pt1 = anchor->position + height * base_ratio;
        Vector2 pt2 = anchor->position - height * (1 - base_ratio) + base / 2;
        Vector2 pt3 = pt2 - base;

        aatrigonRGBA(renderer, pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, 255, 255, 255, 255);
        return;  // Prevents fish from being drawn for debugging
    }

    // Traverses the linked list for each anchor of the fish
    uint i = 0;
    while (anchor != nullptr) {
        // 0xAABBGGRR
        uint color = 0xffab4700;
        bool draw_fins = (i == fish->MAX_SEGMENTS / 5) || (i == fish->MAX_SEGMENTS * 5 / 7);

        // Draws body
        filledCircleColor(renderer, anchor->position.x, anchor->position.y, anchor->radius, color);
        if (draw_fins) DrawFins(anchor);

        anchor = anchor->next;
        i++;
    }
}

/// @brief Draws fins extending from the specified anchor
/// @param anchor The anchor to drawn fins from
void Screen::DrawFins(const Anchor* anchor) const {
    uint color = 0xffcc9066;
    int fin_segments = 4;
    float fin_angle = M_PI * 0.7f;
    float fin_radius = anchor->radius * 0.6f;

    // Pectoral fins
    // Switches between negative and positive angles
    for (int sign = -1; sign <= 1; sign += 2) {
        // Draws a series of circles extending from the body at the fin angle
        for (uint i = 1; i <= fin_segments; i++) {
            Vector2 fin_position =
                anchor->position.MoveTowards(anchor->angle + fin_angle * sign, fin_radius * (1.0f + i / 2.0f));

            filledCircleColor(renderer, fin_position.x, fin_position.y, fin_radius * (1.0f - i / 10.0f), color);
        }
    }
}

/// @brief Clears and redraws screen
void Screen::Render() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draws fish
    for (Fish* fish : spatial_hash->GetFish()) DrawFish(fish);

    if (debug) {
        // Draws optimization grid lines
        for (uint x = 0; x < width; x += GRID_SIZE) {
            vlineColor(renderer, x, 0, height, 0xffffffff);
        }

        for (uint y = 0; y < width; y += GRID_SIZE) {
            hlineColor(renderer, 0, height, y, 0xffffffff);
        }

        for (Fish* fish : spatial_hash->GetFishFromPoint(Vector2(mouse_x, mouse_y))) {
            Anchor* anchor = fish->head;
            // Debug point
            filledCircleColor(renderer, anchor->position.x, anchor->position.y, anchor->radius, 0xff0000ff);
        }
    }

    SDL_RenderPresent(renderer);
}

/// @brief Seraches for boids in the grid coordinate of the point
/// @param point The point within a grid coordinate
/// @return A vector of boids within the grid coordinate of the point
vector<Fish*> Screen::SearchForBoids(Vector2 point) const {
    // Vector of all fish within a 3x3 grid
    vector<Fish*> nearby_boids;

    // Searches within 3x3 grid tiles around the boid
    for (int offset_x = -1; offset_x <= 1; offset_x++) {
        for (int offset_y = -1; offset_y <= 1; offset_y++) {
            Vector2 offset(offset_x, offset_y);
            offset = offset * GRID_SIZE;

            // Boids within a nearby cell
            vector<Fish*> other_boids = spatial_hash->GetFishFromPoint(point + offset);
            nearby_boids.insert(nearby_boids.end(), other_boids.begin(), other_boids.end());
        }
    }

    return nearby_boids;
}

/// @brief Updates the heading of a specified boid
/// @param boid The specified boid to update its velocity
void Screen::UpdateBoid(Fish* boid) const {
    Vector2 position = boid->head->position;
    vector<Fish*> nearby_boids = SearchForBoids(position);

    // Updates boid
    boid->Update(nearby_boids);
}

/// @brief Updates the heading and position of each boid
void Screen::Update(double delta_time) {
    int fps = 1000 / delta_time;
    SDL_SetWindowTitle(window, std::format("Aquarium | {} fps", fps).c_str());

    spatial_hash->Update();
    for (Fish* fish : spatial_hash->GetFish()) {
        UpdateBoid(fish);
    }

    for (Fish* fish : spatial_hash->GetFish()) {
        fish->Move(delta_time);
    }

    SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_UpdateWindowSurface(window);
}
