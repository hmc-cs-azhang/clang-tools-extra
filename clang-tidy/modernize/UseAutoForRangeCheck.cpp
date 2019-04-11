//===--- UseAutoForRangeCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseAutoForRangeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <string>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace modernize {

void UseAutoForRangeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxForRangeStmt(hasLoopVariable(varDecl())).bind("loopInit"), this);
}

void UseAutoForRangeCheck::check(const MatchFinder::MatchResult &Result) {
  const SourceManager *SM = Result.SourceManager;
  const auto *RangeLoop = Result.Nodes.getNodeAs<cxxForRangeStmt>("loopInit");
  llvm::outs()<< "test\n";
  
  if (RangeLoop) {
    std::string DiagText = "Prefer auto in range based loop variable";
    llvm::StringRef ObjName = Lexer::getSourceText(
        CharSourceRange::getCharRange(RangeLoop->getSourceRange()), *SM,
        LangOptions());
    std::string NewText = ObjName.str();
    SourceLocation Target = RangeLoop->getBeginLoc();
    diag(Target, DiagText) << FixItHint::CreateReplacement(CharSourceRange::getTokenRange(Target, RangeLoop->getEndLoc()), NewText);
  }
}

} // namespace modernize
} // namespace tidy
} // namespace clang
