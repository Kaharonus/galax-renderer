//
// Created by tomas on 20.9.22.
//

#pragma once

#include <spng.h>
#include <vector>


namespace Galax::Assets{
    class PngReader{
    public:
        std::vector<unsigned char> read(char* data, size_t size){
            //Prepare
            spng_ctx *ctx = spng_ctx_new(0);
            //set the data
            spng_set_png_buffer(ctx, data, size);
            //read size
            size_t outSize;
            spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &outSize);
            //read image
            std::vector<unsigned char> result(outSize);
            spng_decode_image(ctx, result.data(), outSize, SPNG_FMT_RGBA8, 0);
            //fuck off
            spng_ctx_free(ctx);
            return result;
        };
    };
}