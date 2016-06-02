#include <Pic/Image.hpp>
//for not we only support freeimage so no macros needed
#include <Pic/FreeImage/FIImageImpl.hpp>

namespace pic
{
    using namespace stick;

    Error Image::saveFile(const URI & _uri)
    {
        return detail::saveImageImpl(*this, _uri);
    }
    
    Result<ImageUniquePtr> decodeImage(const ByteArray & _data, Allocator & _alloc)
    {
        return detail::decodeImageImpl(&_data[0], _data.count(), _alloc);
    }

    Result<ImageUniquePtr> loadImage(const URI & _path, Allocator & _alloc)
    {
        return detail::loadImageImpl(_path, _alloc);
    }
}
