#include "raylib.h"
#pragma once

// block.h

#ifndef BLOCK_H
#define BLOCK_H

#include "raylib.h"

#define BLOCK_COUNT 5

struct Block {
    int id;
    Color color;
};

extern struct Block block_list[BLOCK_COUNT];


#endif // BLOCK_H
