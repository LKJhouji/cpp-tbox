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
#ifndef TBOX_FLOW_IF_ELSE_H_20221022
#define TBOX_FLOW_IF_ELSE_H_20221022

#include "../action.h"

namespace tbox {
namespace flow {

/**
 * bool IfElseAction(if_action, succ_action, fail_acton) {
 *   if (if_action())
 *     return succ_action();
 *   else
 *     return fail_acton();
 * }
 */
class IfElseAction : public Action {
  public:
    explicit IfElseAction(event::Loop &loop);
    virtual ~IfElseAction();

    virtual void toJson(Json &js) const override;

    //! role: "if", "succ", "fail"
    virtual bool setChildAs(Action *child, const std::string &role) override;

    virtual bool isReady() const override;

  protected:
    virtual void onStart() override;
    virtual void onStop() override;
    virtual void onPause() override;
    virtual void onResume() override;
    virtual void onReset() override;

  protected:
    void onCondActionFinished(bool is_succ);

  private:
    Action *if_action_   = nullptr;
    Action *succ_action_ = nullptr;
    Action *fail_action_ = nullptr;
};

}
}

#endif //TBOX_FLOW_IF_ELSE_H_20221022
