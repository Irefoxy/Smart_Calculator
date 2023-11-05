//
// Created by ruslan on 08.03.23.
//
#include <gtest/gtest.h>

#include <cmath>

#include "tests.h"

namespace {
TEST_F(ModelTest, credit_00) {
  double loan_amount = 300000.0;
  unsigned loan_term = 12;
  double rate = 6.65;
  bool annuity = true;
  s21::model::vector info =
      s21::model::CalculateCredit(loan_amount, loan_term, rate, annuity);

  EXPECT_EQ(info.size(), 1);
  EXPECT_NEAR(info.front(), 25909.64, fin_eps);
}

TEST_F(ModelTest, credit_01) {
  double loan_amount = 300000.0;
  unsigned loan_term = 11;
  double rate = 6.65;
  bool annuity = false;
  s21::model::vector info =
      s21::model::CalculateCredit(loan_amount, loan_term, rate, annuity);
  EXPECT_EQ(info.size(), loan_term);
  EXPECT_NEAR(info[0], 28967.11, fin_eps);
  EXPECT_NEAR(info[1], 28813.08, fin_eps);
  EXPECT_NEAR(info[2], 28659.04, fin_eps);
  EXPECT_NEAR(info[3], 28465.26, fin_eps);
  EXPECT_NEAR(info[4], 28350.97, fin_eps);
  EXPECT_NEAR(info[5], 28167.12, fin_eps);
  EXPECT_NEAR(info[6], 28042.90, fin_eps);
  EXPECT_NEAR(info[7], 27887.18, fin_eps);
  EXPECT_NEAR(info[8], 27703.83, fin_eps);
  EXPECT_NEAR(info[9], 27579.96, fin_eps);
  EXPECT_NEAR(info[10], 27421.39, fin_eps);
}

TEST_F(ModelTest, deposit_00) {
  std::tm date{};
  date.tm_mday = 9, date.tm_mon = 3 - 1, date.tm_year = 2023 - 1900;
  double deposit_amount = 300000;
  int term_in_days = 366;
  double interest_rate = 6.65;
  double tax_rate = 7.50;
  bool capitalization = false;
  s21::model::lst_vector replenishment, withdrawal;
  s21::model::arg argument{date,           deposit_amount, term_in_days,
                           interest_rate,  tax_rate,       0,
                           capitalization, replenishment,  withdrawal};

  s21::model::result info = s21::model::CalculateDeposit(argument);
  EXPECT_NEAR(info.deposit_amount, 319994.50, fin_eps);
  EXPECT_NEAR(info.tax, 0, fin_eps);
  EXPECT_NEAR(info.accrued_interest, 19994.50, fin_eps);
}

TEST_F(ModelTest, deposit_01) {
  std::tm date{};
  date.tm_mday = 9, date.tm_mon = 3 - 1, date.tm_year = 2023 - 1900;
  double deposit_amount = 300000;
  int term_in_days = 366;
  double interest_rate = 6.65;
  double tax_rate = 7.50;
  int periodicity = s21::model::month;
  bool capitalization = true;
  s21::model::lst_vector replenishment, withdrawal;

  replenishment.push_back({.date = {24, 11, 2023}, .amount = 35200});
  replenishment.push_back({.date = {3, 2, 2024}, .amount = 21500});

  withdrawal.push_back({.date = {14, 12, 2023}, .amount = 72140});

  s21::model::arg argument{date,           deposit_amount, term_in_days,
                           interest_rate,  tax_rate,       periodicity,
                           capitalization, replenishment,  withdrawal};

  s21::model::result info = s21::model::CalculateDeposit(argument);

  EXPECT_NEAR(info.deposit_amount, 304863.21, fin_eps);
  EXPECT_NEAR(info.tax, 0, fin_eps);
  EXPECT_NEAR(info.accrued_interest, 20303.21, fin_eps);
}

TEST_F(ModelTest, deposit_02) {
  std::tm date{};
  date.tm_mday = 9, date.tm_mon = 3 - 1, date.tm_year = 2023 - 1900;
  double deposit_amount = 300000;
  int term_in_days = 366;
  double interest_rate = 6.65;
  double tax_rate = 7.50;
  int periodicity = s21::model::month;
  bool capitalization = true;
  s21::model::lst_vector replenishment, withdrawal;

  replenishment.push_back({.date = {24, 11, 2023}, .amount = 35200});
  replenishment.push_back({.date = {3, 2, 2024}, .amount = 21500});

  withdrawal.push_back({.date = {14, 12, 2023}, .amount = 500000});

  s21::model::arg argument{date,           deposit_amount, term_in_days,
                           interest_rate,  tax_rate,       periodicity,
                           capitalization, replenishment,  withdrawal};

  s21::model::result info{};
  EXPECT_THROW(info = s21::model::CalculateDeposit(argument),
               std::invalid_argument);
}

TEST_F(ModelTest, deposit_03) {
  std::tm date{};
  date.tm_mday = 9, date.tm_mon = 3 - 1, date.tm_year = 2023 - 1900;
  double deposit_amount = 300000;
  int term_in_days = 366;
  double interest_rate = 6.65;
  double tax_rate = 7.50;
  int periodicity = s21::model::year;
  bool capitalization = false;
  s21::model::lst_vector replenishment, withdrawal;
  s21::model::arg argument{date,           deposit_amount, term_in_days,
                           interest_rate,  tax_rate,       periodicity,
                           capitalization, replenishment,  withdrawal};

  s21::model::result info = s21::model::CalculateDeposit(argument);
  EXPECT_NEAR(info.deposit_amount, 319994.50, fin_eps);
  EXPECT_NEAR(info.tax, 0, fin_eps);
  EXPECT_NEAR(info.accrued_interest, 19994.50, fin_eps);
}
}  // namespace