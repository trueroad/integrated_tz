<!-- -*- coding: utf-8 -*- -->
# A time zone library that can handle both POSIX style and IANA time zone database style strings for C++11

[ [Japanese (日本語)](./README.ja.md) / English ]

[https://github.com/trueroad/integrated_tz](https://github.com/trueroad/integrated_tz)

This library can handle both POSIX style (like `JST-9` )
and IANA time zone database style (like `Asia/Tokyo` ) strings.
It can handle multiple time zones simultaneously unlike POSIX `tzset ()`
and `localtime ()` etc.
It is an improvement of the code I posted to
"[How to use POSIX-style environment variable `TZ` #258 
](https://github.com/HowardHinnant/date/issues/258#issuecomment-348945417)"
and uses
[https://github.com/HowardHinnant/date](https://github.com/HowardHinnant/date)
.

## Build samples

There are two ways for building samples.

### Manually compile

The samples can be built by something like the following commands.
You might need different commands and options depending on the environment.

#### Simple sample

```
$ g++ -std=gnu++11 -c -DUSE_AUTOLOAD=0 -DHAS_REMOTE_API=0 -DUSE_OS_TZDB=1 \
	tz/tz.cpp
$ g++ -std=gnu++11 -o sample sample.cc tz.o
```

#### Multiple time zone sample

```
$ g++ -std=gnu++11 -c -DUSE_AUTOLOAD=0 -DHAS_REMOTE_API=0 -DUSE_OS_TZDB=1 \
	tz/tz.cpp
$ g++ -std=gnu++11 -o sample-multiple sample-multiple.cc tz.o
```

### Autotools

This library does not require Autotools.
But, the samples can be built easily by Autotools as the following commands.

```
$ ./autogen.sh
$ ./configure
$ make
```

## Usage

Please copy `integrated_tz.hh`, `integrated_tz_private.hh`,
`tz/ptz.h`, `tz/tz.cpp`,
`tz/date/date.h`, `tz/tz.h`, and `tz_private.h`
to your project directory.
Please add `tz/tz.cpp` to your project as a source code.

### Simple sample

Here is a part of [sample.cc](./sample.cc).

```c++
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
```

Here are some outputs:

Without any command line options,
it shows some time formats of current time on current local time zone and UTC.

```
$ ./sample
zoned_time:
    2018-08-19 08:54:30.737084100 JST
RFC3339 (zoned):
    2018-08-19T08:54:30.737084100+09:00
RFC3339 (UTC):
    2018-08-18T23:54:30.737084100+00:00
    2018-08-18T23:54:30.737084100Z

```

With RFC3339 date-time format command line option,
it shows some time formats of the specified time
on current local time zone and UTC.

```
$ ./sample 1951-08-01T12:34:56+09:00
zoned_time:
    1951-08-01 12:34:56.000000000 JST
RFC3339 (zoned):
    1951-08-01T12:34:56.000000000+09:00
RFC3339 (UTC):
    1951-08-01T03:34:56.000000000+00:00
    1951-08-01T03:34:56.000000000Z

```

Using the environment variable `TZ` can specify a time zone.
You can use the both POSIX style and IANA time zone database style.

```
$ TZ=JST-9 ./sample
zoned_time:
    2018-08-19 08:55:48.481978300 JST
RFC3339 (zoned):
    2018-08-19T08:55:48.481978300+09:00
RFC3339 (UTC):
    2018-08-18T23:55:48.481978300+00:00
    2018-08-18T23:55:48.481978300Z

$ TZ=Asia/Tokyo ./sample
zoned_time:
    2018-08-19 08:55:57.365413700 JST
RFC3339 (zoned):
    2018-08-19T08:55:57.365413700+09:00
RFC3339 (UTC):
    2018-08-18T23:55:57.365413700+00:00
    2018-08-18T23:55:57.365413700Z

$ TZ=JST-9 ./sample 1951-08-01T12:34:56+09:00
zoned_time:
    1951-08-01 12:34:56.000000000 JST
RFC3339 (zoned):
    1951-08-01T12:34:56.000000000+09:00
RFC3339 (UTC):
    1951-08-01T03:34:56.000000000+00:00
    1951-08-01T03:34:56.000000000Z

$ TZ=Asia/Tokyo ./sample 1951-08-01T12:34:56+09:00
zoned_time:
    1951-08-01 13:34:56.000000000 JDT
RFC3339 (zoned):
    1951-08-01T13:34:56.000000000+10:00
RFC3339 (UTC):
    1951-08-01T03:34:56.000000000+00:00
    1951-08-01T03:34:56.000000000Z

```

### Multiple time zone sample

Multiple time zone sample is [sample-multiple.cc](./sample-multiple.cc).
It is the sample that uses multiple time zones simultaneously.

Here are some parts of [sample-multiple.cc](./sample-multiple.cc).

Using the default constructor without any time zone name,
the instance is initialized for current local time zone.

```c++
  // Current local time zone
  integrated_tz::time_zone itz_current;
```

Using a time zone name strings for the constructor's argument,
the instance is initialized for the specified time zone.
Both POSIX style and IANA time zone database style can be acceptable.

```c++
  // Asia/Tokyo (IANA time zone database style)
  integrated_tz::time_zone itz_Tokyo {"Asia/Tokyo"};
```

```c++
  // JST-9 (POSIX style)
  integrated_tz::time_zone itz_jst {"JST-9"};
```

Here are some outputs:

```
$ ./sample-multiple
name:
    JST-9
zoned_time:
    2018-08-19 08:57:39.743906900 JST
RFC3339 (zoned):
    2018-08-19T08:57:39.743906900+09:00

name:
    Asia/Tokyo
zoned_time:
    2018-08-19 08:57:39.743906900 JST
RFC3339 (zoned):
    2018-08-19T08:57:39.743906900+09:00

name:
    JST-9
zoned_time:
    2018-08-19 08:57:39.743906900 JST
RFC3339 (zoned):
    2018-08-19T08:57:39.743906900+09:00

name:
    PST8PDT
zoned_time:
    2018-08-18 16:57:39.743906900 PDT
RFC3339 (zoned):
    2018-08-18T16:57:39.743906900-07:00

name:
    CST-9:30CDT
zoned_time:
    2018-08-19 09:27:39.743906900 CST
RFC3339 (zoned):
    2018-08-19T09:27:39.743906900+09:30

name:
    EST+5EDT,M3.2.0/2,M11.1.0/2
zoned_time:
    2018-08-18 19:57:39.743906900 EDT
RFC3339 (zoned):
    2018-08-18T19:57:39.743906900-04:00

name:
    America/Los_Angeles
zoned_time:
    2018-08-18 16:57:39.743906900 PDT
RFC3339 (zoned):
    2018-08-18T16:57:39.743906900-07:00

name:
    America/New_York
zoned_time:
    2018-08-18 19:57:39.743906900 EDT
RFC3339 (zoned):
    2018-08-18T19:57:39.743906900-04:00

name:
    Australia/Adelaide
zoned_time:
    2018-08-19 09:27:39.743906900 ACST
RFC3339 (zoned):
    2018-08-19T09:27:39.743906900+09:30


$ ./sample-multiple 1951-08-01T12:34:56+09:00
name:
    JST-9
zoned_time:
    1951-08-01 12:34:56.000000000 JST
RFC3339 (zoned):
    1951-08-01T12:34:56.000000000+09:00

name:
    Asia/Tokyo
zoned_time:
    1951-08-01 13:34:56.000000000 JDT
RFC3339 (zoned):
    1951-08-01T13:34:56.000000000+10:00

name:
    JST-9
zoned_time:
    1951-08-01 12:34:56.000000000 JST
RFC3339 (zoned):
    1951-08-01T12:34:56.000000000+09:00

name:
    PST8PDT
zoned_time:
    1951-07-31 19:34:56.000000000 PST
RFC3339 (zoned):
    1951-07-31T19:34:56.000000000-08:00

name:
    CST-9:30CDT
zoned_time:
    1951-08-01 13:04:56.000000000 CST
RFC3339 (zoned):
    1951-08-01T13:04:56.000000000+09:30

name:
    EST+5EDT,M3.2.0/2,M11.1.0/2
zoned_time:
    1951-07-31 23:34:56.000000000 EDT
RFC3339 (zoned):
    1951-07-31T23:34:56.000000000-04:00

name:
    America/Los_Angeles
zoned_time:
    1951-07-31 20:34:56.000000000 PDT
RFC3339 (zoned):
    1951-07-31T20:34:56.000000000-07:00

name:
    America/New_York
zoned_time:
    1951-07-31 23:34:56.000000000 EDT
RFC3339 (zoned):
    1951-07-31T23:34:56.000000000-04:00

name:
    Australia/Adelaide
zoned_time:
    1951-08-01 13:04:56.000000000 ACST
RFC3339 (zoned):
    1951-08-01T13:04:56.000000000+09:30


```

## License

Copyright (C) 2018 Masamichi Hosoda. All rights reserved.

License: BSD-2-Clause

See [LICENSE](./LICENSE).
