#include "math/vector.h"
#include "resources/image.h"

struct SpriteSheet {
    GenHandle image_handle;
    u16 frame_width, frame_height;
    u32 frame_count;
};

struct SpriteAnimation {
    SpriteSheet &sheet;
    u32 current_frame;
    f32 frame_duration;
};
