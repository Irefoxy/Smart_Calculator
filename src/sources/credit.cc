//
// Created by ruslan on 27.02.23.
//

#include "credit.h"

#include <QAbstractButton>
#include <QCheckBox>

#include "ui_credit.h"

namespace s21 {
Credit::Credit(QMainWindow *parent) : QMainWindow(parent), ui(new Ui::Credit) {
  ui->setupUi(this);
  ui->loan->setMaximum(__DBL_MAX__);
  connect(this, SIGNAL(destroyed()), this->parent(), SLOT(Close()));
}

Credit::~Credit() { delete ui; }

void Credit::on_actionBasic_triggered() {
  hide();
  emit ShowMain();
}

void Credit::on_calculate_clicked() {
  const auto &annuity = ui->annuity->isChecked();
  CalculateCredit(ui->loan->value(), ui->term->value(), ui->rate->value(),
                  annuity);

  ui->month_payment->setRowCount(int(credit_.size()));
  auto total_payment = annuity ? credit_[0] * double(ui->term->value())
                               : CalculateTotalPayment();
  ui->total->setText(QString::number(total_payment, 'f', 2));
  ui->overpayment->setText(
      QString::number(total_payment - ui->loan->value(), 'f', 2));
  for (int i = 1; i <= credit_.size(); ++i) {
    auto *month = new QTableWidgetItem("per/month");
    if (!annuity) month->setText(QString("Month %1").arg(i));
    ui->month_payment->setItem(i - 1, 0, month);
    ui->month_payment->setItem(
        i - 1, 1, new QTableWidgetItem(QString::number(credit_[i - 1])));
  }
}

double Credit::CalculateTotalPayment() {
  double sum = 0;
  for (auto &payment : credit_) {
    sum += payment;
  }
  return sum;
}

void Credit::on_actionDeposit_triggered() {
  hide();
  emit ShowSecondWindow();
}

void Credit::SetCredit(Credit::vector &&result) { credit_ = result; }
}  // namespace s21