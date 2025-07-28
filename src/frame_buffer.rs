use bytemuck::{Pod, Zeroable, cast_slice_mut};
use rayon::prelude::*;
use std::io::Write;

/// Amount of bytes needed to store a single pixel
const PIXEL_SIZE: usize = 4;

#[derive(Copy, Clone, Pod, Zeroable)]
#[repr(C)]
pub struct Pixel {
    pub r: u8,
    pub g: u8,
    pub b: u8,
    pub a: u8,
}

pub struct FrameBuffer {
    width: u32,
    height: u32,
    /// pixels in 8 bit RGBA format, row by row, top left to bottom right
    data: Box<[u8]>,
}

impl FrameBuffer {
    pub fn new(width: u32, height: u32) -> Self {
        let size = width as usize * height as usize * PIXEL_SIZE;
        Self {
            width,
            height,
            data: vec![0; size].into_boxed_slice(),
        }
    }

    pub fn width(&self) -> u32 {
        self.width
    }

    pub fn height(&self) -> u32 {
        self.height
    }

    pub fn update_pixels(
        &mut self,
        f: fn(width: u32, height: u32, x: u32, y: u32, pixel: &mut Pixel),
    ) {
        let pixels: &mut [Pixel] = cast_slice_mut(&mut self.data);

        pixels.par_iter_mut().enumerate().for_each(|(i, pixel)| {
            let x = i as u32 % self.width;
            let y = i as u32 / self.width;
            f(self.width, self.height, x, y, pixel);
        });
    }

    pub fn write_png(&self, w: impl Write) -> Result<(), png::EncodingError> {
        let mut encoder = png::Encoder::new(w, self.width, self.height);
        encoder.set_color(png::ColorType::Rgba);

        let mut writer = encoder.write_header()?;
        writer.write_image_data(&self.data)?;

        Ok(())
    }
}
