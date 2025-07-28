use std::{fs::File, io::BufWriter, path::Path};

mod frame_buffer;
use frame_buffer::FrameBuffer;

use crate::renderer::Renderer;

mod renderer;

fn main() {
    let mut frame_buffer = FrameBuffer::new(1920, 1080);

    let renderer = Renderer::new();
    renderer.render(&mut frame_buffer);

    let path = Path::new("./out.png");
    let file = File::create(path).unwrap();
    let w = BufWriter::new(file);
    frame_buffer.write_png(w).unwrap();
}
