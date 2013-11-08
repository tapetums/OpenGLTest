// Mesh.h

#include <stdint.h>
typedef float float32_t;

//---------------------------------------------------------------------------//

template<typename V, typename C, typename I>
struct Mesh
{
    size_t vertex_count;
    size_t index_count;

    V* vertices;
    C* colors;
    I* indices;
};

//---------------------------------------------------------------------------//

static float32_t cube_vertices[] =
{
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
};

static uint8_t cube_colors[] =
{
    0xFF, 0x00, 0x00, 0xFF, // RED
    0x00, 0x00, 0x00, 0xFF, // BLACK
    0xFF, 0x00, 0xFF, 0xFF, // PURPLE
    0xFF, 0xFF, 0x00, 0xFF, // YELLOW
    0x00, 0xFF, 0x00, 0xFF, // GREEN
    0x00, 0x00, 0xFF, 0xFF, // BLUE
    0x00, 0xFF, 0xFF, 0xFF, // CYAN
    0xFF, 0xFF, 0xFF, 0xFF, // WHITE
};

static uint8_t cube_indices[] =
{
    // 時計回りの面が表
    0, 4, 5,   0, 5, 1,
    1, 5, 6,   1, 6, 2,
    2, 6, 7,   2, 7, 3,
    3, 7, 4,   3, 4, 0,
    4, 7, 6,   4, 6, 5,
    3, 0, 1,   3, 1, 2
};

struct Cube : public Mesh<float32_t, uint8_t, uint8_t>
{
    Cube()
    {
        vertex_count = 8;
        index_count  = 12;

        vertices = cube_vertices;
        colors   = cube_colors;
        indices  = cube_indices;
    }
};

//---------------------------------------------------------------------------//

// Mesh.h