#pragma once

#include <string>
#include <vector>

#include "NXH5Support/H5.hpp"
#include "NXH5Support/IO/AttributeIO.hpp"

namespace NX::H5Support
{
class NXH5SUPPORT_EXPORT ObjectIO
{
public:
  /**
   * @brief Constructs an invalid ObjectIO.
   */
  ObjectIO();

  /**
   * @brief Constructs an ObjectIO that wraps a target HDF5 object
   * belonging to the specified parent using the target name.
   * @param parentId
   * @param targetName
   */
  ObjectIO(IdType parentId, const std::string& targetName);

  /**
   * @brief Releases the wrapped HDF5 object.
   */
  virtual ~ObjectIO() noexcept;

  /**
   * @brief Returns true if the ObjectIO has a valid target. Otherwise,
   * this method returns false.
   * @return bool
   */
  virtual bool isValid() const;

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
   * @brief Returns the object's address ID. Returns 0 if the object is invalid.
   * @return IdType
   */
  haddr_t getObjectId() const;

  /**
   * @brief Returns the object's HDF5 ID. Returns 0 if the object is invalid.
   * @return IdType
   */
  IdType getId() const;

  /**
   * @brief Sets the object's HDF5 ID.
   * @return IdType
   */
  void setId(IdType id);

  /**
   * @brief Returns the HDF5 object name. Returns an empty string if the file
   * is invalid.
   * @return std::string
   */
  virtual std::string getName() const;

  /**
   * @brief Returns the name of the parent object. Returns an empty string if
   * the writer is invalid.
   * @return std::string
   */
  std::string getParentName() const;

  /**
   * @brief Returns the number of attributes in the object. Returns 0 if the
   * object is not valid.
   * @return size_t
   */
  size_t getNumAttributes() const;

  /**
   * @brief Returns a vector with each attribute name.
   * @return std::vector<std::string>
   */
  std::vector<std::string> getAttributeNames() const;

  /**
   * @brief Returns the HDF5 object path from the file ID. Returns an empty
   * string if the writer is invalid.
   * @return std::string
   */
  std::string getObjectPath() const;

  /**
   * @brief Returns an AttributeIO for the target HDF5 attribute. Returns
   * an invalid AttributeIO if no matching attribute exists.
   * @param name
   * @return AttributeIO
   */
  AttributeIO getAttribute(const std::string& name) const;

  /**
   * @brief Returns an AttributeIO for the target HDF5 attribute. Returns
   * an invalid AttributeIO if no matching attribute exists.
   * @param idx
   * @return AttributeIO
   */
  AttributeIO getAttributeByIdx(size_t idx) const;

protected:
  /**
   * @brief Constructs an ObjectIO for use in derived classes. This
   * constructor only accepts the parent ID. Derived classes are required to
   * open their own target and provide the ID via getId().
   * @param parentId
   */
  ObjectIO(IdType parentId);

  /**
   * @brief Constructs an ObjectIO for use in derived classes. This
   * constructor only accepts the parent ID and the (object) ID. Derived classes
   * are required to open their own target and provide the ID.
   * @param parentId
   * @param objectId
   */
  ObjectIO(IdType parentId, IdType objectId);

  /**
   * @brief Closes the HDF5 ID and resets it to 0.
   */
  virtual void closeHdf5();

private:
  IdType m_ParentId = 0;
  IdType m_Id = 0; // the object, group, file, or dataset id
};
} // namespace NX::H5Support
