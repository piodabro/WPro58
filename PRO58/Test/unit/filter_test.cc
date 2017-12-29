#include "gtest/gtest.h"
extern "C" {
#include "filter.h"
}
#include <stdint.h>

TEST(FilterTest, AverageTest)
{
  int vals[5]={1,1,1,1,1};
  EXPECT_FLOAT_EQ(1, averageFilter(vals,5));
}
TEST(FilterTest, AverageZeroTest)
{
  int vals[5]={0};
  EXPECT_FLOAT_EQ(0, averageFilter(vals,5));
}
TEST(FilterTest, expInitTest)
{
  expFilter_t fil;
  expFilterInit(&fil, 10, 10);
  EXPECT_EQ(10,fil.gain);
  EXPECT_EQ(0,fil.state);
}
TEST(FilterTest, expInitFailTest)
{
  expFilter_t fil;
  EXPECT_DEATH( expFilterInit(&fil, 10, 1),".*");

}
TEST(FilterTest, expUpdate)
{
  expFilter_t fil;
  expFilterInit(&fil, 10, 10);
  expFilterUpdate(&fil, 10);
  EXPECT_EQ(10,fil.state);
}
TEST(FilterTest, expUpdateG0_5)
{
  expFilter_t fil;
  expFilterInit(&fil, 5, 10);
  expFilterUpdate(&fil, 10);
  EXPECT_EQ(5,fil.state);
  expFilterUpdate(&fil, 10);
  EXPECT_EQ(7,fil.state);
  expFilterUpdate(&fil, 10);
  EXPECT_EQ(8,fil.state);
  expFilterUpdate(&fil, 10);
  EXPECT_EQ(9,fil.state);
}
