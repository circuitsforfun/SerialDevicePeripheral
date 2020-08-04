/*
 *  Mixed Data Type Class:
 *      Used for creating dynamic data structures containing mixed data types.
 *      Subclass this for easy storage, serializing and deserializing when sending
 *      data between devices over serial or network.
 */

#ifndef MIXED_DATA_TYPE_HPP_
#define MIXED_DATA_TYPE_HPP_

#include <string>
#include <vector>

namespace rw {
  namespace mdt {
      typedef enum kDataTypes {
          DATA_TYPE_UNDEFINED,
          DATA_TYPE_INT8,
          DATA_TYPE_UINT8,
          DATA_TYPE_INT16,
          DATA_TYPE_UINT16,
          DATA_TYPE_INT32,
          DATA_TYPE_UINT32,
          DATA_TYPE_FLOAT,
          DATA_TYPE_DOUBLE,
          DATA_TYPE_STRING
      } kDataTypes;

      typedef struct dataStruct {
          uint8_t data_type{};
          std::vector<uint8_t> data;
          std::string key;
      } dataStruct;

      class MixedDataType {
      public:
          MixedDataType() = default;
          virtual ~MixedDataType() = default;

          void add(const std::string &key, int8_t value)
          {
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_INT8)
                  {
                      data_.at(ch).data.at(0) = (uint8_t) value;
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_INT8;
              d.data.push_back((uint8_t)value);
              data_.push_back(d);
          }

          void add(const std::string &key, uint8_t value)
          {
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_UINT8)
                  {
                      data_.at(ch).data.at(0) = value;
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_UINT8;
              d.data.push_back(value);
              data_.push_back(d);

          }

          void add(const std::string &key, int16_t value)
          {
              auto const* p = reinterpret_cast<uint8_t const*>(&value);
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_INT16)
                  {
                      for (std::size_t i = 0; i != sizeof(int16_t); ++i)
                          data_.at(ch).data.at(i) = p[i];
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_INT16;
              for (std::size_t i = 0; i != sizeof(int16_t); ++i)
                  d.data.push_back(p[i]);
              data_.push_back(d);
          }

          void add(const std::string &key, uint16_t value)
          {
              auto const* p = reinterpret_cast<uint8_t const*>(&value);
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_UINT16)
                  {
                      for (std::size_t i = 0; i != sizeof(uint16_t); ++i)
                          data_.at(ch).data.at(i) = p[i];
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_UINT16;
              for (std::size_t i = 0; i != sizeof(uint16_t); ++i)
                  d.data.push_back(p[i]);
              data_.push_back(d);
          }

          void add(const std::string &key, int32_t value)
          {
              auto const* p = reinterpret_cast<uint8_t const*>(&value);
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_INT32)
                  {
                      for (std::size_t i = 0; i != sizeof(int32_t); ++i)
                          data_.at(ch).data.at(i) = p[i];
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_INT32;
              for (std::size_t i = 0; i != sizeof(int32_t); ++i)
                  d.data.push_back(p[i]);
              data_.push_back(d);
          }

          void add(const std::string &key, uint32_t value)
          {
              auto const* p = reinterpret_cast<uint8_t const*>(&value);
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_UINT32)
                  {
                      for (std::size_t i = 0; i != sizeof(uint32_t); ++i)
                          data_.at(ch).data.at(i) = p[i];
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_UINT32;
              for (std::size_t i = 0; i != sizeof(uint32_t); ++i)
                  d.data.push_back(p[i]);
              data_.push_back(d);
          }

          void add(const std::string &key, float value)
          {
              auto const* p = reinterpret_cast<uint8_t const*>(&value);
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_FLOAT)
                  {
                      for (std::size_t i = 0; i != sizeof(float); ++i)
                          data_.at(ch).data.at(i) = p[i];
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_FLOAT;
              for (std::size_t i = 0; i != sizeof(float); ++i)
                  d.data.push_back(p[i]);
              data_.push_back(d);
          }

          void add(const std::string &key, double value)
          {
              auto const* p = reinterpret_cast<uint8_t const*>(&value);
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_DOUBLE)
                  {
                      for (std::size_t i = 0; i != sizeof(double); ++i)
                          data_.at(ch).data.at(i) = p[i];
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_DOUBLE;
              for (std::size_t i = 0; i != sizeof(double); ++i)
                  d.data.push_back(p[i]);
              data_.push_back(d);
          }

