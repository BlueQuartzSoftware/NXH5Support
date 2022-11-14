#include "ObjectIO.hpp"

#include <H5Apublic.h>

#include "NXH5Support/H5Support.hpp"

namespace NX::H5Support
{
ObjectIO::ObjectIO() = default;

ObjectIO::ObjectIO(IdType parentId)
: m_ParentId(parentId)
{
}

ObjectIO::ObjectIO(IdType parentId, IdType objectId)
: m_ParentId(parentId)
, m_Id(objectId)
{
}

ObjectIO::ObjectIO(IdType parentId, const std::string& targetName)
: m_ParentId(parentId)
{
  m_Id = H5Oopen(parentId, targetName.c_str(), H5P_DEFAULT);
}

ObjectIO::~ObjectIO() noexcept
{
  closeHdf5();
}

void ObjectIO::closeHdf5()
{
  if(isValid())
  {
    H5Oclose(m_Id);
    m_Id = 0;
  }
}

bool ObjectIO::isValid() const
{
  return getId() > 0;
}

IdType ObjectIO::getFileId() const
{
  if(!isValid())
  {
    return 0;
  }

  return H5Iget_file_id(getParentId());
}

IdType ObjectIO::getParentId() const
{
  return m_ParentId;
}

haddr_t ObjectIO::getObjectId() const
{
  if(!isValid())
  {
    return 0;
  }

  H5O_info1_t info;
  H5Oget_info(m_Id, &info);
  return info.addr;
}

IdType ObjectIO::getId() const
{
  return m_Id;
}

void ObjectIO::setId(IdType id)
{
  m_Id = id;
}

std::string ObjectIO::getName() const
{
  if(!isValid())
  {
    return "";
  }

  std::string path = GetNameFromId(getId());
  return path;
}

std::string ObjectIO::getParentName() const
{
  if(!isValid())
  {
    return "";
  }

  std::string path = GetNameFromId(getParentId());

  return path;
}

std::string ObjectIO::getObjectPath() const
{
  if(!isValid())
  {
    return "";
  }

  return Support::GetObjectPath(getId());
}

size_t ObjectIO::getNumAttributes() const
{
  if(!isValid())
  {
    return 0;
  }

  return H5Aget_num_attrs(getId());
}

std::vector<std::string> ObjectIO::getAttributeNames() const
{
  auto numAttributes = getNumAttributes();
  std::vector<std::string> attributeNames(numAttributes);
  for(size_t i = 0; i < numAttributes; i++)
  {
    attributeNames[i] = getAttributeByIdx(i).getName();
  }

  return attributeNames;
}

AttributeIO ObjectIO::getAttribute(const std::string& name) const
{
  if(!isValid())
  {
    return AttributeIO();
  }

  return AttributeIO(getId(), name);
}

AttributeIO ObjectIO::getAttributeByIdx(size_t idx) const
{
  if(!isValid())
  {
    return AttributeIO();
  }

  return AttributeIO(getId(), idx);
}
} // namespace NX::H5Support
