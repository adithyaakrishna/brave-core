/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_ads/tooltips/ads_tooltips_controller.h"

#include <memory>
#include <utility>

#include "brave/browser/brave_ads/ads_service_factory.h"
#include "brave/browser/ui/brave_tooltips/brave_tooltip_popup.h"

namespace brave_ads {

namespace {

// A BraveTooltipDelegate that passes through events to the ads service
class PassThroughBraveTooltipDelegate
    : public brave_tooltips::BraveTooltipDelegate {
 public:
  PassThroughBraveTooltipDelegate(Profile* profile,
                                  const std::string& tooltip_id)
      : profile_(profile), tooltip_id_(tooltip_id) {}

  void OnShow() override {
    AdsService* ads_service = AdsServiceFactory::GetForProfile(profile_);
    DCHECK(ads_service);

    ads_service->OnShowTooltip(tooltip_id_);
  }

  void OnOkButtonPressed() override {
    AdsService* ads_service = AdsServiceFactory::GetForProfile(profile_);
    DCHECK(ads_service);

    ads_service->OnOkButtonPressedForTooltip(tooltip_id_);
  }

  void OnCancelButtonPressed() override {
    AdsService* ads_service = AdsServiceFactory::GetForProfile(profile_);
    DCHECK(ads_service);

    ads_service->OnCancelButtonPressedForTooltip(tooltip_id_);
  }

 protected:
  ~PassThroughBraveTooltipDelegate() override = default;

 private:
  Profile* profile_ = nullptr;  // NOT OWNED

  std::string tooltip_id_;

  PassThroughBraveTooltipDelegate(const PassThroughBraveTooltipDelegate&) =
      delete;
  PassThroughBraveTooltipDelegate& operator=(
      const PassThroughBraveTooltipDelegate&) = delete;
};

}  // namespace

AdsTooltipsController::AdsTooltipsController(Profile* profile)
    : profile_(profile) {
  DCHECK(profile_);
}

AdsTooltipsController::~AdsTooltipsController() = default;

void AdsTooltipsController::ShowTooltip(
    std::unique_ptr<brave_tooltips::BraveTooltip> tooltip) {
  // If there's no delegate, replace it with a PassThroughDelegate so clicks go
  // back to the appropriate handler
  tooltip->set_delegate(base::WrapRefCounted(
      new PassThroughBraveTooltipDelegate(profile_, tooltip->id())));

  brave_tooltips::BraveTooltipPopup::Show(profile_, std::move(tooltip));
}

void AdsTooltipsController::CloseTooltip(const std::string& tooltip_id) {
  brave_tooltips::BraveTooltipPopup::Close(tooltip_id,
                                           /* by_user */ false);
}

}  // namespace brave_ads
