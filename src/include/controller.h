//
// Created by ruslan on 23.05.23.
//

#ifndef SMARTCALC2_CONTROLLER_H
#define SMARTCALC2_CONTROLLER_H

#include <QObject>

#include "calc.h"
#include "model.h"

/**
 * @file controller.cc controller class definitions
 */

namespace s21 {

/**
 * @class controller
 * @brief implements the MVC pattern
 */
class controller final : public QObject {
  Q_OBJECT
 public:
  /**
   * Constructor. Get pointers to model and view(MVC). Connects their signals to
   * self slots.
   * @param model - pointer to model
   * @param view - pointer to ciew
   * @param parent - parent fot QObject
   */
  controller(model *model, Calculator *view, QObject *parent = nullptr);

  ~controller() override = default;

 private slots:

  /**
   * slot used to connect graph's signal to calculate an expression with
   * corresponding X and calculation slot. Initializes graph_ pointer
   * @param graph - graph to get
   */
  void ConnectGraph(Graph &graph);

  /**
   * Slot connected to expression calculation signal from main window
   * @param expression - to calculate
   * @param x
   */
  void CalculateExpression(const QString &expression, const double &x) const;

  /**
   * Slot connected to plot calculation signal
   * @param x
   */
  void PlotCalculation(const double &x) const;

  /**
   * Slot connected to precision set signal
   * @param precision
   */
  void SetPrecision(const double &precision);

  /**
   * Slot connected to Credit calculation signal
   * @param loan_amount
   * @param loan_term
   * @param interest_rate
   * @param annuity
   */
  void CalculateCredit(const double &loan_amount, const unsigned &loan_term,
                       const double &interest_rate, const bool &annuity) const;

  /**
   * Slot connected to Deposit calculation signal
   * @param argument
   */
  void CalculateDeposit(const Deposit::arg &argument) const;

  /**
   * Method to convert view::repl/with lists to model::repl/with lists
   * @param vector - view::repl/with
   * @return model::repl/with lists
   */
  static model::lst_vector ConvertArguments(const Deposit::vector &vector);

 private:
  model *model_;
  Calculator *view_;
  Graph *graph_ = nullptr;
  Credit *credit_;
  Deposit *deposit_;
};

}  // namespace s21

#endif  // SMARTCALC2_CONTROLLER_H
