/***************************************************************************
* Copyright (c) 2017, Johan Mabille, Sylvain Corlay and Wolf Vollprecht    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"
#include "xframe/xdimension.hpp"
#include "xtl/xbasic_fixed_string.hpp"

namespace xf
{
    using fstring = xtl::xfixed_string<55>;
    using label_type = std::vector<fstring>;
    using dimension_type = xdimension<fstring, std::size_t>;
    
    TEST(xdimension, broadcast)
    {
        dimension_type d1 = { "a", "b", "d", "e" };
        dimension_type d2 = { "d", "e" };
        dimension_type d3 = { "h", "c", "a", "b", "d", "e" };
        dimension_type d4 = { "h", "b", "c", "e" };

        dimension_type res1;
        bool t1 = broadcast_dimensions(res1, d1, d2);
        EXPECT_TRUE(t1);
        EXPECT_EQ(res1.size(), 4u);
        EXPECT_EQ(res1["a"], 0);
        EXPECT_EQ(res1["b"], 1);
        EXPECT_EQ(res1["d"], 2);
        EXPECT_EQ(res1["e"], 3);

        dimension_type res2;
        bool t2 = broadcast_dimensions(res2, d1, d3);
        EXPECT_TRUE(t2);
        EXPECT_EQ(res2.size(), 6u);
        EXPECT_EQ(res2["h"], 0);
        EXPECT_EQ(res2["c"], 1);
        EXPECT_EQ(res2["a"], 2);
        EXPECT_EQ(res2["b"], 3);
        EXPECT_EQ(res2["d"], 4);
        EXPECT_EQ(res2["e"], 5);

        dimension_type res3;
        bool t3 = broadcast_dimensions(res3, d1, d4);
        EXPECT_FALSE(t3);
        EXPECT_EQ(res3.size(), 6u);
        EXPECT_EQ(res3["h"], 0);
        EXPECT_EQ(res3["c"], 1);
        EXPECT_EQ(res3["a"], 2);
        EXPECT_EQ(res3["b"], 3);
        EXPECT_EQ(res3["d"], 4);
        EXPECT_EQ(res3["e"], 5);
    }
}

