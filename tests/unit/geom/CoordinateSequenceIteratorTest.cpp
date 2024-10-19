#include <tut/tut.hpp>
#include <geos/geom/LineSegment.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <numeric>

namespace tut {

using geos::geom::Coordinate;
using geos::geom::CoordinateArraySequence;

struct test_coordinatesequenceiterator_data {

    CoordinateArraySequence seq_;

    test_coordinatesequenceiterator_data() : seq_(10) {
        for (size_t i = 0; i < seq_.size(); i++) {
            seq_[i] = {static_cast<double>(i), static_cast<double>(i)};
        }
    }
};

typedef test_group<test_coordinatesequenceiterator_data> group;
typedef group::object object;

group test_coordinatesequenceiterator_group("geos::geom::CoordinateSequenceIterator");

// Test iteration
template<>
template<>
void object::test<1>()
{
    // Use modifying iterator to set y values
    for (auto& coord : seq_) {
        coord.y = 2*coord.x;
    }

    // Use const iterator to check values
    std::size_t i = 0;
    for (const auto& coord : seq_) {
        ensure_equals(coord.x, static_cast<double>(i));
        ensure_equals(coord.y, 2*coord.x);
        i++;
    }

    ensure_equals(i, seq_.size());
}

// Test equality operators
template<>
template<>
void object::test<2>()
{
    auto a = seq_.begin();
    auto b = seq_.begin();

    // Test equality
    ensure(a == b);
    ensure(!(a != b));

    ++b;
    ++b;

    ensure(a != b);
    std::advance(a, 2);
    ensure(a == b);
}

// Test comparison operators
template<>
template<>
void object::test<3>()
{
    auto a = seq_.begin();
    auto b = seq_.begin();

    ensure(a <= b);
    ensure(a >= b);
    ensure(!(a < b));
    ensure(!(b > a));

    a++;
    ensure(a > b);
    ensure(!(b >a));
    ensure(a >= b);
    ensure(!(b >= a));
    ensure(b < a);
    ensure(!(a < b));
    ensure(b <= a);
    ensure(!(a <= b));
}

// Test integer add/subtract
template<>
template<>
void object::test<4>()
{
    auto a = seq_.begin();
    auto n = static_cast<long>(seq_.size());

    ensure(a + n ==  seq_.end());
    a += n;
    ensure(a == seq_.end());
    ensure(a - n == seq_.begin());
    a -= n;
    ensure(a == seq_.begin());

    ensure_equals(seq_.end() - seq_.begin(), n);
}

// Test offset deference operator
template<>
template<>
void object::test<5>()
{
    auto a = seq_.begin();
    std::advance(a, 5);

    ensure_equals(a[0], seq_[5]);
    ensure_equals(a[-5], seq_[0]);
    ensure_equals(a[4], seq_[9]);
}

// Test increment/decrement operators
template<>
template<>
void object::test<6>()
{
    auto a = seq_.begin();

    // Prefix increment
    auto b = ++a;
    ensure_equals(a - seq_.begin(), 1);
    ensure(b == a);

    // Postfix increment
    auto c = a++;
    ensure_equals(a - seq_.begin(), 2);
    ensure(c == b);

    // Prefix decrement
    auto d = --a;
    ensure_equals(a - seq_.begin(), 1);
    ensure(d == a);

    // Postfix decrement
    auto e = a--;
    ensure_equals(a - seq_.begin(), 0);
    ensure(e == d);
}

template<>
template<>
void object::test<7>()
{
    std::vector<Coordinate> coords{ Coordinate(8, 7), Coordinate(1, 1), Coordinate(1, 7) };

    auto coords2 = coords; // copy
    CoordinateArraySequence seq(std::move(coords2));

    std::sort(coords.begin(), coords.end());
    std::sort(seq.begin(), seq.end());

    for (size_t i = 0; i < seq.getSize(); i++) {
        ensure_equals(coords[i], seq[i]);
    }

}

}
