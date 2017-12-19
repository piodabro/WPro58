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
  expFilterInit(&fil, 1);
  EXPECT_FLOAT_EQ(1,fil.gain);
  EXPECT_FLOAT_EQ(0,fil.state);
}
TEST(FilterTest, expUpdate)
{
  expFilter_t fil;
  expFilterInit(&fil, 1);
  expFilterUpdate(&fil, 10.0);
  EXPECT_FLOAT_EQ(10,fil.state);
}
TEST(FilterTest, expUpdateG0_5)
{
  expFilter_t fil;
  expFilterInit(&fil, 0.5);
  expFilterUpdate(&fil, 10.0);
  EXPECT_FLOAT_EQ(5,fil.state);
}
