//
// Created by ruslan on 10.02.23.
//

#include <gtest/gtest.h>

#include <cmath>

#include "tests.h"

namespace {

TEST_F(ModelTest, validate_00) {
  model_.SetExpression("(1*");
  EXPECT_THROW(model_.Evaluate(NAN), std::invalid_argument);
}

TEST_F(ModelTest, validate_01) {
  model_.SetExpression("%(2");
  EXPECT_THROW(model_.Evaluate(NAN), std::invalid_argument);
}

TEST_F(ModelTest, validate_02) {
  model_.SetExpression("(%(2)");
  EXPECT_THROW(model_.Evaluate(NAN), std::invalid_argument);
}

TEST_F(ModelTest, validate_03) {
  model_.SetExpression("s(-");
  EXPECT_THROW(model_.Evaluate(NAN), std::invalid_argument);
}

TEST_F(ModelTest, validate_04) {
  model_.SetExpression("(1)*");
  EXPECT_THROW(model_.Evaluate(NAN), std::invalid_argument);
}

TEST_F(ModelTest, validate_05) {
  model_.SetExpression("(");
  EXPECT_THROW(model_.Evaluate(NAN), std::invalid_argument);
}

TEST_F(ModelTest, calculation_00) {
  model_.SetExpression("-1/(4-5)");
  EXPECT_EQ(model_.Evaluate(NAN), 1);
}

TEST_F(ModelTest, calculation_01) {
  model_.SetExpression(".2+.");
  EXPECT_NEAR(model_.Evaluate(NAN), 0.2, eps);
}

TEST_F(ModelTest, calculation_02) {
  model_.SetExpression("+6-sqrt(16+9)+(+14)");
  EXPECT_NEAR(model_.Evaluate(NAN), 15, eps);
}

TEST_F(ModelTest, calculation_03) {
  model_.SetExpression("12%(-1+2^3)");
  EXPECT_NEAR(model_.Evaluate(NAN), 5, eps);
}

TEST_F(ModelTest, calculation_04) {
  model_.SetExpression("1*2+X");
  EXPECT_THROW(model_.Evaluate(NAN), std::invalid_argument);
  EXPECT_NEAR(model_.Evaluate(2), 4, eps);
}

TEST_F(ModelTest, calculation_05) {
  model_.SetExpression("1+X-(1*(sqrt(25)");
  EXPECT_NEAR(model_.Evaluate(5), 1, eps);
}

TEST_F(ModelTest, calculation_06) {
  model_.SetExpression("2-2.00000002");
  EXPECT_NEAR(model_.Evaluate(5), 0, eps);
}
}  // namespace