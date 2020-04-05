/**************************************************************************

   Copyright © 2018 Sergey Poluyan <svpoluyan@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

**************************************************************************/

#include <utility/timer.h>

namespace timer
{
  void Timer::reset()
  {
    beg_ = clock_::now();
  }
  double Timer::elapsed_nanoseconds() const
  {
    return std::chrono::duration_cast<nanoseconds_>(clock_::now() - beg_).count();
  }
  double Timer::elapsed_microseconds() const
  {
    return std::chrono::duration_cast<microseconds_>(clock_::now() - beg_).count();
  }
  double Timer::elapsed_milliseconds() const
  {
    return std::chrono::duration_cast<milliseconds_>(clock_::now() - beg_).count();
  }
  double Timer::elapsed_seconds() const
  {
    return std::chrono::duration_cast<seconds_>(clock_::now() - beg_).count();
  }
  double Timer::elapsed_minutes() const
  {
    return std::chrono::duration_cast<minutes_>(clock_::now() - beg_).count();
  }
  double Timer::elapsed_hours() const
  {
    return std::chrono::duration_cast<hours_>(clock_::now() - beg_).count();
  }
}
