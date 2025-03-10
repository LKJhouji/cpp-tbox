/*
 *     .============.
 *    //  M A K E  / \
 *   //  C++ DEV  /   \
 *  //  E A S Y  /  \/ \
 * ++ ----------.  \/\  .
 *  \\     \     \ /\  /
 *   \\     \     \   /
 *    \\     \     \ /
 *     -============'
 *
 * Copyright (c) 2018 Hevake and contributors, all rights reserved.
 *
 * This file is part of cpp-tbox (https://github.com/cpp-main/cpp-tbox)
 * Use of this source code is governed by MIT license that can be found
 * in the LICENSE file in the root of the source tree. All contributing
 * project authors may be found in the CONTRIBUTORS.md file in the root
 * of the source tree.
 */
#include "loop_if_action.h"

#include <tbox/base/json.hpp>
#include <tbox/base/assert.h>

namespace tbox {
namespace flow {

using namespace std::placeholders;

LoopIfAction::LoopIfAction(event::Loop &loop) :
  Action(loop, "LoopIf")
{ }

LoopIfAction::~LoopIfAction() {
  CHECK_DELETE_RESET_OBJ(if_action_);
  CHECK_DELETE_RESET_OBJ(exec_action_);
}

void LoopIfAction::toJson(Json &js) const {
  Action::toJson(js);
  if_action_->toJson(js["0.if"]);
  exec_action_->toJson(js["1.exec"]);
}

bool LoopIfAction::setChildAs(Action *child, const std::string &role) {
    if (role == "if") {
        CHECK_DELETE_RESET_OBJ(if_action_);
        if_action_ = child;
        if (if_action_ != nullptr)
            if_action_->setFinishCallback(std::bind(&LoopIfAction::onIfFinished, this, _1));
        return true;

    } else if (role == "exec") {
        CHECK_DELETE_RESET_OBJ(exec_action_);
        exec_action_ = child;
        if (exec_action_ != nullptr)
            exec_action_->setFinishCallback(std::bind(&LoopIfAction::onExecFinished, this));
        return true;
    }

    LogWarn("%d:%s[%s], unsupport role:%s", id(), type().c_str(), label().c_str(), role.c_str());
    return false;
}

bool LoopIfAction::isReady() const {
    if (if_action_ == nullptr) {
        LogWarn("%d:%s[%s], if_action == nullptr", id(), type().c_str(), label().c_str());
        return false;
    }

    if (exec_action_ == nullptr) {
        LogWarn("%d:%s[%s], exec_action == nullptr", id(), type().c_str(), label().c_str());
        return false;
    }

    if (!if_action_->isReady() || !exec_action_->isReady())
        return false;

    return true;
}

void LoopIfAction::onStart() {
    TBOX_ASSERT(if_action_ != nullptr);
    if_action_->start();
}

void LoopIfAction::onStop() {
    auto curr_action = if_action_->state() == State::kFinished ? exec_action_ : if_action_;
    TBOX_ASSERT(curr_action != nullptr);
    curr_action->stop();
}

void LoopIfAction::onPause() {
    auto curr_action = if_action_->state() == State::kFinished ? exec_action_ : if_action_;
    TBOX_ASSERT(curr_action != nullptr);
    curr_action->pause();
}

void LoopIfAction::onResume() {
    auto curr_action = if_action_->state() == State::kFinished ? exec_action_ : if_action_;
    TBOX_ASSERT(curr_action != nullptr);
    curr_action->resume();
}

void LoopIfAction::onReset() {
    TBOX_ASSERT(if_action_ != nullptr && exec_action_ != nullptr);
    if_action_->reset();
    exec_action_->reset();
}

void LoopIfAction::onIfFinished(bool is_succ) {
    if (state() == State::kRunning) {
        if (is_succ) {
            exec_action_->start();
        } else {
            finish(finish_result_);
        }
    }
}

void LoopIfAction::onExecFinished() {
    if (state() == State::kRunning) {
        if_action_->reset();
        exec_action_->reset();

        if (!if_action_->start())
            finish(finish_result_);
    }
}

}
}
