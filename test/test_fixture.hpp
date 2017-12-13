/***************************************************************************
* Copyright (c) 2017, Johan Mabille, Sylvain Corlay and Wolf Vollprecht    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <cstddef>
#include "xtl/xbasic_fixed_string.hpp"
#include "xframe/xvariable.hpp"

namespace xf
{
    using fstring = xtl::xfixed_string<55>;
    using saxis_type = xaxis<fstring, std::size_t>; 
    using iaxis_type = xaxis<int, std::size_t>;
    using dimension_type = xdimension<fstring, std::size_t>;
    using data_type = xt::xoptional_assembly<xt::xarray<double>, xt::xarray<bool>>;
    using coordinate_type = xcoordinate<fstring, data_type::size_type>;
    using variable_type = xvariable<fstring, data_type::value_expression, data_type::flag_expression>;

    /********
     * axes *
     ********/

    // { "a", "c", "d" }
    inline saxis_type make_test_saxis()
    {
        return saxis_type({"a", "c", "d"});
    }

    // { "a", "d", "e" }
    inline saxis_type make_test_saxis2()
    {
        return saxis_type({"a", "d", "e"});
    }

    // { 1, 2, 4 }
    inline iaxis_type make_test_iaxis()
    {
        return iaxis_type({1, 2, 4});
    }

    // { 1, 4, 5 }
    inline iaxis_type make_test_iaxis2()
    {
        return iaxis_type({1, 4, 5});
    }

    /***************
     * coordinates *
     ***************/

    // abscissa: { "a", "c", "d" }
    // ordinate: { 1, 2, 4 }
    inline coordinate_type make_test_coordinate()
    {
        fstring s1 = "abscissa";
        fstring s2 = "ordinate";
        return coordinate(std::make_pair(s1, make_test_saxis()), std::make_pair(s2, make_test_iaxis()));
    }

    // abscissa: { "a", "d", "e" }
    // ordinate: { 1, 4, 5 }
    inline coordinate_type make_test_coordinate2()
    {
        fstring s1 = "abscissa";
        fstring s2 = "ordinate";
        return coordinate(std::make_pair(s1, make_test_saxis2()), std::make_pair(s2, make_test_iaxis2()));
    }

    // abscissa: { "a", "d", "e" }
    // ordinate: { 1, 4, 5 }
    // altitude: { 1, 2, 4 }
    inline coordinate_type make_test_coordinate3()
    {
        fstring s1 = "abscissa";
        fstring s2 = "ordinate";
        fstring s3 = "altitude";
        return coordinate(std::make_pair(s1, make_test_saxis2()),
                          std::make_pair(s2, make_test_iaxis2()),
                          std::make_pair(s3, make_test_iaxis()));
    }

    // abscissa: { "a", "c", "d", "e" }
    // ordinate: { 1, 2, 4, 5 }
    // altitude: { 1, 2, 4 }
    inline coordinate_type make_merge_coordinate()
    {
        saxis_type sres = { "a", "c", "d", "e" };
        iaxis_type ires = { 1, 2, 4, 5 };
        return coordinate(std::make_pair(fstring("abscissa"), std::move(sres)),
                          std::make_pair(fstring("ordinate"), std::move(ires)),
                          std::make_pair(fstring("altitude"), make_test_iaxis()));
    }

    // abscissa: { "a", "d" }
    // ordinate: { 1, 4 }
    // altitude: { 1, 2, 4 }
    inline coordinate_type make_intersect_coordinate()
    {
        saxis_type sres = { "a", "d" };
        iaxis_type ires = { 1, 4 };
        return coordinate(std::make_pair(fstring("abscissa"), std::move(sres)),
                          std::make_pair(fstring("ordinate"), std::move(ires)),
                          std::make_pair(fstring("altitude"), make_test_iaxis()));
    }

    /*************
     * variables *
     *************/

    // data = {{ 1. ,  2., N/A },
    //         { N/A,  5.,  6. },
    //         { 7. ,  8.,  9. }}
    inline data_type make_test_data()
    {
        data_type d = {{ 1., 2., 3.},
                       { 4., 5., 6.},
                       { 7., 8., 9.}};
        d(0, 2).has_value() = false;
        d(1, 0).has_value() = false;
        return d;
    }

    inline data_type make_test_data2()
    {
        data_type d2 = {{{ 1., 2., 3.},
                         { 4., 5., 6.},
                         { 7., 8., 9.}},
                        {{ 11., 12., 13.},
                         { 14., 15., 16.},
                         { 17., 18., 19.}},
                        {{ 21., 22., 23.},
                         { 24., 25., 26.},
                         { 27., 28., 29.}}};
        d2(0, 0, 2).has_value() = false;
        d2(1, 1, 0).has_value() = false;
        return d2;
    }

    // abscissa: { "a", "c", "d" }
    // ordinate: { 1, 2, 4 }
    // dims: {{ "abscissa", 0 }, { "ordinate", 1 }}
    // data = {{ 1. ,  2., N/A },
    //         { N/A,  5.,  6. },
    //         { 7. ,  8.,  9. }}
    inline variable_type make_test_variable()
    {
        return variable_type(make_test_data(), make_test_coordinate(), dimension_type({"abscissa", "ordinate"}));
    }

    // abscissa: { "a", "d", "e" }
    // ordinate: { 1, 4, 5 }
    // altitude: { 1, 2, 4 }
    // dims: {{ "abscissa", 0 }, { "ordinate", 1 }, {"altitude", 2}}
    // data = make_test_data2
    inline variable_type make_test_variable2()
    {
        return variable_type(make_test_data2(), make_test_coordinate3(), dimension_type({"abscissa", "ordinate", "altitude"}));
    }

    // abscissa: { "a", "d", "e" }
    // ordinate: { 1, 4, 5 }
    // dims: {{ "abscissa", 0 }, { "ordinate", 1 }}
    // data = {{ 1. ,  2., N/A },
    //         { N/A,  5.,  6. },
    //         { 7. ,  8.,  9. }}
    inline variable_type make_test_variable3()
    {
        return variable_type(make_test_data(), make_test_coordinate2(), dimension_type({"abscissa", "ordinate"}));
    }

    // altitude: { 1, 2, 4 }
    // abscissa: { "a", "d", "e" }
    // ordinate: { 1, 4, 5 }
    // dims: {{"altitude", 0}, { "abscissa", 1 }, { "ordinate", 2 }} 
    // data = make_test_data2
    inline variable_type make_test_variable4()
    {
        return variable_type(make_test_data2(), make_test_coordinate3(), dimension_type({"altitude", "abscissa", "ordinate"}));
    }

    /*************
     * selectors *
     *************/

    using dict_type = typename variable_type::selector_map_type<>;
    using selector_list = std::vector<dict_type>;

    inline selector_list make_selector_list_aa()
    {
        selector_list sl(9);
        sl[0] = { { "abscissa", "a" },{ "ordinate", 1 } };
        sl[1] = { { "abscissa", "a" },{ "ordinate", 2 } };
        sl[2] = { { "abscissa", "a" },{ "ordinate", 4 } };
        sl[3] = { { "abscissa", "c" },{ "ordinate", 1 } };
        sl[4] = { { "abscissa", "c" },{ "ordinate", 2 } };
        sl[5] = { { "abscissa", "c" },{ "ordinate", 4 } };
        sl[6] = { { "abscissa", "d" },{ "ordinate", 1 } };
        sl[7] = { { "abscissa", "d" },{ "ordinate", 2 } };
        sl[8] = { { "abscissa", "d" },{ "ordinate", 4 } };
        return sl;
    }

    inline selector_list make_selector_list_ab()
    {
        selector_list sl(12);
        sl[0] = { { "abscissa", "a" },{ "ordinate", 1 },{ "altitude", 1 } };
        sl[1] = { { "abscissa", "a" },{ "ordinate", 1 },{ "altitude", 2 } };
        sl[2] = { { "abscissa", "a" },{ "ordinate", 1 },{ "altitude", 4 } };
        sl[3] = { { "abscissa", "a" },{ "ordinate", 4 },{ "altitude", 1 } };
        sl[4] = { { "abscissa", "a" },{ "ordinate", 4 },{ "altitude", 2 } };
        sl[5] = { { "abscissa", "a" },{ "ordinate", 4 },{ "altitude", 4 } };
        sl[6] = { { "abscissa", "d" },{ "ordinate", 1 },{ "altitude", 1 } };
        sl[7] = { { "abscissa", "d" },{ "ordinate", 1 },{ "altitude", 2 } };
        sl[8] = { { "abscissa", "d" },{ "ordinate", 1 },{ "altitude", 4 } };
        sl[9] = { { "abscissa", "d" },{ "ordinate", 4 },{ "altitude", 1 } };
        sl[10] = { { "abscissa", "d" },{ "ordinate", 4 },{ "altitude", 2 } };
        sl[11] = { { "abscissa", "d" },{ "ordinate", 4 },{ "altitude", 4 } };
        return sl;
    }

    inline selector_list make_selector_list_cd()
    {
        selector_list sl(27);
        sl[0] = { { "altitude", 1 },{ "abscissa", "a" },{ "ordinate", 1 } };
        sl[1] = { { "altitude", 1 },{ "abscissa", "a" },{ "ordinate", 4 } };
        sl[2] = { { "altitude", 1 },{ "abscissa", "a" },{ "ordinate", 5 } };
        sl[3] = { { "altitude", 1 },{ "abscissa", "d" },{ "ordinate", 1 } };
        sl[4] = { { "altitude", 1 },{ "abscissa", "d" },{ "ordinate", 4 } };
        sl[5] = { { "altitude", 1 },{ "abscissa", "d" },{ "ordinate", 5 } };
        sl[6] = { { "altitude", 1 },{ "abscissa", "e" },{ "ordinate", 1 } };
        sl[7] = { { "altitude", 1 },{ "abscissa", "e" },{ "ordinate", 4 } };
        sl[8] = { { "altitude", 1 },{ "abscissa", "e" },{ "ordinate", 5 } };
        sl[9] = { { "altitude", 2 },{ "abscissa", "a" },{ "ordinate", 1 } };
        sl[10] = { { "altitude", 2 },{ "abscissa", "a" },{ "ordinate", 4 } };
        sl[11] = { { "altitude", 2 },{ "abscissa", "a" },{ "ordinate", 5 } };
        sl[12] = { { "altitude", 2 },{ "abscissa", "d" },{ "ordinate", 1 } };
        sl[13] = { { "altitude", 2 },{ "abscissa", "d" },{ "ordinate", 4 } };
        sl[14] = { { "altitude", 2 },{ "abscissa", "d" },{ "ordinate", 5 } };
        sl[15] = { { "altitude", 2 },{ "abscissa", "e" },{ "ordinate", 1 } };
        sl[16] = { { "altitude", 2 },{ "abscissa", "e" },{ "ordinate", 4 } };
        sl[17] = { { "altitude", 2 },{ "abscissa", "e" },{ "ordinate", 5 } };
        sl[18] = { { "altitude", 4 },{ "abscissa", "a" },{ "ordinate", 1 } };
        sl[19] = { { "altitude", 4 },{ "abscissa", "a" },{ "ordinate", 4 } };
        sl[20] = { { "altitude", 4 },{ "abscissa", "a" },{ "ordinate", 5 } };
        sl[21] = { { "altitude", 4 },{ "abscissa", "d" },{ "ordinate", 1 } };
        sl[22] = { { "altitude", 4 },{ "abscissa", "d" },{ "ordinate", 4 } };
        sl[23] = { { "altitude", 4 },{ "abscissa", "d" },{ "ordinate", 5 } };
        sl[24] = { { "altitude", 4 },{ "abscissa", "e" },{ "ordinate", 1 } };
        sl[25] = { { "altitude", 4 },{ "abscissa", "e" },{ "ordinate", 4 } };
        sl[26] = { { "altitude", 4 },{ "abscissa", "e" },{ "ordinate", 5 } };
        return sl;
    }

#define DEFINE_TEST_VARIABLES()                                           \
        variable_type a = make_test_variable();                           \
        variable_type b = make_test_variable2();                          \
        variable_type c = make_test_variable3();                          \
        variable_type d = make_test_variable4()

#define CHECK_EQUALITY(RES, A, B, SL, OP)                                 \
    for(std::size_t i = 0; i < sl.size(); ++i)                            \
    {                                                                     \
        EXPECT_EQ(RES.select(SL[i]), A.select(SL[i]) OP B.select(SL[i])); \
    }

#define CHECK_SCALAR2_EQUALITY(RES, A, B, SL, OP)                         \
    for(std::size_t i = 0; i < sl.size(); ++i)                            \
    {                                                                     \
        EXPECT_EQ(RES.select(SL[i]), A.select(SL[i]) OP B); \
    }

#define CHECK_SCALAR1_EQUALITY(RES, A, B, SL, OP)                         \
    for(std::size_t i = 0; i < sl.size(); ++i)                            \
    {                                                                     \
        EXPECT_EQ(RES.select(SL[i]), A OP B.select(SL[i])); \
    }
}
