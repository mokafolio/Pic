#ifndef PIC_FREEIMAGE_FIIMAGEIMPL_HPP
#define PIC_FREEIMAGE_FIIMAGEIMPL_HPP

#include <Pic/Image.hpp>
#include <Stick/FileUtilities.hpp>
#include <Stick/UniquePtr.hpp>

namespace pic
{
namespace detail
{

STICK_LOCAL stick::Result<ImageUniquePtr> decodeImageImpl(const void * _data,
                                                          stick::Size _byteCount,
                                                          stick::Allocator & _alloc);

STICK_LOCAL stick::Result<ImageUniquePtr> loadImageImpl(const stick::String & _path,
                                                        stick::Allocator & _alloc);

STICK_LOCAL stick::Error saveImageImpl(const Image & _img, const stick::String & _path);

} // namespace detail
} // namespace pic

#endif // PIC_FREEIMAGE_FIIMAGEIMPL_HPP
