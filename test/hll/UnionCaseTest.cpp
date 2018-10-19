/*
 * Copyright 2018, Oath Inc. Licensed under the terms of the
 * Apache License 2.0. See LICENSE file at the project root for terms.
 */

#include "hll.hpp"
#include "HllUnion.hpp"
#include "HllUtil.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace datasketches {

class HllUnionTest : public CppUnit::TestFixture {

  static uint64_t v;

  CPPUNIT_TEST_SUITE(HllUnionTest);
  CPPUNIT_TEST(checkCase0);
  CPPUNIT_TEST(checkCase1);
  CPPUNIT_TEST(checkCase2);
  CPPUNIT_TEST(checkCase2B);
  CPPUNIT_TEST(checkCase4);
  CPPUNIT_TEST(checkCase5);
  CPPUNIT_TEST(checkCase6);
  CPPUNIT_TEST(checkCase6B);
  CPPUNIT_TEST(checkCase8);
  CPPUNIT_TEST(checkCase9);
  CPPUNIT_TEST(checkCase10);
  CPPUNIT_TEST(checkCase10B);
  CPPUNIT_TEST(checkCase12);
  CPPUNIT_TEST(checkCase13);
  CPPUNIT_TEST(checkCase14);
  CPPUNIT_TEST(checkCase14B);
  CPPUNIT_TEST_SUITE_END();

  void checkCase0() { // src: LIST, gadget: LIST, cases 0, 0
    int n1 = 2;
    int n2 = 3;
    int n3 = 2;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2);
    HllSketch* h3 = buildSketch(10, HLL_8, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase1() { // src: SET, gadget: LIST, cases 0, 1
    int n1 = 5;
    int n2 = 2;
    int n3 = 16;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1); // LIST, 5
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2); // LIST, 2
    HllSketch* h3 = buildSketch(10, HLL_8, n3); // SET
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase2() { // src: HLL, gadget: LIST, swap, cases 0, 2
    int n1 = 5;
    int n2 = 2;
    int n3 = 97;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_8, n2);
    HllSketch* h3 = buildSketch(10, HLL_4, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(10, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase2B() { // src: HLL, gadget: LIST, swap, cases 0, 2; different lgKs
    int n1 = 5;
    int n2 = 2;
    int n3 = 769;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_8, n2);
    HllSketch* h3 = buildSketch(13, HLL_4, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase4() { // src: LIST, gadget: SET, cases 0, 4
    int n1 = 6;
    int n2 = 10;
    int n3 = 6;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2);
    HllSketch* h3 = buildSketch(10, HLL_8, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase5() { // src: SET, gadget: SET, cases 0, 5
    int n1 = 6;
    int n2 = 10;
    int n3 = 10;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2);
    HllSketch* h3 = buildSketch(10, HLL_8, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase6() { // src: HLL, gadget: SET, swap, cases 1, 6
    int n1 = 2;
    int n2 = 192;
    int n3 = 97;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_8, n2);
    HllSketch* h3 = buildSketch(10, HLL_4, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(10, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase6B() { // src: HLL, gadget: SET, swap, downsample, cases 1, 6
    int n1 = 2;
    int n2 = 20;
    int n3 = 769;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_8, n2);
    HllSketch* h3 = buildSketch(13, HLL_4, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase8() { // src: LIST, gadget: HLL, cases 2 (swap), 8
    int n1 = 6;
    int n2 = 193;
    int n3 = 7;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2);
    HllSketch* h3 = buildSketch(10, HLL_8, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(11, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase9() { // src: SET, gadget: HLL, cases 2 (swap), 9
    int n1 = 6;
    int n2 = 193;
    int n3 = 16;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2);
    HllSketch* h3 = buildSketch(10, HLL_8, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(11, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase10() { // src: HLL, gadget: HLL, cases 2 (swap), 10
    int n1 = 6;
    int n2 = 193;
    int n3 = 193;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2);
    HllSketch* h3 = buildSketch(10, HLL_8, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(10, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase10B() { // src: HLL, gadget: HLL, cases 2 (swap), 10, copy to HLL_8
    int n1 = 6;
    int n2 = 193;
    int n3 = 193;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1);
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2);
    HllSketch* h3 = buildSketch(11, HLL_8, n3);
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(11, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase12() { // src: LIST, gadget: empty, case 12
    int n1 = 0;
    int n2 = 0;
    int n3 = 7;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1); // LIST empty
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2); // LIST empty, ignored
    HllSketch* h3 = buildSketch(10, HLL_8, n3); // LIST
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase13() { // src: SET, gadget: empty, case 13
    int n1 = 0;
    int n2 = 0;
    int n3 = 16;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1); // LIST empty
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2); // LIST empty, ignored
    HllSketch* h3 = buildSketch(10, HLL_8, n3); // SET
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::SET, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase14() { // src: HLL, gadget: empty, case 14
    int n1 = 0;
    int n2 = 0;
    int n3 = 97;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1); // LIST empty
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2); // LIST empty, ignored
    HllSketch* h3 = buildSketch(10, HLL_8, n3); // LIST
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(10, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  void checkCase14B() { // src: HLL, gadget: empty, case 14, downsample
    int n1 = 0;
    int n2 = 0;
    int n3 = 395;
    int sum = n1 + n2 + n3;
    HllUnion* u = buildUnion(12, n1); // LIST empty
    HllUnionPvt* uPvt = static_cast<HllUnionPvt*>(u);
    HllSketch* h2 = buildSketch(11, HLL_6, n2); // LIST empty, ignored
    HllSketch* h3 = buildSketch(12, HLL_8, n3); // LIST
    u->update(h2);
    CPPUNIT_ASSERT_EQUAL(CurMode::LIST, uPvt->getCurrentMode());
    u->update(h3);
    CPPUNIT_ASSERT_EQUAL(CurMode::HLL, uPvt->getCurrentMode());
    CPPUNIT_ASSERT_EQUAL(12, u->getLgConfigK());
    CPPUNIT_ASSERT_EQUAL(false, uPvt->isOutOfOrderFlag());
    double err = sum * errorFactor(uPvt->getLgConfigK(), uPvt->isOutOfOrderFlag(), 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, u->getEstimate(), err);
    delete u;
    delete h2;
    delete h3;
  }

  double errorFactor(int lgK, bool oooFlag, double numStdDev) {
    if (oooFlag) {
      return (1.2 * numStdDev) / sqrt(1 << lgK);
    } else {
      return (0.9 * numStdDev) / sqrt(1 << lgK);
    }
  }

  HllUnion* buildUnion(int lgMaxK, int n) {
    HllUnion* u = HllUnion::newInstance(lgMaxK);
    for (int i = 0; i < n; ++i) { u->update(i + v); }
    v += n;
    return u;
  }

  HllSketch* buildSketch(int lgK, TgtHllType type, int n) {
    HllSketch* sk = HllSketch::newInstance(lgK);
    for (int i = 0; i < n; ++i) { sk->update(i); }
    v += n;
    return sk;
  }

};

uint64_t HllUnionTest::v = 0;

CPPUNIT_TEST_SUITE_REGISTRATION(HllUnionTest);

} /* namespace datasketches */
