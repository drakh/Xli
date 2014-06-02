#include "../catch.hpp"
#include <Xli/DateTime.h>
#include <Xli/Console.h>

using namespace Xli;

TEST_CASE("ConvertsToAndFromTimestamp")
{
    DateTime now = DateTime::Now();
    DateTime same = DateTime(now.ToTimestamp());

    Err->WriteFormat("Month:%d v %d\n ",now.Month, same.Month);
    Err->WriteFormat("Day:%d v %d\n ",now.Day, same.Day);

    REQUIRE(now.Year == same.Year);
    REQUIRE(now.Month == same.Month);
    REQUIRE(now.Day == same.Day);
    REQUIRE(now.Hour == same.Hour);
    REQUIRE(now.Minute == same.Minute);
    REQUIRE(now.Second == same.Second);
    REQUIRE(now.Millisecond == same.Millisecond);
}

