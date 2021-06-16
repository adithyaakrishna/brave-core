/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_shields/browser/ad_block_subscription_service.h"

#include "base/files/file_path.h"
#include "base/logging.h"
#include "brave/components/brave_shields/browser/ad_block_service.h"
#include "brave/components/brave_shields/browser/ad_block_service_helper.h"
#include "brave/components/brave_shields/common/brave_shield_constants.h"

namespace brave_shields {

AdBlockSubscriptionService::AdBlockSubscriptionService(
    const FilterListSubscriptionInfo& info,
    OnLoadCallback on_load_callback,
    brave_component_updater::BraveComponent::Delegate* delegate)
    : AdBlockBaseService(delegate),
      id_(info.list_url),
      on_load_callback_(on_load_callback),
      list_file_(info.list_dir.AppendASCII(kCustomSubscriptionListText)),
      load_on_start_(info.last_successful_update_attempt != base::Time::Min()),
      initialized_(false) {}

AdBlockSubscriptionService::~AdBlockSubscriptionService() {}

bool AdBlockSubscriptionService::Init() {
  if (!AdBlockBaseService::Init())
    return false;

  // if we already have local data, go ahead and load it
  if (load_on_start_) {
    load_on_start_ = false;
    OnSuccessfulDownload();
    // return false so we don't set the component to initialized until the data
    // is loaded
    return false;
  }

  return initialized_;
}

void AdBlockSubscriptionService::OnSuccessfulDownload() {
  GetDATFileData(list_file_, false,
                 base::BindOnce(&AdBlockSubscriptionService::OnListLoaded,
                                weak_factory_.GetWeakPtr()));
}

void AdBlockSubscriptionService::OnListLoaded() {
  on_load_callback_.Run(id_);
  initialized_ = true;
}

}  // namespace brave_shields