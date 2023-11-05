//
// Created by ruslan on 21.05.23.
//

#include "model.h"

#include <cassert>
#include <cmath>
#include <stack>
#include <stdexcept>

namespace s21 {

double model::Evaluate(double x) {
  if (modified_) {
    rpn{}.swap(string_in_rpn_);
    TransformStringBeforeParsing();
    Validate();
    ParseStringToRPN();
    modified_ = false;
  }
  return Calculate(x);
}

void model::ParseStringToRPN() {
  stack stack;
  size_t pos = 0, end = expression_.length();

  while (pos < end) {
    if (expression_[pos] == '(') {
      stack.push((unit){.ch = expression_[pos], .type = unit::operator_});
    } else if (expression_[pos] == ')') {
      while (stack.top().ch != '(') {
        string_in_rpn_.push(stack.top());
        stack.pop();
      }
      stack.pop();
    } else if (string("0123456789.X").find(expression_[pos]) != string::npos) {
      PushOperand(pos);
    } else {
      while (!stack.empty() && stack.top().ch != '(' &&
             (Prioritize(expression_[pos]) < Prioritize(stack.top().ch) ||
              (Prioritize(expression_[pos]) == Prioritize(stack.top().ch) &&
               expression_[pos] != stack.top().ch))) {
        string_in_rpn_.push(stack.top());
        stack.pop();
      }
      stack.push((unit){.ch = expression_[pos], .type = unit::operator_});
    }
    ++pos;
  }
  while (!stack.empty()) {
    if (stack.top().ch != '(') string_in_rpn_.push(stack.top());
    stack.pop();
  }
}

void model::PushOperand(size_t &pos) {
  char *saved_locale = setlocale(LC_NUMERIC, "C");
  unit *ptr = nullptr;

  if (expression_[pos] == 'X') {
    ptr = new unit{.ch = expression_[pos], .type = unit::operand_x};
  } else {
    size_t end_pos;
    ptr = new unit{{std::stod(expression_.substr(pos), &end_pos)},
                   unit::operand_};
    pos += end_pos - 1;
  }
  string_in_rpn_.push(*ptr);
  delete ptr;
  setlocale(LC_NUMERIC, saved_locale);
}

double model::Calculate(double x) {
  stack stack;
  rpn local_string_in_rpn = string_in_rpn_;
  while (!local_string_in_rpn.empty()) {
    unit &obj = local_string_in_rpn.front();
    if (obj.type == unit::operand_ || obj.type == unit::operand_x) {
      if (obj.type == unit::operand_x) {
        if (std::isnan(x))
          throw std::invalid_argument("X is nan");
        else
          obj.db = x;
      }
      stack.push(obj);
    } else if (string("+-*/%^").find(obj.ch) != string::npos) {
      obj.CalculateWithTwoOperands(stack, precision_);
    } else {
      obj.CalculateWithOneOperand(stack.top(), precision_);
    }
    local_string_in_rpn.pop();
  }

  return stack.top().db;
}

void model::Validate() const {
  if (!expression_.empty()) {
    if (expression_.front() == '%' ||
        string{"+-/*^%(~"}.find(expression_.back()) != string::npos)
      throw std::invalid_argument("Wrong input");
    for (size_t i = 0, ie = expression_.length() - 1; i < ie; ++i) {
      if (((expression_[i] == '(' && expression_[i + 1] != '~') ||
           expression_[i] == '~' ||
           string("+-/*^%").find(expression_[i]) != string::npos) &&
          !IsOperand(expression_[i + 1])) {
        throw std::invalid_argument("Wrong input");
      }
    }
  }
}

int model::Prioritize(const char &c) noexcept {
  int ret;

  if (c == '~') {
    ret = 5;
  } else if (c == '^') {
    ret = 4;
  } else if (string("oiacstqng").find(c) != string::npos) {
    ret = 3;
  } else if (string("*/%").find(c) != string::npos) {
    ret = 2;
  } else {
    ret = 1;
  }
  return ret;
}

bool model::IsOperand(const char &ch) noexcept {
  return IsDigit(ch) || std::isalpha(ch) || ch == '(';
}

bool model::IsDigit(const char &ch) noexcept {
  return std::isdigit(ch) || ch == '.' || ch == 'X';
}

void model::TransformStringBeforeParsing() {
  ReplaceFuncNameWithSymbol();
  ReplaceUnaryMinusWithTilde();
  DeleteUnaryPlus();
  ReplaceSinglePointWithZero();
}

void model::ReplaceFuncNameWithSymbol() {
  constexpr int funcs_num = 10;
  string funcs[funcs_num] = {"acos", "asin", "atan", "cos", "sin",
                             "tan",  "sqrt", "ln",   "log", "mod"};
  string func_symbol[funcs_num] = {"o", "i", "a", "c", "s",
                                   "t", "q", "n", "g", "%"};

  for (int j = 0; j < funcs_num; ++j) {
    size_t pos;
    while ((pos = expression_.find(funcs[j])) != std::string::npos) {
      expression_.replace(pos, funcs[j].length(), func_symbol[j]);
    }
  }
}

void model::ReplaceUnaryMinusWithTilde() noexcept {
  auto pos = expression_.begin(), end = expression_.end();
  if (*pos == '-') *pos = '~';

  while (pos != end) {
    if (*pos == '-' && *(pos - 1) == '(') *pos = '~';
    ++pos;
  }
}

void model::ReplaceSinglePointWithZero() noexcept {
  size_t pos = -1;

  while ((pos = expression_.find('.', pos + 1)) != string::npos) {
    if ((pos == 0 || !std::isdigit(expression_[pos - 1])) &&
        (pos == expression_.length() - 1 || !isdigit(expression_[pos + 1])))
      expression_[pos] = '0';
    if (pos == expression_.length() - 1) return;
  }
}

void model::DeleteUnaryPlus() noexcept {
  auto pos = expression_.begin(), end = expression_.end();
  if (*(pos++) == '+') expression_.erase(0, 1);
  while (pos != end) {
    if (*pos == '+' && *(pos - 1) == '(')
      expression_.erase(pos++);
    else
      ++pos;
  }
}

void model::SetExpression(const string &string) {
  if (expression_ != string) {
    modified_ = true;
    expression_ = string;
  }
}

void model::SetPrecision(const double &precision) noexcept {
  precision_ = precision;
}

void model::unit::CalculateWithOneOperand(
    unit &value, const double &precision) const noexcept {
  const char &op = ch;
  if (op == '~') {
    value.db *= -1;
  } else {
    double (*function[9])(double) = {&std::acos, &std::asin, &std::atan,
                                     &std::cos,  &std::sin,  &std::tan,
                                     &std::sqrt, &std::log,  &std::log10};

    char func_symbol[] = {'o', 'i', 'a', 'c', 's', 't', 'q', 'n', 'g'};
    for (int i = 0;; ++i) {
      if (func_symbol[i] == op) {
        value.db = function[i](value.db);
        value.RoundToZero(precision);
        break;
      }
    }
  }
}

void model::unit::CalculateWithTwoOperands(stack &stack,
                                           const double &precision) const {
  const char &op = ch;
  unit b = stack.top();
  stack.pop();
  assert(!stack.empty());
  unit &a = stack.top();

  if (op == '+') {
    a.db += b.db;
  } else if (op == '-') {
    a.db -= b.db;
  } else if (op == '*') {
    a.db *= b.db;
  } else if (op == '/') {
    a.db /= b.db;
  } else if (op == '%') {
    a.db = std::fmod(a.db, b.db);
  } else {
    a.db = std::pow(a.db, b.db);
  }
  a.RoundToZero(precision);
}

void model::unit::RoundToZero(const double &precision) noexcept {
  db = std::abs(db) < precision ? 0 : db;
}
}  // namespace s21