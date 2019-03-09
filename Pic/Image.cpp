#include <Pic/Image.hpp>
#ifdef PIC_IMPLEMENTATION_FREEIMAGE
#include <FreeImage.h>
#include <Stick/FileUtilities.hpp>
#elif defined(PIC_IMPLEMENTATION_STB)
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Stick/Path.hpp>
#include <stb/stb_image_write.h>
#else
#error "No implementation specified"
#endif // PIC_IMPLEMENTATION_FREEIMAGE

namespace pic
{

using namespace stick;

Result<ImageUniquePtr> decodeImage(const ByteArray & _data, Allocator & _alloc)
{
    return decodeImage(&_data[0], _data.count(), _alloc);
}

#ifdef PIC_IMPLEMENTATION_FREEIMAGE
Result<ImageUniquePtr> decodeImage(const void * _data, Size _byteCount, Allocator & _alloc)
{
    typedef Result<ImageUniquePtr> ResultType;
    FIMEMORY * memStream = FreeImage_OpenMemory((unsigned char *)_data, _byteCount);
    FREE_IMAGE_FORMAT fileType = FreeImage_GetFileTypeFromMemory(memStream, _byteCount);

    if (fileType != FIF_UNKNOWN)
    {
        FIBITMAP * img = FreeImage_LoadFromMemory(fileType, memStream);
        FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(img);
        FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(img);

        int bpp = FreeImage_GetBPP(img);
        int width = FreeImage_GetWidth(img);
        int height = FreeImage_GetHeight(img);
        int pitch = FreeImage_GetPitch(img);
        int bytePaddingPerRow = pitch - width * (bpp / 8);

        if (imageType == FIT_RGBF)
        {
            return ResultType(ImageUniquePtr(
                _alloc.create<ImageRGB32f>(
                    width, height, (float *)FreeImage_GetBits(img), bytePaddingPerRow, _alloc),
                _alloc));
        }
        else if (imageType == FIT_RGBAF)
        {
            return ResultType(ImageUniquePtr(
                _alloc.create<ImageRGBA32f>(
                    width, height, (float *)FreeImage_GetBits(img), bytePaddingPerRow, _alloc),
                _alloc));
        }
        else
        {
            // grayscale image
            if (colorType == FIC_MINISBLACK)
            {
                if (bpp == 8)
                {
                    return ResultType(ImageUniquePtr(
                        _alloc.create<ImageGray8>(width,
                                                  height,
                                                  (unsigned char *)FreeImage_GetBits(img),
                                                  bytePaddingPerRow,
                                                  _alloc),
                        _alloc));
                }
                else if (bpp == 16)
                {
                    return ResultType(ImageUniquePtr(
                        _alloc.create<ImageGray16>(width,
                                                   height,
                                                   (unsigned short *)FreeImage_GetBits(img),
                                                   bytePaddingPerRow,
                                                   _alloc),
                        _alloc));
                }
                else
                {
                    return Error(ec::Unsupported,
                                 "Grayscale images are only supported in 8 or 16 bits per pixel.",
                                 STICK_FILE,
                                 STICK_LINE);
                }
            }
            else if (colorType == FIC_RGB)
            {
                // free image uses BGR, so we do too for speed
                if (bpp == 24)
                {
                    return ResultType(ImageUniquePtr(
                        _alloc.create<ImageBGR8>(width,
                                                 height,
                                                 (unsigned char *)FreeImage_GetBits(img),
                                                 bytePaddingPerRow,
                                                 _alloc),
                        _alloc));
                }
                // this is a FreeImage oddity that happens if we call FreeImage_ConvertTo32Bits
                // as it won't change the color type to FIC_RGBALPHA (this a bug? who knows daawg)
                else if (bpp == 32)
                {
                    return ResultType(ImageUniquePtr(
                        _alloc.create<ImageBGRA8>(width,
                                                  height,
                                                  (unsigned char *)FreeImage_GetBits(img),
                                                  bytePaddingPerRow,
                                                  _alloc),
                        _alloc));
                }
                else if (bpp == 48)
                {
                    // 48 bit on the other hand are natively stored as rgb in freeimage....
                    return ResultType(ImageUniquePtr(
                        _alloc.create<ImageRGB16>(width,
                                                  height,
                                                  (unsigned short *)FreeImage_GetBits(img),
                                                  bytePaddingPerRow,
                                                  _alloc),
                        _alloc));
                }
                else
                {
                    return Error(ec::Unsupported,
                                 "RGB images are only supported in 24 or 48 bits per pixel.",
                                 STICK_FILE,
                                 STICK_LINE);
                }
            }
            else if (colorType == FIC_RGBALPHA)
            {
                // free image uses BGR, so we do too for speed
                if (bpp == 32)
                {
                    return ResultType(ImageUniquePtr(
                        _alloc.create<ImageBGRA8>(width,
                                                  height,
                                                  (unsigned char *)FreeImage_GetBits(img),
                                                  bytePaddingPerRow,
                                                  _alloc),
                        _alloc));
                }
                else if (bpp == 64)
                {
                    // 64 bit on the other hand are natively stored as rgb in freeimage....
                    return ResultType(ImageUniquePtr(
                        _alloc.create<ImageRGBA16>(width,
                                                   height,
                                                   (unsigned short *)FreeImage_GetBits(img),
                                                   bytePaddingPerRow,
                                                   _alloc),
                        _alloc));
                }
                else
                {
                    return Error(ec::Unsupported,
                                 "RGBA images are only supported in 32 or 64 bits per pixel.",
                                 STICK_FILE,
                                 STICK_LINE);
                }
            }
        }
    }

    FreeImage_CloseMemory(memStream);
    return Error(ec::Unsupported, "The image format is not supported.", STICK_FILE, STICK_LINE);
}

Result<ImageUniquePtr> loadImage(const String & _path, Allocator & _alloc)
{
    auto res = loadBinaryFile(_path, _alloc);
    if (res)
    {
        return decodeImage(&res.get()[0], res.get().count(), _alloc);
    }
    return res.error();
}

Error Image::save(const String & _path, const SaveSettings & _settings)
{
    ByteArray data;
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(toString(_path).cString());
    if (fif != FIF_UNKNOWN)
    {
        // the channel masks don't seem to do anything in FreeImage_ConvertFromRawBits (bug?), so we
        // convert RGB to BGR for RGB images as that is what freeimage expects
        Image * tmp;
        const Image * imgToSave = this;
        if (channelLayoutTypeID() == ChannelLayoutRGB::TypeInfo::typeID() ||
            channelLayoutTypeID() == ChannelLayoutRGBA::TypeInfo::typeID())
        {
            tmp = clone();
            tmp->swapChannels(0, 2);
            imgToSave = tmp;
        }

        FIBITMAP * img = FreeImage_ConvertFromRawBits((BYTE *)imgToSave->bytePtr(),
                                                      imgToSave->width(),
                                                      imgToSave->height(),
                                                      imgToSave->bytesPerRow(),
                                                      imgToSave->bitsPerPixel(),
                                                      FI_RGBA_RED_MASK,
                                                      FI_RGBA_GREEN_MASK,
                                                      FI_RGBA_BLUE_MASK,
                                                      false);

        FIMEMORY * mem = FreeImage_OpenMemory();
        bool result = FreeImage_SaveToMemory(fif, img, mem, 0);
        if (result)
        {
            BYTE * ptr = NULL;
            DWORD sizeInBytes = 0;
            FreeImage_AcquireMemory(mem, &ptr, &sizeInBytes);
            data.resize(sizeInBytes);
            memcpy(&data[0], ptr, sizeInBytes);
            return saveBinaryFile(data, _path);
        }
        else
        {
            return Error(ec::ComposeFailed, "Could not encode image.", STICK_FILE, STICK_LINE);
        }

        FreeImage_CloseMemory(mem);
        FreeImage_Unload(img);
    }
    else
    {
        return Error(
            ec::Unsupported, "The requested image type is not supported.", STICK_FILE, STICK_LINE);
    }
    return Error();
}
#elif defined(PIC_IMPLEMENTATION_STB)

Result<ImageUniquePtr> createImageFromSTBData(
    UInt8 * _data, int _w, int _h, int _channelCount, Allocator & _alloc)
{
    //@NOTE: stb image does not provide a way to load into a preallocated buffer right now. Not
    // optimal as we need to do one final copy, but should be fine in most cases.
    ImageUniquePtr ret;
    Error err;
    if (_channelCount == 1)
        ret = makeUnique<ImageGray8>(_alloc, (Size)_w, (Size)_h, _data, (Size)0, _alloc);
    else if (_channelCount == 2)
        ret = makeUnique<ImageGrayAlpha8>(_alloc, (Size)_w, (Size)_h, _data, (Size)0, _alloc);
    else if (_channelCount == 3)
        ret = makeUnique<ImageRGB8>(_alloc, (Size)_w, (Size)_h, _data, (Size)0, _alloc);
    else if (_channelCount == 4)
        ret = makeUnique<ImageRGBA8>(_alloc, (Size)_w, (Size)_h, _data, (Size)0, _alloc);
    else
        err = Error(ec::InvalidOperation, "Unsupported channel count", STICK_FILE, STICK_LINE);

    stbi_image_free(_data);
    if (err)
        return err;
    return ret;
}

Result<ImageUniquePtr> decodeImage(const void * _data, Size _byteCount, Allocator & _alloc)
{
    //@TODO: allow loading of HDR/float images using the corresponding stb api
    int w, h, n;
    auto * data = stbi_load_from_memory((const stbi_uc *)_data, _byteCount, &w, &h, &n, 0);
    if (!data)
    {
        return Error(ec::InvalidOperation,
                     "Could not parse image from compressed data",
                     STICK_FILE,
                     STICK_LINE);
    }
    return createImageFromSTBData(data, w, h, n, _alloc);
}

Result<ImageUniquePtr> loadImage(const String & _path, Allocator & _alloc)
{
    int w, h, n;
    auto * data = stbi_load(_path.cString(), &w, &h, &n, 0);
    if (!data)
    {
        return Error(ec::InvalidOperation,
                     String::formatted("Could not parse image from file at %s", _path.cString()),
                     STICK_FILE,
                     STICK_LINE);
    }
    return createImageFromSTBData(data, w, h, n, _alloc);
}

Error Image::save(const String & _path, const SaveSettings & _settings)
{
    //@TODO simply use a strncmp based approach so we dont need to allocate the extension... this is
    // nice and easy for now dough
    String ext = path::extension(_path, allocator());
    if (ext == ".png")
    {
        int success = stbi_write_png(
            _path.cString(), width(), height(), channelCount(), bytePtr(), bytesPerRow());
        if (!success)
            return Error(ec::InvalidOperation, "stbi_write_png failed", STICK_FILE, STICK_LINE);
    }
    else
    {
        if (rowPadding() != 0)
            return Error(ec::InvalidOperation,
                         "Stb only supports row padding for writing png files",
                         STICK_FILE,
                         STICK_LINE);

        if (ext == ".jpg")
        {
            //@TODO: put jpg quality in SaveSettings and use it
            int success =
                stbi_write_jpg(_path.cString(), width(), height(), channelCount(), bytePtr(), 90);
            if (!success)
                return Error(ec::InvalidOperation, "stbi_write_jpg failed", STICK_FILE, STICK_LINE);
        }
        else if (ext == ".bmp")
        {
            int success =
                stbi_write_bmp(_path.cString(), width(), height(), channelCount(), bytePtr());
            if (!success)
                return Error(ec::InvalidOperation, "stbi_write_bmp failed", STICK_FILE, STICK_LINE);
        }
        else if (ext == ".tga")
        {
            int success =
                stbi_write_tga(_path.cString(), width(), height(), channelCount(), bytePtr());
            if (!success)
                return Error(ec::InvalidOperation, "stbi_write_tga failed", STICK_FILE, STICK_LINE);
        }
    }
    return Error();
}
#endif // PIC_IMPLEMENTATION_FREEIMAGE

} // namespace pic
