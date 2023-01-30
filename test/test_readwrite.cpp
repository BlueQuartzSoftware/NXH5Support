#include <catch2/catch.hpp>

#include "NX/H5Support/Readers/DatasetReader.hpp"
#include "NX/H5Support/Readers/FileReader.hpp"
#include "NX/H5Support/TestGenConstants.hpp"
#include "NX/H5Support/Writers/DatasetWriter.hpp"
#include "NX/H5Support/Writers/FileWriter.hpp"

#include "nonstd/span.hpp"
#include <vector>

namespace
{
inline const std::string k_FileName = "test_ReadWrite.h5";
inline const std::string k_GroupName = "Group";
inline const std::string k_DatasetInt8Name = "Int8";
inline const std::string k_DatasetInt16Name = "Int16";
inline const std::string k_DatasetInt32Name = "Int32";
inline const std::string k_DatasetInt64Name = "Int64";
inline const std::string k_DatasetUInt8Name = "UInt8";
inline const std::string k_DatasetUInt16Name = "UInt16";
inline const std::string k_DatasetUInt32Name = "UInt32";
inline const std::string k_DatasetUInt64Name = "UInt64";
inline const std::string k_DatasetBoolName = "Bool";
inline const std::string k_DatasetFloat32Name = "Float32";
inline const std::string k_DatasetFloat64Name = "Float64";
inline const std::string k_DatasetStringName = "String";

constexpr size_t k_DatasetSize = 10;

inline std::filesystem::path targetFilePath()
{
  return NX::H5Support::constants::TestDataDir / k_FileName;
}
} // namespace

template <typename T>
void createDataset(NX::H5Support::GroupWriter& groupWriter, const std::string& name)
{
  auto datasetWriter = groupWriter.createDatasetWriter(name);
  REQUIRE(datasetWriter.isValid());

  constexpr size_t count = k_DatasetSize;
  std::vector<T> vector(count);
  for(size_t i = 0; i < count; i++)
  {
    vector[i] = static_cast<T>(i);
  }

  const NX::H5Support::DatasetWriter::DimsType dimensions{count};

  // nonstd::span<const T> values(vector.data(), count);
  const T* dataPtr = vector.data();
  REQUIRE(datasetWriter.writeSpan(dimensions, nonstd::span<const T>(dataPtr, count)) == 0);
}

template <typename T>
void checkDataset(const NX::H5Support::GroupReader& groupReader, const std::string& name)
{
  auto datasetReader = groupReader.openDataset(name);
  REQUIRE(datasetReader.isValid());

  auto values = datasetReader.readAsVector<T>();
  const size_t count = values.size();
  REQUIRE(count == k_DatasetSize);

  const auto dimensions = datasetReader.getDimensions();
  REQUIRE(dimensions.size() == 1);
  REQUIRE(dimensions[0] == k_DatasetSize);

  for(size_t i = 0; i < count; i++)
  {
    REQUIRE(values[i] == static_cast<T>(i));
  }
}

template <typename T>
void checkDatasetf(const NX::H5Support::GroupReader& groupReader, const std::string& name)
{
  auto datasetReader = groupReader.openDataset(name);
  REQUIRE(datasetReader.isValid());

  auto values = datasetReader.readAsVector<T>();
  const size_t count = values.size();
  REQUIRE(count == k_DatasetSize);
  for(size_t i = 0; i < count; i++)
  {
    const T value = static_cast<T>(i);
    const T storedValue = values[i];
    const T diff = std::abs(storedValue - value);
    REQUIRE(diff <= std::numeric_limits<T>::epsilon());
  }
}

TEST_CASE("File Reader/Writer", "H5Support")
{
  // std::filesystem::remove(k_FilePath);
  const std::filesystem::path k_FilePath = targetFilePath();

  {
    auto fileWriterResult = NX::H5Support::FileWriter::CreateFile(k_FilePath);
    REQUIRE(fileWriterResult.valid());

    NX::H5Support::FileWriter& fileWriter = fileWriterResult.value();
    auto groupWriter = fileWriter.createGroupWriter(k_GroupName);
    REQUIRE(groupWriter.isValid());

    createDataset<int8_t>(groupWriter, k_DatasetInt8Name);
    createDataset<int16_t>(groupWriter, k_DatasetInt16Name);
    createDataset<int32_t>(groupWriter, k_DatasetInt32Name);
    createDataset<int64_t>(groupWriter, k_DatasetInt64Name);
    createDataset<uint8_t>(groupWriter, k_DatasetUInt8Name);
    createDataset<uint16_t>(groupWriter, k_DatasetUInt16Name);
    createDataset<uint32_t>(groupWriter, k_DatasetUInt32Name);
    createDataset<uint64_t>(groupWriter, k_DatasetUInt64Name);
    createDataset<float>(groupWriter, k_DatasetFloat32Name);
    createDataset<double>(groupWriter, k_DatasetFloat64Name);
    // createDataset<bool>(groupWriter, k_DatasetBoolName);
  }

  {
    NX::H5Support::FileReader fileReader(k_FilePath);
    REQUIRE(fileReader.isValid());

    auto groupReader = fileReader.openGroup(k_GroupName);
    REQUIRE(groupReader.isValid());

    checkDataset<int8_t>(groupReader, k_DatasetInt8Name);
    checkDataset<int16_t>(groupReader, k_DatasetInt16Name);
    checkDataset<int32_t>(groupReader, k_DatasetInt32Name);
    checkDataset<int64_t>(groupReader, k_DatasetInt64Name);
    checkDataset<uint8_t>(groupReader, k_DatasetUInt8Name);
    checkDataset<uint16_t>(groupReader, k_DatasetUInt16Name);
    checkDataset<uint32_t>(groupReader, k_DatasetUInt32Name);
    checkDataset<uint64_t>(groupReader, k_DatasetUInt64Name);
    checkDatasetf<float>(groupReader, k_DatasetFloat32Name);
    checkDatasetf<double>(groupReader, k_DatasetFloat64Name);
    // checkDataset<bool>(groupReader, k_DatasetBoolName);
  }
}
