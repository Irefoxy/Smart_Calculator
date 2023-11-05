//
// Created by ruslan on 28.02.23.
//

#include "deposit.h"

#include <QDateEdit>

#include "ui_deposit.h"

namespace s21 {

Deposit::Deposit(QMainWindow *parent)
    : QMainWindow(parent), ui(new Ui::Deposit) {
  ui->setupUi(this);
  connect(this, SIGNAL(destroyed()), this->parent(), SLOT(Close()));
  InitDate(ui->top_date);
  InitDate(ui->bot_date);
  InitDate(ui->start_date);
  SetResize(ui->top_table);
  SetResize(ui->bot_table);
  ManageBox(top, remove_);
  ManageBox(bot, remove_);
}

Deposit::~Deposit() {
  emit Close();
  delete ui;
}

void Deposit::on_actionBasic_triggered() {
  this->hide();
  emit ShowMain();
}

void Deposit::on_actionCredit_triggered() {
  this->hide();
  emit ShowSecondWindow();
}

void Deposit::on_replenishments_clicked() {
  if (ui->replenishments->isChecked()) {
    ManageBox(top, add_);
  } else {
    ManageBox(top, remove_);
  }
}

void Deposit::on_withdrawals_clicked() {
  if (ui->withdrawals->isChecked()) {
    ManageBox(bot, add_);
  } else {
    ManageBox(bot, remove_);
  }
}

void Deposit::ManageBox(bool where, int action) {
  int layout_index, table_index;
  if (where == top) {
    layout_index = ui->gridLayout->indexOf(ui->top_layout);
    table_index = ui->gridLayout->indexOf(ui->top_table);
  } else {
    layout_index = ui->gridLayout->indexOf(ui->bot_layout);
    table_index = ui->gridLayout->indexOf(ui->bot_table);
  }
  QLayoutItem *child, *cell = ui->gridLayout->itemAt(layout_index);
  for (int i = 0; i < 3; ++i) {
    child = cell->layout()->itemAt(i);
    action == add_ ? child->widget()->show() : child->widget()->hide();
  }
  cell = ui->gridLayout->itemAt(table_index);
  action == add_ ? cell->widget()->show() : cell->widget()->hide();
}

void Deposit::SetResize(QTableWidget *widget) {
  QSizePolicy sp_retain = widget->sizePolicy();
  sp_retain.setRetainSizeWhenHidden(true);
  widget->setSizePolicy(sp_retain);
}

void Deposit::InitDate(QDateEdit *date) { date->setDate(QDate::currentDate()); }

void Deposit::delete_row_from_top_table() {
  auto *btn = dynamic_cast<QPushButton *>(sender());
  bool stop = false;
  for (int i = 0; !stop && i < ui->top_table->rowCount(); ++i) {
    if (ui->top_table->cellWidget(i, 2) == btn) {
      ui->top_table->removeRow(i);
      stop = true;
    }
  }
}

void Deposit::delete_row_from_bot_table() {
  auto *btn = dynamic_cast<QPushButton *>(sender());
  bool stop = false;
  for (int i = 0; !stop && i < ui->bot_table->rowCount(); ++i) {
    if (ui->bot_table->cellWidget(i, 2) == btn) {
      ui->bot_table->removeRow(i);
      stop = true;
    }
  }
}

void Deposit::on_add_buttonClicked(QAbstractButton *button) {
  QTableWidget *table;
  QDateEdit *date;
  QDoubleSpinBox *spin;
  if (button == ui->top_add) {
    table = ui->top_table;
    date = ui->top_date;
    spin = ui->top_spin;
  } else {
    table = ui->bot_table;
    date = ui->bot_date;
    spin = ui->bot_spin;
  }
  int cur_row_number = table->rowCount();
  table->insertRow(cur_row_number);
  table->setItem(cur_row_number, 0, new QTableWidgetItem(date->text()));
  table->setItem(cur_row_number, 1, new QTableWidgetItem(spin->text()));
  auto *btn = new QPushButton();
  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy.setHeightForWidth(btn->sizePolicy().hasHeightForWidth());
  btn->setSizePolicy(sizePolicy);
  QIcon icon;
  icon.addFile(QString::fromUtf8(":/delete_icon.png"), QSize(), QIcon::Normal,
               QIcon::Off);
  btn->setIcon(icon);
  btn->setIconSize(QSize(16, 16));
  table->setCellWidget(cur_row_number, 2, btn);
  if (button == ui->top_add) {
    connect(btn, SIGNAL(clicked()), this, SLOT(delete_row_from_top_table()));
  } else {
    connect(btn, SIGNAL(clicked()), this, SLOT(delete_row_from_bot_table()));
  }
}

void Deposit::on_calculate_clicked() {
  error_ = false;
  vector replenishments, withdrawals;
  if (ui->replenishments->isChecked())
    replenishments = GetListFromTable(ui->top_table);
  if (ui->withdrawals->isChecked())
    withdrawals = GetListFromTable(ui->bot_table);
  CalculateDeposit({ui->start_date->date(), ui->deposit_amount->value(),
                    CalculateNumberOfDays(), ui->rate->value(),
                    ui->tax_rate->value(), ui->periodicity->currentIndex(),
                    ui->capitalization->isChecked(), replenishments,
                    withdrawals});
  if (!error_) {
    ui->accrued_interest->setText(
        QString::number(result_.accrued_interest, 'f', 2));
    ui->tax->setText(QString::number(result_.tax, 'f', 2));
    ui->dep_by_end->setText(
        QString::number(result_.deposit_amount - result_.tax, 'f', 2));
  } else {
    ui->accrued_interest->setText("Wrong withdrawals");
  }
}

Deposit::vector Deposit::GetListFromTable(QTableWidget *table) {
  vector ret;
  if (table->rowCount()) {
    for (int i = 0; i < table->rowCount(); ++i) {
      ret.push_back({QDate::fromString(table->item(i, 0)->text(), "dd.MM.yyyy"),
                     table->item(i, 1)->text().toDouble()});
    }
  }
  return ret;
}

long long int Deposit::CalculateNumberOfDays() {
  const int &term = ui->term->value();
  QDate finish_date = ui->start_date->date();
  switch (ui->deposut_term_type->currentIndex()) {
    case 1:
      finish_date = finish_date.addMonths(term);
      break;
    case 2:
      finish_date = finish_date.addYears(term);
      break;
    default:
      finish_date = finish_date.addDays(term);
  }
  return ui->start_date->date().daysTo(finish_date);
}

void Deposit::SetResult(Deposit::result &&result) { result_ = result; }

void Deposit::SetError() { error_ = true; }
}  // namespace s21