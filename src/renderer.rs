use crate::frame_buffer::FrameBuffer;

pub struct Renderer {}

impl Renderer {
    pub fn new() -> Self {
        Self {}
    }

    pub fn render(&self, frame_buffer: &mut FrameBuffer) {
        frame_buffer.update_pixels(|width, height, x, y, pixel| {
            pixel.r = x as u8;
            pixel.g = y as u8;
            pixel.a = 255;
        });
    }
}
