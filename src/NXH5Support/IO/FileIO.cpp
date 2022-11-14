#include "FileIO.hpp"

#include <fmt/format.h>

#include <H5Apublic.h>

#include "NXH5Support/H5Support.hpp"

using namespace NX::Common;

namespace NX::H5Support
{
Result<FileIO> FileIO::CreateFile(const std::filesystem::path& filepath)
{
  Result<FileIO> result;
  if(std::filesystem::exists(filepath))
  {
    FileIO file(filepath);
    if(!file.isValid())
    {
      return MakeErrorResult<FileIO>(-303, fmt::format("Error opening HDF5 file at path '{}'.", filepath.string()));
    }
  }

  auto parentPath = filepath.parent_path();
  if(!std::filesystem::exists(parentPath))
  {
    if(!std::filesystem::create_directories(parentPath))
    {
      return MakeErrorResult<FileIO>(-300, fmt::format("Error creating HDF5 file at path '{}'. "
                                                       "Parent path could not be created.",
                                                       filepath.string()));
    }
  }

  try
  {
    FileIO file(filepath);
    if(!file.isValid())
    {
      return MakeErrorResult<FileIO>(-301, fmt::format("Error creating HDF5 file at path '{}'. "
                                                       "Parent path could not be created.",
                                                       filepath.string()));
    }
    return Result<FileIO>{std::move(file)};
  } catch(const std::runtime_error& error)
  {
    return MakeErrorResult<FileIO>(-302, fmt::format("Error creating HDF5 file at path '{}'. "
                                                     "Parent path could not be created.",
                                                     filepath.string()));
  }
  return {}; // Code should not get here. We return everywhere else.
}

Result<FileIO> FileIO::WrapHdf5FileId(IdType fileId)
{
  if(fileId <= 0)
  {
    return MakeErrorResult<FileIO>(-302, fmt::format("Error wrapping existing HDF5 FileId with value '{}'.", fileId));
  }
  return {FileIO(fileId)};
}

hid_t createOrOpenFile(const std::filesystem::path& filepath)
{
  if(std::filesystem::exists(filepath))
  {
    return H5Fopen(filepath.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  }
  return H5Fcreate(filepath.string().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
}

FileIO::FileIO()
: GroupIO()
{
}

FileIO::FileIO(const std::filesystem::path& filepath)
: GroupIO(0, createOrOpenFile(filepath))
{
}

FileIO::FileIO(IdType fileId)
: GroupIO(0, fileId)
{
}

FileIO::FileIO(FileIO&& rhs) noexcept
: GroupIO()
{
  auto rhsId = rhs.getId();
  setId(rhsId);
  rhs.setId(-1);
}

FileIO::~FileIO()
{
  closeHdf5();
}

void FileIO::closeHdf5()
{
  if(isValid())
  {
    H5Fclose(getId());
    setId(0);
  }
}

std::string FileIO::getName() const
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
