
#include <diFieldFunctions.h>
#include <diMetConstants.h>

#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>

namespace /* anonymous */ {
const float UNDEF = 12356789, T0 = 273.15;
} // anonymous namespace

TEST(FieldFunctionsTest, ParseCompute)
{
  std::vector<std::string> lines, errors;

  lines.push_back("the=the.plevel_tk_rh(air_temperature:unit=kelvin,relative_humidity:unit=0.01)\n");
  lines.push_back("precipitation_percentile_1h_70=percentile(accumprecip.1h,70,10)\n");
  EXPECT_TRUE(FieldFunctions::parseComputeSetup(lines, errors));
  EXPECT_TRUE(errors.empty());
}

TEST(FieldFunctionsTest, ParseComputeBogus)
{
  std::vector<std::string> lines, errors;

  lines.push_back("fish=different(from,wood)\n");
  EXPECT_TRUE(FieldFunctions::parseComputeSetup(lines, errors));
  EXPECT_FALSE(errors.empty());
}

namespace {
struct levelhum_params_t {
  int cah; // 'compute' for alevelhum and hlevelhum
  int cp;  // 'compute' for plevelhum
  float t; // temperature input
  float humin; // humidity input
  float p; // pressure input
  float expect; // expected output
  float near; // max deviation from expected
};
} // namespace

TEST(FieldFunctionsTest, XLevelHum)
{
  FieldFunctions ff;

  const levelhum_params_t levelhum_params[] = {
    // alevelhum/hlevelhum and plevelhum have compute numbers >= 5 switched
    {  1,  1, 30.68+T0, .025, 1013, 91.9, 0.1 },
    {  2,  2, 302.71,   .025, 1013, 91.9, 0.1  },
    {  3,  3, 30.68+T0, 55,   1013, 0.014963, .000001 },
    {  4,  4, 302.71,   55,   1013, 0.014963, .000001 },
    {  5,  7, 30.68+T0, .015, 1013, 20.6, 0.1 },
    {  6,  8, 302.71,   .015, 1013, 20.6, 0.1 },
    {  7,  5, 30.68+T0, 55,   1013, 20.6, 0.1 },
    {  8,  6, 302.71,   55,   1013, 20.6, 0.1 },
    { -1, -1, 0, 0, 0, 0, 0 }
  };
  const float alevel = 0, blevel = 1;

  for (int i=0; levelhum_params[i].cah >= 0; ++i) {
    const levelhum_params_t& p = levelhum_params[i];

    float humout = 2*UNDEF;
    bool allDefined = true;
    EXPECT_TRUE(ff.alevelhum(p.cah, 1, 1, &p.t, &p.humin, &p.p, &humout, allDefined, UNDEF, "celsius"));
    EXPECT_NEAR(p.expect, humout, p.near) << "alevelhum C i=" << i << " compute=" << p.cah;
    EXPECT_TRUE(allDefined);

    humout = 2*UNDEF;
    allDefined = true;
    EXPECT_TRUE(ff.hlevelhum(p.cah, 1, 1, &p.t, &p.humin, &p.p, &humout, alevel, blevel, allDefined, UNDEF, "celsius"));
    EXPECT_NEAR(p.expect, humout, p.near) << "hlevelhum C i=" << i << " compute=" << p.cah;
    EXPECT_TRUE(allDefined);

    humout = 2*UNDEF;
    allDefined = true;
    EXPECT_TRUE(ff.plevelhum(p.cp, 1, 1, &p.t, &p.humin, &humout, p.p, allDefined, UNDEF, "celsius"));
    EXPECT_NEAR(p.expect, humout, p.near) << "plevelhum C i=" << i << " compute=" << p.cp;
    EXPECT_TRUE(allDefined);

    humout = 2*UNDEF;
    allDefined = false;
    EXPECT_TRUE(ff.alevelhum(p.cah, 1, 1, &p.t, &p.humin, &p.p, &humout, allDefined, UNDEF, "celsius"));
    EXPECT_NEAR(p.expect, humout, p.near) << "alevelhum C i=" << i << " compute=" << p.cah;
    EXPECT_FALSE(allDefined);

    humout = 2*UNDEF;
    allDefined = false;
    EXPECT_TRUE(ff.hlevelhum(p.cah, 1, 1, &p.t, &p.humin, &p.p, &humout, alevel, blevel, allDefined, UNDEF, "celsius"));
    EXPECT_NEAR(p.expect, humout, p.near) << "hlevelhum C i=" << i << " compute=" << p.cah;
    EXPECT_FALSE(allDefined);

    humout = 2*UNDEF;
    allDefined = false;
    EXPECT_TRUE(ff.plevelhum(p.cp, 1, 1, &p.t, &p.humin, &humout, p.p, allDefined, UNDEF, "celsius"));
    EXPECT_NEAR(p.expect, humout, p.near) << "plevelhum C i=" << i << " compute=" << p.cp;
    EXPECT_FALSE(allDefined);

    if (p.cah < 5)
      continue;

    allDefined = true;
    EXPECT_TRUE(ff.alevelhum(p.cah, 1, 1, &p.t, &p.humin, &p.p, &humout, allDefined, UNDEF, "kelvin"));
    EXPECT_NEAR(p.expect+T0, humout, p.near) << "alevelhum K i=" << i << " compute=" << p.cah;
    EXPECT_TRUE(allDefined);

    allDefined = true;
    EXPECT_TRUE(ff.hlevelhum(p.cah, 1, 1, &p.t, &p.humin, &p.p, &humout, alevel, blevel, allDefined, UNDEF, "kelvin"));
    EXPECT_NEAR(p.expect+T0, humout, p.near) << "hlevelhum K i=" << i << " compute=" << p.cah;
    EXPECT_TRUE(allDefined);

    allDefined = true;
    EXPECT_TRUE(ff.plevelhum(p.cp, 1, 1, &p.t, &p.humin, &humout, p.p, allDefined, UNDEF, "kelvin"));
    EXPECT_NEAR(p.expect+T0, humout, p.near) << "plevelhum K i=" << i << " compute=" << p.cp;
    EXPECT_TRUE(allDefined);
  }
}

