//
// Created by ruslan on 24.05.23.
//

#include <stdexcept>
#include <valarray>

#include "model.h"

namespace s21 {

model::vector model::CalculateCredit(double loan_amount,
                                     const unsigned &loan_term,
                                     const double &interest_rate,
                                     const bool &annuity) {
  vector ret;
  if (annuity) {
    ret.push_back(
        loan_amount * interest_rate / 100 / 12 /
        (1 - std::pow(1 + interest_rate / 100 / 12, -(double)loan_term)));
  } else {
    DateHolder date;
    double fixed_payment = loan_amount / loan_term;
    for (unsigned i = 1; i <= loan_term; ++i) {
      ret.push_back(loan_amount * interest_rate / 100 *
                        double(date.DaysInMonth()) / double(date.DaysInYear()) +
                    fixed_payment);
      loan_amount -= fixed_payment;
      date.AddMonth();
    }
  }
  return ret;
}

model::result model::CalculateDeposit(const arg &argument) {
  DateHolder cur_date{argument.date};
  double deposit_amount = argument.deposit_amount;
  size_t days_till_capitalization;
  if (argument.capitalization)
    days_till_capitalization =
        cur_date.CalculateNumberOfDays(argument.periodicity);
  double interest_rate_by_day =
             argument.rate / double(cur_date.DaysInYear()) / 100,
         income = 0.0, total_income = 0.0, year_income = 0.0, tax = 0.0,
         deduction = argument.tax_rate / 100 * 1000000;
  for (int i = 0; i < argument.term; ++i) {
    cur_date.AddDay();
    if (cur_date.Tm().tm_mon == 0 && cur_date.Tm().tm_mday == 1) {
      tax += year_income < deduction ? 0 : (year_income - deduction) * 0.13;
      year_income = 0.0;
      interest_rate_by_day =
          argument.rate / double(cur_date.DaysInYear()) / 100;
    }
    double day_income = deposit_amount * interest_rate_by_day;
    income += day_income;
    year_income += day_income;
    --days_till_capitalization;
    cur_date.CheckIfReplenishmentWithdrawalToday(argument.withdrawals,
                                                 deposit_amount, true);
    cur_date.CheckIfReplenishmentWithdrawalToday(argument.replenishments,
                                                 deposit_amount);
    if (argument.capitalization && !days_till_capitalization) {
      deposit_amount += income;
      total_income += income;
      income = 0.0;
      days_till_capitalization =
          cur_date.CalculateNumberOfDays(argument.periodicity);
    }
  }
  if (!argument.capitalization) {
    total_income = income;
    deposit_amount += total_income;
  }
  tax += year_income < deduction ? 0 : (year_income - deduction) * 0.13;
  return result{total_income, tax, deposit_amount};
}

model::DateHolder::DateHolder() noexcept {
  std::time_t currentTime = std::time(nullptr);
  std::tm *timeinfo = std::localtime(&currentTime);
  timeinfo->tm_hour = 0;
  timeinfo->tm_min = 0;
  timeinfo->tm_sec = 0;
  date = std::mktime(timeinfo);
}

bool model::DateHolder::Compare(const DateHolder &other) const noexcept {
  return date == other.date;
}

model::DateHolder::DateHolder(int day, int month, int year) noexcept {
  std::tm timeinfo = {};

  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;

  date = std::mktime(&timeinfo);
}

size_t model::DateHolder::DaysInMonth() const noexcept {
  std::tm *tm_ = std::localtime(&date);

  tm_->tm_mday = 1;
  std::time_t start = std::mktime(tm_);

  tm_ = std::localtime(&date);
  tm_->tm_mday = 1;
  ++tm_->tm_mon;
  if (tm_->tm_mon > 11) {
    tm_->tm_mon = 0;
    ++tm_->tm_year;
  }
  std::time_t next = std::mktime(tm_);
  return (next - start) / 60 / 60 / 24;
}

size_t model::DateHolder::DaysInYear() const noexcept {
  std::tm *tm_ = std::localtime(&date);

  tm_->tm_mday = 1;
  tm_->tm_mon = 0;
  std::time_t start = std::mktime(tm_);
  tm_ = std::localtime(&date);

  tm_->tm_mon = 11;
  tm_->tm_mday = 31;

  std::time_t next = std::mktime(tm_);
  return (next - start) / 60 / 60 / 24 + 1;
}

model::DateHolder &model::DateHolder::AddMonth() noexcept {
  std::tm *tm_ = std::localtime(&date);

  ++tm_->tm_mon;
  if (tm_->tm_mon > 11) {
    tm_->tm_mon = 0;
    ++tm_->tm_year;
  }
  date = std::mktime(tm_);
  return *this;
}

model::DateHolder &model::DateHolder::AddDay() noexcept {
  int days_num = DaysInMonth();
  std::tm *tm_ = std::localtime(&date);
  ++tm_->tm_mday;
  if (tm_->tm_mday > days_num) {
    tm_->tm_mday = 1;
    date = std::mktime(tm_);
    AddMonth();
  } else {
    date = std::mktime(tm_);
  }
  return *this;
}

model::DateHolder &model::DateHolder::AddMonth(int n) noexcept {
  for (int i = 0; i < n; ++i) AddMonth();
  return *this;
}

model::DateHolder::DateHolder(const std::tm other) noexcept {
  std::tm tmp = other;
  date = std::mktime(&tmp);
}

size_t model::DateHolder::ToDay(const model::DateHolder &other) const noexcept {
  return (other.date - date) / 60 / 60 / 24;
}

std::tm model::DateHolder::Tm() const noexcept {
  std::tm *tm_ = std::localtime(&date);
  return *tm_;
}

size_t model::DateHolder::CalculateNumberOfDays(
    const int &periodicity) const noexcept {
  DateHolder tmp = *this;
  int ret = 1;
  switch (periodicity) {
    case week:
      ret *= 7;
      break;
    case month:
      ret = ToDay(tmp.AddMonth());
      break;
    case quarter:
      ret = ToDay(tmp.AddMonth(3));
      break;
    case half_year:
      ret = ToDay(tmp.AddMonth(6));
      break;
    default:
      ret = ToDay(tmp.AddMonth(12));
      break;
  }
  return ret;
}

void model::DateHolder::CheckIfReplenishmentWithdrawalToday(
    const lst_vector &lst_vector, double &deposit, bool sub) const {
  if (!lst_vector.empty()) {
    for (auto &lst : lst_vector) {
      if (lst.date.Compare(*this)) {
        sub ? deposit -= lst.amount : deposit += lst.amount;
        if (deposit < 0.01) throw std::invalid_argument("Wrong withdrawals");
        break;
      }
    }
  }
}
}  // namespace s21
