// This software was partially written using Suggestions from GitHub Copilot.
#pragma once

#include <array>
#include <iostream>
#include <cmath>
#include <cstdint>

namespace aoc_utils {
template <typename T = int, int N = 2>
struct PointTmpl {
    static_assert(N > 1, "Number of coordinates should be bigger than 1");
    std::array<T, N> coords;

    explicit PointTmpl(const std::array<T, N> &arr) : coords(arr) {}

    PointTmpl(T y, T x)
        requires(N == 2)
        : coords({y, x}) {}

    PointTmpl(T x, T y, T z)
        requires(N == 3)
        : coords({x, y, z}) {}

    PointTmpl() : coords({0}) {}

    PointTmpl(const PointTmpl &rhs) = default;

    PointTmpl(PointTmpl &&rhs) = default;

    PointTmpl &operator=(const PointTmpl &rhs) = default;

    PointTmpl &operator=(PointTmpl &&rhs) = default;

    [[nodiscard]] const T &x() const {
        return coords[0];
    }

    T &x() {
        return coords[0];
    }

    [[nodiscard]] const T &y() const {
        return coords[1];
    }

    T &y() {
        return coords[1];
    }

    [[nodiscard]] const T &z() const
        requires(N >= 2)
    {
        return coords[2];
    }

    T &z()
        requires(N >= 2)
    {
        return coords[2];
    }

    [[nodiscard]] auto sqr() const {
        static_assert(std::is_integral_v<T>);
        std::uint64_t res = 0;
        for (int i = 0; i < N; ++i) {
            res += coords[i] * coords[i];
        }
        return res;
    }

    [[nodiscard]] auto distSquared(const PointTmpl &rhs) const {
        static_assert(std::is_integral_v<T>);
        std::uint64_t res = 0;
        for (int i = 0; i < N; ++i) {
            auto coords_diff =
                static_cast<std::make_signed_t<T>>(coords[i]) - rhs.coords[i];
            res += coords_diff * coords_diff;
        }
        return res;
    }

    [[nodiscard]] double dist(const PointTmpl &rhs) const {
        return std::sqrt(static_cast<double>(distSquared(rhs)));
    }

    T &operator[](int i) {
        if (i >= N) {
            throw std::runtime_error(
                "operator is out of range for coordinates, i=" +
                std::to_string(i) + "/" + std::to_string(N));
        }
        return coords[i];
    }

    const T &operator[](int i) const {
        if (i >= N) {
            throw std::runtime_error(
                "operator is out of range for coordinates, i=" +
                std::to_string(i) + "/" + std::to_string(N));
        }
        return coords[i];
    }

    template <typename U, int M>
    friend std::ostream &operator<<(std::ostream &os, const PointTmpl<U, M> &p);

    [[nodiscard]] auto left() const
        requires(N == 2)
    {
        std::array<T, N> newCoords = coords;
        newCoords[1]--;
        return PointTmpl<T, N>(newCoords);
    }

    [[nodiscard]] auto right() const
        requires(N == 2)
    {
        std::array<T, N> newCoords = coords;
        newCoords[1]++;
        return PointTmpl<T, N>(newCoords);
    }

    [[nodiscard]] auto up() const
        requires(N == 2)
    {
        std::array<T, N> newCoords = coords;
        newCoords[0]--;
        return PointTmpl<T, N>(newCoords);
    }

    [[nodiscard]] auto down() const
        requires(N == 2)
    {
        std::array<T, N> newCoords = coords;
        newCoords[0]++;
        return PointTmpl<T, N>(newCoords);
    }
};

template <typename T, int N>
std::ostream &operator<<(std::ostream &os, const PointTmpl<T, N> &p) {
    for (int i = 0; i < N; ++i) {
        if (i) {
            os << ", ";
        }
        os << p[i];
    }
    return os;
}


template<typename T, int N>
PointTmpl<T, N> operator+(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    PointTmpl<T, N> res;
    for (int i = 0; i < N; ++i) {
        res[i] = lhs[i] + rhs[i];
    }
    return res;
}

template<typename T, int N, typename MultiplierType>
PointTmpl<MultiplierType, N> operator*(const PointTmpl<T, N> &lhs, const MultiplierType x) {
    PointTmpl<MultiplierType, N> res;
    for (int i = 0; i < N; ++i) {
        res[i] = lhs[i] * x;
    }
    return res;
}

template<typename T, int N>
PointTmpl<T, N> operator<(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    PointTmpl<T, N> res;
    for (int i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) return lhs[i] < rhs[i];
    }
    return false;
}

template<typename T, int N>
PointTmpl<T, N> operator==(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    PointTmpl<T, N> res;
    for (int i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}


template<typename T, int N>
PointTmpl<T, N> operator!=(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    return !(lhs == rhs);
}

};  // namespace aoc_utils
