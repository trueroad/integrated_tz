//
// A time zone library that can handle both POSIX style
// and IANA time zone database style strings for C++11
// https://github.com/trueroad/integrated_tz
//
// sample.cc: Sample code
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

  integrated_tz::time_zone itz; // Current local time zone
  auto zt {date::make_zoned (&itz, tp)};

  std::cout << "zoned_time:" << std::endl
            << "    " << zt << std::endl;
  std::cout << "RFC3339 (zoned):" << std::endl
            << "    " << date::format ("%FT%T%Ez", zt) << std::endl;

  auto st {zt.get_sys_time ()};
  std::cout << "RFC3339 (UTC):" << std::endl
            << "    " << date::format ("%FT%T%Ez", st) << std::endl
            << "    " << date::format ("%FT%TZ", st) << std::endl;

  return 0;
}
