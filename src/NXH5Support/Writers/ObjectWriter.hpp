#pragma once

#include <string>

#include "NXH5Support/H5.hpp"
#include "NXH5Support/Writers/AttributeWriter.hpp"
#include "NXH5Support/NXH5Support_export.hpp"

namespace NX::H5Support
{
class NXH5SUPPORT_EXPORT ObjectWriter
{
public:
  /**
   * @brief Constructs an invalid ObjectWriter.
   */
  ObjectWriter();

  /**
   * @brief Constructs an ObjectWriter that wraps a target HDF5 object
   * belonging to the specified parent. Opening the target HDF5 object is
   * left to the derived class.
   * @param parentId
   * @param objectId
   */
  ObjectWriter(IdType parentId, IdType objectId = 0);

  /**
   * @brief Releases the wrapped HDF5 object.
   */
  virtual ~ObjectWriter();

  /**
   * @brief Returns true if the ObjectWriter has a valid target. Otherwise,
   * this method returns false.
   * @return bool
   */
  virtual bool isValid() const = 0;

  /**
   * @brief Returns the target HDF5 file ID. Returns 0 if the writer is invalid.
   * @return IdType
   */
  IdType getFileId() const;

  /**
   * @brief Returns the parent object ID. Returns 0 if no parent was set.
   * @return IdType
   */
  IdType getParentId() const;

  /**
   * @brief Returns the name of the parent object. Returns an empty string if
   * the writer is invalid.
   * @return std::string
   */
  std::string getParentName() const;

  /**
   * @brief Returns the group's HDF5 ID. Returns 0 if the object is invalid.
   * @return IdType
   */
  IdType getId() const;

  /**
   * @brief Sets the object's HDF5 ID.
   * @return IdType
   */
  void setId(IdType id);

  /**
   * @brief Returns the HDF5 object name. Returns an empty string if the writer
   * is invalid.
   * @return std::string
   */
  virtual std::string getName() const;

  /**
   * @brief Returns the HDF5 object path from the file ID. Returns an empty
   * string if the writer is invalid.
   * @return std::string
   */
  std::string getObjectPath() const;

  /**
   * @brief Returns an AttributeWriter for the target HDF5 attribute. Returns
   * an invalid AttributeWriter if the writer is invalid.
   * @param name
   * @return AttributeWriter
   */
  AttributeWriter createAttribute(const std::string& name);

protected:
  /**
   * @brief Closes the HDF5 ID and resets it to 0.
   */
  virtual void closeHdf5() = 0;

private:
  IdType m_ParentId = 0;
  IdType m_Id = 0;
};
} // namespace NX::H5Support