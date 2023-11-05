//
// Created by ruslan on 29.05.23.
//

#ifndef SMARTCALC2_TESTS_H
#define SMARTCALC2_TESTS_H

#include <gtest/gtest.h>

#include "model.h"

class ModelTest : public ::testing::Test {
 protected:
  static constexpr double eps = 1e-7;
  static constexpr double fin_eps = 1;
  s21::model model_;

  void SetUp() override {}

  void TearDown() override {}
};

#endif  // SMARTCALC2_TESTS_H