TEST(FieldFunctionsTest, ALevelTempPerformance)
{
  FieldFunctions ff;
  const float UNDEF = 1e30, T0 = 273.15;
  bool allDefined = true;

  { // compute == 3, T(Kelvin) -> TH
    const int N = 719*929; //1000000;
    const float F = 0.00001;
    float *tk = new float[N], *p = new float[N], *th = new float[N], *expected = new float[N];
    for (int i=0; i<N; ++i) {
      tk[i] = 20 + (i*F) + T0;
      p[i]  = 1005 + (i*F);
      th[i] = 2*UNDEF;
    }
    for (int i=0; i<1; ++i)
      EXPECT_TRUE(ff.aleveltemp(3, 1, N, tk, p, th, allDefined, UNDEF, "kelvin"));
    for (int i=0; i<N; ++i) {
      const float ex = tk[i] / powf(p[i] * MetNo::Constants::p0inv, MetNo::Constants::kappa);
      EXPECT_EQ(ex, th[i]);
    }
    delete[] tk;
    delete[] p;
    delete[] th;
    delete[] expected;
  }
}

namespace {
struct oper_params_t {
  int c; // 'compute'
  float a, b; // input
  bool input_defined;
  float expect; // expected output
  bool expect_defined;
};

enum { PLUS = 1, MINUS = 2, MUL = 3, DIV = 4 };

const oper_params_t oper_params[] = {
  { PLUS, 1, 3, true,   4, true },
  { PLUS, 1, 3, false,  4, false },
  { MINUS, 1, 3, true,  -2, true },
  { MINUS, 1, 3, false, -2, false },
  { MUL, 1.5, 3, true,  4.5, true },
  { MUL, 1.5, 3, false, 4.5, false },
  { DIV, 3, 1.5, true,  2, true },
  { DIV, 3, 1.5, false, 2, false },
  { DIV, 3, 0, true,  UNDEF, false },
  { DIV, 3, 0, false, UNDEF, false },
  { -1, 0, 0, false, 0, false }
};
} // namespace

TEST(FieldFunctionsTest, XOperX)
{
  float out;
  bool allDefined;
  
  for (int i=0; oper_params[i].c > 0; ++i) {
    const oper_params_t& op = oper_params[i];

    out = 2*UNDEF;
    allDefined = op.input_defined;
    EXPECT_TRUE(FieldFunctions::fieldOPERfield(op.c, 1, 1, &op.a, &op.b, &out, allDefined, UNDEF));
    EXPECT_EQ(op.expect_defined, allDefined);
    EXPECT_NEAR(op.expect, out, 1e-6) << " fOPf i=" << i << " c=" << op.c << " a=" << op.a << " b=" << op.b;
  
    out = 2*UNDEF;
    allDefined = op.input_defined;
    EXPECT_TRUE(FieldFunctions::fieldOPERconstant(op.c, 1, 1, &op.a, op.b, &out, allDefined, UNDEF));
    EXPECT_EQ(op.expect_defined, allDefined);
    EXPECT_NEAR(op.expect, out, 1e-6) << " fOPc i=" << i << " c=" << op.c << " a=" << op.a << " b=" << op.b;
  
    out = 2*UNDEF;
    allDefined = op.input_defined;
    EXPECT_TRUE(FieldFunctions::constantOPERfield(op.c, 1, 1, op.a, &op.b, &out, allDefined, UNDEF));
    EXPECT_EQ(op.expect_defined, allDefined);
    EXPECT_NEAR(op.expect, out, 1e-6) << " cOPf i=" << i << " c=" << op.c << " a=" << op.a << " b=" << op.b;
  }
}
