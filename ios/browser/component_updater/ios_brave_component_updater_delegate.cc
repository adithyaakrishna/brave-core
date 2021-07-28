/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/ios/browser/component_updater/ios_brave_component_updater_delegate.h"

#include <utility>

#include "base/sequenced_task_runner.h"
#include "base/task/post_task.h"
#include "base/task/thread_pool.h"
#include "brave/components/brave_component_updater/browser/brave_on_demand_updater.h"
#include "brave/ios/browser/component_updater/ios_brave_component_installer.h"
#include "components/component_updater/component_updater_service.h"
#include "ios/chrome/browser/application_context.h"

using brave_component_updater::BraveComponent;
using brave_component_updater::BraveOnDemandUpdater;

namespace brave {

IOSBraveComponentUpdaterDelegate::IOSBraveComponentUpdaterDelegate()
    : task_runner_(base::ThreadPool::CreateSequencedTaskRunner(
          {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
           base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN})) {}

IOSBraveComponentUpdaterDelegate::~IOSBraveComponentUpdaterDelegate() {}

void IOSBraveComponentUpdaterDelegate::Register(
    const std::string& component_name,
    const std::string& component_base64_public_key,
    base::OnceClosure registered_callback,
    BraveComponent::ReadyCallback ready_callback) {
  brave::RegisterComponent(GetApplicationContext()->GetComponentUpdateService(),
                           component_name, component_base64_public_key,
                           std::move(registered_callback),
                           std::move(ready_callback));
}

bool IOSBraveComponentUpdaterDelegate::Unregister(
    const std::string& component_id) {
  return GetApplicationContext()
      ->GetComponentUpdateService()
      ->UnregisterComponent(component_id);
}

void IOSBraveComponentUpdaterDelegate::OnDemandUpdate(
    const std::string& component_id) {
  BraveOnDemandUpdater::GetInstance()->OnDemandUpdate(component_id);
}

void IOSBraveComponentUpdaterDelegate::AddObserver(
    ComponentObserver* observer) {
  GetApplicationContext()->GetComponentUpdateService()->AddObserver(observer);
}

void IOSBraveComponentUpdaterDelegate::RemoveObserver(
    ComponentObserver* observer) {
  GetApplicationContext()->GetComponentUpdateService()->RemoveObserver(
      observer);
}

scoped_refptr<base::SequencedTaskRunner>
IOSBraveComponentUpdaterDelegate::GetTaskRunner() {
  return task_runner_;
}

const std::string IOSBraveComponentUpdaterDelegate::locale() const {
  return GetApplicationContext()->GetApplicationLocale();
}

PrefService* IOSBraveComponentUpdaterDelegate::local_state() {
  return GetApplicationContext()->GetLocalState();
}

}  // namespace brave
