//
// Created by ruslan on 27.02.23.
//

#ifndef CALC_SRC_QT_CREDIT_H_
#define CALC_SRC_QT_CREDIT_H_

#include <QMainWindow>

namespace Ui {
class Credit;
}

namespace s21 {
/**
 * @file Credit.cc - Credit class definition
 */

/**
 * @class Credit
 * @brief Class for Credit window widget
 */
class Credit : public QMainWindow {
  Q_OBJECT
 public:
  using vector = QVector<double>;

  /**
   * Constructor. Set ui and connects.
   * @param parent
   */
  explicit Credit(QMainWindow *parent = nullptr);

  /**
   * Destructor
   */
  ~Credit() override;

  /**
   * Public method for controller to set result
   * @param result
   */
  void SetCredit(vector &&result);

 signals:

  void ShowMain();

  void ShowSecondWindow();

  void Close();

  void CalculateCredit(const double &loan_amount, const unsigned &loan_term,
                       const double &interest_rate, const bool &annuity);

 private slots:

  void on_actionBasic_triggered();

  void on_actionDeposit_triggered();

  void on_calculate_clicked();

 private:
  /**
   * Summarize payments from months
   * @return sum
   */
  double CalculateTotalPayment();

 private:
  Ui::Credit *ui;
  vector credit_;
};
}  // namespace s21
#endif  // CALC_SRC_QT_CREDIT_H_
