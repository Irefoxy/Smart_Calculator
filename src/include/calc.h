#ifndef CALC_H
#define CALC_H

#include <QAbstractButton>
#include <QMainWindow>

#include "credit.h"
#include "deposit.h"
#include "graph.h"

/**
 * @namespace
 * @brief standard namespace for autogen gui
 */
namespace Ui {
class Calculator;
}

namespace s21 {
/**
 * @file calc.cc - base widget methods definitions
 */

/**
 * @file calc-insertion.cc - insertion logic methods definitions
 */

/**
 * @class Calculator
 * @brief Main window class
 */
class Calculator : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * Base constructor with corresponding connects to child windows
   * @param parent - QObject dependency
   */
  explicit Calculator(QWidget *parent = nullptr);

  /**
   * Destructor
   */
  ~Calculator() override;

  /**
   * Public method for controller to set result
   */
  void SetResult(double &);

  /**
   * Public method for controller to set error
   * @param message exception message
   */
  void SetError(const QString &message);

  /**
   * Public getter for controller to get child window
   * @return pointer to child window
   */
  Credit *GetCredit();

  /**
   * Public getter for controller to get child window
   * @return pointer to child window
   */
  Deposit *GetDeposit();

 signals:

  void EvaluateExpression(const QString &expression, const double &x);

  void GraphCreated(Graph &graph);

 private slots:

  void on_Appended_buttonClicked(QAbstractButton *button);

  void on_Button_c_clicked();

  void on_Button_backspace_clicked();

  void on_Button_eq_clicked();

  void on_Button_xc_clicked();

  void on_Button_xs_clicked();

  void on_Button_graph_clicked();

  void on_actionCredit_triggered();

  void on_actionDeposit_triggered();

  void Close();

 private:
  /**
   * Main function to provide insertion
   * @param button_text - text on a button
   */
  void ComplementInsertion(const QString &button_text);

  /**
   * Function that provide base insertion logic
   * @param new_button_text - modifying text from a button
   */
  QString ProvideInsertionLogic(QString &new_button_text) const;

  /**
   * Checks if operator insertion should be skipped
   * @return bool
   */
  [[nodiscard]] bool ShouldSkipOperatorInsertion() const;

  /**
   * Checks if last number in expression already contains a point
   * @return bool
   */
  [[nodiscard]] bool CheckIfAlreadyContainPoint() const;

  /**
   * Checks if multiplication symbol should be added
   * @return bool
   */
  [[nodiscard]] bool ShouldAddMultSymbol(const QString &new_button_text) const;

  /**
   * Checks if symbol is Digit
   * @param ch - symbol to check
   * @return bool
   */
  static bool IsDigit(const QChar &ch) noexcept;

  /**
   * Provides pne token deletion
   */
  void DeleteOneToken();

  /**
   * Finds position to cut off a string
   * @return qsizetype
   */
  [[nodiscard]] qsizetype FindPositionToTerminate() const;

  /**
   * Convert scientific exponent to dec representation
   * @return  transformed string
   */
  [[nodiscard]] QString ConvertExponentToDec() const;

  /**
   * Provides evaluation signal and error's handling
   * @param x
   * @return
   */
  bool Evaluate(const double &x = 0.0);

  /**
   * Set error to false
   */
  void SetStatusOK();

  /**
   * clears expression if needed and drops the flag
   */
  void ClearIfNeeded();

 private:
  Ui::Calculator *ui{};
  double x_ = NAN;
  double result_ = NAN;
  bool clear_screen_ = true;
  bool error = false;
  Credit *credit_w_;
  Deposit *deposit_w_;
};
}  // namespace s21
#endif  // CALC_H
