

# Terminal Desmos-like app

## Install
Just compile with `g++ src/main.cpp -o bin/termos`

## Description
This is eventually intended to be a terminal-based alternative to Desmos, the online graphing calculator (hence the name).

So far, it can only draw functions pre-defined at compile-time, mostly because I'm too lazy and stupid to write an expression parser ... and I'm still waiting for a friend to do it for me :)) 

Nonetheless, don't worry, it 's still fun to use :P so far it's defaulted to rendering the mandelbrot set with sub-char rendering using unicode block characters and truecolor. The color drawing has alpha clipping using color-reset, so if you're sporting a fancy transparent terminal, the rendered graphics will keep the transparency outside the rendered graphs.

## Usage

Start using the `termos.sh` script to view in full resolution (running the executable directly will start it on the default resolution)


`+`, `-` to zoom in and out
`h`, `j`, `k`, `l`, to move the view, hold `Shift` to move slower, 

experimental: `asdf` moves in _"parameter-space"_ (this is not perticularily useful when viewing the Mandelbrot set, but support for the Julia sets will be added shortly)
