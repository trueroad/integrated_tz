//
// A time zone library that can handle both POSIX style
// and IANA time zone database style strings for C++11
// https://github.com/trueroad/integrated_tz
//
// sample-multiple.cc: Sample code for using multiple time zones
//
// Copyright (C) 2018 Masamichi Hosoda.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//

#include "integrated_tz.hh"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

template <class Duration, class TimeZonePtr>
void show (const date::zoned_time<Duration, TimeZonePtr>& zt)
{
  std::cout << "name:" << std::endl
            << "    " << zt.get_time_zone ()->name() << std::endl;
  std::cout << "zoned_time:" << std::endl
            << "    " << zt << std::endl;
  std::cout << "RFC3339 (zoned):" << std::endl
            << "    " << date::format ("%FT%T%Ez", zt) << std::endl;
  std::cout << std::endl;
}

int main (int argc, char *argv[])
{
  auto tp {std::chrono::system_clock::now ()};

  if (argc > 1)
    {
      std::istringstream in {argv[1]};
      in >> date::parse ("%FT%T%Ez", tp); // RFC3339
      if (in.fail ())
        {
          in.clear ();
          in.seekg (0, in.beg);
          in >> date::parse ("%a, %d %b %Y %T %z", tp); // RFC2822
        }
    }

  // Current local time zone
  integrated_tz::time_zone itz_current;
  auto zt_current {date::make_zoned (&itz_current, tp)};

  // Asia/Tokyo (IANA time zone database style)
  integrated_tz::time_zone itz_Tokyo {"Asia/Tokyo"};
  auto zt_Tokyo {date::make_zoned (&itz_Tokyo, tp)};

  // JST-9 (POSIX style)
  integrated_tz::time_zone itz_jst {"JST-9"};
  auto zt_jst {date::make_zoned (&itz_jst, tp)};

  // PST8PDT (POSIX style)
  integrated_tz::time_zone itz_pst {"PST8PDT"};
  auto zt_pst {date::make_zoned (&itz_pst, tp)};

  // CST-9:30CDT (POSIX style)
  integrated_tz::time_zone itz_cst {"CST-9:30CDT"};
  auto zt_cst {date::make_zoned (&itz_cst, tp)};

  // EST+5EDT,M3.2.0/2,M11.1.0/2 (POSIX style)
  integrated_tz::time_zone itz_est {"EST+5EDT,M3.2.0/2,M11.1.0/2"};
  auto zt_est {date::make_zoned (&itz_est, tp)};

  // America/Los_Angeles (IANA time zone database style)
  integrated_tz::time_zone itz_LA {"America/Los_Angeles"};
  auto zt_LA {date::make_zoned (&itz_LA, tp)};

  // America/New_York (IANA time zone database style)
  integrated_tz::time_zone itz_NY {"America/New_York"};
  auto zt_NY {date::make_zoned (&itz_NY, tp)};

  // Australia/Adelaide (IANA time zone database style)
  integrated_tz::time_zone itz_Adelaide {"Australia/Adelaide"};
  auto zt_Adelaide {date::make_zoned (&itz_Adelaide, tp)};

  show (zt_current);
  show (zt_Tokyo);
  show (zt_jst);
  show (zt_pst);
  show (zt_cst);
  show (zt_est);
  show (zt_LA);
  show (zt_NY);
  show (zt_Adelaide);

  return 0;
}