          void add(const std::string &key, std::string value)
          {
              int ch = findIndex(key);
              if (ch != -1)
              {
                  if (data_.at(ch).data_type == DATA_TYPE_STRING)
                  {
                      data_.at(ch).data.clear();
                      for (auto c : value)
                          data_.at(ch).data.push_back((uint8_t)c);
                      return;
                  }
                  else
                      data_.erase(data_.begin() + ch);
              }
              dataStruct d;
              d.key = key;
              d.data_type = DATA_TYPE_STRING;
              for (auto c : value)
                  d.data.push_back((uint8_t)c);
              data_.push_back(d);
          }

          int findIndex(const std::string &key)
          {
              int index = 0;
              for (auto d : data_)
              {
                  if (d.key == key)
                      return index;
                  index++;
              }
              return -1;
          }

          void clearData()
          {
              data_.clear();
          }

          template <typename T>
          T get(const std::string &key)
          {
              T data;
              int i = findIndex(key);
              if (i != -1)
              {
                  data_read_ = true;
                  if (data_.at(i).data_type != DATA_TYPE_STRING)
                  {
                      auto* p = reinterpret_cast<uint8_t*>(&data);
                      if (sizeof(T) == data_.at(i).data.size())
                      {
                          for (std::size_t j = 0; j != sizeof(T); ++j)
                          {
                              p[j] = data_.at(i).data.at(j);
                          }
                      }
                      return data;
                  }
                  else
                  {
                      for (auto c : data_.at(i).data)
                      {
                          data += (char)c;
                      }
                      return data;
                  }
              }
              return data;
          }

      protected:
          std::vector<dataStruct> data_;
          bool data_read_{};
          int serialize(std::vector<uint8_t>& buffer)
          {
              int bSize = 0;
              for (const auto& d : data_)
              {
                  buffer.push_back((uint8_t)d.data_type);
                  bSize++;
                  buffer.push_back((uint8_t)d.key.size());
                  bSize++;
                  for (auto c : d.key)
                  {
                      buffer.push_back((uint8_t)c);
                      bSize++;
                  }
                  if (d.data_type != DATA_TYPE_STRING)
                  {
                      for (auto p : d.data)
                      {
                          buffer.push_back(p);
                          bSize++;
                      }
                  }
                  else
                  {
                      buffer.push_back((uint8_t)d.data.size());
                      bSize++;
                      for (auto p : d.data)
                      {
                          buffer.push_back(p);
                          bSize++;
                      }
                  }
              }
              return bSize;
          }

          void deserialize(const std::vector<uint8_t>& raw)
          {
              data_.clear();
              for (int i = 0; i < raw.size(); i++)
              {
                  dataStruct d;
                  d.data_type = raw.at(i);
                  i++;
                  int keySize = raw.at(i);
                  for (int j = 0; j < keySize; j++)
                  {
                      i++;
                      d.key += (char)raw.at(i);
                  }
                  int dSize = 1;
                  if (d.data_type == DATA_TYPE_INT8)
                      dSize = 1;
                  else if (d.data_type == DATA_TYPE_UINT8)
                      dSize = 1;
                  else if (d.data_type == DATA_TYPE_INT16)
                      dSize = 2;
                  else if (d.data_type == DATA_TYPE_UINT16)
                      dSize = 2;
                  else if (d.data_type == DATA_TYPE_INT32)
                      dSize = 4;
                  else if (d.data_type == DATA_TYPE_UINT32)
                      dSize = 4;
                  else if (d.data_type == DATA_TYPE_FLOAT)
                      dSize = 4;
                  else if (d.data_type == DATA_TYPE_DOUBLE)
                      dSize = 8;
                  else if (d.data_type == DATA_TYPE_STRING)
                  {
                      i++;
                      dSize = raw.at(i);
                  }
                  for (int j = 0; j < dSize; j++)
                  {
                      i++;
                      d.data.push_back(raw.at(i));
                  }
                  data_.push_back(d);
              }
          }
      }; // End MixedDataType Class
  } // End mdt namespace
} // End rw namespace
#endif // MIXED_DATA_TYPE_HPP_


