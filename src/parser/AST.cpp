#include "parser/AST.h"

namespace minic {

// =============================
// NumberExpr
// =============================

NumberExpr::NumberExpr(const std::string& value)
    : value_(value) {}

const std::string& NumberExpr::getValue() const {
    return value_;
}

// =============================
// VariableExpr
// =============================

VariableExpr::VariableExpr(const std::string& name)
    : name_(name) {}

const std::string& VariableExpr::getName() const {
    return name_;
}

// =============================
// BinaryExpr
// =============================

BinaryExpr::BinaryExpr(
    std::string op,
    std::unique_ptr<Expr> left,
    std::unique_ptr<Expr> right
)
    : op_(std::move(op)),
      left_(std::move(left)),
      right_(std::move(right)) {}

const std::string& BinaryExpr::getOperator() const {
    return op_;
}

const Expr* BinaryExpr::getLeft() const {
    return left_.get();
}

const Expr* BinaryExpr::getRight() const {
    return right_.get();
}

// =============================
// VariableDeclaration
// =============================

VariableDeclaration::VariableDeclaration(
    std::string type,
    std::string name,
    std::unique_ptr<Expr> initializer
)
    : type_(std::move(type)),
      name_(std::move(name)),
      initializer_(std::move(initializer)) {}

const std::string& VariableDeclaration::getType() const {
    return type_;
}

const std::string& VariableDeclaration::getName() const {
    return name_;
}

const Expr* VariableDeclaration::getInitializer() const {
    return initializer_.get();
}

// =============================
// ReturnStatement
// =============================

ReturnStatement::ReturnStatement(
    std::unique_ptr<Expr> expression
)
    : expression_(std::move(expression)) {}

const Expr* ReturnStatement::getExpression() const {
    return expression_.get();
}

// =============================
// BlockStatement
// =============================

void BlockStatement::addStatement(
    std::unique_ptr<Statement> statement
) {
    statements_.push_back(std::move(statement));
}

const std::vector<std::unique_ptr<Statement>>&
BlockStatement::getStatements() const {
    return statements_;
}

// =============================
// FunctionDeclaration
// =============================

FunctionDeclaration::FunctionDeclaration(
    std::string returnType,
    std::string name,
    std::unique_ptr<BlockStatement> body
)
    : returnType_(std::move(returnType)),
      name_(std::move(name)),
      body_(std::move(body)) {}

const std::string& FunctionDeclaration::getReturnType() const {
    return returnType_;
}

const std::string& FunctionDeclaration::getName() const {
    return name_;
}

const BlockStatement* FunctionDeclaration::getBody() const {
    return body_.get();
}

// =============================
// Program
// =============================

void Program::addFunction(
    std::unique_ptr<FunctionDeclaration> function
) {
    functions_.push_back(std::move(function));
}

const std::vector<std::unique_ptr<FunctionDeclaration>>&
Program::getFunctions() const {
    return functions_;
}

} // namespace minic