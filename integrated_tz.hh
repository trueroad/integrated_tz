//
// A time zone library that can handle both POSIX style
// and IANA time zone database style strings for C++11 2018-08-19.05
// https://github.com/trueroad/integrated_tz
//
// integrated_tz.hh: Main class header file
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

#ifndef INCLUDE_GUARD_INTEGRATED_TZ_HH
#define INCLUDE_GUARD_INTEGRATED_TZ_HH

#include <memory>
#include <string>

#include "tz/ptz.h"

namespace integrated_tz
{
  class time_zone
  {
  public:
    time_zone (const std::string&);
    time_zone ();

    void set_zone (const std::string&);
    void set_zone (void);

    const std::string& name () const;

    template <class Duration>
    date::sys_info
    get_info (const date::sys_time<Duration>&) const;
    template <class Duration>
    date::local_info
    get_info (const date::local_time<Duration>&) const;

    template <class Duration>
    date::sys_time<typename
                   std::common_type<Duration, std::chrono::seconds>::type>
    to_sys (const date::local_time<Duration>&) const;

    template <class Duration>
    date::sys_time<typename
                   std::common_type<Duration, std::chrono::seconds>::type>
    to_sys (const date::local_time<Duration>&, date::choose) const;

    template <class Duration>
    date::local_time<typename
                     std::common_type<Duration, std::chrono::seconds>::type>
    to_local(const date::sys_time<Duration>&) const;

  private:
    enum class tz_type { tzdb, posix } type_;

    const date::time_zone *tzdb_tz_;
    std::unique_ptr<const Posix::time_zone> posix_tz_;

    std::string posix_name_;

    static constexpr auto default_zone_ {"UTC0"};
    static constexpr auto env_name_ {"TZ"};
  };
};

#include "integrated_tz_private.hh"

#endif // INCLUDE_GUARD_INTEGRATED_TZ_HH
