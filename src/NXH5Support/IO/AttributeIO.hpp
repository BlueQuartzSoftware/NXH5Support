#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <H5Apublic.h>

#include "NXH5Support/H5.hpp"
#include "NXH5Support/H5Support.hpp"

#include "NXH5Support/NXH5Support_export.hpp"

namespace NX::H5Support
{
class NXH5SUPPORT_EXPORT AttributeIO
{
public:
  using DimsVector = std::vector<SizeType>;

  /**
   * @brief Constructs an invalid AttributeIO.
   */
  AttributeIO();

  /**
   * @brief Constructs an AttributeIO wrapping a target HDF5 attribute
   * belonging to the specified object at the target index.
   * @param objectId
   * @param attrIdx
   */
  AttributeIO(IdType objectId, size_t attrIdx);

  /**
   * @brief Constructs an AttributeIO wrapping a target HDF5 attribute
   * belonging to the specified object with the target name.
   * @param objectId
   * @param attrName
   */
  AttributeIO(IdType objectId, const std::string& attrName);

  /**
   * @brief Releases the wrapped HDF5 attribute.
   */
  virtual ~AttributeIO();

  /**
   * @brief Returns true if the AttributeIO has a valid target.
   * Otherwise, this method returns false.
   * @return bool
   */
  bool isValid() const;

  /**
   * @brief Returns the parent object's ID. Returns 0 if no object was provided.
   * @return H5:IdType
   */
  IdType getObjectId() const;

  /**
   * @brief Returns the attribute's HDF5 ID. Returns 0 if the attribute is
   * invalid.
   * @return IdType
   */
  IdType getAttributeId() const;

  /**
   * @brief Returns the dataspace's HDF5 ID. Returns 0 if the attribute is
   * invalid.
   * @return IdType
   */
  IdType getDataspaceId() const;

  /**
   * @brief Returns the HDF5 attribute name. Returns an empty string if the
   * attribute is invalid.
   * @return std::string
   */
  std::string getName() const;

  /**
   * @brief Returns the HDF5 object name. Returns an empty string if the object
   * is invalid.
   * @return std::string
   */
  std::string getObjectName() const;

  /**
   * @brief Returns an enum representation of the attribute's type.
   * @return Type
   */
  Type getType() const;

  /**
   * @brief Returns an H5T_class_t enum representation of the attribute's class
   * type.
   * @return Type
   */
  IdType getClassType() const;

  /**
   * @brief Returns the HDF5 type ID for the attribute. Returns 0 if the
   * attribute is invalid.
   * @return TypeId
   */
  IdType getTypeId() const;

  /**
   * @brief Returns the number of elements in the attribute.
   * @return size_t
   */
  size_t getNumElements() const;

  /**
   * @brief Returns the value for the attribute.
   * Returns the type's default value if no attribute exists or the attribute
   * is not of the specified type.
   * @tparam T
   * @return T
   */
  template <typename T>
  T readAsValue() const;

  /**
   * @brief Returns a vector of values for the attribute.
   * Returns an empty vector if no attribute exists or the attribute is not of
   * the specified type.
   * @tparam T
   * @return std::vector<T>
   */
  template <typename T>
  std::vector<T> readAsVector() const
  {
    if(!isValid())
    {
      return {};
    }

    std::vector<T> values(getNumElements());
    IdType typeId = getTypeId();

    ErrorType error = H5Aread(getAttributeId(), typeId, values.data());
    if(error != 0)
    {
      std::cout << "Error Reading Attribute." << error << std::endl;
      return {};
    }

    return values;
  }

  /**
   * @brief Returns the value for the attribute as a string. Returns an empty
   * string if the AttributeIO is not valid or the type is not a string.
   * @return std::string
   */
  std::string readAsString() const;

  /**
   * @brief Writes the specified string to the HDF5 attribute.
   * Returns the resulting HDF5 error code if any occurred.
   * @param text
   * @return ErrorType
   */
  ErrorType writeString(const std::string& text);

  /**
   * @brief Writes the specified value to the HDF5 attribute.
   * Returns the resulting HDF5 error code, should an error occur.
   * @tparam T
   * @param value
   * @return ErrorType
   */
  template <typename T>
  ErrorType writeValue(T value);

  /**
   * @brief Writes a vector of values to the HDF5 attribute.
   * @tparam T
   * @param dims HDF5 data dimensions
   * @param vector std::vector of data
   */
  template <typename T>
  ErrorType writeVector(const DimsVector& dims, const std::vector<T>& vector)
  {
    if(!isValid())
    {
      return -1;
    }

    herr_t returnError = 0;
    int32_t rank = static_cast<int32_t>(dims.size());

    hid_t dataType = Support::HdfTypeForPrimitive<T>();
    if(dataType == -1)
    {
      std::cout << "dataType was unknown" << std::endl;
      return -1;
    }

    hid_t dataspaceId = H5Screate_simple(rank, dims.data(), nullptr);
    if(dataspaceId >= 0)
    {
      // Delete any existing attribute
      herr_t error = findAndDeleteAttribute();

      if(error >= 0)
      {
        /* Create the attribute. */
        hid_t attributeId = H5Acreate(getObjectId(), getName().c_str(), dataType, dataspaceId, H5P_DEFAULT, H5P_DEFAULT);
        if(attributeId >= 0)
        {
          /* Write the attribute data. */
          error = H5Awrite(attributeId, dataType, static_cast<const void*>(vector.data()));
          if(error < 0)
          {
            std::cout << "Error Writing Attribute" << std::endl;
            returnError = error;
          }
        }
        /* Close the attribute. */
        error = H5Aclose(attributeId);
        if(error < 0)
        {
          std::cout << "Error Closing Attribute" << std::endl;
          returnError = error;
        }
      }
      /* Close the dataspace. */
      error = H5Sclose(dataspaceId);
      if(error < 0)
      {
        std::cout << "Error Closing Dataspace" << std::endl;
        returnError = error;
      }
    }
    else
    {
      returnError = static_cast<herr_t>(dataspaceId);
    }

    return returnError;
  }

protected:
  /**
   * @brief Closes the HDF5 ID and resets it to 0.
   */
  void closeHdf5();

  /**
   * @brief Finds and deletes any existing attribute with the current name.
   * Returns any error that might occur when deleting the attribute.
   * @return ErrorType
   */
  ErrorType findAndDeleteAttribute();

private:
  IdType m_ObjectId = 0;
  IdType m_AttributeId = 0;
  const std::string m_AttributeName;
};
} // namespace NX::H5Support
