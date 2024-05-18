#ifndef MSL_IMAGE_H
#define MSL_IMAGE_H

#include <stdlib.h>
#include <stdio.h>

typedef unsigned int msl_ui;
typedef unsigned short msl_us;
typedef unsigned char msl_uc;

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
    msl_uc *data, *data_end;
    msl_uc *buffer, *buffer_end;
} msl_image;

static msl_ui msl_read_32_le(msl_uc *buff, size_t pos);
static msl_us msl_read_16_le(msl_uc *buff, size_t pos);
static msl_ui msl_read_32_be(msl_uc *buff, size_t pos);
static msl_us msl_read_16_be(msl_uc *buff, size_t pos);
static msl_uc msl_read_8(msl_uc *buff, size_t pos);

static int msl_image_read_from_file(FILE *f, msl_image *image);
static int msl_image_read(char *filename, msl_image *image);
static int msl_image_parse(msl_image *image);

static enum msl_image_format msl_image_check_header(msl_image *image);
static int msl_image_check_header_bmp(msl_image *image);
static int msl_image_check_header_png(msl_image *image);
static int msl_image_check_header_jpeg(msl_image *image);
static int msl_image_check_header_gif(msl_image *image);

static int msl_image_load_bmp(msl_image *image);
static int msl_image_load_png(msl_image *image);

static msl_ui msl_read_32_le(msl_uc *buff, size_t pos)
{
    return msl_read_16_le(buff, pos + 2) << 16 | msl_read_16_le(buff, pos);
}
static msl_us msl_read_16_le(msl_uc *buff, size_t pos)
{
    return msl_read_8(buff, pos + 1) << 8 | msl_read_8(buff, pos);
}
static msl_ui msl_read_32_be(msl_uc *buff, size_t pos)
{
    return msl_read_16_be(buff, pos) << 16 | msl_read_16_be(buff, pos + 2);
}
static msl_us msl_read_16_be(msl_uc *buff, size_t pos)
{
    return msl_read_8(buff, pos) << 8 | msl_read_8(buff, pos + 1);
}
static msl_uc msl_read_8(msl_uc *buff, size_t pos)
{
    return *(buff + pos);
}


static int msl_image_read(char *filename,  msl_image *image)
{
    FILE *f = fopen(filename, "r");
    if(!f)
    {
        printf("Failed to open file\n");
        return -1;
    }
    msl_image_read_from_file(f, image);
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
            printf("The file is a BMP\n");
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
    if(msl_image_check_header_bmp(image) != 0) return -1;
    msl_uc *buff = image->buffer;
    msl_ui offset = msl_read_32_le(buff, 10);

    image->data = image->buffer + offset;
    image->data_end = image->buffer_end;

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
