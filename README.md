# Raytracing in C

## Building

Dependencies:

- gcc
- zlib

```bash
gcc src/*.c -O3 -lz -fopenmp -Wall
```

## Development

```bash
gcc src/*.c -O1 -lz -fopenmp -Wall && time ./a.out ./data/cube.glb && eog -f -n out.png
```
