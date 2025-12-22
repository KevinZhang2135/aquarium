#include "spatial_hash.h"

bool SpatialHash::CellFishPair::Compare(CellFishPair a, CellFishPair b) { return a.cell_key < b.cell_key; }

SpatialHash::SpatialHash(const int num_fish, const int grid_size, const Vector2 screen_size)
    : num_fish(num_fish), grid_size(grid_size) {
    num_grid_cells = ceilf(screen_size.x / grid_size) * ceilf(screen_size.y / grid_size);
    start_indices = vector<uint>(num_grid_cells);

    // Generates fish
    spatial_list = vector<CellFishPair>(num_fish);
    for (uint i = 0; i < num_fish; i++) {
        // Random position and orientation
        Vector2 position(Randint(0, screen_size.x), Randint(0, screen_size.y));
        float angle = M_PI * (Randint(0, 360) / 180.0f);

        // Maps fish with their grid cell
        spatial_list[i] = (CellFishPair){
            HashPoint(position),
            new Fish(position, angle, grid_size, screen_size),
        };
    }
    
    Update();
}

SpatialHash::~SpatialHash() {
    // Deletes all fish pointers
    for (uint i = 0; i < num_fish; i++) {
        delete spatial_list.at(i).fish;
    }

    spatial_list.clear();
    start_indices.clear();
}

/// @brief Generates a compressed hash value by from cell coordinates
/// @param point The point on the cell grid
/// @return An unsigned hash representation of a point
uint SpatialHash::HashPoint(const Vector2 point) const {
    int cell_x = point.x / grid_size;
    int cell_y = point.y / grid_size;

    // Arbitrarily large primes
    return abs((cell_x * 67474109 + cell_y * 87513581) % (int)num_grid_cells);
}

/// @brief Retrieves the indices of all fish in a cell
/// @param point The point to be hashed and lookup
/// @return A vector of the indices of all fish in the specified cell
vector<Fish*> SpatialHash::GetFishFromPoint(const Vector2 point) const {
    uint cell_key = HashPoint(point);
    vector<Fish*> fishes;

    // Loops from the index of the first fish of a cell
    for (uint i = start_indices[cell_key]; i < num_fish; i++) {
        // End of fish in that cell
        if (spatial_list[i].cell_key != cell_key) {
            break;
        }

        fishes.push_back(spatial_list[i].fish);
    }

    return fishes;
}

vector<Fish*> SpatialHash::GetFish() const {
    vector<Fish*> fishes(num_fish);
    transform(spatial_list.begin(), spatial_list.end(), fishes.begin(), [](CellFishPair pair) { return pair.fish; });

    return fishes;
}

/// @brief Updates all fish in the spatial hash
void SpatialHash::Update() {
    // Performs counting sort
    fill(start_indices.begin(), start_indices.end(), 0);  // Clears counts

    // Hashes the position of each fish and counts the occurrence of hashes
    for (uint i = 0; i < num_fish; i++) {
        Fish* fish = spatial_list[i].fish;
        Vector2 position = fish->head->position;

        // Converts fish position into hash
        uint cell_key = HashPoint(position);
        spatial_list[i].cell_key = cell_key;

        start_indices[cell_key]++;
    }

    // Adds each start index with culmulative sum of all previous indices get the ending indices of each run of
    // duplicate elements
    for (uint i = 1; i < num_grid_cells; i++) {
        start_indices[i] += start_indices[i - 1];
    }

    // Inserts all elements into a temporary list before copying it back
    vector<CellFishPair> sorted_temp(num_fish);
    for (int i = num_fish - 1; i >= 0; i--) {
        uint cell_key = spatial_list[i].cell_key;

        sorted_temp[start_indices[cell_key] - 1] = spatial_list[i];
        start_indices[cell_key]--;
    }

    copy(sorted_temp.begin(), sorted_temp.end(), spatial_list.begin());
}

/// @brief Generates a random number from min inclusive to max inclusive
/// @param min The lower bound of random numbers
/// @param max The upper bound of random numbers
/// @return A random number in the range [min, max]
int Randint(int min, int max) { return rand() % (max - min + 1) + min; }