#include "FileReader.hpp"

#include <H5Apublic.h>

#include "NXH5Support/H5Support.hpp"

namespace NX::H5Support
{
FileReader::FileReader(const std::filesystem::path& filepath)
: GroupReader(0, H5Fopen(filepath.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT))
{
}

FileReader::FileReader(IdType fileId)
: GroupReader(0, fileId)
{
}

FileReader::~FileReader()
{
  closeHdf5();
}

void FileReader::closeHdf5()
{
  if(isValid())
  {
    H5Fclose(getId());
    setId(0);
  }
}

std::string FileReader::getName() const
{
  if(!isValid())
  {
    return "";
  }

  constexpr size_t size = 1024;
  char buffer[size];
  H5Fget_name(getId(), buffer, size);

  return GetNameFromBuffer(buffer);
}
} // namespace NX::H5Support
