#pragma once

#include <algorithm>
#include <climits>
#include <iostream>
#include <limits>
#include <vector>

#include "fish.h"
#include "vector2.h"

class SpatialHash {
    struct CellFishPair {
        uint cell_key;
        Fish* fish;
        static bool Compare(CellFishPair a, CellFishPair b);
    };

   public:
    SpatialHash(const int num_fish, const int grid_size, const Vector2 screen_size);
    ~SpatialHash();

    uint HashPoint(const Vector2 point) const;
    vector<Fish*> GetFishFromPoint(const Vector2 point) const;

    vector<Fish*> GetFish() const;
    void Update();

   private:
    uint num_fish;
    uint grid_size;
    uint num_grid_cells;

    // List of cell-fish pairs containing the cell that the fish is in
    vector<CellFishPair> spatial_list;

    // List of the start indices of consecutive cell positions for a
    // sorted spatial list
    vector<uint> start_indices;
};

int Randint(const int min, const int max);