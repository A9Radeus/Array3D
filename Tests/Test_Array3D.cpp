#include "pch.h"

#include "../Array3D/Array3D.h"

using namespace rch;

namespace utests {

struct RChTestStruct1 {
  static constexpr double s_def_val = 1.23;
  double m_val = 1.23;
};

template <std::size_t D3, std::size_t D2, std::size_t D1>
std::size_t dget_Test_Helper(const Array3D<std::size_t, D3, D2, D1>& arr,
                             const std::size_t& defv,
                             const std::array<std::size_t, 3>& inds) {
  return arr.dget(defv, inds[0], inds[1], inds[2]);
}

struct Array3DFixture1 : public testing::Test {
  // public:
  Array3DFixture1() {}

  void SetUp() {
    assignLinInds(m_arr1);
    assignLinInds(m_arr21);
    assignLinInds(m_arr22);
    assignLinInds(m_arr23);
  }

  void TearDown() {}

  template <std::size_t D3, std::size_t D2, std::size_t D1>
  constexpr std::size_t linIdx(const Array3D<std::size_t, D3, D2, D1>& arr,
                               const std::array<std::size_t, 3>& inds) const {
    constexpr auto off3 = D2 * D1;
    constexpr auto off2 = D1;
    return (inds[0] * off3 + inds[1] * off2 + inds[2]);
  }

  template <std::size_t D3, std::size_t D2, std::size_t D1>
  constexpr void assignLinInds(Array3D<std::size_t, D3, D2, D1>& arr) const {
    for (std::size_t i = 0; i < arr.dim3(); ++i) {
      for (std::size_t j = 0; j < arr.dim2(); ++j) {
        for (std::size_t k = 0; k < arr.dim1(); ++k) {
          arr.assign(linIdx(arr, {i, j, k}), i, j, k);
        }
      }
    }
  }

