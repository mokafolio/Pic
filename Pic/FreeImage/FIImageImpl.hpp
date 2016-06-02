#ifndef PIC_FREEIMAGE_FIIMAGEIMPL_HPP
#define PIC_FREEIMAGE_FIIMAGEIMPL_HPP

#include <Stick/FileUtilities.hpp>
#include <Stick/URI.hpp>
#include <Stick/UniquePtr.hpp>
#include <Pic/Image.hpp>

namespace pic
{
    namespace detail
    {
        STICK_LOCAL stick::Result<ImageUniquePtr> decodeImageImpl(const void * _data, stick::Size _byteCount, stick::Allocator & _alloc);

        STICK_LOCAL stick::Result<ImageUniquePtr> loadImageImpl(const stick::URI & _path, stick::Allocator & _alloc);

        STICK_LOCAL stick::Error saveImageImpl(const Image & _img, const stick::URI & _path);
    }
}

#endif //PIC_FREEIMAGE_FIIMAGEIMPL_HPP
