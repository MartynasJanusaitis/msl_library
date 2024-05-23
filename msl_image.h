#ifndef MSL_IMAGE_H
#define MSL_IMAGE_H

#include <stdint.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint32_t msl_ui;
typedef uint16_t msl_us;
typedef uint8_t msl_uc;

enum msl_image_format{
    BMP,
    PNG,
    JPEG,
    GIF,
    NONE
};

typedef struct msl_image
{
    enum msl_image_format format;
    msl_ui img_x, img_y;
    msl_uc bits_per_pixel;
    msl_uc *data, *data_end;
    msl_uc *buffer, *buffer_end;
} msl_image;
static msl_ui msl_read_32_le(msl_uc **buff);
static msl_us msl_read_16_le(msl_uc **buff);
static msl_ui msl_read_32_be(msl_uc **buff);
static msl_us msl_read_16_be(msl_uc **buff);
static msl_uc msl_read_8(msl_uc **buff);


static int msl_image_read_from_file(FILE *f, msl_image *image);
static int msl_image_read(char *filename, msl_image *image);
static int msl_image_parse(msl_image *image);
static void msl_image_free(msl_image *image);

static enum msl_image_format msl_image_check_header(msl_image *image);
static int msl_image_check_header_bmp(msl_image *image);
static int msl_image_check_header_png(msl_image *image);
static int msl_image_check_header_jpeg(msl_image *image);
static int msl_image_check_header_gif(msl_image *image);

static int msl_image_load_bmp(msl_image *image);
static int msl_image_load_png(msl_image *image);

static msl_ui msl_read_32_le(msl_uc **buff)
{
    return msl_read_16_le(buff) | msl_read_16_le(buff) << 16;
}
static msl_us msl_read_16_le(msl_uc **buff)
{
    return msl_read_8(buff) | msl_read_8(buff) << 8;
}
static msl_ui msl_read_32_be(msl_uc **buff)
{
    return msl_read_16_be(buff) << 16 | msl_read_16_be(buff);
}
static msl_us msl_read_16_be(msl_uc **buff)
{
    return msl_read_8(buff) << 8 | msl_read_8(buff);
}
static msl_uc msl_read_8(msl_uc **buff)
{
    msl_uc ret = **(buff);
    (*buff)++;
    return ret;
}


static int msl_image_read(char *filename,  msl_image *image)
{
    FILE *f = fopen(filename, "r");
    if(!f)
    {
        printf("Failed to open file\n");
        return -1;
    }
    if (msl_image_read_from_file(f, image) != 0)
    {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

static int msl_image_read_from_file(FILE *f, msl_image *image)
{
    long filelength;

    fseek(f, 0, SEEK_END);
    filelength = ftell(f);
    rewind(f);

    image->buffer = (msl_uc*) malloc(filelength * sizeof(msl_uc));
    fread(image->buffer, filelength, 1, f);

    image->buffer_end = image->buffer + filelength;
    return msl_image_parse(image);
}

static int msl_image_parse(msl_image *image)
{
    if(image->buffer == NULL) return -1;
    
    switch(msl_image_check_header(image))
    {
        case BMP :
            return msl_image_load_bmp(image);
        case PNG :
            printf("The file is a PNG\n");
            break;
        case JPEG :
            printf("The file is a JPEG\n");
            break;
        case GIF :
            printf("The file is a GIF\n");
            break;
        case NONE :
        default:
            printf("File format unsupported\n");
            return -1;
            break;
    }

    return -1;
}
static void msl_image_free(msl_image *image)
{
    free(image->data);
    free(image->buffer);
}

static enum msl_image_format msl_image_check_header(msl_image *image)
{
    if(msl_image_check_header_bmp(image) == 0) return BMP;
    if(msl_image_check_header_png(image) == 0) return PNG;
    return NONE;
}

// -- BMP --
static int msl_image_check_header_bmp(msl_image *image)
{
    msl_uc *header = image->buffer;
    return (header[0] == 0x42 && header[1] == 0x4D) ? 0 : -1;
}

static int msl_image_load_bmp(msl_image *image)
{
    msl_uc *buff = image->buffer;
    
    // BMP file header
    if(msl_read_8(&buff) != 0x42) return -1; // BMP identifier B
    if(msl_read_8(&buff) != 0x4D) return -1; // BMP identifier M
    if(msl_read_32_le(&buff) == 0) return -1; // BMP image is zero bytes
    msl_read_16_le(&buff); // Reserved bytes
    msl_read_16_le(&buff); // Reserved bytes
    msl_ui offset = msl_read_32_le(&buff); // Pixel data offset

    // Windows BITMAPINFOHEADER
    msl_ui header_size = msl_read_32_le(&buff);
    image->img_x = msl_read_32_le(&buff);
    image->img_y = msl_read_32_le(&buff);
    msl_ui color_planes = msl_read_16_le(&buff);
    if(color_planes != 1) // Number of color planes must be 1 
    {
        printf("BMP file might be corrupted\n");
        printf("The number of color planes was: %x\n", color_planes);
        return -1;
    }     
    image->bits_per_pixel = msl_read_16_le(&buff);
    if(image->bits_per_pixel != 24)
    {
        printf("Only 24 bits per pixel are supported\n");
        printf("Currently bits per pixel: %d\n", image->bits_per_pixel);
        return -1;
    }
    if(msl_read_32_le(&buff) != 0)
    {
        printf("BMP image compression method unsupported\n");
        return -1;
    }
    msl_read_32_le(&buff); // Raw bitmap data size
    msl_read_32_le(&buff); // Horizontal resolution
    msl_read_32_le(&buff); // Vertical resolution
    msl_read_32_le(&buff); // Colors in the color palette
    msl_read_32_le(&buff); // Number of important colors

    
    image->data = (msl_uc*) malloc(image->img_x * image->img_y * 3);
    image->data_end = image->data + (image->img_x * image->img_y * 3);

    buff = image->buffer + offset;
    msl_ui i = 0;
    for(buff = image->buffer + offset; buff < image->buffer_end;)
    {
        image->data[i + 2] = msl_read_8(&buff); // Blue
        image->data[i + 1] = msl_read_8(&buff); // Green
        image->data[i] = msl_read_8(&buff); // Red 
        
        i += 3;
        if(i % 15 == 0) buff++;
    }

    image->format = BMP;

    return 0;
}

// -- PNG --
static int msl_image_check_header_png(msl_image *image)
{
    msl_uc *header = image->buffer;
    msl_uc expected_header[] = {137, 80, 78, 71, 13, 10, 26, 10};
    for(int i = 0; i < 8; i++)
    {
        if(header[i] != expected_header[i]) return -1;
    }
    return 0;
}

#endif
