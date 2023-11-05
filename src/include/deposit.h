//
// Created by ruslan on 28.02.23.
//

#ifndef CALC_SRC_QT_DEPOSIT_H_
#define CALC_SRC_QT_DEPOSIT_H_

#include <QAbstractButton>
#include <QCheckBox>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

namespace Ui {
class Deposit;
}

namespace s21 {
/**
 * @file Deposit.cc  - Deposit class definitions
 */

/**
 * Class for Deposit widget window
 */
class Deposit : public QMainWindow {
  Q_OBJECT
 public:
  /**
   * @struct lists
   * @brief struct to keep date and amount of money
   */
  struct lists {
    QDate date;
    double amount{};
  };

  using vector = QVector<lists>;

  /**
   * @struct arg
   * @brief Struct to keep all info for Deposit calculation
   */
  struct arg {
    QDate start_date;
    double deposit_amount{};
    long long term{};
    double rate{};
    double tax_rate{};
    int periodicity{};
    bool capitalization{};
    vector replenishments{}, withdrawals{};
  };

  /**
   * @struct result
   * @brief Struct to keep result info from Deposit calculation
   */
  struct result {
    double accrued_interest;
    double tax;
    double deposit_amount;
  };

 public:
  /**
   * Ctor. Set ui and connects
   * @param parent
   */
  explicit Deposit(QMainWindow *parent = nullptr);

  /**
   * Dtor.
   */
  ~Deposit() override;

  /**
   * Public method for controller to set result
   * @param result
   */
  void SetResult(result &&result);

  /**
   * Public method for controller to set error
   */
  void SetError();

 signals:

  void ShowMain();

  void ShowSecondWindow();

  void Close();

  void CalculateDeposit(const arg &argument);

 private slots:

  void on_actionBasic_triggered();

  void on_actionCredit_triggered();

  void on_replenishments_clicked();

  void on_withdrawals_clicked();

  void on_add_buttonClicked(QAbstractButton *button);

  /**
   * Slot that deletes corresponding row from top table
   */
  void delete_row_from_top_table();

  /**
   * Slot that deletes corresponding row from bot table
   */
  void delete_row_from_bot_table();

  void on_calculate_clicked();

 private:
  /**
   * @enum
   * @brief enum for managing corresponding tables
   */
  enum { top, bot, add_, remove_ };

 private:
  /**
   * Managing tables with corresponding params from enum
   * @param where
   * @param action
   */
  void ManageBox(bool where, int action);

  /**
   * Fixes tables sizes
   * @param date
   */
  static void SetResize(QTableWidget *widget);

  /**
   * Init dates with current date
   * @param date - date to init
   */
  static void InitDate(QDateEdit *date);

  /**
   * Converts table info to vector
   * @param table
   * @return
   */
  static vector GetListFromTable(QTableWidget *table);

  /**
   * Convert input term to days number
   * @return days number
   */
  long long int CalculateNumberOfDays();

 private:
  Ui::Deposit *ui;
  result result_{};
  bool error_ = false;
};
}  // namespace s21
#endif  // CALC_SRC_QT_DEPOSIT_H_
