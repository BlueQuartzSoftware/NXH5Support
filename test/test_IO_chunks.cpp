#include <catch2/catch.hpp>

#include "NX/H5Support/IO/DatasetIO.hpp"
#include "NX/H5Support/IO/FileIO.hpp"
#include "NX/H5Support/TestGenConstants.hpp"

#include "nonstd/span.hpp"
#include <vector>

namespace
{
inline const std::string k_FileName = "test_IO_Chunk.h5";
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
constexpr size_t k_ChunkSize = 5;

inline std::filesystem::path targetFilePath()
{
  return NX::H5Support::constants::TestDataDir / k_FileName;
}
} // namespace

template <typename T>
void createDatasetChunk(NX::H5Support::GroupIO& groupWriter, const std::string& name)
{
  auto datasetWriter = groupWriter.openDataset(name);
  REQUIRE(datasetWriter.isValid());
  // REQUIRE(datasetWriter.open());

  const NX::H5Support::DatasetIO::DimsType dimensions{k_DatasetSize};
  const NX::H5Support::DatasetIO::DimsType chunkShape{k_ChunkSize};
  // datasetWriter.createOrOpenChunkedDataset<T>(dimensions, {k_ChunkSize});

  constexpr size_t count = k_ChunkSize;
  T array1[k_ChunkSize];
  T array2[k_ChunkSize];
  nonstd::span<T> values1(&array1[0], count);
  nonstd::span<T> values2(&array2[0], count);
  for(size_t i = 0; i < count; i++)
  {
    values1[i] = static_cast<T>(i);
    values2[i] = static_cast<T>(i * 2);
  }

  std::vector<hsize_t> offset1{0};
  std::vector<hsize_t> offset2{k_ChunkSize};
  REQUIRE(datasetWriter.writeChunk<T>(dimensions, values1, chunkShape, offset1) == 0);
  REQUIRE(datasetWriter.writeChunk<T>(dimensions, values2, chunkShape, offset2) == 0);
}

template <typename T>
void checkDatasetChunk(const NX::H5Support::GroupIO& groupReader, const std::string& name)
{
  auto datasetReader = groupReader.openDataset(name);
  REQUIRE(datasetReader.open());
  REQUIRE(datasetReader.isValid());

  T vector1[k_ChunkSize];
  T vector2[k_ChunkSize];
  nonstd::span<T> values1(&vector1[0], k_ChunkSize);
  nonstd::span<T> values2(&vector2[0], k_ChunkSize);

  std::vector<hsize_t> offset1Vec{0};
  std::vector<hsize_t> offset2Vec{k_ChunkSize};
  nonstd::span<const hsize_t> offset1(offset1Vec.data(), offset1Vec.size());
  nonstd::span<const hsize_t> offset2(offset2Vec.data(), offset2Vec.size());

  REQUIRE(datasetReader.readChunkIntoSpan<T>(values1, offset1) == true);
  REQUIRE(datasetReader.readChunkIntoSpan<T>(values2, offset2) == true);

  const auto dimensions = datasetReader.getDimensions();
  REQUIRE(dimensions.size() == 1);
  REQUIRE(dimensions[0] == k_DatasetSize);

  const size_t count = values1.size();
  for(size_t i = 0; i < count; i++)
  {
    REQUIRE(values1[i] == static_cast<T>(i));
    REQUIRE(values2[i] == static_cast<T>(i * 2));
  }
}

TEST_CASE("File IO Chunk", "H5Support")
{
  const std::filesystem::path k_FilePath = targetFilePath();
  std::filesystem::remove(k_FilePath);

  {
    NX::H5Support::FileIO fileWriter(k_FilePath);
    REQUIRE(fileWriter.isValid());

    auto groupWriter = fileWriter.createGroup(k_GroupName);
    REQUIRE(groupWriter.isValid());

    createDatasetChunk<int8_t>(groupWriter, k_DatasetInt8Name);
    createDatasetChunk<int16_t>(groupWriter, k_DatasetInt16Name);
    createDatasetChunk<int32_t>(groupWriter, k_DatasetInt32Name);
    createDatasetChunk<int64_t>(groupWriter, k_DatasetInt64Name);
    createDatasetChunk<uint8_t>(groupWriter, k_DatasetUInt8Name);
    createDatasetChunk<uint16_t>(groupWriter, k_DatasetUInt16Name);
    createDatasetChunk<uint32_t>(groupWriter, k_DatasetUInt32Name);
    createDatasetChunk<uint64_t>(groupWriter, k_DatasetUInt64Name);
    createDatasetChunk<float>(groupWriter, k_DatasetFloat32Name);
    createDatasetChunk<double>(groupWriter, k_DatasetFloat64Name);
    createDatasetChunk<bool>(groupWriter, k_DatasetBoolName);
  }

  {
    NX::H5Support::FileIO fileReader(k_FilePath);
    REQUIRE(fileReader.isValid());

    auto groupReader = fileReader.openGroup(k_GroupName);
    REQUIRE(groupReader.isValid());

    checkDatasetChunk<int8_t>(groupReader, k_DatasetInt8Name);
    checkDatasetChunk<int16_t>(groupReader, k_DatasetInt16Name);
    checkDatasetChunk<int32_t>(groupReader, k_DatasetInt32Name);
    checkDatasetChunk<int64_t>(groupReader, k_DatasetInt64Name);
    checkDatasetChunk<uint8_t>(groupReader, k_DatasetUInt8Name);
    checkDatasetChunk<uint16_t>(groupReader, k_DatasetUInt16Name);
    checkDatasetChunk<uint32_t>(groupReader, k_DatasetUInt32Name);
    checkDatasetChunk<uint64_t>(groupReader, k_DatasetUInt64Name);
    checkDatasetChunk<float>(groupReader, k_DatasetFloat32Name);
    checkDatasetChunk<double>(groupReader, k_DatasetFloat64Name);
    checkDatasetChunk<bool>(groupReader, k_DatasetBoolName);
  }
}
