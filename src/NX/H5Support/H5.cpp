#include "H5.hpp"

//#include "H5Support/Utilities/StringUtilities.hpp"

#include <stdexcept>
#include <vector>

#include <H5Apublic.h>

NX::H5Support::Type NX::H5Support::getTypeFromId(IdType typeId)
{
  if(H5Tequal(typeId, H5T_NATIVE_INT8))
  {
    return Type::int8;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_INT16))
  {
    return Type::int16;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_INT32))
  {
    return Type::int32;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_INT64))
  {
    return Type::int64;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_UINT8))
  {
    return Type::uint8;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_UINT16))
  {
    return Type::uint16;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_UINT32))
  {
    return Type::uint32;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_UINT64))
  {
    return Type::uint64;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_FLOAT))
  {
    return Type::float32;
  }
  else if(H5Tequal(typeId, H5T_NATIVE_DOUBLE))
  {
    return Type::float64;
  }

  return Type::unknown;
}

NX::H5Support::IdType NX::H5Support::getIdForType(Type type)
{
  switch(type)
  {
  case Type::int8:
    return H5T_NATIVE_INT8;
  case Type::int16:
    return H5T_NATIVE_INT16;
  case Type::int32:
    return H5T_NATIVE_INT32;
  case Type::int64:
    return H5T_NATIVE_INT64;
  case Type::uint8:
    return H5T_NATIVE_UINT8;
  case Type::uint16:
    return H5T_NATIVE_UINT16;
  case Type::uint32:
    return H5T_NATIVE_UINT32;
  case Type::uint64:
    return H5T_NATIVE_UINT64;
  case Type::float32:
    return H5T_NATIVE_FLOAT;
  case Type::float64:
    return H5T_NATIVE_DOUBLE;
  default:
    throw std::runtime_error("getIdForType does not support this type");
    return -1;
  }
}

std::string NX::H5Support::GetNameFromBuffer(std::string_view buffer)
{
  size_t substrIndex = buffer.find_last_of('/');
  if(substrIndex > 0)
  {
    substrIndex++;
  }
  return std::string(buffer.substr(substrIndex));
}

std::string NX::H5Support::GetPathFromId(IdType id)
{
  ssize_t nameLength = H5Iget_name(id, nullptr, 0);
  if(nameLength <= 0)
  {
    throw std::runtime_error("H5Support::Support::GetName failed: H5Iget_name call failed");
  }
  std::string buffer(nameLength, 'A');
  ssize_t size = H5Iget_name(id, buffer.data(), buffer.size() + 1);
  if(size <= 0)
  {
    throw std::runtime_error("complex::Support::GetName failed: H5Iget_name call failed");
  }
  return buffer;
}

std::string NX::H5Support::GetNameFromId(IdType id)
{
  return GetNameFromBuffer(GetPathFromId(id));
}

std::string NX::H5Support::GetParentPath(const std::string& objectPath)
{
  if(objectPath.empty())
  {
    return "";
  }
  auto back = objectPath.find_last_not_of("/");
  if(back == std::string::npos)
  {
    return "";
  }
  return objectPath.substr(0, ++back);
}
