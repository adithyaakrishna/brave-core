/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/ios/browser/component_updater/ios_brave_component_updater_configurator.h"

#define MakeIOSComponentUpdaterConfigurator \
  MakeIOSComponentUpdaterConfigurator_ChromiumImpl
#include "../../../../../../ios/chrome/browser/component_updater/ios_component_updater_configurator.cc"  // NOLINT
#undef MakeIOSComponentUpdaterConfigurator

namespace component_updater {

scoped_refptr<update_client::Configurator> MakeIOSComponentUpdaterConfigurator(
    const base::CommandLine* cmdline) {
  return base::MakeRefCounted<IOSBraveConfigurator>(cmdline);
}

}  // namespace component_updater