  Array3D<std::size_t, 3, 4, 2> m_arr1;
  Array3D<std::size_t, 4, 1, 1> m_arr21;
  Array3D<std::size_t, 1, 4, 1> m_arr22;
  Array3D<std::size_t, 1, 1, 4> m_arr23;
};

TEST_F(Array3DFixture1, Basic_ElementAccess_And_Assignment) {
  auto r1 = m_arr1[{0, 0, 0}];
  EXPECT_EQ(r1, 0);
  EXPECT_EQ(r1, m_arr1.get(0, 0, 0));
  EXPECT_EQ(m_arr1.get(0, 0, 1), 1);
  EXPECT_EQ(m_arr1.get(0, 1, 0), 2);
  EXPECT_EQ(m_arr1.get(0, 1, 1), 3);
  EXPECT_EQ(m_arr1.get(1, 0, 0), 8);

  auto r2 = m_arr21[{0, 0, 0}];
  EXPECT_EQ(r1, 0);
  EXPECT_EQ(r1, m_arr21.get(0, 0, 0));
  EXPECT_EQ(m_arr21.get(1, 0, 0), 1);
  EXPECT_EQ(m_arr21.get(2, 0, 0), 2);
  EXPECT_EQ(m_arr21.get(3, 0, 0), 3);

  auto r3 = m_arr22[{0, 0, 0}];
  EXPECT_EQ(r3, 0);
  EXPECT_EQ(r3, m_arr22.get(0, 0, 0));
  EXPECT_EQ(m_arr22.get(0, 1, 0), 1);
  EXPECT_EQ(m_arr22.get(0, 2, 0), 2);
  EXPECT_EQ(m_arr22.get(0, 3, 0), 3);

  auto r4 = m_arr23[{0, 0, 0}];
  EXPECT_EQ(r4, 0);
  EXPECT_EQ(r4, m_arr23.get(0, 0, 0));
  EXPECT_EQ(m_arr23.get(0, 0, 1), 1);
  EXPECT_EQ(m_arr23.get(0, 0, 2), 2);
  EXPECT_EQ(m_arr23.get(0, 0, 3), 3);
}

TEST_F(Array3DFixture1, Bounds_Checking) {
  EXPECT_TRUE(m_arr1.in_bounds(1, 1, 1));
  EXPECT_TRUE(m_arr1.in_bounds_signed(0, 0, 1));
  EXPECT_FALSE(m_arr1.in_bounds_signed(0, 0, -1));
  EXPECT_FALSE(m_arr1.in_bounds(3, 0, 0));
  EXPECT_FALSE(m_arr1.in_bounds(0, 4, 0));
  EXPECT_FALSE(m_arr1.in_bounds(0, 0, 2));
  EXPECT_FALSE(m_arr1.in_bounds(3, 4, 2));
  EXPECT_FALSE(m_arr1.in_bounds(100, 100, 100));

  EXPECT_TRUE(m_arr21.in_bounds(1, 0, 0));
  EXPECT_TRUE(m_arr21.in_bounds_signed(1, 0, 0));
  EXPECT_FALSE(m_arr21.in_bounds_signed(-1, 0, 0));
  EXPECT_FALSE(m_arr21.in_bounds(0, 0, 1));
  EXPECT_FALSE(m_arr21.in_bounds(0, 1, 0));
  EXPECT_FALSE(m_arr21.in_bounds(4, 0, 0));

  EXPECT_TRUE(m_arr22.in_bounds(0, 1, 0));
  EXPECT_TRUE(m_arr22.in_bounds_signed(0, 1, 0));
  EXPECT_FALSE(m_arr22.in_bounds_signed(0, -1, 0));
  EXPECT_FALSE(m_arr22.in_bounds(0, 0, 1));
  EXPECT_FALSE(m_arr22.in_bounds(0, 4, 0));
  EXPECT_FALSE(m_arr22.in_bounds(1, 0, 0));

  EXPECT_TRUE(m_arr23.in_bounds(0, 0, 1));
  EXPECT_TRUE(m_arr23.in_bounds_signed(0, 0, 1));
  EXPECT_FALSE(m_arr23.in_bounds_signed(0, 0, -1));
  EXPECT_FALSE(m_arr23.in_bounds(0, 0, 4));
  EXPECT_FALSE(m_arr23.in_bounds(0, 1, 0));
  EXPECT_FALSE(m_arr23.in_bounds(1, 0, 0));
}

// @DEV_NOTE: For some reason these didn't cause a segmentation fault,
// etc. when dget used to return reference to a local variable.
TEST_F(Array3DFixture1, Default_Value_Access) {
  const std::size_t def = 123;

  // Copying the const ref into a local var
  std::size_t copy_res0 = m_arr1.dget(def, 0, 0, 1);
  EXPECT_EQ(copy_res0, 1);

  // Copying the const ref into a local var and using "{}"
  std::size_t copy_res1 = m_arr1.dget(def, {3, 0, 0});
  EXPECT_EQ(copy_res1, def);

  // Copying the const ref into a local var
  // and passing an r-value as default_value (44)
  std::size_t copy_res2 = m_arr1.dget(44, 3, 0, 0);
  EXPECT_EQ(copy_res2, 44);

  // Copying the const ref into a local var, passing an r-value
  // as default_value and using not fundamental type in Array3D
  Array3D<RChTestStruct1, 1, 2, 3> struct_arr1;
  RChTestStruct1 copy_res3 = struct_arr1.dget({44.0}, 0, 1, 1);
  EXPECT_EQ(copy_res3.m_val, RChTestStruct1::s_def_val);
  RChTestStruct1 copy_res4 = struct_arr1.dget({44.0}, 100, 1, 1);
  EXPECT_EQ(copy_res4.m_val, 44.0);

  //
  // General tests:
  //

  EXPECT_NE(dget_Test_Helper(m_arr1, def, {1, 1, 1}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr1, def, {0, 0, 1}), 1);
  EXPECT_EQ(dget_Test_Helper(m_arr1, def, {3, 0, 0}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr1, def, {0, 4, 0}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr1, def, {0, 0, 2}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr1, def, {3, 4, 2}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr1, def, {100, 100, 100}), def);

  EXPECT_NE(dget_Test_Helper(m_arr21, def, {1, 0, 0}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr21, def, {0, 0, 1}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr21, def, {0, 1, 0}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr21, def, {4, 0, 0}), def);

  EXPECT_NE(dget_Test_Helper(m_arr22, def, {0, 1, 0}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr22, def, {0, 0, 1}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr22, def, {0, 4, 0}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr22, def, {1, 0, 0}), def);

  EXPECT_NE(dget_Test_Helper(m_arr23, def, {0, 0, 1}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr23, def, {0, 0, 4}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr23, def, {0, 1, 0}), def);
  EXPECT_EQ(dget_Test_Helper(m_arr23, def, {1, 0, 0}), def);
}

TEST_F(Array3DFixture1, Linear_Idx_Calc_And_Access) {
  // Init:

  Array3D<std::size_t, 3, 4, 2> dArr1Lin;
  for (std::size_t i = 0; i < dArr1Lin.dim3(); ++i) {
    for (std::size_t j = 0; j < dArr1Lin.dim2(); ++j) {
      for (std::size_t k = 0; k < dArr1Lin.dim1(); ++k) {
        dArr1Lin.assign(dArr1Lin.calc_idx_l(i, j, k), i, j, k);
      }
    }
  }

  Array3D<std::size_t, 3, 4, 2> sArr1Lin;
  for (std::size_t i = 0; i < sArr1Lin.dim3(); ++i) {
    for (std::size_t j = 0; j < sArr1Lin.dim2(); ++j) {
      for (std::size_t k = 0; k < sArr1Lin.dim1(); ++k) {
        sArr1Lin.assign(Array3D<std::size_t, 3, 4, 2>::calc_idx_l(i, j, k), i,
                        j, k);
      }
    }
  }

  // Automatic tests:

  for (std::size_t i = 0; i < dArr1Lin.dim3(); ++i) {
    for (std::size_t j = 0; j < dArr1Lin.dim2(); ++j) {
      for (std::size_t k = 0; k < dArr1Lin.dim1(); ++k) {
        auto v1 = dArr1Lin[{i, j, k}];
        auto v2 = m_arr1[{i, j, k}];
        EXPECT_EQ(v1, v2);
      }
    }
  }

  for (std::size_t i = 0; i < sArr1Lin.dim3(); ++i) {
    for (std::size_t j = 0; j < sArr1Lin.dim2(); ++j) {
      for (std::size_t k = 0; k < sArr1Lin.dim1(); ++k) {
        auto v1 = sArr1Lin[{i, j, k}];
        auto v2 = m_arr1[{i, j, k}];
        EXPECT_EQ(v1, v2);
      }
    }
  }

  // Selective tests:

  auto sr1 = sArr1Lin[{0, 2, 1}];
  auto dr1 = dArr1Lin[{0, 2, 1}];
  auto sr1_lin = sArr1Lin.get_l((1 + 2 * 2));
  auto dr1_lin = dArr1Lin.get_l((1 + 2 * 2));
  EXPECT_EQ(sr1, (1 + 2 * 2));
  EXPECT_EQ(dr1, (1 + 2 * 2));
  EXPECT_EQ(sr1_lin, (1 + 2 * 2));
  EXPECT_EQ(dr1_lin, (1 + 2 * 2));
  EXPECT_EQ(sArr1Lin.calc_idx_l({0, 2, 1}), (1 + 2 * 2));
  EXPECT_EQ(sArr1Lin.calc_idx_l(0, 2, 1), (1 + 2 * 2));
  EXPECT_EQ(dArr1Lin.calc_idx_l({0, 2, 1}), (1 + 2 * 2));
  EXPECT_EQ(dArr1Lin.calc_idx_l(0, 2, 1), (1 + 2 * 2));
}

TEST(Array3D_General, Size_And_Dimensions) {
  constexpr Array3D<double, 3, 4, 2> arr1_1;

  EXPECT_EQ((Array3D<double, 3, 4, 2>::s_dim_count), 3);
  EXPECT_EQ(arr1_1.s_dim_count, 3);

  constexpr auto dsize1 = arr1_1.size();
  constexpr auto ssize1 = Array3D<double, 3, 4, 2>::size();
  EXPECT_EQ(dsize1, 24);
  EXPECT_EQ(ssize1, 24);

  // Use a reference
  constexpr auto& a1_1_Dims = arr1_1.dimensions();
  EXPECT_EQ(a1_1_Dims[0], 3);
  EXPECT_EQ(a1_1_Dims[1], 4);
  EXPECT_EQ(a1_1_Dims[2], 2);

  const Array3D<double, 4, 1, 5> arr1_2;

  constexpr auto dsize2 = arr1_2.size();
  constexpr auto ssize21 = Array3D<double, 4, 1, 5>::size();
  constexpr auto ssize22 = Array3D<double, 4, 5, 1>::size();
  EXPECT_EQ(dsize2, 20);
  EXPECT_EQ(ssize21, 20);
  EXPECT_EQ(ssize22, ssize21);

  // Make a copy
  std::array<std::size_t, 3> a1_2_Dims = arr1_2.dimensions();
  EXPECT_EQ(a1_2_Dims[0], 4);
  EXPECT_EQ(a1_2_Dims[1], 1);
  EXPECT_EQ(a1_2_Dims[2], 5);
}

}  // namespace utests