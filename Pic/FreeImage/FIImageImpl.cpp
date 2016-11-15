#include <Pic/FreeImage/FIImageImpl.hpp>
#include <Pic/Image.hpp>
#include <FreeImage.h>

namespace pic
{
    namespace detail
    {
        using namespace stick;

        Result<ImageUniquePtr> decodeImageImpl(const void * _data, Size _byteCount, Allocator & _alloc)
        {
            typedef Result<ImageUniquePtr> ResultType;
            FIMEMORY * memStream = FreeImage_OpenMemory ((unsigned char *)_data, _byteCount);
            FREE_IMAGE_FORMAT fileType = FreeImage_GetFileTypeFromMemory (memStream, _byteCount);

            if (fileType  !=  FIF_UNKNOWN)
            {
                FIBITMAP * img = FreeImage_LoadFromMemory (fileType, memStream);
                FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType (img);
                FREE_IMAGE_TYPE imageType = FreeImage_GetImageType (img);

                int bpp = FreeImage_GetBPP (img);
                int width = FreeImage_GetWidth (img);
                int height = FreeImage_GetHeight (img);
                int pitch = FreeImage_GetPitch (img);
                int bytePaddingPerRow = pitch - width * (bpp / 8);

                if (imageType  ==  FIT_RGBF)
                {
                    return ResultType(ImageUniquePtr(_alloc.create<ImageRGB32f>(width, height, (float *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                }
                else if (imageType  ==  FIT_RGBAF)
                {
                    return ResultType(ImageUniquePtr(_alloc.create<ImageRGBA32f>(width, height, (float *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                }
                else
                {
                    //grayscale image
                    if (colorType  ==  FIC_MINISBLACK)
                    {
                        if (bpp  ==  8)
                        {
                            return ResultType(ImageUniquePtr(_alloc.create<ImageGray8>(width, height, (unsigned char *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                        }
                        else if (bpp  ==  16)
                        {
                            return ResultType(ImageUniquePtr(_alloc.create<ImageGray16>(width, height, (unsigned short *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                        }
                        else
                        {
                            return Error(ec::Unsupported, "Grayscale images are only supported in 8 or 16 bits per pixel.", STICK_FILE, STICK_LINE);
                        }
                    }
                    else if (colorType  ==  FIC_RGB)
                    {
                        //free image uses BGR, so we do too for speed
                        if (bpp  ==  24)
                        {
                            return ResultType(ImageUniquePtr(_alloc.create<ImageBGR8>(width, height, (unsigned char *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                        }
                        //this is a FreeImage oddity that happens if we call FreeImage_ConvertTo32Bits
                        //as it won't change the color type to FIC_RGBALPHA (this a bug? who knows daawg)
                        else if (bpp  ==  32)
                        {
                            return ResultType(ImageUniquePtr(_alloc.create<ImageBGRA8>(width, height, (unsigned char *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                        }
                        else if (bpp  ==  48)
                        {
                            //48 bit on the other hand are natively stored as rgb in freeimage....
                            return ResultType(ImageUniquePtr(_alloc.create<ImageRGB16>(width, height, (unsigned short *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                        }
                        else
                        {
                            return Error(ec::Unsupported, "RGB images are only supported in 24 or 48 bits per pixel.", STICK_FILE, STICK_LINE);
                        }
                    }
                    else if (colorType  ==  FIC_RGBALPHA)
                    {
                        //free image uses BGR, so we do too for speed
                        if (bpp  ==  32)
                        {
                            return ResultType(ImageUniquePtr(_alloc.create<ImageBGRA8>(width, height, (unsigned char *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                        }
                        else if (bpp  ==  64)
                        {
                            //64 bit on the other hand are natively stored as rgb in freeimage....
                            return ResultType(ImageUniquePtr(_alloc.create<ImageRGBA16>(width, height, (unsigned short *)FreeImage_GetBits (img), bytePaddingPerRow, _alloc)));
                        }
                        else
                        {
                            return Error(ec::Unsupported, "RGBA images are only supported in 32 or 64 bits per pixel.", STICK_FILE, STICK_LINE);
                        }
                    }
                    else
                    {
                        return Error(ec::Unsupported, "The image format is not supported.", STICK_FILE, STICK_LINE);
                    }
                }
            }

            FreeImage_CloseMemory (memStream);
        }

        Result<ImageUniquePtr> loadImageImpl(const URI & _path, Allocator & _alloc)
        {
            auto res = loadBinaryFile(_path, _alloc);
            if (res)
            {
                return decodeImageImpl(&res.get()[0], res.get().count(), _alloc);
            }
            return res.error();
        }

        Error saveImageImpl(const Image & _image, const URI & _path)
        {
            ByteArray data;
            FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(toString(_path).cString());
            if (fif  !=  FIF_UNKNOWN)
            {
                //the channel masks don't seem to do anything in FreeImage_ConvertFromRawBits (bug?), so we convert RGB to BGR
                //for RGB images as that is what freeimage expects
                Image * tmp;
                const Image * imgToSave = &_image;
                if (_image.channelLayoutTypeID() == ChannelLayoutRGB::TypeInfo::typeID() || _image.channelLayoutTypeID() == ChannelLayoutRGBA::TypeInfo::typeID ())
                {
                    tmp = _image . clone ();
                    tmp->swapChannels (0, 2);
                    imgToSave = tmp;
                }

                FIBITMAP * img = FreeImage_ConvertFromRawBits((BYTE *)imgToSave->bytePtr (),
                                 imgToSave->width(), imgToSave->height(),
                                 imgToSave->bytesPerRow(),
                                 imgToSave->bitsPerPixel(),
                                 FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);

                FIMEMORY * mem = FreeImage_OpenMemory();
                bool result = FreeImage_SaveToMemory(fif, img, mem, 0);
                if (result)
                {
                    BYTE * ptr = NULL;
                    DWORD sizeInBytes = 0;
                    FreeImage_AcquireMemory (mem, &ptr, &sizeInBytes);
                    data.resize (sizeInBytes);
                    memcpy(&data[0], ptr, sizeInBytes);
                    return saveBinaryFile(data, _path);
                }
                else
                {
                    return Error(ec::ComposeFailed, "Could not encode image.", STICK_FILE, STICK_LINE);
                }

                FreeImage_CloseMemory (mem);
                FreeImage_Unload (img);
            }
            else
            {
                return Error(ec::Unsupported, "The requested image type is not supported.", STICK_FILE, STICK_LINE);
            }
            return Error();
        }
    }
}
