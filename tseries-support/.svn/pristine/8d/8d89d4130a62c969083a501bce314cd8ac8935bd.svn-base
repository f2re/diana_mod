
#include "miCoordinates.h"

#include <gtest/gtest.h>

#include <cmath>
static const float BLINDERN_LON = 10.72005f, BLINDERN_LAT = 59.9423f;
static const float FANNARAK_LON =  7.9058f,  FANNARAK_LAT = 61.5158f;
static const LonLat blindern = LonLat::fromDegrees(BLINDERN_LON, BLINDERN_LAT);
static const LonLat fannarak = LonLat::fromDegrees(FANNARAK_LON, FANNARAK_LAT);

static double r2d(double radian)
{
  return radian * 180 / M_PI;
}

static double d2r(double degrees)
{
  return degrees * M_PI / 180;
}

double dms2r(int deg, int min, int sec)
{
  return d2r(deg + (min/60.0) + (sec/3600.0));
}

TEST(LonLatTest, Basic)
{
  EXPECT_FLOAT_EQ(BLINDERN_LON, r2d(blindern.lon()));
  EXPECT_FLOAT_EQ(BLINDERN_LAT, r2d(blindern.lat()));
}


TEST(LonLatTest, DistanceTo)
{
  EXPECT_NEAR(232400, blindern.distanceTo(fannarak), 100);
  EXPECT_NEAR(222400, LonLat::fromDegrees(10, 89).distanceTo(LonLat::fromDegrees(190, 89)), 100);

  EXPECT_EQ(LonLat::fromDegrees(10, 60).distanceTo(LonLat::fromDegrees(10, 70)),
      LonLat::fromDegrees(0, 60).distanceTo(LonLat::fromDegrees(0, 70)));
  EXPECT_GT(LonLat::fromDegrees(10, 60).distanceTo(LonLat::fromDegrees(20, 60)),
      LonLat::fromDegrees(10, 70).distanceTo(LonLat::fromDegrees(20, 70)));
  EXPECT_EQ(LonLat::fromDegrees(10, 60).distanceTo(LonLat::fromDegrees(20, 70)),
      LonLat::fromDegrees(20, 60).distanceTo(LonLat::fromDegrees(10, 70)));
  EXPECT_EQ(LonLat::fromDegrees(175, -5).distanceTo(LonLat::fromDegrees(-175, 5)),
      LonLat::fromDegrees(-175, -5).distanceTo(LonLat::fromDegrees(175, 5)));
}

TEST(LonLatTest, BearingTo)
{
  const float D = 0.1;
  EXPECT_NEAR( 90, r2d(blindern.bearingTo(LonLat::fromDegrees(BLINDERN_LON+D, BLINDERN_LAT))), 0.25);
  EXPECT_NEAR(270, r2d(blindern.bearingTo(LonLat::fromDegrees(BLINDERN_LON-D, BLINDERN_LAT))), 0.25);

  EXPECT_EQ(  0, r2d(blindern.bearingTo(LonLat::fromDegrees(BLINDERN_LON, BLINDERN_LAT+D))));
  EXPECT_EQ(180, r2d(blindern.bearingTo(LonLat::fromDegrees(BLINDERN_LON, BLINDERN_LAT-D))));

  EXPECT_NEAR(320, r2d(blindern.bearingTo(fannarak)), 0.25);

  // next test disabled as it has roundoff problems (it might return 360deg)
  //EXPECT_NEAR(0, r2d(LonLat::fromDegrees(10, 89).bearingTo(LonLat::fromDegrees(190, 89))), 1e-5);
  EXPECT_NEAR(5, r2d(LonLat::fromDegrees(10, 89).bearingTo(LonLat::fromDegrees(180, 89))), 0.25);

  EXPECT_NEAR(180, r2d(LonLat::fromDegrees(190, -89).bearingTo(LonLat::fromDegrees( 10, -89))), 1e-5);
  EXPECT_NEAR(180, r2d(LonLat::fromDegrees( 10, -89).bearingTo(LonLat::fromDegrees(190, -89))), 1e-5);
  EXPECT_NEAR(185, r2d(LonLat::fromDegrees(180, -89).bearingTo(LonLat::fromDegrees( 10, -89))), 0.25);
}

TEST(MiCoordinatesTest, Distance)
{
  const miCoordinates bl(BLINDERN_LON, BLINDERN_LAT);
  const miCoordinates fa(FANNARAK_LON, FANNARAK_LAT);

  EXPECT_NEAR(232400, bl.distanceTo(fa), 100); // m
  EXPECT_NEAR(232,    bl.distance  (fa), 1); // km!
}

TEST(MiCoordinatesTest, StepDirection)
{
  const LonLat step100 = blindern.stepDirection(100000, d2r(320));
  EXPECT_NEAR(dms2r( 9,32,29), step100.lon(), 1e-4);
  EXPECT_NEAR(dms2r(60,37,34), step100.lat(), 1e-4);
}

TEST(MiCoordinatesTest, StepTo)
{
  const LonLat step50 = blindern.stepTo(50000, fannarak);
  EXPECT_NEAR(dms2r(10, 8,13), step50.lon(), 1e-4);
  EXPECT_NEAR(dms2r(60,17, 8), step50.lat(), 1e-4);
}
