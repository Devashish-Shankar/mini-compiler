#pragma once

#include <memory>
#include <string>
#include <vector>

namespace minic {

// =============================
// AST Base Node
// =============================

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// =============================
// Expression Base
// =============================

class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
};

// =============================
// Statement Base
// =============================

class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

// =============================
// Number Expression
// =============================

class NumberExpr : public Expr {
public:
    explicit NumberExpr(const std::string& value);

    const std::string& getValue() const;

private:
    std::string value_;
};

// =============================
// Variable Expression
// =============================

class VariableExpr : public Expr {
public:
    explicit VariableExpr(const std::string& name);

    const std::string& getName() const;

private:
    std::string name_;
};

// =============================
// Binary Expression
// =============================

class BinaryExpr : public Expr {
public:
    BinaryExpr(
        std::string op,
        std::unique_ptr<Expr> left,
        std::unique_ptr<Expr> right
    );

    const std::string& getOperator() const;
    const Expr* getLeft() const;
    const Expr* getRight() const;

private:
    std::string op_;
    std::unique_ptr<Expr> left_;
    std::unique_ptr<Expr> right_;
};

// =============================
// Variable Declaration
// =============================

class VariableDeclaration : public Statement {
public:
    VariableDeclaration(
        std::string type,
        std::string name,
        std::unique_ptr<Expr> initializer
    );

    const std::string& getType() const;
    const std::string& getName() const;
    const Expr* getInitializer() const;

private:
    std::string type_;
    std::string name_;
    std::unique_ptr<Expr> initializer_;
};

// =============================
// Return Statement
// =============================

class ReturnStatement : public Statement {
public:
    explicit ReturnStatement(std::unique_ptr<Expr> expression);

    const Expr* getExpression() const;

private:
    std::unique_ptr<Expr> expression_;
};

// =============================
// Block Statement
// =============================

class BlockStatement : public Statement {
public:
    void addStatement(std::unique_ptr<Statement> statement);

    const std::vector<std::unique_ptr<Statement>>& getStatements() const;

private:
    std::vector<std::unique_ptr<Statement>> statements_;
};

// =============================
// Function Declaration
// =============================

class FunctionDeclaration : public ASTNode {
public:
    FunctionDeclaration(
        std::string returnType,
        std::string name,
        std::unique_ptr<BlockStatement> body
    );

    const std::string& getReturnType() const;
    const std::string& getName() const;
    const BlockStatement* getBody() const;

private:
    std::string returnType_;
    std::string name_;
    std::unique_ptr<BlockStatement> body_;
};

// =============================
// Program
// =============================

class Program : public ASTNode {
public:
    void addFunction(std::unique_ptr<FunctionDeclaration> function);

    const std::vector<std::unique_ptr<FunctionDeclaration>>&
    getFunctions() const;

private:
    std::vector<std::unique_ptr<FunctionDeclaration>> functions_;
};

} // namespace minic