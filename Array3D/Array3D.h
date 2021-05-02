// Author: Radoslaw Choma
// Github: https://github.com/A9Radeus/Array3D

#pragma once
#include <array>
#include <vector>

namespace rch {

/**
 * High performance, constexpr-based, 3-dimensional array container.
 *
 * Dim3, Dim2, Dim1 describe resolution of a given dimension. For instance:
 * - Array3D<4, 3, 2> ~ 4 "planes", each containing 3 "rows", each row
 *                      has 2 elements.
 * - Array3D<1, 3, 5> ~ 1 "plane", containing 3 "rows", each row has 5 elements,
 *                      thus it is equivalent to 3x5 2D-array.
 * - Array3D<3, 1, 5> ~ 3 "planes", each containing 1 "row" with 5 elements,
 *                      thus it is equivalent to 3x5 2D-array.
 * - Array3D<1, 1, 5> ~ 1 "plane", containing 1 "row", containing 5 elements,
 *                      thus it is equivalent to 1D-array.
 */
template <typename TData, std::size_t Dim3, std::size_t Dim2, std::size_t Dim1>
class Array3D {
 public:
  static constexpr std::size_t s_dim_count = 3;
  using IndsType = std::array<std::size_t, s_dim_count>;

  /*
      Modifiers
  */

  void assign(const TData& newVal, std::size_t idx3, std::size_t idx2,
              std::size_t idx1) {
    m_data[calc_idx_l(idx3, idx2, idx1)] = newVal;
  }

  void assign(TData&& newVal, std::size_t idx3, std::size_t idx2,
              std::size_t idx1) {
    m_data[calc_idx_l(idx3, idx2, idx1)] = std::move(newVal);
  }

  /*
      Element access
  */

  constexpr const TData& operator[](const IndsType& inds) const {
    return m_data[calc_idx_l(inds)];
  };

  constexpr TData& operator[](const IndsType& inds) {
    return m_data[calc_idx_l(inds)];
  };

  // Helper function equivalent to calling operator[].
  // Useful when macros wouldn't evaluate {} in the operator[] call.
  constexpr const TData& get(std::size_t idx3, std::size_t idx2,
                             std::size_t idx1) const {
    return m_data[calc_idx_l(idx3, idx2, idx1)];
  };

  // Helper function equivalent to calling operator[].
  // Useful when macros wouldn't evaluate {} in the operator[] call.
  constexpr TData& get(std::size_t idx3, std::size_t idx2, std::size_t idx1) {
    return m_data[calc_idx_l(idx3, idx2, idx1)];
  };

  // Returns m_data[idx], where m_data is the inner, linear, array buffer
  const TData& get_l(std::size_t idx) const { return m_data[idx]; };

  // Returns ref to m_data[idx], where m_data is the inner, linear, array buffer
  TData& get_l(std::size_t idx) { return m_data[idx]; };

  // Returns const reference to data at given indices if these are not
  // out of bounds. Otherwise, returns const reference to the defaultVal.
  const TData& dget(const TData& defaultVal, const IndsType& inds) const {
    if (in_bounds(inds)) {
      return m_data[calc_idx_l(inds[0], inds[1], inds[2])];
    } else {
      return defaultVal;
    }
  };

  // Overload for raw indices
  const TData& dget(const TData& defaultVal, std::size_t idx3, std::size_t idx2,
                    std::size_t idx1) const {
    return dget(defaultVal, {idx3, idx2, idx1});
  };

  // Version of dget() that uses linear indexing.
  const TData& dget_l(const TData& defaultVal, std::size_t idx) const {
    if (idx < s_size) {
      return m_data[idx];
    } else {
      return defaultVal;
    }
  };

  /*
      Attributes
  */

  static constexpr std::size_t size() { return s_size; }

  static constexpr std::size_t dim3() { return Dim3; }
  static constexpr std::size_t dim2() { return Dim2; }
  static constexpr std::size_t dim1() { return Dim1; }

  static constexpr const std::array<std::size_t, 3>& dimensions() {
    return s_dims;
  }

  /*
      Utility functions
  */

  // Checks if given indices are NOT out of range.
  static constexpr bool in_bounds(const IndsType& inds) {
    // By definition std::size_t has to be >= 0
    return ((inds[0] < Dim3) && (inds[1] < Dim2) && (inds[2] < Dim1));
  }

  // Overload for raw indices
  static constexpr bool in_bounds(std::size_t idx3, std::size_t idx2,
                                  std::size_t idx1) {
    return in_bounds({idx3, idx2, idx1});
  }

  // Checks if given indices (int-s) are NOT out of range. Ints might not
  // be able to cover all potential indices due to them being std::size_t.
  static constexpr bool in_bounds_signed(const std::array<int, 3>& inds) {
    return ((0 <= inds[0] && inds[0] < Dim3) &&
            (0 <= inds[1] && inds[1] < Dim2) &&
            (0 <= inds[2] && inds[2] < Dim1));
  }

  // Overload for raw indices
  static constexpr bool in_bounds_signed(int idx3, int idx2, int idx1) {
    return in_bounds_signed({idx3, idx2, idx1});
  }

  // Calculates the index of Array3D[idx3][idx2][idx1] in the inner linear
  // buffer
  static constexpr std::size_t calc_idx_l(const IndsType& inds) {
    return inds[0] * s_offsets[0] + inds[1] * s_offsets[1] +
           inds[2] * s_offsets[2];
  }

  // Overlod for raw indices
  static constexpr std::size_t calc_idx_l(std::size_t idx3, std::size_t idx2,
                                          std::size_t idx1) {
    return calc_idx_l({idx3, idx2, idx1});
  }

 private:
  std::array<TData, Dim3* Dim2* Dim1> m_data = {};

  static constexpr std::size_t s_size = Dim3 * Dim2 * Dim1;

  static constexpr std::array<std::size_t, s_dim_count> s_dims = {Dim3, Dim2,
                                                                  Dim1};
  static constexpr std::array<std::size_t, s_dim_count> s_offsets = {
      Dim2 * Dim1, Dim1, 1};
};

/*
    Out-of-class Utilities
*/

template <typename TData, std::size_t Dim3, std::size_t Dim2, std::size_t Dim1>
inline std::vector<TData> subset_of(Array3D<TData, Dim3, Dim2, Dim1>& arr,
                                    const std::array<std::size_t, 3>& startInds,
                                    const std::array<std::size_t, 3>& endInds) {
  if (arr.checkBounds(startInds) == false) {
    return std::vector<TData>();
  }
  if (arr.checkBounds(endInds) == false) {
    return std::vector<TData>();
  }

  std::size_t startIdx =
      arr.calc_idx_l(startInds[0], startInds[1], startInds[2]);
  std::size_t endIdx = arr.calc_idx_l(endInds[0], endInds[1], endInds[2]);

  if (endIdx < startIdx) {
    return std::vector<TData>();
  }

  std::vector<TData> result;
  result.reserve(endIdx - startIdx + 1);

  for (auto i = startIdx; i <= endIdx; ++i) {
    result.emplace_back(arr.get_l(i));
  }

  return result;
};

}  // namespace rch