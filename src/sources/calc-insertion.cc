//
// Created by ruslan on 23.05.23.
//

#include "calc.h"
#include "ui_calc.h"

namespace s21 {
void Calculator::ComplementInsertion(const QString &button_text) {
  QString new_button_text = button_text;

  if (button_text == "10 ^ y") new_button_text = "*10^(";
  QString input_string = ProvideInsertionLogic(new_button_text);
  if (ShouldAddMultSymbol(new_button_text)) input_string += "*";
  ui->lineEdit->setText(input_string + new_button_text);
}

QString Calculator::ProvideInsertionLogic(QString &new_button_text) const {
  QString input_string = ui->lineEdit->text();
  /* Case "()" button pressed */
  if (new_button_text.contains("()")) {
    if (input_string.count('(') > input_string.count(')') &&
        !QString("(*/+-^").contains(input_string.back())) {
      new_button_text = ")";
    } else {
      new_button_text = "(";
    }
    /* Case operator button pressed */
  } else if (QString("*/+-^").contains(new_button_text.front())) {
    /* Case input string is empty */
    if (input_string.isEmpty()) {
      /* Case "0" -> "0*", "0/", etc */
      if (QString("*/^").contains(new_button_text.front())) input_string = "0";
      /* Case "(" + "*" -> "("; "(" + "-" -> "(-"; "(-" + "+" -> "(+"; "(+" +
       * "*" -> "(+" */
    } else if (QString("*/^").contains(new_button_text.front()) &&
               ShouldSkipOperatorInsertion()) {
      new_button_text.clear();
      /* Case "123*" + "/" -> "123/" */
    } else if (QString("*/+-^").contains(input_string.back())) {
      /* Case "-" + "*" -> "0*"; "-" + "+" ->  "+" */
      if (input_string.length() == 1 &&
          !QString("+-").contains(new_button_text.front())) {
        input_string = "0";
      } else {
        input_string.chop(1);
      }
    }
    /* Case funcs are pressed */
  } else if (new_button_text.length() > 1) {
    new_button_text += "(";
    /* Case "123.12" + "." -> "123.12"; "123" + "." -> "123." */
  } else if (new_button_text == "." && CheckIfAlreadyContainPoint()) {
    new_button_text.clear();
  }
  return input_string;
}

void Calculator::DeleteOneToken() {
  QString input_string = ui->lineEdit->text();
  input_string.resize(FindPositionToTerminate());
  if (input_string.isEmpty()) {
    ui->lineEdit->setText("0");
    clear_screen_ = true;
  } else {
    ui->lineEdit->setText(input_string);
  }
}

bool Calculator::CheckIfAlreadyContainPoint() const {
  const QString &line_text = ui->lineEdit->text();

  int i = int(line_text.length() - 1);
  for (; i >= 0 && line_text[i].isDigit(); --i)
    ;
  return i >= 0 && line_text[i] == '.';
}

bool Calculator::ShouldSkipOperatorInsertion() const {
  const QString &line_text = ui->lineEdit->text();
  return line_text.back() == '(' ||
         (line_text.length() > 1 && line_text[line_text.length() - 2] == '(' &&
          QString("+-").contains(line_text.back()));
}

bool Calculator::ShouldAddMultSymbol(const QString &new_button_text) const {
  const QString &line_text = ui->lineEdit->text();
  return !new_button_text.isEmpty() && !line_text.isEmpty() &&
         !QString(")*/+-^m").contains(new_button_text.front()) &&
         (QString("X)").contains(line_text.back()) ||
          (IsDigit(line_text.back()) && (!IsDigit(new_button_text.front()) ||
                                         new_button_text.front() == 'X')));
}

bool Calculator::IsDigit(const QChar &ch) noexcept {
  return ch.isDigit() || ch == '.' || ch == 'X';
}

qsizetype Calculator::FindPositionToTerminate() const {
  const QString &line_text = ui->lineEdit->text();
  qsizetype position = 0;

  if (!line_text.isEmpty()) {
    position = line_text.length() - 1;
    if (line_text.length() > 1 && line_text[position] == '(' &&
        line_text[position - 1].isLetter()) {
      while (--position >= 0 && line_text[position].isLetter())
        ;
      ++position;
    }
  }
  return position;
}
}  // namespace s21