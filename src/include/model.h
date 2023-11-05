//
// Created by ruslan on 21.05.23.
//

#ifndef SMARTCALC2_MODEL_H
#define SMARTCALC2_MODEL_H

#include <ctime>
#include <queue>
#include <stack>
#include <string>

/** @file model.cc
 * All methods for calculations are here
 */
/** @file model-financial.cc
 * Bonus part with Credit and Deposit calculators
 */

/**
 * @namespace s21
 * @brief standard namespace for school project
 */
namespace s21 {
using std::string;

/**
 * @class model
 * @brief main class for calculations
 */
class model final {
 public:
  struct lists;
  using lst_vector = std::vector<lists>;
  using vector = std::vector<double>;

  /**
   * @enum
   * @brief enum for periodicity
   */
  enum { day, week, month, quarter, half_year, year };

  /**
   * @struct DateHolder
   * @brief Class that keeps dates, simple version of QDate
   */
  struct DateHolder {
    /**
     * @brief construct current date
     */
    DateHolder() noexcept;

    /**
     * Construct from explicit date
     * @param day
     * @param month
     * @param year
     */
    DateHolder(int day, int month, int year) noexcept;

    /**
     * Construct from std::tm
     * @param other - from where to construct
     */
    explicit DateHolder(std::tm other) noexcept;

    /**
     * Date comparison
     * @param other - date to compare with
     * @return true, if equal
     */
    [[nodiscard]] bool Compare(const DateHolder &other) const noexcept;

    /**
     * Days in stored date's month
     * @return number of days
     */
    [[nodiscard]] size_t DaysInMonth() const noexcept;

    /**
     * Days in stored date's year
     * @return number of days
     */
    [[nodiscard]] size_t DaysInYear() const noexcept;

    /**
     * Days from stored date till the @param other date
     * @return number of days
     */
    [[nodiscard]] size_t ToDay(const DateHolder &other) const noexcept;

    /**
     * Increase stored date by month
     * @return stored date
     */
    DateHolder &AddMonth() noexcept;

    /**
     * Increase stored date by @param n months
     * @return stored date
     */
    DateHolder &AddMonth(int n) noexcept;

    /**
     * Increase stored date by day
     * @return stored date
     */
    DateHolder &AddDay() noexcept;

    /**
     * Represent stored date as std::tm
     * @return std::tm
     */
    [[nodiscard]] std::tm Tm() const noexcept;

    /**
     * Transform periodicity to day's number
     * @param periodicity - periodicity from enum
     * @return number of days
     */
    [[nodiscard]] size_t CalculateNumberOfDays(
        const int &periodicity) const noexcept;

    /**
     * Checks if there was a Deposit or withdrawal today and performs the
     * corresponding operations
     * @param lst_vector - Deposit or withdrawal
     * @param deposit current amount of Deposit
     * @param sub - Deposit and withdrawal diffs
     */
    void CheckIfReplenishmentWithdrawalToday(const lst_vector &lst_vector,
                                             double &deposit,
                                             bool sub = false) const;

    time_t date;
  };

  /**
   * @struct lists
   * @brief Stores date and amount of money
   */
  struct lists {
    model::DateHolder date;
    double amount{};
  };

  /**
   * @struct arg
   * @brief Keeps all information to provide Deposit calculation
   */
  struct arg {
    std::tm date{};
    double deposit_amount{};
    long long term{};
    double rate{};
    double tax_rate{};
    int periodicity{};
    bool capitalization{};
    lst_vector replenishments, withdrawals;
  };

  /**
   * @struct result
   * @brief Keeps result information after Deposit calculation
   */
  struct result {
    double accrued_interest;
    double tax;
    double deposit_amount;
  };

 public:
  model() = default;

  /**
   * Calculates expression_ with
   * @param x
   * @return calculation result
   */
  double Evaluate(double x);

  /**
   * Sets expression_, that will be calculated
   * @param string - expression_
   */
  void SetExpression(const string &string);

  /**
   * Sets precision_
   * @param precision - precision_
   */
  void SetPrecision(const double &precision) noexcept;

  /**
   * Calculate Credit info
   * @param loan_amount
   * @param loan_term
   * @param interest_rate
   * @param annuity
   * @return std::vector of <doubles>, that contains month's payments. Only one
   * record, if annuity == true
   */
  static vector CalculateCredit(double loan_amount, const unsigned &loan_term,
                                const double &interest_rate,
                                const bool &annuity);

  /**
   * Calculate Deposit info from given
   * @param argument
   * @return  \b struct result
   */
  static model::result CalculateDeposit(const arg &argument);

 private:
  struct unit;
  using rpn = std::queue<unit>;
  using stack = std::stack<unit>;

  /**
   * @struct unit
   * @brief Internal struct for rpn and calculation
   */
  struct unit {
    /**
     * Calculates operation with one operand and places a result to stack
     * @param value - stack's top elem
     * @param precision - precision to round a value
     */
    void CalculateWithOneOperand(unit &value,
                                 const double &precision) const noexcept;

    /**
     * Calculates operation with two operands and places a result to stack
     * @param stack - stack
     * @param precision - precision to round a value
     */
    void CalculateWithTwoOperands(stack &stack, const double &precision) const;

    /**
     * Rounds the value to zero if abs(value) < eps
     * @param precision - precision to round a value
     */
    void RoundToZero(const double &precision) noexcept;

    union {
      double db;
      char ch;
    };
    enum { operator_ = 1, operand_ = 2, operand_x = 3 } type;
  };

 private:
  /**
   * Modifies string before parsing, Deleting unary pluses, transforms unary
   * minuses to tilda and so on.
   */
  void TransformStringBeforeParsing();

  /**
   * Replaces function names with corresponding symbols
   */
  void ReplaceFuncNameWithSymbol();

  /**
   * Replaces unary minuses with tilda symbol
   */
  void ReplaceUnaryMinusWithTilde() noexcept;

  /**
   * String still needs to be checked if it is incorrect
   */
  void Validate() const;

  /**
   * Checks if operand
   * @param ch - symbol to check
   * @return true, if yes
   */
  static bool IsOperand(const char &ch) noexcept;

  /**
   * Checks if symbol is digit (== digit, . or X ).
   * @param ch - symbol to check
   * @return - true, if yes
   */
  static bool IsDigit(const char &ch) noexcept;

  /**
   * Convert string to rpn representation
   */
  void ParseStringToRPN();

  /**
   * Returns corresponding priority
   * @param c - operator to get prority
   * @return number
   */
  static int Prioritize(const char &c) noexcept;

  /**
   * calculate result with given params from rpn
   * @param x
   * @return result
   */
  [[nodiscard]] double Calculate(double x);

  /**
   * Replaces '.' with 0
   */
  void ReplaceSinglePointWithZero() noexcept;

  /**
   * Just deletes all unary pluses
   */
  void DeleteUnaryPlus() noexcept;

  /**
   * Operand smart push depending on X
   * @param pos - position of a symbol
   */
  void PushOperand(size_t &pos);

 private:
  string expression_;

  rpn string_in_rpn_{};

  double precision_ = 1e-7;

  bool modified_ = false;
};

}  // namespace s21

#endif  // SMARTCALC2_MODEL_H
