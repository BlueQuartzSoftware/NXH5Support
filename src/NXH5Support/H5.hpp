#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "NXH5Support/NXH5Support_export.hpp"

namespace NX::H5Support
{
using IdType = int64_t;
using ErrorType = int32_t;
using SizeType = unsigned long long;

enum class Type
{
  int8,
  int16,
  int32,
  int64,
  uint8,
  uint16,
  uint32,
  uint64,
  float32,
  float64,
  string,
  unknown = 255
};

/**
 * @brief Returns a Type enum representing the corresponding type matching the
 * specified HDF5 ID.
 * @param typeId
 * @return Type
 */
Type NXH5SUPPORT_EXPORT getTypeFromId(IdType typeId);

/**
 * @brief Returns the HDF5 type for the specified Type enum.
 * @param type
 * @return IdType
 */
IdType NXH5SUPPORT_EXPORT getIdForType(Type type);

/**
 * @brief Returns a std::string representation from the provided view
 * buffer. If the buffer contains segments divided by '/', only the last
 * segment is returned.
 * @param buffer
 * @return std::string
 */
std::string NXH5SUPPORT_EXPORT GetNameFromBuffer(std::string_view buffer);

/**
 * @brief Returns a std::string of a valid path to the object with the given id.
 * Uses H5Iget_name internally.
 * @param id
 * @return std::string
 */
std::string NXH5SUPPORT_EXPORT GetPathFromId(IdType id);

/**
 * @brief Returns a std::string of the name of the given object.
 * Equivalent to GetNameFromBuffer(GetPathFromId(id)).
 * @param id
 * @return std::string
 */
std::string NXH5SUPPORT_EXPORT GetNameFromId(IdType id);

/**
 * @brief Returns the path to an object's parent
 * @param objectPath The HDF5 path to the object
 * @return  The path to the object's parent
 */
std::string NXH5SUPPORT_EXPORT GetParentPath(const std::string& objectPath);

} // namespace NX::H5Support
