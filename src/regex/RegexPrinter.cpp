#include "RegexPrinter.h"

#include "RegexNodes.h"

namespace {
struct RegexPrinterVisitor : RegexConstNodeVisitor {
  std::ostream& os;

  explicit RegexPrinterVisitor(std::ostream& os) : os(os) {}

  void visit(const SymbolNode& node) override { os << node.symbol; }
  void visit(const ZeroNode& node) override { os << "∅"; }
  void visit(const OneNode& node) override { os << "ε"; }
  void visit(const ConcatenationNode& node) override {
    node.left->accept(*this);
    node.right->accept(*this);
  }
  void visit(const OrNode& node) override {
    os << "(";
    node.left->accept(*this);
    os << " + ";
    node.right->accept(*this);
    os << ")";
  }
  void visit(const StarNode& node) override {
    os << "(";
    node.child->accept(*this);
    os << ")*";
  }
};
}  // namespace

RegexPrinter::RegexPrinter(const Regex& regex) : regex_(regex) {}

std::ostream& operator<<(std::ostream& os, const RegexPrinter& printer) {
  auto print_visitor = RegexPrinterVisitor(os);
  printer.get_root().accept(print_visitor);
  return os;
}