#include "../catch.hpp"
#include <Xli/DateTime.h>
#include <Xli/Console.h>

using namespace Xli;

DateTime CreateDateTime(int year, int month, int day)
{
    DateTime now;
    now.Year = year;
    now.Month = month;
    now.Day = day;
    return now;
}

void Check(const DateTime& expected, const DateTime& actual)
{
    CHECK(expected.Year == actual.Year);
    CHECK(expected.Month == actual.Month);
    CHECK(expected.Day == actual.Day);
    CHECK(expected.Hour == actual.Hour);
    CHECK(expected.Minute == actual.Minute);
    CHECK(expected.Second == actual.Second);
    CHECK(expected.Millisecond == actual.Millisecond);
}

TEST_CASE("January")
{
    DateTime now = CreateDateTime(2014, 1, 1);
    DateTime same = DateTime(now.ToTimestamp());

    Check(now, same);
}

TEST_CASE("February")
{
    DateTime now = CreateDateTime(2014, 2, 10);
    DateTime same = DateTime(now.ToTimestamp());

    Check(now, same);
}

TEST_CASE("March")
{
    DateTime now = CreateDateTime(2014, 3, 20);
    DateTime same = DateTime(now.ToTimestamp());

    Check(now, same);
}

TEST_CASE("April")
{
    DateTime now = CreateDateTime(2014, 4, 30);
    DateTime same = DateTime(now.ToTimestamp());

    Check(now, same);
}
