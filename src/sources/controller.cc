//
// Created by ruslan on 23.05.23.
//

#include "controller.h"

#include "credit.h"

namespace s21 {
controller::controller(model *model, Calculator *view, QObject *parent)
    : QObject(parent),
      model_{model},
      view_{view},
      credit_{view_->GetCredit()},
      deposit_{view_->GetDeposit()} {
  connect(view_, &Calculator::EvaluateExpression, this,
          &controller::CalculateExpression);
  connect(view_, &Calculator::GraphCreated, this, &controller::ConnectGraph);

  connect(credit_, &Credit::CalculateCredit, this,
          &controller::CalculateCredit);
  connect(deposit_, &Deposit::CalculateDeposit, this,
          &controller::CalculateDeposit);
}

void controller::CalculateExpression(const QString &expression,
                                     const double &x) const {
  model_->SetExpression(expression.toStdString());
  try {
    double result = model_->Evaluate(x);
    view_->SetResult(result);
  } catch (std::invalid_argument &e) {
    view_->SetError(QString::fromStdString(e.what()));
  }
}

void controller::ConnectGraph(Graph &graph) {
  connect(&graph, &Graph::RequestCalculation, this,
          &controller::PlotCalculation);
  connect(&graph, &Graph::SetPrecision, this, &controller::SetPrecision);
  graph_ = &graph;
}

void controller::PlotCalculation(const double &x) const {
  graph_->SetResult(model_->Evaluate(x));
}

void controller::CalculateCredit(const double &loan_amount,
                                 const unsigned int &loan_term,
                                 const double &interest_rate,
                                 const bool &annuity) const {
  auto result =
      model::CalculateCredit(loan_amount, loan_term, interest_rate, annuity);
  credit_->SetCredit(Credit::vector{result.begin(), result.end()});
}

void controller::CalculateDeposit(const Deposit::arg &argument) const {
  std::tm date{};
  date.tm_mday = argument.start_date.day(),
  date.tm_mon = argument.start_date.month() - 1,
  date.tm_year = argument.start_date.year() - 1900;
  auto arg = model::arg{date,
                        argument.deposit_amount,
                        argument.term,
                        argument.rate,
                        argument.tax_rate,
                        argument.periodicity,
                        argument.capitalization,
                        ConvertArguments(argument.replenishments),
                        ConvertArguments(argument.withdrawals)};
  try {
    auto result = model::CalculateDeposit(arg);
    deposit_->SetResult(Deposit::result{result.accrued_interest, result.tax,
                                        result.deposit_amount});
  } catch (std::invalid_argument &e) {
    deposit_->SetError();
  }
}

model::lst_vector controller::ConvertArguments(const Deposit::vector &vector) {
  model::lst_vector lst_vector;

  for (auto &obj : vector) {
    model::lists lst{{obj.date.day(), obj.date.month(), obj.date.year()},
                     obj.amount};
    lst_vector.push_back(lst);
  }
  return lst_vector;
}

void controller::SetPrecision(const double &precision) {
  model_->SetPrecision(precision);
};

}  // namespace s21