#include "def_mdl.h"

#include "brender/br_types.h"

br_face default_model_faces[12] = {
    { { 0, 1, 2, }, 1, NULL, 0, 0, 0, 0, BR_MATF_SMOOTH, 0, 0, { { 0 } }, 0.f },
    { { 0, 2, 3, }, 1, NULL, 0, 0, 0, 0, BR_MATF_LIGHT, 0, 0, { { 0 } }, 0.f, },
    { { 0, 4, 5, }, 1, NULL, 0, 0, 0, 0, BR_MATF_SMOOTH, 0, 0, { { 0 } }, 0.f, },
    { { 0, 5, 1, }, 1, NULL, 0, 0, 0, 0, BR_MATF_LIGHT, 0, 0, { { 0 } }, 0.f, },
    { { 1, 5, 6, }, 1, NULL, 0, 0, 0, 0, BR_MATF_SMOOTH, 0, 0, { { 0 } }, 0.f, },
    { { 1, 6, 2, }, 1, NULL, 0, 0, 0, 0, BR_MATF_LIGHT, 0, 0, { { 0 } }, 0.f, },
    { { 2, 6, 7, }, 1, NULL, 0, 0, 0, 0, BR_MATF_SMOOTH, 0, 0, { { 0 } }, 0.f, },
    { { 2, 7, 3, }, 1, NULL, 0, 0, 0, 0, BR_MATF_LIGHT, 0, 0, { { 0 } }, 0.f, },
    { { 3, 7, 4, }, 1, NULL, 0, 0, 0, 0, BR_MATF_SMOOTH, 0, 0, { { 0 } }, 0.f, },
    { { 3, 4, 0, }, 1, NULL, 0, 0, 0, 0, BR_MATF_LIGHT, 0, 0, { { 0 } }, 0.f, },
    { { 4, 7, 6, }, 1, NULL, 0, 0, 0, 0, BR_MATF_SMOOTH, 0, 0, { { 0 } }, 0.f, },
    { { 4, 6, 5, }, 1, NULL, 0, 0, 0, 0, BR_MATF_LIGHT, 0, 0, { { 0 } }, 0.f, },
};

br_vertex default_model_vertices[8] = {
    { { { -1.f, -1.f,  1.f }, }, { { .0f,  .99f, }, }, 0, 0, 0, 0, 0, { { 0 } }, },
    { { {  1.f, -1.f,  1.f }, }, { { .99f, .99f, }, }, 0, 0, 0, 0, 0, { { 0 } }, },
    { { {  1.f,  1.f,  1.f }, }, { { .99f, .99f, }, }, 0, 0, 0, 0, 0, { { 0 } }, },
    { { { -1.f,  1.f,  1.f }, }, { { .0f,  .99f, }, }, 0, 0, 0, 0, 0, { { 0 } }, },
    { { { -1.f, -1.f, -1.f }, }, { { .0f,  .0f,  }, }, 0, 0, 0, 0, 0, { { 0 } }, },
    { { {  1.f, -1.f, -1.f }, }, { { .99f, .0f,  }, }, 0, 0, 0, 0, 0, { { 0 } }, },
    { { {  1.f,  1.f, -1.f }, }, { { .99f, .0f,  }, }, 0, 0, 0, 0, 0, { { 0 } }, },
    { { { -1.f,  1.f, -1.f }, }, { { .0f,  .0f,  }, }, 0, 0, 0, 0, 0, { { 0 } }, },
};

br_model _BrDefaultModel = {
    0,
    "default_model",
    default_model_vertices,
    default_model_faces,
    BR_ASIZE(default_model_vertices),
    BR_ASIZE(default_model_faces),
    { { 0.f, 0.f, 0.f,}, },
    BR_MODF_UPDATEABLE,
    NULL,
    NULL,
    0,
    0.f,
    { { { 0 } }, { { 0 } } },
    NULL,
    NULL,
};
