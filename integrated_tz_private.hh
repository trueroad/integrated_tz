//
// A time zone library that can handle both POSIX style
// and IANA time zone database style strings for C++11 2018-08-19.05
// https://github.com/trueroad/integrated_tz
//
// integrated_tz_private.hh: Main implementation header
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

#ifndef INCLUDE_GUARD_INTEGRATED_TZ_PRIVATE_HH
#define INCLUDE_GUARD_INTEGRATED_TZ_PRIVATE_HH

#include <exception>
#include <memory>
#include <string>

#include <cstdlib>

#include "tz/ptz.h"

namespace integrated_tz
{
  inline time_zone::time_zone (const std::string& name)
  {
    set_zone (name);
  }

  inline time_zone::time_zone ()
  {
    set_zone ();
  }

  inline void time_zone::set_zone (const std::string& name)
  {
    type_ = tz_type::tzdb;
    try
      {
        tzdb_tz_ = date::locate_zone (name);
      }
    catch (std::runtime_error)
      {
        type_ = tz_type::posix;
        try
          {
            posix_tz_.reset (new Posix::time_zone (name));
            posix_name_ = name;
          }
        catch (std::exception) // ! std::runtime_error
          {
            posix_tz_.reset (new Posix::time_zone (default_zone_));
            posix_name_ = default_zone_;
          }
      }
  }

  inline void time_zone::set_zone (void)
  {
    char *env_tz = std::getenv (env_name_);
    if (env_tz)
      {
        set_zone (env_tz);
      }
    else
      {
        type_ = tz_type::tzdb;
        try
          {
            tzdb_tz_ = date::current_zone ();
          }
        catch (std::runtime_error)
          {
            set_zone (default_zone_);
          }
      }
  }

  inline const std::string& time_zone::name () const
  {
    switch (type_)
      {
      case tz_type::tzdb:
        return tzdb_tz_->name ();
      case tz_type::posix:
        return posix_name_; // Posix::time_zone does not have name ()
      }
    throw std::runtime_error ("integrated_tz::time_zone::name:"
                              " unknown type");
  }

  template <class Duration>
  date::sys_info
  time_zone::get_info (const date::sys_time<Duration>& st) const
  {
    switch (type_)
      {
      case tz_type::tzdb:
        return tzdb_tz_->get_info (st);
      case tz_type::posix:
        return posix_tz_->get_info (st);
      }
    throw std::runtime_error ("integrated_tz::time_zone::get_info"
                              " (sys_time): unknown type");
  }

  template <class Duration>
  date::local_info
  time_zone::get_info (const date::local_time<Duration>& tp) const
  {
    switch (type_)
      {
      case tz_type::tzdb:
        return tzdb_tz_->get_info (tp);
      case tz_type::posix:
        return posix_tz_->get_info (tp);
      }
    throw std::runtime_error ("integrated_tz::time_zone::get_info"
                              " (local_time): unknown type");
  }

  template <class Duration>
  date::sys_time<typename
                 std::common_type<Duration, std::chrono::seconds>::type>
  time_zone::to_sys (const date::local_time<Duration>& tp) const
  {
    switch (type_)
      {
      case tz_type::tzdb:
        return tzdb_tz_->to_sys (tp);
      case tz_type::posix:
        return posix_tz_->to_sys (tp);
      }
    throw std::runtime_error ("integrated_tz::time_zone::to_sys:"
                              " unknown type");
  }

  template <class Duration>
  date::sys_time<typename
                 std::common_type<Duration, std::chrono::seconds>::type>
  time_zone::to_sys (const date::local_time<Duration>& tp,
                     date::choose z) const
  {
    switch (type_)
      {
      case tz_type::tzdb:
        return tzdb_tz_->to_sys (tp, z);
      case tz_type::posix:
        return posix_tz_->to_sys (tp, z);
      }
    throw std::runtime_error ("integrated_tz::time_zone::to_sys"
                              " (local_time, choose): unknown type");
  }

  template <class Duration>
  date::local_time<typename
                   std::common_type<Duration, std::chrono::seconds>::type>
  time_zone::to_local(const date::sys_time<Duration>& tp) const
  {
    switch (type_)
      {
      case tz_type::tzdb:
        return tzdb_tz_->to_local (tp);
      case tz_type::posix:
        return posix_tz_->to_local (tp);
      }
    throw std::runtime_error ("integrated_tz::time_zone::to_local:"
                              " unknown type");
  }
};

#endif // INCLUDE_GUARD_INTEGRATED_TZ_PRIVATE_HH
