#include "spatial_hash.h"

bool SpatialHash::KeyIndexPair::Compare(KeyIndexPair a, KeyIndexPair b) { return a.cell_key < b.cell_key; }

SpatialHash::SpatialHash(int num_fish, int grid_size, Vector2 screen_size) : num_fish(num_fish), grid_size(grid_size) {
    // Generates fish
    for (uint i = 0; i < num_fish; i++) {
        Vector2 position(Randint(0, screen_size.x), Randint(0, screen_size.y));
        float angle = M_PI * (Randint(0, 360) / 180.0f);

        fishes.push_back(new Fish(position, angle, grid_size, screen_size));
    }

    spatial_list = vector<KeyIndexPair>(grid_size);
    start_indices = vector<int>(grid_size);

   // Update();
}

SpatialHash::~SpatialHash() {
    for (Fish* fish : fishes) {
        delete fish;
    }

    fishes.clear();
    spatial_list.clear();
    start_indices.clear();
}

/// @brief Generates a compressed hash value by from cell coordinates
/// @param point The point on the cell grid
/// @return An unsigned hash representation of a point
uint SpatialHash::HashPoint(Vector2 point) const {
    int cell_x = point.x / grid_size;
    int cell_y = point.y / grid_size;

    return abs((cell_x * 67474109 + cell_y * 87513581) % num_fish);
}

/// @brief Retrieves the indices of all fish in a cell
/// @param point The point to be hashed and lookup
/// @return A vector of the indices of all fish in the specified cell
vector<Fish*> SpatialHash::GetFishFromPoint(Vector2 point) const {
    uint key = HashPoint(point);
    vector<Fish*> indices;

    // Loops from the index of the first fish of a cell
    for (uint i = start_indices[key]; i < num_fish; i++) {
        // No fish are found in that cell
        if (i == -1) {
            break;
        }

        // End of fish in that cell
        if (spatial_list[i].cell_key != key) {
            break;
        }

        uint fish_index = spatial_list[i].fish_index;
        Fish* fish = fishes[fish_index];
        indices.push_back(fish);
    }

    return indices;
}

/// @brief Updates all fish in the spatial hash
void SpatialHash::Update() {
    // Generates a hash for the position of each fish
    for (uint i = 0; i < num_fish; i++) {
        Fish* fish = fishes[i];
        Vector2 position = fish->head->position;

        // Converts fish position into hash
        uint key = HashPoint(position);

        spatial_list[i] = KeyIndexPair{key, i};
        start_indices[i] = -1;
    }

    // Sorts spatial list by cell key
    std::sort(spatial_list.begin(), spatial_list.end(), KeyIndexPair::Compare);

    // Finds the index of a first occurrence of consecutive cell keys
    for (uint i = 0; i < num_fish; i++) {
        uint key = spatial_list[i].cell_key;
        uint prev_key = (i == 0) ? UINT_MAX : spatial_list[i - 1].cell_key;

        if (key != prev_key) {
            start_indices[key] = i;
        }
    }
}

/// @brief Generates a random number from min inclusive to max inclusive
/// @param min The lower bound of random numbers
/// @param max The upper bound of random numbers
/// @return A random number in the range [min, max]
int Randint(int min, int max) { return rand() % (max - min + 1) + min; }