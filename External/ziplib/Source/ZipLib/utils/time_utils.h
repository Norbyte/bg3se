#pragma once
#include <cstdint>
#include <ctime>

#if defined(_MSC_VER)
# define _utils_stream_localtime(dt, ts)  do { localtime_s((ts), (dt)); } while (0)
#elif defined(__GNUC__) || defined(__GNUG__)
# define _utils_stream_localtime(dt, ts)  do { localtime_r((dt), (ts)); } while (0)
#else
# define _utils_stream_localtime(dt, ts)  do { tm* _tmp = localtime(dt); memcpy((ts), _tmp, sizeof(tm)); } while (0)
#endif

namespace utils { namespace time {

void timestamp_to_datetime(time_t dateTime, uint16_t& date, uint16_t& time)
{
  tm timeStruct;

  _utils_stream_localtime(&dateTime, &timeStruct);

  date = ((timeStruct.tm_year - 80) << 9) + ((timeStruct.tm_mon + 1) << 5) + timeStruct.tm_mday;
  time = (timeStruct.tm_hour << 11) + (timeStruct.tm_min << 5) + (timeStruct.tm_sec >> 1);
}

time_t datetime_to_timestamp(uint16_t date, uint16_t time)
{
  tm timeStruct;

  timeStruct.tm_year = ((date >> 9) & 0x7f) + 80;
  timeStruct.tm_mon = ((date >> 5) & 0x0f) - 1;
  timeStruct.tm_mday = ((date) & 0x1f);

  timeStruct.tm_hour = ((time >> 11) & 0x1f);
  timeStruct.tm_min = ((time >> 5) & 0x3f);
  timeStruct.tm_sec = ((time << 1) & 0x3f);

  return mktime(&timeStruct);
}

} }

#undef _utils_stream_localtime
